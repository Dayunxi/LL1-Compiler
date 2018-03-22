parser: parser.o word.o common.o table.o semantic.o syntax.o
	g++ -o parser parser.o word.o common.o table.o semantic.o syntax.o
parser.o: parser.cpp src/common.h src/word.h src/table.h src/global.h src/syntax.h src/semantic.h
	g++ -c parser.cpp
common.o: common.cpp src/word.h src/table.h
	g++ -c common.cpp
word.o: word.cpp src/word.h src/global.h
	g++ -c word.cpp
table.o: table.cpp src/global.h src/table.h
	g++ -c table.cpp
syntax.o: syntax.cpp src/common.h src/syntax.h src/word.h src/semantic.h
	g++ -c syntax.cpp
semantic.o: semantic.cpp src/global.h src/table.h src/common.h
	g++ -c semantic.cpp
clean:
	rm *.o
