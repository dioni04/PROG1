#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct coordenada{
    int x;
    int y;
};

//lista usada para habilidades e presentes na base
typedef struct lista{
    int elem;
    struct lista *prox;
}lista;

struct fila{
    int *f;
    int fim;
    int tam;
};

//vetor de distancias relativas de missao a bases
struct distancia{
    int dist;
    int idBase;
};

/*
Documentação dos eventos
0 - Chega(tempo, heroi, base)
1 - Espera(tempo, heroi, base)
2 - Desiste(tempo, heroi base)
3 - Avisa(tempo, base)
4 - Entra(tempo, heroi, base)
5 - Sai(tempo + tpb, heroi, base)
6 - Viaja(tempo, heroi, base)
7 - Missão(tempo, missao)
8 - Fim(tempo, contador, tentativas)
*/

//se algum dado estiver -1 ele é considerado nulo
typedef struct lef{
    int tempo;//horario do evento
    int evento;//evento
    int heroi;//heroi relacionado OU id de missao
    int base;//base relacionada
    struct lef *prox;
}LEF;

typedef struct missao{
    int id;  
    struct lista *habilidadesMissao;//habilidades necessarias missao
    struct coordenada local;//local da missao
    struct distancia *d;
}MISSAO;

typedef struct heroi{
    int id;
    struct lista *habilidadesHeroi;//lista de habilidades do heroi
    int paciencia;
    int velocidade;
    int xp;//num de missoes completas
    int idBase;//id da base atual
}HEROI;

typedef struct base{
    int id;
    struct lista *presentes;//conjuntos de ids dos presentes na base
    int lotacao;//maximo suportado na base
    struct fila espera;//vetor da fila
    struct coordenada localBase;//coordenadas x,y
}BASE;

typedef struct mundo{
    int nHerois;//num de herois
    HEROI *herois;//vetor herois
    int nBases;//num de bases
    BASE *bases;//vetor bases
    int nMissoes;//num de missoes
    MISSAO *missoes;//vetor missoes
    int nHabildades; //num de habilidades distintas possiveis
    int tamanhoMundo;//maior coordenada do mundo
    int relogio;//tempo
}MUNDO;