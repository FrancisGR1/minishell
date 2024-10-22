<<EOF1 <tmp_files/new_file >tmp_files/thre >>tmp_files/two cat <<EOF2 <tmp_files/new_file >tmp_files/thre >>tmp_files/two | ls

-> não funciona com pipes ((múltiples rediroções) | (comando))

<<EOF1 cat >tmp_files/file | grep a

-> o output esta a ser apanhado pelo grep em vez de ir para o ficheiro


->heredoc
    ->não bloqueia ctrl + \
    ->ctrl + d no fim leva a comportamento estranho

citações vazias: ''
--
nota: 
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
-------------------------------------------------
