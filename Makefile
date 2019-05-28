CFLAGS=-g -O2 -Wall -Wextra -Wstrict-prototypes
CXXFLAGS=-g -O2 -Wall -Wextra

all: test example-c

test: rogueutil.h
	$(CXX) $(CXXFLAGS) -o cpp-test tests/cpp-test.cpp
	$(CC) $(CXXFLAGS) -o c-test tests/c-test.c

example-c: rogueutil.h
	$(CC) $(CFLAGS) -o c-example examples/example.c

.PHONY: clean

clean:
	rm -f cpp-test c-test c-example
	rm -rf *.dSYM
