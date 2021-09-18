#!/bin/bash

sudo mkdir /mnt/adsl_ramdisk

# "Here, size=2G means, the RAMDISK will be 2 GB in size. 
# To create RAMDISK of several MB, use M. 
# For example, to create 256 MB RAMDISK, put size=256M"
sudo mount -t tmpfs -o rw,size=2G tmpfs /mnt/adsl_ramdisk

# to unmount: sudo umount /mnt/adsl_ramdisk