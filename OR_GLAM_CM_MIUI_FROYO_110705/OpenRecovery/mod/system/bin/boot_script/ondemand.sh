#!/system/bin/sh

#activate interactive
insmod /system/lib/modules/cpufreq_interactive.ko nr_running_addr=0xc00623a8
#activate conservative
insmod /system/lib/modules/cpufreq_conservative.ko
echo ondemand > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

