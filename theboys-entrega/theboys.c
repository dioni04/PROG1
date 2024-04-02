#include "theboys.h"
#include "conjuntos.h"
#include "eventos.h"
#include "base.h"

int main(){
    srand(time(0));
    
    MUNDO m;
   
    LEF *listaEventos = NULL;//ponteiro para comeco da LEF
    LEF *auxLEF = NULL;

    lista *auxLista = NULL;
    
    int tempo;
    int contador = 0;
    int tentativas = 0;
    int tFimDoMundo = 525600; //tempo final
    
    /*INICIALIZAÇÃO DO MUNDO*/
    m.relogio = 0; //tempo incial
    
    m.tamanhoMundo = 20000;

    m.nHabildades = 10; //num de habiliades distintas
    m.nHerois = m.nHabildades * 5; //num de herois
    m.nBases = m.nHerois / 6; //num de bases
    m.nMissoes =  tFimDoMundo / 100;

    /*INICIALIZAÇÃO DE HEROIS*/
    //alloc dos herois
    if((m.herois = (HEROI*) malloc((sizeof(HEROI) * m.nHerois))) == NULL){
        printf("Falha de Alocação de Heróis\n");
        return 1;
    }
    for(int i = 0; i < m.nHerois; i++){
        int tamHabilidades = (rand() % 3) + 1; //quantas habilidades o heroi tem

        m.herois[i].id = i;
        m.herois[i].xp = 0;
        m.herois[i].paciencia = rand() % 101;
        m.herois[i].velocidade = (rand() % 4951) + 50;//metros por min
        m.herois[i].habilidadesHeroi = NULL;

        //habilidades do heroi
        while(tamHabilidades > 0){
            int temp = rand() % m.nHabildades;

            //habilidades distintas
            if(!(existe(m.herois[i].habilidadesHeroi, temp))){
                if(m.herois[i].habilidadesHeroi == NULL){
                    //inicializa habilidades com num de nhabil.
                    if(!(inicializarLista(&m.herois[i].habilidadesHeroi, temp))){
                        printf("Falha na Alocação de Habilidades Heroi");
                        return 1;
                    }
                }
                else{
                    if(!(inicializarLista(&auxLista, temp))){
                        printf("Falha na Alocação de Habilidades Heroi");
                    return 1;
                    }
                inserirLista(&m.herois[i].habilidadesHeroi, auxLista);
                auxLista = NULL;
                }
            tamHabilidades--;
            }
        }
    }

    /*INICIALIZAÇÃO DE BASES*/
    //alloc de vetor de bases
    if((m.bases = (BASE*) malloc((sizeof(BASE) * m.nBases))) == NULL){
        printf("Falha de Alocação de Bases\n");
        return 1;
    }
    for(int i = 0; i < m.nBases; i++){
        m.bases[i].id = i;
        m.bases[i].localBase.x = rand() % (m.tamanhoMundo);
        m.bases[i].localBase.y = rand() % (m.tamanhoMundo);
        m.bases[i].lotacao = (rand() % 8) + 3; 
        m.bases[i].presentes = NULL;
        m.bases[i].espera.fim = 0;
        m.bases[i].espera.tam = m.nHerois;
        //alocacao do vetor da fila de espera 
        if((m.bases[i].espera.f = (int*) malloc((sizeof(int) * m.nHerois))) == NULL){
            printf("Falha de Alocação de espera da Base\n");
            return 1;
        }
        //inicializacao da espera da base
        for (int j = 0; j < m.nHerois; j++)
            m.bases[i].espera.f[j] = 0;
    }

    auxLista = NULL;

    //alloc missoes
    if((m.missoes = (MISSAO*) malloc((sizeof(MISSAO) * m.nMissoes))) == NULL){
        printf("Falha de Alocação de Missoes\n");
        return 1;
    }
    for(int i = 0; i < m.nMissoes; i++){
        int tamHabilidades = (rand() % 5) + 6; //quantas habilidades a missao precisa
        
        m.missoes[i].id = i;
        m.missoes[i].local.x = rand() % (m.tamanhoMundo);
        m.missoes[i].local.y = rand() % (m.tamanhoMundo);
        m.missoes[i].habilidadesMissao = NULL;
        
        //alloc de vetor de distancias
        if((m.missoes[i].d = (struct distancia*) malloc(sizeof(struct distancia) * m.nBases)) == NULL){
            printf("Falha de Alocação de vetor de distancias\n");
            return 1;
        }
        //calcula distancias relativa as bases da missao e inicializa structure de distancia
        distanciasRelativas(&m, &m.missoes[i]);
        //ordena vetor de distancias as bases
        selectionSort(m.missoes[i].d, 0, m.nBases);
        
        //criacao de listas de habilidades necessarias da missao
        while(tamHabilidades > 0){
            int temp = rand() % m.nHabildades;

            //nao coloco numeros repetidos
            if(!(existe(m.missoes[i].habilidadesMissao, temp))){
                if(m.missoes[i].habilidadesMissao == NULL){
                    //inicializa habilidades com num de nhabil.
                    if(!(inicializarLista(&m.missoes[i].habilidadesMissao, temp))){
                        printf("Falha na Alocação de Habilidades Missao");
                        return 1;
                    }
                }
                else{
                    if(!(inicializarLista(&auxLista, temp))){
                        printf("Falha na Alocação de Habilidades Missao");
                        return 1;
                    }
                    inserirLista(&m.missoes[i].habilidadesMissao, auxLista);
                    auxLista = NULL;
                }
            tamHabilidades--;
            }
        }
    }


    /* INICIALIZACAO DE EVENTOS INICIAIS*/
    //herois e suas chegadas
    for(int i = 0; i < m.nHerois; i++){
        m.herois[i].idBase = rand() % m.nBases; //base inicial do heroi
        tempo = rand() % 4321; //entre 0 a 3 dias
        if(listaEventos == NULL)//Eventos chega
            inicializarLef(&listaEventos, tempo, 0, i, m.herois[i].idBase);
        else{
            inicializarLef(&auxLEF, tempo, 0, i, m.herois[i].idBase);
            inserirLef(&m ,&listaEventos, auxLEF);
        }
    }   
    
    //missoes
    for(int i = 0; i < m.nMissoes; i++){
        tempo = rand() % (tFimDoMundo + 1);//horario da missao
        //insere evento missao(7) em LEF
        inicializarLef(&auxLEF, tempo, 7, m.missoes[i].id, 1);
        inserirLef(&m, &listaEventos, auxLEF);
    }
    //insere fim do mundo(8) na LEF
    inicializarLef(&auxLEF, tFimDoMundo, 8, -1, -1);//Evento FIM
    inserirLef(&m, &listaEventos, auxLEF);

    /*SIMULACAO*/
    auxLEF = listaEventos;
    do{
        switch (listaEventos->evento){
        case 0://CHEGA
            if(!(eChega(&m, &listaEventos, listaEventos->tempo,
                        &m.herois[listaEventos->heroi], 
                        &m.bases[listaEventos->base]))){
                printf("Erro de Alocação em evento CHEGA");
                return 1;
            }
            break;
        case 1://ESPERA
            if(!(eEspera(&m, &listaEventos, listaEventos->tempo,
                        m.herois[listaEventos->heroi],
                        &m.bases[listaEventos->base]))){
                printf("Erro de Alocação em evento ESPERA");
                return 1;
            }
            break;
        case 2://DESISTE
            if(!(eDesiste(&m, &listaEventos, listaEventos->tempo,
                        m.herois[listaEventos->heroi],
                        &m.bases[listaEventos->base]))){
                printf("Erro de Alocação em evento DESISTE");
                return 1;
            }

            break;
        case 3://AVISA
            if(!(eAvisa(&m, &listaEventos, listaEventos->tempo,
                        &m.bases[listaEventos->base]))){
                printf("Erro de Alocação em evento AVISA");
                return 1;
            }
            break;
        case 4://ENTRA
            if(!(eEntra(&m, &listaEventos, listaEventos->tempo,
                        m.herois[listaEventos->heroi],
                        &m.bases[listaEventos->base]))){
                printf("Erro de Alocação em evento ENTRA");
                return 1;
            }
            break;
        case 5://SAI
            if(!eSai(&m, &listaEventos, listaEventos->tempo,
                        m.herois[listaEventos->heroi],
                        &m.bases[listaEventos->base])){
                printf("Erro de Alocação em evento SAI");
                return 1;
            }
            break;
        case 6://VIAJA
            if(!(eViaja(&m, &listaEventos, listaEventos->tempo,
                        m.herois[listaEventos->heroi],
                        &m.bases[listaEventos->base]))){
                printf("Erro de Alocação em evento VIAJA");
                return 1;     
            }
            break;
        case 7://MISSAO
            tentativas++;
            if(eMissao(&m, &listaEventos, &m.missoes[listaEventos->heroi],
                        listaEventos->tempo))
                contador++;//se for sucesso conta +1
            break;
        case 8://FIM
            eFim(&m, listaEventos->tempo, contador, tentativas);
            //fim = 1;
            break;
        default:
            break;
        }
        m.relogio = listaEventos->tempo;
        listaEventos = listaEventos->prox;
    } while (m.relogio != tFimDoMundo);
    
    /*FREE DE HEAPS DO PROGAMA*/

    //HEROIS
    for(int i = 0; i < m.nHerois; i++)
        freeLista(m.herois[i].habilidadesHeroi);
    free(m.herois);

    //MISSOES
    for(int i = 0; i < m.nMissoes; i++){
        freeLista(m.missoes[i].habilidadesMissao);
        free(m.missoes[i].d);
    }
    free(m.missoes);

    //BASES
    for(int i = 0; i < m.nBases; i++){
        free(m.bases[i].espera.f);
        freeLista(m.bases[i].presentes);
    }
    free(m.bases);

    //LEF
    freeLEF(&auxLEF);
    return 0;
    /*FIM DA SIMULACAO*/
}
