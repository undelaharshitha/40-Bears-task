#!/bin/bash

# Configuration
LOG_FILE="/var/log/system_health.log"
INTERVAL=60
CPU_THRESHOLD=80
MEMORY_THRESHOLD=80
DISK_THRESHOLD=80
PING_TARGET="8.8.8.8"
PING_COUNT=2

# Start monitoring
start_monitoring() {
  echo "Starting system health monitoring..."
  nohup bash -c "while true; do check_system_health; sleep $INTERVAL; done" &> /dev/null &
  echo $! > /var/run/system_health_monitor.pid
  echo "System health monitoring started with PID: $(cat /var/run/system_health_monitor.pid)"
}

# Stop monitoring
stop_monitoring() {
  if [ -f /var/run/system_health_monitor.pid ]; then
    kill $(cat /var/run/system_health_monitor.pid)
    rm /var/run/system_health_monitor.pid
    echo "System health monitoring stopped."
  else
    echo "No system health monitoring process found."
  }
}

# Check status of monitoring
check_status() {
  if [ -f /var/run/system_health_monitor.pid ]; then
    echo "System health monitoring is running with PID: $(cat /var/run/system_health_monitor.pid)"
  else
    echo "System health monitoring is not running."
  }
}

# Check system health
check_system_health() {
  TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')

  # CPU usage
  CPU_USAGE=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}')
  CPU_WARNING=""
  if (( $(echo "$CPU_USAGE > $CPU_THRESHOLD" | bc -l) )); then
    CPU_WARNING="WARNING: High CPU usage: $CPU_USAGE%"
  fi

  # Memory usage
  MEMORY_USAGE=$(free | grep Mem | awk '{print $3/$2 * 100.0}')
  MEMORY_WARNING=""
  if (( $(echo "$MEMORY_USAGE > $MEMORY_THRESHOLD" | bc -l) )); then
    MEMORY_WARNING="WARNING: High memory usage: $MEMORY_USAGE%"
  fi

  # Disk usage
  DISK_USAGE=$(df / | grep / | awk '{ print $5}' | sed 's/%//g')
  DISK_WARNING=""
  if [ "$DISK_USAGE" -gt "$DISK_THRESHOLD" ]; then
    DISK_WARNING="WARNING: High disk usage: $DISK_USAGE%"
  fi

  # Network connectivity
  NETWORK_STATUS=$(ping -c $PING_COUNT $PING_TARGET > /dev/null 2>&1 && echo "Online" || echo "Offline")
  NETWORK_WARNING=""
  if [ "$NETWORK_STATUS" == "Offline" ]; then
    NETWORK_WARNING="WARNING: Network connectivity lost"
  fi

  # Log system health
  echo "$TIMESTAMP - CPU: $CPU_USAGE%, Memory: $MEMORY_USAGE%, Disk: $DISK_USAGE%, Network: $NETWORK_STATUS" >> $LOG_FILE

  # Log warnings if any
  if [ ! -z "$CPU_WARNING" ]; then
    echo "$TIMESTAMP - $CPU_WARNING" >> $LOG_FILE
  fi
  if [ ! -z "$MEMORY_WARNING" ]; then
    echo "$TIMESTAMP - $MEMORY_WARNING" >> $LOG_FILE
  fi
  if [ ! -z "$DISK_WARNING" ]; then
    echo "$TIMESTAMP - $DISK_WARNING" >> $LOG_FILE
  fi
  if [ ! -z "$NETWORK_WARNING" ]; then
    echo "$TIMESTAMP - $NETWORK_WARNING" >> $LOG_FILE
  fi
}

# Main script
case "$1" in
  start)
    start_monitoring
    ;;
  stop)
    stop_monitoring
    ;;
  status)
    check_status
    ;;
  *)
    echo "Usage: $0 {start|stop|status}"
    exit 1
    ;;


exit 0
