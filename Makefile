all: main

main: main.cpp fun.cpp clustering.cpp
	g++ -g3  main.cpp fun.cpp clustering.cpp -o pro
