#!/bin/bash

echo loop >> output/testlog.data

while true; do
	echo looping >> output/looping.data
	sleep 1
done
