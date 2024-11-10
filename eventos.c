#include "theboys.h"
#include "base.h"
#include "conjuntos.h"

#define DISTANCIA(x,xo,y,yo) sqrt(pow((x) - (xo), 2) + pow((y) - (yo), 2))

/*
Essa biblioteca possui funcoes relacionadas a:
- Funcoes para manipulação de LEF
- Tratamento de eventos
- Missoes
*/

/* 
0 - Erro de alocacao
1 - Sucesso
*/

//incializa um nodo da lista de eventos
//-1 quando dado não é usado

int inicializarLef(LEF **nodeEventos ,int tempo, int evento, int heroi, int base){
    LEF *aux = NULL;

    if((aux = (LEF*) malloc(sizeof(LEF))) == NULL)
        return 0;
    
    aux->tempo = tempo;
    aux->evento = evento;
    aux->heroi = heroi;
    aux->base = base;
    aux->prox = NULL;

    *nodeEventos = aux;
    return 1;
}

//libera memoria da LEF
int freeLEF(LEF **l){
    LEF *aux = *l;

    while(*l != NULL){
        *l = (*l)->prox;
        
        free(aux);
        
        aux = *l;
    }
    return 1;
}

//insere novo evento ordenado na LEF
int inserirLef(MUNDO *m, LEF **listaEventos, LEF *novo){
    LEF *aux = *listaEventos;

    //se for primeiro na lista 
    if(((*listaEventos))->tempo > novo->tempo){
        novo->prox = *listaEventos;
        *listaEventos = novo;
        return 0;
    }
    //percorre a lista ate ponto correto na lista
    while((aux->prox != NULL) && (aux->prox->tempo <= novo->tempo))
        aux = aux->prox;
    
    novo->prox = aux->prox;
    aux->prox = novo;

    return 0;
}

//todos herois na base recebem +1 XP
int ganharXP(MUNDO *m, BASE b){
    lista *aux = b.presentes;

    if(aux == NULL)
        return 0;
    while(aux != NULL){
        m->herois[aux->elem].xp++;
        aux = aux->prox;
    }
    return 1;
}

//troca elementos da struct de indices a e b
void troca(struct distancia vetor[],int a, int b){
	int aux;
	int auxId;

	aux = vetor[a].dist;
	vetor[a].dist = vetor[b].dist;
	vetor[b].dist = aux;
	
    auxId = vetor[a].idBase;
    vetor[a].idBase = vetor[b].idBase;
	vetor[b].idBase = auxId;
    return;
}

//acha indice da menor distancia
int menor(struct distancia vetor[], int a, int b){
	int men = a;

	for (int i = a; i < b; i++){
		if (vetor[i].dist < vetor[men].dist)
			men = i;
	}
	return men;
}

//selection sort para ordenar vetor de distancias da missao
void selectionSort(struct distancia vetor[], int a, int b){
	if (a >= b)
		return;
	troca(vetor,a,menor(vetor,a,b));
	selectionSort(vetor,a+1,b);
	return;
}

//calcula distancias de missao de todas as bases e coloca em vetor
void distanciasRelativas(MUNDO *mun, MISSAO *mis){
    for (int i = 0; i < mun->nBases; i++){
        mis->d[i].dist = DISTANCIA(mun->bases[i].localBase.x, mis->local.x,
                                mun->bases[i].localBase.y, mis->local.y);
        mis->d[i].idBase = i;
    }
    return; 
}

//heroi chega em base e decide se vai esperar ou nao
int eChega(MUNDO *m, LEF **listaEventos, int tempo, HEROI *h, BASE *b){
    LEF *aux = NULL;

    int espera = 0;

    h->idBase = b->id;//atualiza base de H

    if(!(baseCheia(*b)) && (vaziaFila(*b)))
        espera = 1;
    else
        espera = h->paciencia > (10 * (b->espera.fim + 1));
    
    //se espera for true insere evento espera(1) na LEF
    if(espera){
        if(!(inicializarLef(&aux, tempo, 1, h->id, b->id)))
            return 0;
        inserirLef(m, listaEventos, aux);
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
                            (*listaEventos)->tempo, (*listaEventos)->heroi,
                            (*listaEventos)->base, contaHeroi(*b), b->lotacao);
    }
    //se nao insere evento desiste(2) na LEF
    else{
        if(!(inicializarLef(&aux, tempo, 2, h->id, b->id)))
            return 0;
        inserirLef(m, listaEventos, aux);
            printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
                            (*listaEventos)->tempo, (*listaEventos)->heroi,
                            (*listaEventos)->base, contaHeroi(*b), b->lotacao);
    }
    return 1;
}

//se heroi espera coloca evento avisar na LEF
int eEspera(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b){
    LEF *aux = NULL;

    enfileiraBase(b, h);
    //insere evento avisa(3) na LEF
    if(!(inicializarLef(&aux, tempo, 3, -1, b->id)))
        return 0;
    inserirLef(m, listaEventos, aux);
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", 
                            (*listaEventos)->tempo, (*listaEventos)->heroi,
                            (*listaEventos)->base, b->espera.fim-1);
    return 1;
}

//se heroi desiste da fila da base viaja a outra base aleatoria
int eDesiste(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b){
    LEF *aux = NULL;
    int baseAleat = rand() % m->nBases;//base para viagem

    //insere evento viaja(6) na LEF
    if(!(inicializarLef(&aux, tempo, 6, h.id, baseAleat)))
        return 0;
    inserirLef(m, listaEventos, aux);
    printf("%6d: DESIST HEROI %2d BASE %d\n", 
                            (*listaEventos)->tempo, (*listaEventos)->heroi, 
                            (*listaEventos)->base);
    return 1;
}

//avisa porteiro da base 
int eAvisa(MUNDO *m, LEF **listaEventos, int tempo, BASE *b){
    int idHeroi;
    lista *auxLista = NULL;
    LEF *aux = NULL;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
                            (*listaEventos)->tempo, (*listaEventos)->base,
                            contaHeroi(*b), b->lotacao);      

    printf("[");
    imprimeFila(*b);
    printf("]\n");
    //enquanto base nao estiver cheia e fila nao vazia
    while(!(baseCheia(*b)) && !(vaziaFila(*b))){
        desinfileiraBase(b, &idHeroi);
        //insere id do heroi em presentes da base
        if(b->presentes == NULL)
            inicializarLista(&b->presentes, idHeroi);
        else{
            inicializarLista(&auxLista, idHeroi);
            inserirLista(&b->presentes, auxLista);
        }
        //insere evento entra(4) na LEF
        if(!(inicializarLef(&aux, tempo, 4, idHeroi, b->id)))
            return 0;
        inserirLef(m, listaEventos, aux);
        aux = NULL;
        auxLista = NULL;
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n",
                            tempo, (*listaEventos)->base, idHeroi);
    }
    return 1;
}

//heroi entra na base e coloca evento de saida com tempo+tpb
int eEntra(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b){
    LEF *aux = NULL;
    int TPB = 15 + (h.paciencia * ((rand() % 20) + 1));

    //insere evento sai(5) na LEF
    if(!(inicializarLef(&aux, tempo + TPB, 5, h.id, b->id)))
        return 0;
    inserirLef(m, listaEventos, aux);
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
                            (*listaEventos)->tempo,(*listaEventos)->heroi,
                            (*listaEventos)->base, contaHeroi(*b),
                            b->lotacao, tempo + TPB);
    return 1;
}

//heroi sai da base e faz viagem e avisa porteiro
int eSai(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *b){
    LEF *aux = NULL;
    int baseAleat = rand() % m->nBases;

    //remove heroi do conjunto de presentes
    removerHeroi(b, h);
    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", 
                            (*listaEventos)->tempo, (*listaEventos)->heroi,
                            (*listaEventos)->base, contaHeroi(*b), 
                            b->lotacao);
    //insere evento viaja(6) na LEF
    if(!(inicializarLef(&aux, tempo, 6, h.id, baseAleat)))
        return 0;
    inserirLef(m, listaEventos, aux);
    aux = NULL;
    //insere evento avisa(3) na LEF
    if(!(inicializarLef(&aux, tempo, 3, -1, b->id)))
        return 0;
    inserirLef(m, listaEventos, aux);
    return 1;
}

//heroi viaja e chega depois de duração de viajem
int eViaja(MUNDO *m, LEF **listaEventos, int tempo, HEROI h, BASE *d){
    LEF *aux = NULL;
    int distancia;
    int duracao;

    //distancia cartesiana
    distancia = DISTANCIA(m->bases[h.idBase].localBase.x, d->localBase.x, 
                        m->bases[h.idBase].localBase.y, d->localBase.y);
    
    duracao = distancia / h.velocidade;
    //insere evento chega(0) na LEF
    if(!(inicializarLef(&aux, tempo + duracao, 0, h.id, d->id)))
        return 0;
    inserirLef(m, listaEventos, aux);
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
                            (*listaEventos)->tempo, (*listaEventos)->heroi,
                            h.idBase, d->id, distancia, 
                            h.velocidade, tempo + duracao);
    return 1;
}

//checa a partir das bases mais proximas se são aptas a missão
int eMissao (MUNDO *m, LEF **listaEventos, MISSAO *mis, int tempo){
    lista *aux = NULL; //auxilia na alocacao
    lista *auxUniao = NULL; 
    lista *u = NULL; //uniao resultante
    LEF *auxLEF = NULL;

    printf("%6d: MISSAO %d HAB REQ: [ ", tempo, mis->id);
    imprimeLista(mis->habilidadesMissao);
    printf("]\n");
    //vai pelas bases mais proximas e ve se é possivel
    for(int i = 0; i < m->nBases; i++){
        printf("%6d: MISSAO %d HAB BASE %d: [ ", 
                            tempo, mis->id, mis->d[i].idBase);
        
        if(m->bases[mis->d[i].idBase].presentes != NULL)//se base nao for vazia
            auxUniao = m->bases[mis->d[i].idBase].presentes; //id do primeiro heroi
        else{
            printf("]\n");
            continue;
        }
        //constroi conjunto de habilidades
        while(auxUniao != NULL){
            uniao(m->herois[auxUniao->elem].habilidadesHeroi, u, &aux);
            freeLista(u);//libera uniao temporaria
            auxUniao = auxUniao->prox;
            u = aux;
            aux = NULL;
        }
        imprimeLista(u);
        printf("]\n");
        //se conjunto de habilididades necessarias estiver contido ganham XP
        if(contem(u, mis->habilidadesMissao)){
            freeLista(u);
            ganharXP(m ,m->bases[mis->d[i].idBase]);
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: [",
                            tempo, mis->id,
                            mis->d[i].idBase);
            imprimeLista(m->bases[mis->d[i].idBase].presentes);
            printf("]\n");
            return 1;
        }
        freeLista(u);
        u = NULL;
        aux = NULL;
    }
    //adia missao em 24 horas
    inicializarLef(&auxLEF, tempo + 1440, 7, mis->id, -1);
    inserirLef(m, listaEventos, auxLEF);
    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, mis->id);
    return 0;
}

//apresenta dados ao final de simulacao
void eFim(MUNDO *m, int tempo, int contador, int tentativas){
    float media = (float) contador / m->nMissoes * 100; //media de sucesso
    float mediaMissao = (float) tentativas / contador;//media de tentativas por missao

    printf("%6d: FIM\n", tempo);
    //dados de cada heroi
    for(int i = 0; i < m->nHerois; i++){
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS [ ",
                            m->herois[i].id, m->herois[i].paciencia,
                            m->herois[i].velocidade, m->herois[i].xp);
        imprimeLista(m->herois[i].habilidadesHeroi);
        printf("]\n");
    }
    //dados de missoes feitas
    printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
                            contador, m->nMissoes, media, mediaMissao);
    return;
}



