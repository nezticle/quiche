#!/bin/bash

rm output/*.data
../../src/processmanager/processmanager ./launcher

if cmp -s testlog.reference output/testlog.data; then 
    echo test passed
else 
    diff -u testlog.reference output/testlog.data
    echo test failed
    exit 1
fi
