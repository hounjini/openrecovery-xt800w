#!/system/bin/sh.bin

PATH=/system/persistent/orbootstrap/utils:/system/bin/
export PATH

mount -o rw,remount rootfs /
mount -o rw,remount system /system

rm /system/persistent/recovery_mode

#exchange the init scripts
rm /init.rc
rm /init.mapphone_umts.rc
rm /init.mapphone_cdma.rc
rm /default.prop

cp -af /system/persistent/orbootstrap/init.rc /init.rc
cp -af /system/persistent/orbootstrap/default.prop /default.prop

rm /etc

cp -af /system/persistent/orbootstrap/menu /
cp -af /system/persistent/orbootstrap/res /
cp -af /system/persistent/orbootstrap/etc /

cp -af /system/persistent/orbootstrap/sbin/updater /sbin/updater
cp -af /system/persistent/orbootstrap/sbin/recovery /sbin/recovery

#write recovery values back
echo 0 > /proc/sys/kernel/panic_on_oops 
echo 480 > /proc/sys/kernel/hung_task_timeout_secs
echo 2 > /proc/cpu/alignment
echo 20000000 > /proc/sys/kernel/sched_latency_ns 
echo 5000000 > /proc/sys/kernel/sched_wakeup_granularity_ns
echo 0 > /proc/sys/kernel/sched_compat_yield 
echo 1 > /proc/sys/kernel/sched_child_runs_first

#umount everything
umount /mnt/secure/asec
umount /mnt/asec/com.androirc-1

umount /sdcard
umount /mnt/sdcard

umount /dev/cpuctl
umount /dev/pts
umount /sqlite_stmt_journals
umount /cdrom
umount /pds
umount /cache
umount /tmp
umount /data

rm /sdcard
mkdir /sdcard
 
rmdir /config
rmdir /sqlite_stmt_journals
rmdir /pds

#seems not to exist actually
mkdir /tmp
#/init is restarted
exec /system/persistent/orbootstrap/utils/2nd-init
