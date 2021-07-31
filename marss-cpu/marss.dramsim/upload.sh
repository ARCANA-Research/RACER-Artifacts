#!/bin/bash
./qemu-mount  /hdd0/minhtruong/disk_image/lenet.qcow2 /hdd0/minhtruong/mnt/marss_image/
sudo cp ../RACER-CPU-GPUBaseline/cpu/bin/*.exe /hdd0/minhtruong/mnt/marss_image/root/
./qemu-umount /hdd0/minhtruong/mnt/marss_image

