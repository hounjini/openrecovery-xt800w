#!/system/bin/sh
mount -o rw,remount system /system
echo 1 > /system/persistent/recovery_mode
/system/persistent/orbootstrap/reboot
