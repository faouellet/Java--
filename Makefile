CC=clang++

LFLAGS=`llvm-config --cppflags --ldflags --libs core`

all:
	$(CC) -o out main.cpp $(FLAGS)
