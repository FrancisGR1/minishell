->heredoc
    ->não bloqueia ctrl + \
    ->ctrl + d no fim leva a comportamento estranho

citações vazias: '' no primeiro argumento (o comando) leval a segfault
e leaks


expander: 
    caso de "$? $" só imprime o último código de saída
    caso de  "$!_" não imprime _
    caso de "$PWD asd asd  $_!asdasd" dá fugas
nota: 
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
