.default: all

all: Task4

clean:
	rm -rf Task4 *.o *.dSYM

Task4: Main4.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
