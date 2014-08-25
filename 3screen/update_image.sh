#!/bin/bash
MNT=/home/cos/mnt
#sudo umount $MNT
#sudo /sbin/losetup -d /dev/loop4
 /sbin/losetup /dev/loop4 floppy.img
 mount /dev/loop4 $MNT
 cp src/kernel $MNT/kernel
 ls -al  $MNT
 echo "1"
 fuser -m -v $MNT
 umount $MNT #/dev/loop4
 /sbin/losetup -d /dev/loop4
