#!/bin/bash

sleep_time=1
file='./record'

while true
do
	sleep $sleep_time
	date=$(date '+%Y-%m-%d %H:%M:%S')

	echo $date >> $file
	netstat -atnp | grep 9877 >>$file 2>/dev/null
	echo "" >> $file
done