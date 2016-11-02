#!/bin/bash

make clean; make;
./bms1A 1_test_file1.0M.dat;
./errInjecter -i 1_test_file1.0M.dat.out -b 2910476; 
./bms1B 1_test_file1.0M.dat.out.err; 
cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
