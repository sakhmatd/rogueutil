CFLAGS=-g -O2 -Wall -Wextra -Wstrict-prototypes
CXXFLAGS=-g -O2 -Wall -Wextra

all: test example-c

test: rogueutil.h test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp

example-c: rogueutil.h example.c
	$(CC) $(CFLAGS) -o example-c example.c

.PHONY: clean

clean:
	rm -f test example-c
	rm -rf *.dSYM
