wrappers.c
safe_close() e outras funções seguras?



---
se o string for vazio: 
"" 
---> '': command not found

sigquite em "blocking commands"

Makefile produz um ficheiro supressor de leaks SE não existir

Reorganizar código:
    estrutura própria para informação de heredoc?
norminette
--
--
--
--
--
--
--
builtins:
--
exit
cd
pwd
unset
echo
env
--
export


lembretes:
testar com env -i + valgrind



outros:
às vezes o ctrl +d não fecha logo
    ->será que é só quando faço ctrl + shift + v? (copy paste no terminal)

com este input não fecha logo:echo asdasddecho asdasd$lol123123lolçç
porquê?
->vou assumir que isto é um erro de Makefile + valgrind + minishell,
pq nunca acontece qd só uso ./minishell
