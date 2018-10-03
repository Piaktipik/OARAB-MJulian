#!/bin/bash
USBNAME=534d:0021
LSUSB=$(lsusb | grep $USBNAME)
FOLD="/dev/bus/usb/"$(echo $LSUSB | cut --delimiter=' ' --fields='2')"/"$(echo $LSUSB | cut --delimiter=' ' --fields='4' | tr --delete ":")
echo $LSUSB
echo $FOLD
sudo ./usbreset $FOLD