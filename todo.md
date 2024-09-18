IDEIAS:
comandos com múltiplas redireções passam por duas fases de processamento:
    processamento total
    divisão em subtokens
Exemplo:
    cat << END > hello.txt
    1ª fase: cat << END > hello.txt
    2ª fase: cmd: cat
            heredoc: <<
            delimitador: END
