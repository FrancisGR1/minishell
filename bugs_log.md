->heredoc
    ->não bloqueia ctrl + \
    ->ctrl + d no fim leva a comportamento estranho

leaks se variável for nula -> string é vazio e causa comportamento indefinido
citações vazias: ''
--
nota: 
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
