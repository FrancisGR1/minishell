<<EOF1 <tmp_files/new_file >tmp_files/thre >>tmp_files/two cat <<EOF2 <tmp_files/new_file >tmp_files/thre >>tmp_files/two

<<EOF1 cat | ls

<<EOF1 <file cat

de vez enquando as redireções falham

---
comando para eliminar todos os subprocessos mortos-vivos
    --> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9
