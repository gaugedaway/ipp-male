#!/bin/bash

VALGRIND_ARGS="--error-exitcode=15
       	      --leak-check=full
       	      --show-leak-kinds=all
       	      --errors-for-leak-kinds=all"

for f in $2/*.in
do
    echo "$f:"

    ./$1 < $f > test.out 2> test.err

    if diff test.out ${f%.in}.out > /dev/null && diff test.err ${f%.in}.err > /dev/null
    then
	echo "[OK] Correct output"
    else
	echo "[ERROR] Incorrect output"
    fi


    if valgrind $VALGRIND_ARGS ./$1 < $f > valgrind.out 2>&1
    then
	echo "[OK] No memory problems"
    else
	echo "[ERROR] Valgrind found memory problems"
    fi

    rm test.out test.err valgrind.out
    echo
done
