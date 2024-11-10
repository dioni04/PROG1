parametrosCompilacao=-Wall #-Wshadow
nomePrograma=theboys

all: $(nomePrograma)

$(nomePrograma): theboys.o conjuntos.o eventos.o base.o
	gcc -o $(nomePrograma) theboys.o conjuntos.o eventos.o base.o -lm $(parametrosCompilacao)

theboys.o: theboys.h theboys.c
	gcc -c theboys.c $(parametrosCompilacao)

conjuntos.o: conjuntos.h conjuntos.c
	gcc -c conjuntos.c $(parametrosCompilacao)

eventos.o: eventos.h eventos.c
	gcc -c eventos.c $(parametrosCompilacao)

base.o: base.h base.c
	gcc -c base.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)

entrega:
	cd .. && tar zcvf theboys-entrega.tgz theboys-entrega