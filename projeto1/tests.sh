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
						echo '\n=====Testando arquivo:' tests/$j/input/$i 
						./lukacompiler < tests/$j/input/$i > tests/$j/output/$NOME.out
						diff tests/$j/output/$NOME.out tests/$j/expected/$NOME.ex
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
	 				cd ../../..
	 		done	
			 ;;
			
		*)	LISTA=`ls tests/v$VERSAO/input`
			for i in $LISTA 
			do
				NOME=$(echo $i|cut -d"." -f1)
				echo '\n=====Testando arquivo:' $i 
				./lukacompiler < tests/v$VERSAO/input/$i > tests/v$VERSAO/output/$NOME.out
				diff tests/v$VERSAO/output/$NOME.out tests/v$VERSAO/expected/$NOME.ex
			done ;;

	esac 