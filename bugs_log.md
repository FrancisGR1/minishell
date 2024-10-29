nota: 
múltiplas redireções sem comandos dão leaks
<(ficheiro não existente) tem de dar erro
--
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
