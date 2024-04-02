#include "theboys.h"

/*
Essa biblioteca possui funcoes relacionadas a base, que é
- Fila da Base 
- Conjunto de presentes
*/

//conta quantos herois estao presentes na base
int contaHeroi(BASE b){
    lista *aux = b.presentes;
    int contador = 0;

    while(aux != NULL){
        aux = aux->prox;
        contador++;
    }
    return contador;
}

//checa se base esta cheia
int baseCheia(BASE b){
    int contador = 0;

    contador = contaHeroi(b);
    if(contador >= b.lotacao)//1 se lotado
        return 1;
    return 0;
}

//remove heroi do conjunto de presentes na base
int removerHeroi(BASE *b, HEROI h){
    lista *aux = b->presentes;
    lista *freeAux;   
    int idHeroi = h.id;

    //se heroi estar no comeco da lista
    if(aux->elem == idHeroi){
        b->presentes = b->presentes->prox;
        free(aux);
        return 1;
    }
    while((aux->prox != NULL) && (aux->prox->elem != idHeroi))
        aux = aux->prox;
    if(aux->prox != NULL){//id do heroi esta em aux->prox
        freeAux = aux->prox;
        aux->prox = aux->prox->prox;
        free(freeAux);
        return 1;
    }
    return 0;//se heroi nao estiver na lista
}

void imprimeFila(BASE b){
	//escreve a fila
	for (int i = 0; i < b.espera.fim; i++)
		printf(" %2d ",b.espera.f[i]);
	return;
}

int vaziaFila(BASE b){
	//testa se vazia
	if (b.espera.fim == 0)
		return 1;
	return 0;
}

int cheiaFila(BASE b){
	//testa se cheia
	if (b.espera.fim >= b.espera.tam)
		return 1;
	return 0;
}

//move a fila 
void move_left(struct fila *espera){	
	//move a fila para esquerda
	for (int i = 1; i < espera->fim; i++)
		espera->f[i-1] = espera->f[i];
	return;
}

int enfileiraBase(BASE *b, HEROI h){
	//enfileira se possivel
	if (!(cheiaFila(*b))){
		b->espera.f[b->espera.fim] = h.id;
		b->espera.fim++;
		return 1;
	}
	return 0;
}

int desinfileiraBase(BASE *b, int *heroi){
	//desinfileira se nao vazia
	if (!(vaziaFila(*b))){
		*heroi = b->espera.f[0];
		move_left(&b->espera);
		b->espera.fim--;
		return 1;
	}
	return 0;
}
	

