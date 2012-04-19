#!/bin/bash

echo crashing $1 >> output/testlog.data
if [ $1 ]; then
# crash later
    echo waiting for $1 seconds...
    sleep $1
fi

kill -11 $$
sleep 100

