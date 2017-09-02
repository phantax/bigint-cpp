.PHONY: all info clean

# Use gcc as default compiler and linker
# May be changed by passing arguments to make
ifeq ($(origin CC), default)
    CC=g++
endif
ifeq ($(origin LD), default)
    LD=g++
endif


all: BigInteger.o

test: BigInteger.o
	@echo "\033[01;32m=> Compiling and linking '$<' ...\033[00;00m"
	$(CC) BigInteger.o test.cpp -o $@
	@echo ""

BigInteger.o: BigInteger.cpp BigInteger.h
	@echo "\033[01;32m=> Compiling '$<' ...\033[00;00m"
	$(CC) -c $(CFLAGS) BigInteger.cpp -o $@
	@echo ""

info:
	@echo "Compiler is \"$(CC)\" defined by $(origin CC)"
	@echo "Linker is \"$(LD)\" defined by $(origin LD)"
	@echo ""

clean:
	rm -f BigInteger.o




