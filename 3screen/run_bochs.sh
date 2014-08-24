#!/bin/bash

sudo losetup /dev/loop4 floppy.img
sudo bochs -f bochsrc.txt
sudo fuser -m -v /home/cos/mnt
sudo losetup -d /dev/loop4
