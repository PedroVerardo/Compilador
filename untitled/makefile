all:
	gcc -Wall -Wa,--execstack -o main main.c geraCodigo.c
	clear
	./main
clean:
	clear
dump:
	clear
	gcc -c TestDump.s
	objdump -d TestDump.o
