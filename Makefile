FLAGS = -Wall

all: cut count srep

# Regla para cut
cut: cut.o
	gcc $(FLAGS) -o cut cut.o

cut.o: cut.c
	gcc $(FLAGS) -c cut.c

# Regla para count
count: count.o
	gcc $(FLAGS) -o count count.o

count.o: count.c
	gcc $(FLAGS) -c count.c

# Regla para srep
srep: srep.o funcionessrep.o
	gcc $(FLAGS) -o srep srep.o funcionessrep.o

srep.o: srep.c funcionessrep.h
	gcc $(FLAGS) -c srep.c

funcionessrep.o: funcionessrep.c funcionessrep.h
	gcc $(FLAGS) -c funcionessrep.c

# Limpiar archivos generados
clear:
	rm -f cut count srep *.o
