CC = gcc
CCFLAGS = -std=gnu99 -Wall -O3 -g -DNDEBUG

LDFLAGS = -pthread

EXECUTABLES = \
	clone-vm \
	launch-benchmark

all: $(EXECUTABLES)

clone-vm: clone-vm.c
	$(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)

launch-benchmark: launch-benchmark.c
	$(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean format

clean:
	rm -f $(EXECUTABLES) *.o

format:
	clang-format -style=file -i *.c
