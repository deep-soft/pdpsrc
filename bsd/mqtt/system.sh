#!/bin/sh

# Define MQTT server details
MQTT_BROKER="192.168.1.194" # Replace with your MQTT broker address
MQTT_USERNAME="MQTT"        # Replace with your MQTT username, if required
MQTT_PASSWORD="asdasdasd"   # Replace with your MQTT password, if required
MQTT_TOPIC_PREFIX="pdp11"   # Prefix for MQTT topics

# Run vmstat and capture the output, skipping headers
VMSTAT_OUTPUT=`/usr/ucb/vmstat | /usr/ucb/tail -1`

# Parse memory info (convert to floating-point with one decimal)
AVM=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $4}'` # Active virtual memory
FRE=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $5}'` # Free memory

# Parse disk info (convert to floating-point with one decimal)
R0=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $13}'` # Disk r0 activity
R1=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $14}'` # Disk r1 activity
R2=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $15}'` # Disk r2 activity
R3=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $16}'` # Disk r3 activity

# Parse CPU info (convert to floating-point with one decimal)
US=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $18}'` # User CPU time
SY=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $19}'` # System CPU time
ID=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $20}'` # Idle CPU time

# Calculate total CPU usage
CPU_USAGE=`echo "$US + $SY" | bc`

# Print results
echo "Memory Info:"
echo "  Active Virtual Memory (AVM): $AVM"
echo "  Free Memory (FRE): $FRE"

echo "Disk Activity:"
echo "  r0: $R0"
echo "  r1: $R1"
echo "  r2: $R2"
echo "  r3: $R3"

echo "CPU Usage:"
echo "  Total CPU Usage: $CPU_USAGE%"
echo "  User Time (US): $US%"
echo "  System Time (SY): $SY%"
echo "  Idle Time (ID): $ID%"

# Publish metrics to MQTT broker with an echo before each line
echo "Publishing: Virtual Memory ($AVM) to topic pdp11/virtual_memory"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/virtual_memory" -v "$AVM"

echo "Publishing: Free Memory ($FRE) to topic pdp11/free_memory"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/free_memory" -v "$FRE"

echo "Publishing: Disk Activity r0 ($R0) to topic pdp11/disk_activity_r0"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r0" -v "$R0"

echo "Publishing: Disk Activity r1 ($R1) to topic pdp11/disk_activity_r1"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r1" -v "$R1"

echo "Publishing: Disk Activity r2 ($R2) to topic pdp11/disk_activity_r2"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r2" -v "$R2"

echo "Publishing: Disk Activity r3 ($R3) to topic pdp11/disk_activity_r3"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r3" -v "$R3"

echo "Publishing: CPU Usage ($CPU_USAGE) to topic pdp11/cpu_usage"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage" -v "$CPU_USAGE"

echo "Publishing: CPU Usage User ($US) to topic pdp11/cpu_usage_user"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage_user" -v "$US"

echo "Publishing: CPU Usage System ($SY) to topic pdp11/cpu_usage_system"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage_system" -v "$SY"

echo "Publishing: CPU Usage Idle ($ID) to topic pdp11/cpu_usage_idle"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage_idle" -v "$ID"

echo "Publishing: CPU Usage Idle ($ID) to topic pdp11/cpu_usage_idle"
mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage" -v "$CPU_USAGE"#!/bin/sh

# Define MQTT server details
MQTT_BROKER="192.168.1.194" # Replace with your MQTT broker address
MQTT_USERNAME="MQTT"        # Replace with your MQTT username, if required
MQTT_PASSWORD="Smone2326"   # Replace with your MQTT password, if required
MQTT_TOPIC_PREFIX="pdp11"   # Prefix for MQTT topics

# Run vmstat and capture the output, skipping headers
VMSTAT_OUTPUT=`vmstat | tail -1`

# Parse memory info (convert to floating-point with one decimal)
AVM=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $4}'` # Active virtual memory
FRE=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $5}'` # Free memory

# Parse disk info (convert to floating-point with one decimal)
R0=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $13}'` # Disk r0 activity
R1=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $14}'` # Disk r1 activity
R2=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $15}'` # Disk r2 activity
R3=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $16}'` # Disk r3 activity

# Parse CPU info (convert to floating-point with one decimal)
US=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $18}'` # User CPU time
SY=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $19}'` # System CPU time
ID=`echo "$VMSTAT_OUTPUT" | awk '{printf "%.1f", $20}'` # Idle CPU time

# Calculate total CPU usage
CPU_USAGE=`echo "$US + $SY" | bc`

# Print results
echo "Memory Info:"
echo "  Active Virtual Memory (AVM): $AVM"
echo "  Free Memory (FRE): $FRE"

echo "Disk Activity:"
echo "  r0: $R0"
echo "  r1: $R1"
echo "  r2: $R2"
echo "  r3: $R3"

echo "CPU Usage:"
echo "  Total CPU Usage: $CPU_USAGE%"
echo "  User Time (US): $US%"
echo "  System Time (SY): $SY%"
echo "  Idle Time (ID): $ID%"

# Publish metrics to MQTT broker with an echo before each line
echo "Publishing: Virtual Memory ($AVM) to topic pdp11/virtual_memory"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/virtual_memory" -v "$AVM"

echo "Publishing: Free Memory ($FRE) to topic pdp11/free_memory"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/free_memory" -v "$FRE"

echo "Publishing: Disk Activity r0 ($R0) to topic pdp11/disk_activity_r0"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r0" -v "$R0"

echo "Publishing: Disk Activity r1 ($R1) to topic pdp11/disk_activity_r1"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r1" -v "$R1"

echo "Publishing: Disk Activity r2 ($R2) to topic pdp11/disk_activity_r2"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r2" -v "$R2"

echo "Publishing: Disk Activity r3 ($R3) to topic pdp11/disk_activity_r3"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/disk_activity_r3" -v "$R3"

echo "Publishing: CPU Usage ($CPU_USAGE) to topic pdp11/cpu_usage"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage" -v "$CPU_USAGE"

echo "Publishing: CPU Usage User ($US) to topic pdp11/cpu_usage_user"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage_user" -v "$US"

echo "Publishing: CPU Usage System ($SY) to topic pdp11/cpu_usage_system"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage_system" -v "$SY"

echo "Publishing: CPU Usage Idle ($ID) to topic pdp11/cpu_usage_idle"
./mqtt -h "$MQTT_BROKER" -u "$MQTT_USERNAME" -p "$MQTT_PASSWORD" -t "$MQTT_TOPIC_PREFIX/cpu_usage_idle" -v "$ID"
