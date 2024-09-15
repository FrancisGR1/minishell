#!/bin/bash 
cc obj/parse.o obj/reader.o obj/tokens.o main_tests.c -lreadline ../libft/libft.a  -o main
