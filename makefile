# Make file

net367: host.o utilities.o link.o man.o main.o net.o switch.o
	gcc -o net367 host.o utilities.o link.o man.o main.o net.o switch.o

main.o: main.c main.h utilities.h link.h man.h host.h net.h switch.h
	gcc -c main.c

host.o: host.c main.h utilities.h link.h man.h host.h
	gcc -c host.c  

man.o:  man.c main.h utilities.h link.h man.h
	gcc -c man.c

net.o:  net.c main.h utilities.h link.h man.h host.h switch.h
	gcc -c net.c

utilities.o: utilities.c utilities.h
	gcc -c utilities.c

link.o:  link.c utilities.h main.h link.h
	gcc -c link.c

switch.o: switch.c main.h utilities.h link.h man.h switch.h
	gcc -c switch.c
