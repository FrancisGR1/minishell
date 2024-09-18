IDEIAS:
comandos com múltiplas redireções passam por duas fases de processamento:
    processamento total -> um só token
    divisão em subtokens -> lista de tokens associados
    Exemplo:
        cat << END > hello.txt
        1ª fase: cat << END > hello.txt
        2ª fase: cmd: cat
                heredoc: <<
                delimitador: END


ABORDAGENS:
    processamento total -> um só token
        token: echo "hello" > file.txt
    divisão em subtokens -> lista de tokens associados
        árvore:
                >
    echo hello      file.txt
