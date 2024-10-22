->heredoc
    ->não bloqueia ctrl + \
    ->ctrl + d no fim leva a comportamento estranho

citações vazias: ''
--
nota: 
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
