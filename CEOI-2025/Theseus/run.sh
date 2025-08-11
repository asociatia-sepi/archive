#!/bin/bash
g++ -std=c++20 -O2 stub.cpp mizerii.cpp -o concurent
g++ -std=c++20 -O2 -static manager.cpp -o manager
# g++ -std=c++20 -O2 generator.cpp -o generator
# echo "20 4 10" | ./generator > date.in 

mkfifo 0_in
mkfifo 0_out
mkfifo 1_in
mkfifo 1_out
mkfifo 2_in
mkfifo 2_out
mkfifo 3_in
mkfifo 3_out
mkfifo 4_in
mkfifo 4_out

./concurent 0_out 0_in 2> concurent0_cerr & 
./concurent 1_out 1_in 2> concurent1_cerr &
./concurent 2_out 2_in 2> concurent2_cerr &
./concurent 3_out 3_in 2> concurent2_cerr &
./concurent 4_out 4_in 2> concurent2_cerr &
./manager 0_in 0_out 1_in 1_out 2_in 2_out 3_in 3_out 4_in 4_out < date.in

wait

rm concurent
rm *_in 
rm *_out