#!/bin/bash
qemu-system-i386 -fda floppy.img -boot a -m 32 -net none # -s -S
