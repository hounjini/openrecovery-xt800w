#/system/bin/sh

echo "Overclock 1100/800/500/250/125"
echo ""

echo "modifying boot..."

if [ ! -f "/system/bin/mot_boot_mode.bin" ];
then
	mkdir -p /sdcard/OpenRecovery/backup/bin
	cp -f /system/bin/mot_boot_mode /sdcard/OpenRecovery/backup/bin/mot_boot_mode
	cp -f /system/bin/mot_boot_mode /system/bin/mot_boot_mode.bin
fi
cp -f /sdcard/OpenRecovery/mod/system/bin/mot_boot_mode /system/bin/mot_boot_mode
chmod 755 /system/bin/mot_boot_mode
chmod 755 /system/bin/mot_boot_mode.bin
chown 0.2000 /system/bin/mot_boot_mo*

echo "copying libs..."

cp -f /sdcard/OpenRecovery/mod/system/lib/modules/overclock21.ko /system/lib/modules/overclock21.ko
chmod 644 /system/lib/modules/overclock21.ko
chown -R 0.2000 /system/lib/modules

echo "copying scripts..."

if [ ! -d "/system/bin/boot_script" ]; then
	mkdir /system/bin/boot_script
fi

cp -fR /sdcard/OpenRecovery/mod/system/bin/boot_script/overcloc1100.sh /system/bin/boot_script/overclock.sh


chmod -R 755 /system/bin/boot_script
chown -R 0.2000 /system/bin/boot_script


sleep 1
