#!/bin/bash

for file in "$@"
do
	c_formatter_42 < "$file" > tmp && mv tmp "$file"
	vim -e -c ':Stdheader' -c 'wq' "$file"
done
