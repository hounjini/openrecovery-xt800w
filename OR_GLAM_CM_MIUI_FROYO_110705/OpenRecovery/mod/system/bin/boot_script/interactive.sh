#!/system/bin/sh

#activate conservative
insmod /system/lib/modules/cpufreq_conservative.ko
#activate interactive
nr_running=`busybox grep -w nr_running /proc/kallsyms|busybox sed -e "s/\([0-9A-Fa-f]\{8\}\).*/\1/"`
insmod /system/lib/modules/cpufreq_interactive.ko nr_running_addr=0x$nr_running

echo interactive > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo 125000 > /sys/devices/system/cpu/cpu0/cpufreq/interactive/min_sampling_rate

