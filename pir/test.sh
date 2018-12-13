insmod /home/pi/Desktop/sp/pir/pir_km.ko;
mknod -m 666 /dev/pir_km c 261 0;
insmod /home/pi/Desktop/sp/led/led_dev.ko;
mknod -m 666 /dev/led_dev c 262 0;
/home/pi/Desktop/sp/pir/pir2;
