build : main.c  io list.o statistic.o
	gcc -Wall -lm main.c io.o list.o statistic.o -o csvReader
io : io.h list.h statistic.h
	gcc -c -lm -Wall  io.c 
statistic : statistic.h list.h io.h
	gcc -c -lm -Wall statistic.c
list : list.h io.h
	gcc -c -lm -Wall list.c
purge : clean
	rm -f *.o
clean :
	rm -f csvReader

