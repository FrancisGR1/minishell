"echo hello > output.txt"
"echo", "hello", ">", "output.txt"

"grep 'pattern' file.txt | sort"
"grep", "'pattern'", "file.txt", "|", "sort"

"ls -la > out.txt"
"ls", "-la", ">", "out.txt"

"cat file1.txt file2.txt | grep 'data'"
"cat", "file1.txt", "file2.txt", "|", "grep", "'data'"

"echo \"Hello; world\""
"echo", "\"Hello; world\""

"echo 'Don\'t parse this'"
"echo", "'Don\'t parse this'"

"echo $HOME"
"echo", "$HOME"

"ls > file.txt && cat file.txt"
"ls", ">", "file.txt", "&&", "cat", "file.txt"

"echo \"hello\" >> log.txt"
"echo", "\"hello\"", ">>", "log.txt"

"cat << END"
"cat", "<<", "END"

"echo $? && echo Done"
"echo", "$?", "&&", "echo", "Done"

"mkdir test && cd test"
"mkdir", "test", "&&", "cd", "test"

"ls -l | grep \".c\" | wc -l"
"ls", "-l", "|", "grep", "\".c\"", "|", "wc", "-l"

"echo \"\$VAR is not expanded\""
"echo", "\"\\$VAR is not expanded\""

"grep \"hello world\" < file.txt"
"grep", "\"hello world\"", "<", "file.txt"

"cat file.txt | grep 'pattern' && echo 'Found'"
"cat", "file.txt", "|", "grep", "'pattern'", "&&", "echo", "'Found'"

"echo \"error\" > error.log 2>&1"
"echo", "\"error\"", ">", "error.log", "2>&1"

"echo 'Multi; command' && ls"
"echo", "'Multi; command'", "&&", "ls"

"echo hello && echo world || echo error"
"echo", "hello", "&&", "echo", "world", "||", "echo", "error"

"cd .. && pwd"
"cd", "..", "&&", "pwd"

"echo \"Wildcard * test\" | grep 'test'"
"echo", "\"Wildcard * test\"", "|", "grep", "'test'"
