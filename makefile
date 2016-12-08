compile:
	gcc semaphore.c
run: a.out
	-./a.out
clean:
	rm a.out
	rm -f *~
