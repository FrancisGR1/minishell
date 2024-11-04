wrappers.c
safe_close() e outras funções seguras?



---


lidar melhor com erros na execução:
se tiver $Asdas $Adsasd $adasd
    ->não fazer nada
se tiver "" 
    -> '' command not found

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
