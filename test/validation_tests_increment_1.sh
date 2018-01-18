#!/bin/bash

#Creation of the file where the results of the tests are stored
RESDIR=./results
mkdir -p "$RESDIR"
resfile="$RESDIR"/validation_tests_increment_1_res.txt
rm -f "$resfile"

#Launch the pricer on all the .dat files
echo "######################################################################################" >> "$resfile"
echo "#############################   TESTS ON THE .dat FILES   ############################" >> "$resfile"
echo "######################################################################################" >> "$resfile"
echo >> "$resfile"
tmpfile=$(mktemp)

for datFile in '../data/asian.dat' '../data/basket.dat' '../data/perf.dat'
do
  nbFail=0
  nbSuccess=0
  nbTot=50
  for i in `seq 1 $nbTot`;
  do
    ../build/pricer "$datFile" >> "$tmpfile" 2>&1
    if [ "$?" -eq 1 ]
    then
      nbFail=$(echo "$nbFail + 1" | bc)
    else
      res=$(grep -e 'Price at t=0:' "$tmpfile")
      price=${res##*: }
      dev=$(grep -e 'IC:' "$tmpfile")
      stdev=${res##*: }
      borneMinComputed=$(echo "$price - $stdev" | bc)
      borneMaxComputed=$(echo "$price + $stdev" | bc)

      pgrep=$(grep -e 'mc price:' "$datFile")
      p=${pgrep##*: }
      sgrep=$(grep -e 'mc standard deviation:' "$datFile")
      s=${sgrep##*: }
      borneMin=$(echo "$p - 1.96*$s" | bc)
      borneMax=$(echo "$p + 1.96*$s" | bc)

      if [ $(bc -l <<<"$borneMaxComputed < $borneMin") = 1 ] || [ $(bc -l <<<"$borneMax < $borneMinComputed") = 1 ]
      then
        nbFail=$(echo "$nbFail + 1" | bc)
      else
        nbSuccess=$(echo "$nbSuccess + 1" | bc)
      fi
    fi
  done
  failurePercent=$(echo "scale=3; $nbFail / $nbTot" | bc)
  if [ $(bc -l <<<"$failurePercent*100 > 5") = 1 ]
  then
    echo -e "\e[31m[FAILED]\e[0m : '../build/pricer "$datFile"' computed "$nbFail" out of "$nbTot" wrong initial prices." >> "$resfile"
  else
    echo -e "\e[32m[PASSED]\e[0m : '../build/pricer "$datFile"' returned "$nbSuccess" out of "$nbTot" valid results" >> "$resfile"
  fi
done
