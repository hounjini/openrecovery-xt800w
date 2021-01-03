#/system/bin/sh

echo "No overclocking!"
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

echo "delete overclock lib..."

rm /system/lib/modules/overclock12.ko

echo "delete overclock script..."

if [ ! -d "/system/bin/boot_script" ]; then
	mkdir /system/bin/boot_script
fi

rm /system/bin/boot_script/overclock.sh

chmod -R 755 /system/bin/boot_script
chown -R 0.2000 /system/bin/boot_script


sleep 1
