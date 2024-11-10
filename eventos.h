int inicializarLef(LEF **nodeEventos ,int tempo, int evento, int heroi, int base);

int freeLEF(LEF **l);

int inserirLef(MUNDO *m, LEF **listaEventos, LEF *novo);

void selectionSort(struct distancia vetor[], int a, int b);

void distanciasRelativas(MUNDO *mun, MISSAO *mis);

int eChega(MUNDO *m, LEF **listaEventos, int tempo, HEROI *h, BASE *b);

int eEspera(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b);

int eDesiste(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b);

int eAvisa(MUNDO *m, LEF **listaEventos, int tempo, BASE *b);

int eEntra(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b);

int eSai(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b);

int eViaja(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *d);

int eMissao (MUNDO *m, LEF **listaEventos, MISSAO *mis, int tempo);

void eFim(MUNDO *m, int tempo, int contador, int tentativas);