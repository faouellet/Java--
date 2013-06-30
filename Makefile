OBJS = parser.o \
       codegen.o \
       main.o \
       tokens.o \

CC = clang++
CPPFLAGS = `llvm-config --cppflags`
LDFLAGS = `llvm-config --ldflags`
LIBS = `llvm-config --libs`

all: parser

clean:
	$(RM) -rf parser.cpp parser.hpp parser tokens.cpp $(OBJS)

parser.cpp: parser.y
	bison -d -o $@ $^

parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

%.o: %.cpp
	$(CC) --std=c++0x -c $(CPPFLAGS) -o $@ $<


parser: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)
