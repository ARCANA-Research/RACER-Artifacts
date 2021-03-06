/*
 * QEMU MIPS timer support
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "hw.h"
#include "mips_cpudevs.h"
#include "qemu-timer.h"

#define TIMER_FREQ	100 * 1000 * 1000

/* XXX: do not use a global */
uint32_t cpu_mips_get_random (CPUState *env)
{
    static uint32_t lfsr = 1;
    static uint32_t prev_idx = 0;
    uint32_t idx;
    /* Don't return same value twice, so get another value */
    do {
        lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xd0000001u);
        idx = lfsr % (env->tlb->nb_tlb - env->CP0_Wired) + env->CP0_Wired;
    } while (idx == prev_idx);
    prev_idx = idx;
    return idx;
}

/* MIPS R4K timer */
static void cpu_mips_timer_update(CPUState *env)
{
    uint64_t now, next;
    uint32_t wait;

    now = qemu_get_clock(vm_clock);
    wait = env->CP0_Compare - env->CP0_Count -
	    (uint32_t)muldiv64(now, TIMER_FREQ, get_ticks_per_sec());
    next = now + muldiv64(wait, get_ticks_per_sec(), TIMER_FREQ);
    qemu_mod_timer(env->timer, next);
}

/* Expire the timer.  */
static void cpu_mips_timer_expire(CPUState *env)
{
    cpu_mips_timer_update(env);
    if (env->insn_flags & ISA_MIPS32R2) {
        env->CP0_Cause |= 1 << CP0Ca_TI;
    }
    qemu_irq_raise(env->irq[(env->CP0_IntCtl >> CP0IntCtl_IPTI) & 0x7]);
}

uint32_t cpu_mips_get_count (CPUState *env)
{
    if (env->CP0_Cause & (1 << CP0Ca_DC)) {
        return env->CP0_Count;
    } else {
        uint64_t now;

        now = qemu_get_clock(vm_clock);
        if (qemu_timer_pending(env->timer)
            && qemu_timer_expired(env->timer, now)) {
            /* The timer has already expired.  */
            cpu_mips_timer_expire(env);
        }

        return env->CP0_Count +
            (uint32_t)muldiv64(now, TIMER_FREQ, get_ticks_per_sec());
    }
}

void cpu_mips_store_count (CPUState *env, uint32_t count)
{
    if (env->CP0_Cause & (1 << CP0Ca_DC))
        env->CP0_Count = count;
    else {
        /* Store new count register */
        env->CP0_Count =
            count - (uint32_t)muldiv64(qemu_get_clock(vm_clock),
                                       TIMER_FREQ, get_ticks_per_sec());
        /* Update timer timer */
        cpu_mips_timer_update(env);
    }
}

void cpu_mips_store_compare (CPUState *env, uint32_t value)
{
    env->CP0_Compare = value;
    if (!(env->CP0_Cause & (1 << CP0Ca_DC)))
        cpu_mips_timer_update(env);
    if (env->insn_flags & ISA_MIPS32R2)
        env->CP0_Cause &= ~(1 << CP0Ca_TI);
    qemu_irq_lower(env->irq[(env->CP0_IntCtl >> CP0IntCtl_IPTI) & 0x7]);
}

void cpu_mips_start_count(CPUState *env)
{
    cpu_mips_store_count(env, env->CP0_Count);
}

void cpu_mips_stop_count(CPUState *env)
{
    /* Store the current value */
    env->CP0_Count += (uint32_t)muldiv64(qemu_get_clock(vm_clock),
                                         TIMER_FREQ, get_ticks_per_sec());
}

static void mips_timer_cb (void *opaque)
{
    CPUState *env;

    env = opaque;
#if 0
    qemu_log("%s\n", __func__);
#endif

    if (env->CP0_Cause & (1 << CP0Ca_DC))
        return;

    /* ??? This callback should occur when the counter is exactly equal to
       the comparator value.  Offset the count by one to avoid immediately
       retriggering the callback before any virtual time has passed.  */
    env->CP0_Count++;
    cpu_mips_timer_expire(env);
    env->CP0_Count--;
}

void cpu_mips_clock_init (CPUState *env)
{
    env->timer = qemu_new_timer(vm_clock, &mips_timer_cb, env);
    env->CP0_Compare = 0;
    cpu_mips_store_count(env, 1);
}
