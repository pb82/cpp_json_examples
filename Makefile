json/parser.o:
	$(CXX) -Wall -g -c -fPIC json/parser.cpp -o json/parser.o

json/printer.o:
	$(CXX) -Wall -g -c -fPIC json/printer.cpp -o json/printer.o

io/file.o:
	$(CXX) -Wall -g -c -fPIC io/file.cpp -o io/file.o

all: json/parser.o json/printer.o io/file.o
	$(CXX) -Wall -std=c++11 json/parser.o json/printer.o io/file.o main.cpp

clean:
	@rm a.out
	@rm json/*.o
	@rm io/*.o