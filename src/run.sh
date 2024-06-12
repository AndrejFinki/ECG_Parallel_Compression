#!/bin/bash
/usr/bin/mpicxx -o src/driver src/driver.cpp

a=1
while [ $a -le 8 ]
do
    /usr/bin/mpirun --oversubscribe -np $a src/driver
    a=$(($a + 1))
done
