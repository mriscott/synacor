machine: machine.c
	gcc -o machine machine.c

debug:
	gcc -o machine -DDBG=1 machine.c

run: machine
	./machine


clean:
	rm machine
