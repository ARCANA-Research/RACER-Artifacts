#!/bin/bash
./qemu-mount  <please specify full path to your disk image> <disk mounting point>
sudo cp ../../programs/marss/bin/*.exe <disk mounting point>
./qemu-umount <disk mounting point>
