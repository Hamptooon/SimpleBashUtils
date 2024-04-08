#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./s21_cat"
    )
sys_command=(
    "cat"
    )

tests=(
"OPTIONS TestFiles/test8.txt"
"OPTIONS TestFiles/test1.txt"
"OPTIONS TestFiles/test1.txt TestFiles/test2.txt"
)
flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)
manual=(
"-b TestFiles/test1.txt"
"-b TestFiles/test1.txt nofile.txt"
"-e TestFiles/test1.txt"
"-e TestFiles/test1.txt nofile.txt"
"-n TestFiles/test2.txt"
"-n TestFiles/test2.txt nofile.txt"
"-s TestFiles/test3.txt"
"-s TestFiles/test3.txt nofile.txt -n"
"-s TestFiles/test3.txt nofile.txt"
"-t TestFiles/test3.txt"
"-t TestFiles/test3.txt nofile.txt"
"-n TestFiles/test4.txt"
"-n TestFiles/test4.txt TestFiles/test5.txt"
"-v TestFiles/test5.txt"
"-v catTest1.txt"
"-- TestFiles/test5.txt"
"no_file.txt"
"-n -b TestFiles/test1.txt"
"-s -n -e TestFiles/test4.txt"
"-b -e -n -s -t -v TestFiles/test3.txt"
"-benstv TestFiles/test3.txt"
)


run_test() {
    param=$(echo "$@" | sed "s/OPTIONS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32mSuccess\033[0m  $param"
    else
        let "FAIL++"
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mFail\033[0m  $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done
printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "+++++++++++++++++++++++"
echo "1 PARAMETER"
echo "+++++++++++++++++++++++"
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\n"
echo "+++++++++++++++++++++++"
echo "2 PARAMETERS"
echo "+++++++++++++++++++++++"
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done
printf "\n"
echo "+++++++++++++++++++++++"
echo "3 PARAMETERS"
echo "+++++++++++++++++++++++"
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "+++++++++++++++++++++++"
echo "4 PARAMETERS"
echo "+++++++++++++++++++++++"
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done
# 2 параметра
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

# 3 параметра
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo -e "\033[31mFAILED: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
printf "\n"


