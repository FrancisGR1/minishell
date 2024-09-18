#!/bin/bash 
cp ../obj/* .
rm main.o
cc  -g *.o main_tests.c -lreadline ../libft/libft.a  -o test
