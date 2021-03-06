/*
 * Copyright (c) 2006-2008 Openedhand Ltd.
 * Written by Andrzej Zaborowski <balrog@zabor.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 or
 * (at your option) version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "hw.h"
#include "pxa.h"
#include "sharpsl.h"
#include "sysbus.h"

#undef REG_FMT
#define REG_FMT			"0x%02lx"

/* SCOOP devices */

typedef struct ScoopInfo ScoopInfo;
struct ScoopInfo {
    SysBusDevice busdev;
    qemu_irq handler[16];
    uint16_t status;
    uint16_t power;
    uint32_t gpio_level;
    uint32_t gpio_dir;
    uint32_t prev_level;

    uint16_t mcr;
    uint16_t cdr;
    uint16_t ccr;
    uint16_t irr;
    uint16_t imr;
    uint16_t isr;
};

#define SCOOP_MCR	0x00
#define SCOOP_CDR	0x04
#define SCOOP_CSR	0x08
#define SCOOP_CPR	0x0c
#define SCOOP_CCR	0x10
#define SCOOP_IRR_IRM	0x14
#define SCOOP_IMR	0x18
#define SCOOP_ISR	0x1c
#define SCOOP_GPCR	0x20
#define SCOOP_GPWR	0x24
#define SCOOP_GPRR	0x28

static inline void scoop_gpio_handler_update(ScoopInfo *s) {
    uint32_t level, diff;
    int bit;
    level = s->gpio_level & s->gpio_dir;

    for (diff = s->prev_level ^ level; diff; diff ^= 1 << bit) {
        bit = ffs(diff) - 1;
        qemu_set_irq(s->handler[bit], (level >> bit) & 1);
    }

    s->prev_level = level;
}

static uint32_t scoop_readb(void *opaque, target_phys_addr_t addr)
{
    ScoopInfo *s = (ScoopInfo *) opaque;

    switch (addr & 0x3f) {
    case SCOOP_MCR:
        return s->mcr;
    case SCOOP_CDR:
        return s->cdr;
    case SCOOP_CSR:
        return s->status;
    case SCOOP_CPR:
        return s->power;
    case SCOOP_CCR:
        return s->ccr;
    case SCOOP_IRR_IRM:
        return s->irr;
    case SCOOP_IMR:
        return s->imr;
    case SCOOP_ISR:
        return s->isr;
    case SCOOP_GPCR:
        return s->gpio_dir;
    case SCOOP_GPWR:
    case SCOOP_GPRR:
        return s->gpio_level;
    default:
        zaurus_printf("Bad register offset " REG_FMT "\n", (unsigned long)addr);
    }

    return 0;
}

static void scoop_writeb(void *opaque, target_phys_addr_t addr, uint32_t value)
{
    ScoopInfo *s = (ScoopInfo *) opaque;
    value &= 0xffff;

    switch (addr & 0x3f) {
    case SCOOP_MCR:
        s->mcr = value;
        break;
    case SCOOP_CDR:
        s->cdr = value;
        break;
    case SCOOP_CPR:
        s->power = value;
        if (value & 0x80)
            s->power |= 0x8040;
        break;
    case SCOOP_CCR:
        s->ccr = value;
        break;
    case SCOOP_IRR_IRM:
        s->irr = value;
        break;
    case SCOOP_IMR:
        s->imr = value;
        break;
    case SCOOP_ISR:
        s->isr = value;
        break;
    case SCOOP_GPCR:
        s->gpio_dir = value;
        scoop_gpio_handler_update(s);
        break;
    case SCOOP_GPWR:
    case SCOOP_GPRR:	/* GPRR is probably R/O in real HW */
        s->gpio_level = value & s->gpio_dir;
        scoop_gpio_handler_update(s);
        break;
    default:
        zaurus_printf("Bad register offset " REG_FMT "\n", (unsigned long)addr);
    }
}

static CPUReadMemoryFunc * const scoop_readfn[] = {
    scoop_readb,
    scoop_readb,
    scoop_readb,
};
static CPUWriteMemoryFunc * const scoop_writefn[] = {
    scoop_writeb,
    scoop_writeb,
    scoop_writeb,
};

static void scoop_gpio_set(void *opaque, int line, int level)
{
    ScoopInfo *s = (ScoopInfo *) opaque;

    if (level)
        s->gpio_level |= (1 << line);
    else
        s->gpio_level &= ~(1 << line);
}

static int scoop_init(SysBusDevice *dev)
{
    ScoopInfo *s = FROM_SYSBUS(ScoopInfo, dev);
    int iomemtype;

    s->status = 0x02;
    qdev_init_gpio_out(&s->busdev.qdev, s->handler, 16);
    qdev_init_gpio_in(&s->busdev.qdev, scoop_gpio_set, 16);
    iomemtype = cpu_register_io_memory(scoop_readfn,
                    scoop_writefn, s, DEVICE_NATIVE_ENDIAN);

    sysbus_init_mmio(dev, 0x1000, iomemtype);

    return 0;
}

static bool is_version_0 (void *opaque, int version_id)
{
    return version_id == 0;
}


static const VMStateDescription vmstate_scoop_regs = {
    .name = "scoop",
    .version_id = 1,
    .minimum_version_id = 0,
    .minimum_version_id_old = 0,
    .fields = (VMStateField []) {
        VMSTATE_UINT16(status, ScoopInfo),
        VMSTATE_UINT16(power, ScoopInfo),
        VMSTATE_UINT32(gpio_level, ScoopInfo),
        VMSTATE_UINT32(gpio_dir, ScoopInfo),
        VMSTATE_UINT32(prev_level, ScoopInfo),
        VMSTATE_UINT16(mcr, ScoopInfo),
        VMSTATE_UINT16(cdr, ScoopInfo),
        VMSTATE_UINT16(ccr, ScoopInfo),
        VMSTATE_UINT16(irr, ScoopInfo),
        VMSTATE_UINT16(imr, ScoopInfo),
        VMSTATE_UINT16(isr, ScoopInfo),
        VMSTATE_UNUSED_TEST(is_version_0, 2),
        VMSTATE_END_OF_LIST(),
    },
};

static SysBusDeviceInfo scoop_sysbus_info = {
    .init           = scoop_init,
    .qdev.name      = "scoop",
    .qdev.desc      = "Scoop2 Sharp custom ASIC",
    .qdev.size      = sizeof(ScoopInfo),
    .qdev.vmsd      = &vmstate_scoop_regs,
    .qdev.props     = (Property[]) {
        DEFINE_PROP_END_OF_LIST(),
    }
};

static void scoop_register(void)
{
    sysbus_register_withprop(&scoop_sysbus_info);
}
device_init(scoop_register);

/* Write the bootloader parameters memory area.  */

#define MAGIC_CHG(a, b, c, d)	((d << 24) | (c << 16) | (b << 8) | a)

static struct __attribute__ ((__packed__)) sl_param_info {
    uint32_t comadj_keyword;
    int32_t comadj;

    uint32_t uuid_keyword;
    char uuid[16];

    uint32_t touch_keyword;
    int32_t touch_xp;
    int32_t touch_yp;
    int32_t touch_xd;
    int32_t touch_yd;

    uint32_t adadj_keyword;
    int32_t adadj;

    uint32_t phad_keyword;
    int32_t phadadj;
} zaurus_bootparam = {
    .comadj_keyword	= MAGIC_CHG('C', 'M', 'A', 'D'),
    .comadj		= 125,
    .uuid_keyword	= MAGIC_CHG('U', 'U', 'I', 'D'),
    .uuid		= { -1 },
    .touch_keyword	= MAGIC_CHG('T', 'U', 'C', 'H'),
    .touch_xp		= -1,
    .adadj_keyword	= MAGIC_CHG('B', 'V', 'A', 'D'),
    .adadj		= -1,
    .phad_keyword	= MAGIC_CHG('P', 'H', 'A', 'D'),
    .phadadj		= 0x01,
};

void sl_bootparam_write(target_phys_addr_t ptr)
{
    cpu_physical_memory_write(ptr, (void *)&zaurus_bootparam,
                              sizeof(struct sl_param_info));
}
