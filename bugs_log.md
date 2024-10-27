->heredoc
    ->não bloqueia ctrl + \
    ->ctrl + d no fim leva a comportamento estranho

citações vazias: '' no primeiro argumento (o comando) leval a segfault

nota: 
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
