HOJE:
"export a='ls -la' -> tem de executar quando:
    minishell> $a

verificar com env -i

wrappers.c
safe_close() e outras funções seguras?
se o string for vazio: 
"" -> '': command not found
mas se tiver uma VAR vazie: 
    $vazia echo hello -> hello


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
