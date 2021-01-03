#!/system/bin/sh

if [ ! -f /sdcard/openrecovery/sbin/busybox ]; then
	echo "Cannot find busybox - sdcard part of OpenRecovery not installed."
	exit 2
fi

BUSYBOX=/tmp/busybox

cp /sdcard/openrecovery/sbin/busybox $BUSYBOX
chmod 777 $BUSYBOX

#now check using Busybox [ if we have first argument

#copying

$BUSYBOX mount -o remount,rw /system

if [ -d /system/persistent ]; then
	$BUSYBOX rm -R /system/persistent
fi
 
$BUSYBOX mkdir /system/persistent
$BUSYBOX cp -R "/sdcard/openrecovery/orbootstrap/TITA" /system/persistent
$BUSYBOX mv "/system/persistent/TITA" /system/persistent/orbootstrap

#sh
#$BUSYBOX cp /system/bin/sh /system/bin/sh.bin
#$BUSYBOX chmod 755 /system/persistent/orbootstrap/sh
$BUSYBOX cp -f /system/persistent/orbootstrap/sh /system/bin/sh
$BUSYBOX chmod 755 /system/bin/sh

$BUSYBOX cp -f /system/persistent/orbootstrap/sh_hijack.sh /system/bin/sh_hijack.sh
$BUSYBOX cp -f /system/persistent/orbootstrap/normalBoot.sh /system/bin/normalBoot.sh
$BUSYBOX cp -f /system/persistent/orbootstrap/ORLauncher /system/xbin/ORLauncher
$BUSYBOX chmod 755 /system/bin/*.sh
$BUSYBOX chmod 755 /system/xbin/ORLauncher

#permissions

$BUSYBOX chown -R 0:0 /system/persistent/orbootstrap/

$BUSYBOX chmod -R 644 /system/persistent/orbootstrap/res
$BUSYBOX chmod -R 644 /system/persistent/orbootstrap/menu
$BUSYBOX chmod -R 644 /system/persistent/orbootstrap/etc
$BUSYBOX chmod -R 755 /system/persistent/orbootstrap/sbin
$BUSYBOX chmod -R 755 /system/persistent/orbootstrap/utils

$BUSYBOX chmod -R 644 /system/persistent/orbootstrap/default.prop
$BUSYBOX chmod -R 755 /system/persistent/orbootstrap/init.rc

$BUSYBOX chmod 755 /system/persistent/orbootstrap/openrecovery.sh
$BUSYBOX chmod 755 /system/bin/openrecovery/bootToRecoveryOption

$BUSYBOX chmod 755 /system/persistent/orbootstrap/reboot
$BUSYBOX chmod 777 /system/persistent/orbootstrap/bootToRecoveryOption

#symlinks
CWD="$PWD"
cd /system/persistent/orbootstrap/utils 
$BUSYBOX ln -s bootstrapbox cp
$BUSYBOX ln -s bootstrapbox mkdir
$BUSYBOX ln -s bootstrapbox mount
$BUSYBOX ln -s bootstrapbox rm
$BUSYBOX ln -s bootstrapbox rmdir
$BUSYBOX ln -s bootstrapbox umount
cd "$CWD"

#delete /tmp/busybox
$BUSYBOX rm $BUSYBOX

echo "Installation done."
