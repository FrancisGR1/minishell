tratar de buggy input (bugs_log.md)

"export a='ls -la' -> tem de executar quando:
    minishell> $a

não pode interpretar variáveis se estiverem dentro de plicas:
'$var'

verificar com env -i
--
A SEGUIR:
TODO:bloquear teclas no heredoc
--
Na execução, mudar  
	int			fds[t->cmds_num][2];
	pid_t		pids[t->cmds_num];
tem de ser dinamicamente alocados ("VLA forbidden")
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
