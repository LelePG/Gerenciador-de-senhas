run: main
	./src/main

main : src/main.cpp src/Funcoes.h src/Senha.h 
	g++ ./src/main.cpp -lmysqlcppconn -o ./src/main
