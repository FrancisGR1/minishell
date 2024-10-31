problemas com a história do readline após heredoc
    nota: não consigo perceber em que input específico é que isto acontece

o bug tem a ver de certeza com as funções reescritas
    
	if (dlim_ptr.s == str.s && dlim_ptr.end == str.end)
		return ;
	if (dlim_ptr.s == str.s || dlim_ptr.end == str.end)

" $PWD asd sd $OLDPWD asd "
->dá leaks
expand() tem uma série de erros de contagem

" $PWD asd sd $OLDPWD asd "

$PWD
dollar_pos = 0
dollar_end_pos = 3

/home/francis/Documents/School_42/minishel asda $OLDPWD
dollar_pos = 49 
dollar_pos = 55 
