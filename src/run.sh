#!/bin/bash
/usr/bin/mpicxx -o src/driver src/driver.cpp

a=9
while [ $a -le 12 ]
do
    /usr/bin/mpirun --oversubscribe -np $a src/driver
    a=$(($a + 1))
done
