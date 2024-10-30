--

> cat + ctrl c
> echo $?
$?


readline com pipes

comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
