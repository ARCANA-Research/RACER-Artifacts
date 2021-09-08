#!/bin/bash
./qemu/qemu-system-x86_64 -m 8G -hda ../disk_image/ubuntu-natty.qcow2 -simconfig run.simcfg
