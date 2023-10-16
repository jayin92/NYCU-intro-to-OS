#!/bin/bash
# write a shell scritps to compare the output with the real answers

# Compile the program
g++ -std=c++11 -o hw2_110550088 hw2_110550088.cpp

# Run the program and compare the output
total=10
correct=0
for i in {1..5}; do
    for j in {1..2}; do
        echo "Part $i Test $j"
        ./hw2_110550088 < ./testcase/part_$i/t$j.txt > output.txt
        res=$(diff output.txt answer/part_$i/a$j.txt)
        if [ "$res" == "" ]; then
            echo "Correct"
            correct=$((correct+1))
        else
            echo "Wrong"
            echo "Your output:"
            cat output.txt
            echo "Correct answer:"
            cat answer/part_$i/a$j.txt
        fi
    done
done
rm output.txt
echo "Total: $correct/$total"