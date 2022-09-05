.default: all

all: Task3

clean:
	rm -rf Task3 *.o *.dSYM

Task3: Main3.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
