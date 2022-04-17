CC=gcc

check: check.c
	$(CC) check.c -o check -Os -g

runcheck: check
	taskset 2 ./check

all: benign malicious

benign: benign.c
	$(CC) benign.c -o benign -Os -g

malicious: malicious.c
	$(CC) malicious.c -o malicious -Os -g

clean:
	rm -rf *.o malicious benign