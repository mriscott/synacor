machine: machine.c
	gcc -o machine machine.c


run: machine
	./machine
