#!/bin/bash


lsb_test() {
	./stegobmp -embed -in dummy.txt -p ./images/testing.bmp -out testimage.bmp -steg $1 >> ./log/lsb_test.log
	./stegobmp -extract -out "output_extract" -p "testimage.bmp" -steg $1 >> ./log/lsb_test.log
	diff dummy.txt output_extract.txt
	return $?
}

check_test_failed(){
	if [ $1 != 0 ]; then
		echo "$2 test failed"
		exit 1
	fi
}


lsb_test "LSBI"
echo "Running LSBI test"
check_test_failed $? "LSBI"
echo "OK"

echo "Running LSB1 test"
lsb_test "LSB1"
check_test_failed $? "LSB1"
echo "OK"

echo "Running LSB4 test"
lsb_test "LSB4"
check_test_failed $? "LSB4"
echo "OK"

echo "Testing finished successfully"
