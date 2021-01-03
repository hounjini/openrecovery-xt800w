#!/system/bin/sh

# clock unit: MHz
max_clk=1100
max_vsel=65

clk4=800
vsel4=56

clk3=500
vsel3=35

clk2=250
vsel2=25

clk1=125
vsel1=20

insmod /system/lib/modules/overclock12.ko
echo 0xc02ba05c > /proc/overclock/cpufreq_stats_update_addr
echo 0xc004f788 > /proc/overclock/omap2_clk_init_cpufreq_table_addr
#using these two commands to find exact value
#busybox grep cpufreq_stats_update /proc/kallsyms
#busybox grep omap2_clk_init_cpufreq_table /proc/kallsyms
#http://android.doshaska.net/statspatch

echo "${max_clk}000" > /proc/overclock/max_rate
echo $max_vsel > /proc/overclock/max_vsel

echo "5 ${max_clk}000000 $max_vsel" > /proc/overclock/mpu_opps
echo "4 ${clk4}000000 $vsel4" > /proc/overclock/mpu_opps
echo "3 ${clk3}000000 $vsel3" > /proc/overclock/mpu_opps
echo "2 ${clk2}000000 $vsel2" > /proc/overclock/mpu_opps
echo "1 ${clk1}000000 $vsel1" > /proc/overclock/mpu_opps

echo "0 ${max_clk}000" > /proc/overclock/freq_table
echo "1 ${clk4}000" > /proc/overclock/freq_table
echo "2 ${clk3}000" > /proc/overclock/freq_table
echo "3 ${clk2}000" > /proc/overclock/freq_table
echo "4 ${clk1}000" > /proc/overclock/freq_table

echo ${max_clk}000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
echo ${clk1}000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq

