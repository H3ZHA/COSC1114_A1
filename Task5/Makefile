.default: all

all: Task5

clean:
	rm -rf Task5 *.o *.dSYM

Task5: Main5.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
