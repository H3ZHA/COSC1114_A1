.default: all

all: Task1filter

clean:
	rm -rf Task1filter *.o *.dSYM

Task1filter: Main.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
