#!/bin/bash
echo "--- ENV INFO ---" > env_report.txt
echo "Date: $(date)" >> env_report.txt
echo "Node: $(hostname)" >> env_report.txt
echo "CPU: $(lscpu | grep 'Model name' | cut -d':' -f2 | sed 's/^[ \t]*//')" >> env_report.txt
echo "Governor: $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)" >> env_report.txt
echo "Max Freq: $(cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq)" >> env_report.txt
echo "Compiler: $(gcc --version | head -n 1)" >> env_report.txt
echo "----------------" >> env_report.txt
