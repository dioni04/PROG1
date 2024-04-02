#include "theboys.h"
#include "conjuntos.h"

/*
Essa biblioteca possui funcoes relacionadas aos conjuntos 
para realização de missoes 
- Uniao
- Interseccao
- Igual 
- Contem
Junto com funções de manipulação de listas Encadeadas
*/

void imprimeLista(lista *l){
    lista *aux;
    
    aux = l;
    while (aux != NULL){
        printf("%d ",aux->elem);
        aux = aux->prox;
    }
    return;
}

//funcao de liberar memoria de conjuntos
void freeLista(lista *l){
    lista *aux;

    while (l != NULL){ 
        //libera l
        aux = l;
        l = l->prox;
        free(aux);
    }
}

int inicializarLista(lista **l, int num){
    //inicializa a lista com num no elem
	lista *aux;
	aux = *l;
	if((aux = (lista*)malloc(sizeof(lista))) == NULL)
		return 0;
    aux->elem = num;
	aux->prox = NULL;
	*l = aux;
	return 1;
}

int inserirLista(lista **l, lista *x){
    //insere x no final da lista l
	lista *aux;

	aux = *l;
	while ((*l)->prox != NULL)
        (*l) = (*l)->prox;
    (*l)->prox = x;
	*l = aux;
    return 1;
}

int existe(lista *l, int num){
    //busca num na lista
	lista *aux;

	aux = l;
	while(aux != NULL){
		if (aux->elem == num)
			return 1;
        aux = aux->prox;
    }
	return 0;
}

int uniao(lista *l1, lista *l2, lista **u){
    lista *aux;
    lista *cabeca;

    if(!(inicializarLista(u,l1->elem))){
        printf("Falha na Alocação de União");
        return 0;
    }
    cabeca = *u;
    l1 = l1->prox;
    //feito para conseguir referencia da cabeca e rodar a funcao existe
    while(l1 != NULL){
        if (!(existe(cabeca,l1->elem))){
                inicializarLista(&aux, l1->elem);
                inserirLista(u, aux);
                //insere na uniao um node com elem se nao houver ainda em u
            }
        l1 = l1->prox;
    }
    while(l2 != NULL){
        if (!(existe(cabeca,l2->elem))){
            inicializarLista(&aux, l2->elem);
            inserirLista(u, aux);
            //insere na uniao um node com elem se nao houver ainda em u
        }
        l2 = l2->prox;
    }
    return 1;
}

int interseccao(lista *l1, lista *l2, lista **in){
    lista *aux;
    
    *in = NULL;
    while(l1 != NULL){
        if (existe(l2, l1->elem)){ 
            //tenta achar em l2 os numeros de l1
            if(*in == NULL)
                inicializarLista(in, l1->elem);
            else{
                inicializarLista(&aux, l1->elem);
                inserirLista(in, aux);
                //insere na interseccao um node com elem
            }
        }
        l1 = l1->prox;
    }
    if (*in != NULL)
        //funcionou
        return 1;
    return 0;
}

int igual(lista *l1, lista *l2){
    lista *aux;

    aux = l1;
    while((l1 != NULL) && (l2 != NULL)){
        if (!(existe(aux,l2->elem)))//se nao houver em l1 nao sao iguais
            return 0;
        l1 = l1->prox;
        l2 = l2->prox;
    }
    //se um deles nao for NULL eles sao diferentes
    if (l1 != l2)
        return 0;
    return 1;
}

int contem(lista *l1, lista *l2){
    lista *aux;//usado para inserccao 
    
    interseccao(l1, l2, &aux);
    if (igual (aux, l2)){//se a interseccao for igual a l2 ela esta contida
        freeLista(aux);//free aqui se nao teria memory leak
        return 1;
    }
    freeLista(aux);  
    return 0;
 }    
