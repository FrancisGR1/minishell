<<EOF1 <tmp_files/new_file >tmp_files/thre >>tmp_files/two cat <<EOF2 <tmp_files/new_file >tmp_files/thre >>tmp_files/two

--> ps -aux | grep minishell | awk '{print $2}' | xargs kill -9


<<EOF1 cat
EOF1

<<EOF1 cat | ls

de vez enquanto as redireções falham
