#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a tests=(
"s test_0_grep.txt VAR"
"for grep_functional.c s21_grep.h Makefile VAR"
"for grep_functional.c VAR"
"-e for -e ^text s21_grep.c TestFiles/test1.txt s21_grep.h Makefile VAR"
"-e for -e ^t TestFiles/test1.txt VAR"
"-e regex -e ^\} grep_options.c VAR -f TestFiles/patterns.txt"
"-e while -e number grep_options.c Makefile VAR -f TestFiles/patterns.txt"
"VAR -f TestFiles/patterns.txt TestFiles/test1.txt"
"VAR t TestFiles/test5.txt"
"VAR TestFiles/test1.txt TestFiles/test1.txt"
"VAR TestFiles/test1.txt nofile.txt TestFiles/test2.txt"
"VAR TestFiles/test7.txt TestFiles/test3.txt TestFiles/test4.txt"
"VAR -f TestFiles/patterns.txt TestFiles/test2.txt nofile.txt TestFiles/test8.txt TestFiles/test7.txt TestFiles/test4.txt"
)

declare -a extra=(
"-n text TestFiles/test2.txt  TestFiles/test3.txt"
"-n text TestFiles/test3.txt"
"-n -e ^\} grep_functional.c"
"-ce ^text TestFiles/test2.txt  TestFiles/test3.txt"
"-e ^t test_3_grep.txt"
"-nivh n TestFiles/test2.txt  TestFiles/test3.txt TestFiles/test4.txt TestFiles/test5.txt TestFiles/test6.txt"
"-e"
"-ie TEXT TestFiles/test3.txt"
"-echar TestFiles/test2.txt TestFiles/test3.txt TestFiles/test5.txt"
"-ne n -e text TestFiles/test2.txt"
"-iv text TestFiles/test3.txt"
"-in text TestFiles/test3.txt"
"-c -l number TestFiles/test2.txt TestFiles/test6.txt"
"-v TestFiles/test2.txt -e te"
"-noe ) TestFiles/test2.txt"
"-l number TestFiles/test5.txt TestFiles/test6.txt"
"-o -e int grep_functional.c"
"-e = -e text TestFiles/test2.txt"
"-noe ing -e as -e include -e int -e is grep_functional.c"
"-e text -e numb -e t -e aboba -e is TestFiles/test6.txt"
"-c -e . TestFiles/test3.txt -e '.'"
"-l for no_file.txt grep_functional.c"
"-f TestFiles/patterns.txt grep_functional.c"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    leaks -quiet -atExit -- ./s21_grep $t > test_s21_grep.log
    leak=$(grep -A100000 leaks test_s21_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]
    then
      (( SUCCESS++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
