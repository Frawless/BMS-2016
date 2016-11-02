#!/bin/bash
#make clean; make; 

BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color


declare -a arr=("-r 15" "-r 16" "-r 17" "-r 18" "-r 19" "-r 20"  "-b 2910476" "-b 3056000" "-b 3201524" "-b 3347048")
# "-r 21" "-r 22" "-r 23" "-r 24" "-r 25"
	
for i in "${arr[@]}"
do
   #echo "$i"
   printf "Test: ${BLUE}$i${NC}: "
		
	./bms1A 1_test_file1.0M.dat > /dev/null 2>&1
	./errInjecter -i 1_test_file1.0M.dat.out $i;  > /dev/null 2>&1
	./bms1B 1_test_file1.0M.dat.out.err;  > /dev/null 2>&1
	cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok > /dev/null 2>&1
	if [ $? -eq 0 ]; then
			echo -e "${GREEN}OK${NC}"
	else
		echo -e "${RED}KO ($?)${NC}"
	fi
done


# You can access them using echo "${arr[0]}", "${arr[1]}" also


#echo "-r 17"
#	
#	
##!/bin/bash
#make clean; make; 
#
#echo "-r 
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -r 17; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -r 18; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -r 19; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -r 20; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -r 20; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -b 2910476; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -b 3056000; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok
#
#./bms1A 1_test_file1.0M.dat; 
#./errInjecter -i 1_test_file1.0M.dat.out -b 3201524; 
#./bms1B 1_test_file1.0M.dat.out.err; 
#cmp 1_test_file1.0M.dat 1_test_file1.0M.dat.out.err.ok