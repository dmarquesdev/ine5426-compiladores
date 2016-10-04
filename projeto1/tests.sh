#!/bin/sh  
# Script para automação de testes do lukacompiler

VERSAO=$1
	
	case $VERSAO in 
	 	all)
			cd tests
			PASTAS=`ls -d */`
			cd ..
	 		for j in $PASTAS 
	 			do
	 				LISTA=`ls tests/$j/input`
					for i in $LISTA 
						do
						NOME=$(echo $i|cut -d"." -f1)
						cd tests/$j
						mkdir output
						cd output
						mkdir erro
						cd ../../..
						echo '\n=====Testando arquivo:' tests/$j/input/$i 
						./lukacompiler < tests/$j/input/$i > tests/$j/output/$NOME.out 2> tests/$j/output/erro/$NOME.err
						echo '\nDiferenças entre arquivos de saída padrão:\n'
						diff tests/$j/output/$NOME.out tests/$j/expected/$NOME.ex
						echo '\nDiferenças entre arquivos de erro:\n'
						diff tests/$j/output/erro/$NOME.err tests/$j/expected/erro/$NOME.ex_err
						echo '======================================'
					done
	 			done	
			 ;;

		clearOut)
			cd tests
			PASTAS=`ls -d */`
			cd ..
	 		for j in $PASTAS 
	 			do
	 				echo $j
	 				cd tests/$j/output
	 				rm -dfr *
	 				mkdir erro
	 				cd ../../..
	 		done	
			 ;;
			
		*)	LISTA=`ls tests/v$VERSAO/input`
			for i in $LISTA 
			do
				NOME=$(echo $i|cut -d"." -f1)
				cd tests/v$VERSAO
				mkdir output
				cd output
				mkdir erro
				cd ../../..
				echo '\n=====Testando arquivo: versão '$VERSAO $i 
				./lukacompiler < tests/v$VERSAO/input/$i > tests/v$VERSAO/output/$NOME.out 2> tests/v$VERSAO/output/erro/$NOME.err
				echo '\nDiferenças entre arquivos de saída padrão:\n'
				diff tests/v$VERSAO/output/$NOME.out tests/v$VERSAO/expected/$NOME.ex
				echo '\nDiferenças entre arquivos de erro:\n'
				diff tests/v$VERSAO/output/erro/$NOME.err tests/v$VERSAO/expected/erro/$NOME.ex_err
				echo '======================================'
			done ;;

	esac 