#!/bin/bash

PROJECT="$1"
FOLDER="$2"
BLANK='\e[0m'
RED='\e[31;21m'
GREEN='\e[32;21m'
counter=0
maximum=0

for f in "${FOLDER}"/*.in
do
    NAME=${f%.in}
    NAME=${NAME#"${FOLDER}"/}
    OUT=$(mktemp)
    ERR=$(mktemp)
    oks=0
    valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --main-stacksize=999999999 -q ./$1  <$f >$OUT 2>$ERR
	valueOfValgrind=$?
    

    echo "	TEST $((maximum + 1)): $f"
    echo -n -e "\e[1mOUTPUT: $BLANK"
    
    if diff $OUT ${f%.in}.out > /dev/null
    then
        echo -e -n "$GREEN - OK!$BLANK"
        ((oks++))
    else
        echo -e -n "$RED - WRONG!$BLANK"
    fi

    echo -e -n "	\e[1mERROR: $BLANK"
    if diff $ERR ${f%.in}.err > /dev/null
    then
        echo -e -n "$GREEN - OK!$BLANK"
        ((oks++))
    else
        echo -e -n "$RED - WRONG!$BLANK"
    fi

    echo -e -n "	\e[1mVALGRIND: $BLANK"
    if ((valueOfValgrind != 15))
    then
        echo -e -n "$GREEN - OK!$BLANK"
        ((oks++))
    else
        echo -e -n "$RED - WRONG!$BLANK"
    fi

    ((maximum++))

    if ((oks == 3))
    then
        ((counter++))
    fi
    rm $OUT $ERR
    echo " "

done

echo "$counter/$maximum tests passed"