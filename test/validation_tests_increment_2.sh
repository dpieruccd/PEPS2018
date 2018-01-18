#!/bin/bash

#Creation of the file where the results of the tests are stored
RESDIR=./results
mkdir -p "$RESDIR"
resfile="$RESDIR"/validation_tests_increment_2_res.txt
rm -f "$resfile"

#Launch the pricer on all the .dat files
echo "######################################################################################" >> "$resfile"
echo "#############################       TESTS AT t = 0        ############################" >> "$resfile"
echo "######################################################################################" >> "$resfile"
echo >> "$resfile"
tmpfile=$(mktemp)
for datFile in 'asian.dat' 'basket.dat' 'perf.dat'
do
    ../build/pricer -d 0 -c "../data/simul_$datFile" "../data/$datFile" >> "$tmpfile" 2>&1
    if [ "$?" -eq 1 ]
    then
	nbFail=$(echo "$nbFail + 1" | bc)
    else
	res=$(grep -e 'Price at t= 0:' "$tmpfile")
	price=${res##*: }
	dev=$(grep -e 'IC:' "$tmpfile")
	stdev=${res##*: }
	borneMinComputed=$(echo "$price - $stdev" | bc)
	borneMaxComputed=$(echo "$price + $stdev" | bc)
	
	pgrep=$(grep -e 'mc price:' "../data/$datFile")
	p=${pgrep##*: }
	sgrep=$(grep -e 'mc standard deviation:' "../data/$datFile")
	s=${sgrep##*: }
	borneMin=$(echo "$p - 1.96*$s" | bc)
	borneMax=$(echo "$p + 1.96*$s" | bc)
	
	if [ $(bc -l <<<"$borneMaxComputed < $borneMin") = 1 ] || [ $(bc -l <<<"$borneMax < $borneMinComputed") = 1 ]
	then
	    echo -e "\e[31m[FAILED]\e[0m : '../build/pricer -d 0 -c ../data/simul_$datFile ../data/$datFile' computed a wrong price." >> "$resfile" 
	else
            echo -e "\e[32m[PASSED]\e[0m : '../build/pricer -d 0 -c ../data/simul_$datFile ../data/$datFile' returned a valid result" >> "$resfile"
	fi
    fi
done
