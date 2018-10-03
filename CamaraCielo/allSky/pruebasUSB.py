import os
from time import gmtime, strftime, localtime, strptime, time, sleep#, timedelta

print ("Reiniciando USBs...")
os.system("echo '1-1' |sudo tee /sys/bus/usb/drivers/usb/unbind")
sleep(10) 
os.system("echo '1-1' |sudo tee /sys/bus/usb/drivers/usb/bind")