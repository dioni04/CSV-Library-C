all : main.c io list statistic filter comp #target principal
	gcc -Wall main.c io.o list.o statistic.o filter.o comp.o -o csvReader -lm
io : io.h list.h statistic.h
	gcc -c -Wall  io.c
statistic : statistic.h list.h io.h
	gcc -c -Wall statistic.c
list : list.h io.h
	gcc -c -Wall list.c
filter : filter.h io.h
	gcc -c -Wall filter.c
comp : comp.h
	gcc -c -Wall comp.c
tar : #target para tar do projeto
	tar zcvf ../jeza23-A2.tar.gz --exclude='*.csv' ../jeza23-A2
purge : clean
	rm -f csvReader
clean :
	rm -f *.o
