#!/bin/sh  
# Script para automação de testes do lukacompiler

LISTA=`ls tests/input`
			for i in $LISTA 
			do
				NOME=$(echo $i|cut -d"." -f1)
				cd tests
				mkdir output
				cd ..
				echo '\nDiferenças entre arquivos de saída padrão:\n'
				diff tests/output/$NOME.out tests/expected/$NOME.ex
				echo '\nDiferenças entre arquivos de erro:\n'
				diff tests/output/erro/$NOME.err tests/expected/erro/$NOME.ex_err
				echo '======================================'
			done 
