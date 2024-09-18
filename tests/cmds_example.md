---------------------------------------------------
---------------- COMANDOS SIMPLES -----------------
---------------------------------------------------

cd ..
var=value
var="value"

---------------------------------------------------
---------------- COMANDOS COMPLEXOS ---------------
---------------------------------------------------

REDIREÇÕES
command < input.txt > output.txt 2> error.log
command >> output.txt 2>> error.log < input.txt
command 2>&1 > combined.log < input.txt
command < input1.txt < input2.txt > output.txt
command > output.txt 2> error.log << EOF
command1 > file1.txt | command2 < file2.txt > file3.txt
command1 2> /dev/null | command2 > output.txt < input.txt
command < input.txt >> output.txt 2>> error.log
command1 < input1.txt | command2 > output.txt 2> error.log
command >> output.txt < <(command2) 2> error.log
command1 > >(command2) 2> error.log < input.txt
command1 2>&1 > /dev/null < <(command2 < input.txt)
command < input.txt > output.txt >> append.txt 2> error.log
command1 < <(command2 < input.txt) > output.txt 2>> error.log
command1 > file1.txt 2>&1 | command2 < file2.txt >> file3.txt 2> /dev/null
