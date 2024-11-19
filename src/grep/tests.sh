#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""

RED="$(tput setab 1)"
GREEN="$(tput setab 2)"
RESET="$(tput sgr0)"

test_diff() {
  if [ "$1" == "" ]; then
    ((SUCCESS++))
    echo "$GREEN SUCCESS $RESET ./s21_grep $2"
  else
    ((FAIL++))
    echo "$RED FAIL $RESET ./s21_grep $2"
    echo "$DIFF_RES"
  fi
}

for var3 in -v -c -l -n; do
      LINE="$var3 chi chapa.txt dubi-dubi.txt"
      ./s21_grep $LINE >1.txt
      grep $LINE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$LINE"

      LINE="$var3 chi dubi-dubi.txt"
      ./s21_grep $LINE >1.txt
      grep $LINE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$LINE"

done

for var2 in -v -c -l -n; do
    for var3 in -v -c -l -n; do
      if [ $var2 == $var3 ]; then
        continue
      fi
      LINE="$var2 $var3 chi chapa.txt dubi-dubi.txt"
      ./s21_grep $LINE >1.txt
      grep $LINE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$LINE"

      LINE="$var2 $var3 chi chapa.txt"
      ./s21_grep $LINE >1.txt
      grep $LINE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$LINE"

    done
done


echo ""
echo "$GREEN SUCCEEDED $RESET: $SUCCESS"
echo "$RED FAILED $RESET: $FAIL"