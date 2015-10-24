#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define cross 60
#define mut 10


typedef struct dados {
    char label;
    int valor;
} Dados;

typedef struct individuo {
    Dados v[10];
    int aptidao;
} Individuo;


/*
    S E N D M O R Y || X || K ||
    3 8 2 1 0 4 6 9 || 5 || 7 ||
    Nota: 0
*/


/*
    S E N D M O R Y || X || K ||
    6 4 1 5 0 7 3 9 || 2 || 8 ||
    Nota: 0
*/
struct individuo avaliarIndivido(struct individuo individuo);
int avaliarPopulacao(struct individuo *individuo, int qtdI,int *quantidade,int geracao);
Individuo* crossPopulacao(Individuo *individuo, int qtdI, int selecao,int geracao);
void ordenaPopulacao(struct individuo *individuo, int qtdI);
void imprimirPopulacao(Individuo i[50], int qtdI);
struct individuo torneio3(struct individuo *individuo, int qtdI);
struct individuo mutacaoIndivido(struct individuo individuo);
Individuo* melhoresPaisFilhos(struct individuo *individuo, int tNovaPopulacao,int pf);
void imprimirIndividuo(Individuo *individuo,int geracao,int indice);
void escreverArquivo(char *nome,Individuo individuo,int indice,int geracao);



int main() {

    char p1[10], p2[10], p3[11], labelTemp;
    int i, j, k, aux = 0, tamA = 0, tamB = 0, tamC = 0, qtdeI;
    int n, somaA, somaB, somaC, res, melhor, ex;
    int p = 0;
    int geracao = 0;
    int quantidade;
    int atingiu=0;

    float porcentagem, quantidadeI = 0,tamanho = 50000;

    Dados v[10];


    srand(time(NULL));

    printf("**************************\n");
    printf("**** Trabalho de IC ******\n");
    printf("**************************\n");
    printf("Aluno: Renan Carlos Pereira de Moraes\n\n");

    printf("Palavra1: ");
    gets(p1);
    tamA = strlen(p1);

    printf("Palavra2: ");
    gets(p2);
    tamB = strlen(p2);

    printf("Palavra3: ");
    scanf("%s", &p3);
    tamC = strlen(p3);

    printf("\nQTDE de Individuos: ");
    scanf("%d", &qtdeI);

    Individuo  *populacao;

    populacao = (Individuo*) malloc (qtdeI*(sizeof(Individuo)));


    printf("\n\nTamanho da populacao: %d\nTaxa de CrossOver: %d\nTaxa de Mutacao: %d", qtdeI, cross, mut);

    printf("\nFazendo o calculo de %s + %s = %s\n", p1, p2, p3);

    for (i = 1; i < 10; i++) {
        //Sleep(1000);
        printf(".");
        fflush(stdout);
    }


    int qtdeIndCross = (cross * qtdeI) / 100;


    printf("\n\n");

    for (i = 0; i < tamA + tamB + tamC; i++) {
        if (i >= 0 && i < tamA)
            labelTemp = p1[i];
        else if (i >= tamA && i < tamA + tamB)
            labelTemp = p2[i - tamA];
        else if (i >= tamA + tamB && i <= tamA + tamB + tamC)
            labelTemp = p3[i - tamA - tamB];

        for (j = 0; j < aux; j++) {
            if (v[j].label == labelTemp) {
                aux--;
                break;
            }
        }
        v[j].label = labelTemp;
        aux++;
    }
    v[8].label = 'X';
    v[9].label = 'K';


    while (p < qtdeI) {
        for (i = 0; i < 8; i++) {
            populacao[p].v[i].label = v[i].label;
        }
        populacao[p].v[8].label = 'X';
        populacao[p].v[9].label = 'K';
        p++;
    }

    do {



        for (i = 0; i < qtdeI; i++) { // GERAÇÃO SINGULAR

            for (j = 0; j < 10; j++) {
                n = rand() % 10;

                somaA = 0;
                somaB = 0;
                somaC = 0;


                for (k = 0; k < j; k++) {
                    if (v[k].valor == n)
                        break;
                }
                if (k == j) {
                    v[k].valor = n;
                    populacao[i].v[j].valor = n;
                }
                else j--;


            }

            for (j = 0; j < tamA + tamB + tamC; j++) {

                if (j >= 0 && j < tamA) {
                    for (k = 0; k < aux; k++) {
                        if (p1[j] == v[k].label) {
                            somaA = 10 * somaA + v[k].valor;
                            break;
                        }
                    }
                }
                else if (j >= tamA && j < tamA + tamB) {
                    for (k = 0; k < aux; k++) {
                        if (p2[j - tamA] == v[k].label) {
                            somaB = 10 * somaB + v[k].valor;
                            break;
                        }
                    }
                }
                else if (j >= tamA + tamB && j < tamA + tamB + tamC) {
                    for (k = 0; k < aux; k++) {
                        if (p3[j - tamA - tamB] == v[k].label) {
                            somaC = 10 * somaC + v[k].valor;
                            break;
                        }
                    }
                }
            }

            res = abs((somaA + somaB) - somaC);
            populacao[i].aptidao = res;


            if (res == 0) {
                melhor = res;
                ex = i + 1;
                break;
            } else if (i == 0 && res > 0) {
                melhor = res;
                ex = 1;
            } else if (i == 0 && res < 0) {
                melhor = -1 * res;
                ex = 1;
            } else if (res < melhor && res > 0) {
                melhor = res;
                ex = i + 1;
            } else if (res > -1 * melhor && res < 0) {
                melhor = -1 * res;
                ex = i + 1;
            }

        }

        printf("\n********** Geracao: %d ************ \n", geracao + 1);

        imprimirPopulacao(populacao,qtdeI);


        atingiu = avaliarPopulacao(populacao, qtdeI,&quantidade,geracao);


        if (atingiu == 0)  // Não atingiu o criterio de termino
        {
            populacao = crossPopulacao(populacao, qtdeI, qtdeIndCross,geracao);
        }
        else {
            quantidadeI = quantidadeI + quantidade;

        }
        geracao++;
    } while (geracao < 1000);

    porcentagem = (quantidadeI/tamanho) * 100;

    printf("\nDepois de 1000 Geracoes,tem se: %.1f acertos... porcentagem de: %.5f%%",quantidadeI,porcentagem);



    printf("\n\n");
    system("pause");

    return 0;
}

void ordenaPopulacao(struct individuo *individuo, int qtdI) {

    int i, j;
    struct individuo aux;


    for (i = 1; i < qtdI; i++) {
        aux = individuo[i];

        for (j = i; (j > 0) && (aux.aptidao < individuo[j - 1].aptidao); j--)
            individuo[j] = individuo[j - 1];

        individuo[j] = aux;
    }
}


int avaliarPopulacao(struct individuo *individuo, int qtdI,int *quantidade,int geracao) {

    int i, atingiu = 0,k=0;

    for (i = 0; i < qtdI; i++) {
        if (individuo[i].aptidao == 0) {
            k++;
            escreverArquivo("resultados.txt",individuo[i],i,geracao);
            atingiu = 1;
        }
    }
    *quantidade = k;
    return atingiu;
}




void imprimirPopulacao(Individuo individuo[50], int qtdI) {

    int j, k;

    for (k = 0; k < qtdI; k++) {
        printf("\n");
        printf("********* Individuo: %d ***********\n", k + 1);

        for (j = 0; j < 10; j++) {
            if (j < 8) {
                printf("%c ", individuo[k].v[j].label);
            }
            else if (j >= 8)
                printf("|| %c ", individuo[k].v[j].label);
        }

        printf("||\n");

        for (j = 0; j < 10; j++) {
            if (j < 8) {
                printf("%d ", individuo[k].v[j].valor);
            }
            else if (j >= 8)
                printf("|| %d ", individuo[k].v[j].valor);
        }


        printf("||\n");
        printf("Nota: %d", individuo[k].aptidao);
        printf("\n");

    }

}


Individuo* crossPopulacao(Individuo *individuo, int qtdI, int selecao,int geracao) {

    int tNovaPopulacao = qtdI+selecao;
    Individuo *populacao;
    Individuo *populacaoRetornada;
    Individuo *pais;
    Individuo *filhos;

    populacaoRetornada = (Individuo*) malloc (qtdI*(sizeof(Individuo)));
    populacao = (Individuo*) malloc (tNovaPopulacao*(sizeof(Individuo)));
    pais = (Individuo*) malloc (selecao*(sizeof(Individuo)));
    filhos = (Individuo*) malloc (selecao*(sizeof(Individuo)));

    int i, j, aux, aux2, k, w = 0;
    int pmxI = 0;
    int pmxF = 3;
    int mutI = (mut * selecao) / 100;


    // Seleciona os Pais
    for (i = 0; i < selecao; i++) {
        pais[i] = torneio3(individuo, qtdI);
        escreverArquivo("pais.txt",pais[i],i,geracao);
        filhos[i] = pais[i];

    }
    // CrossOver PMX
    for (i = 0; i < (selecao / 2); i = i + 2) {
        for (j = pmxI; j < pmxF; j++) {
            aux = filhos[i].v[j].valor;
            aux2 = filhos[i + 1].v[j].valor;

            filhos[i].v[j].valor = aux2;
            filhos[i + 1].v[j].valor = aux;

            for (k = 3; k < 8; k++) {
                if (filhos[i].v[k].valor == aux2) {
                    filhos[i].v[k].valor = aux;
                }
            }

            for (k = 3; k < 8; k++) {
                if (filhos[i + 1].v[k].valor == aux) {
                    filhos[i + 1].v[k].valor = aux2;
                }
            }

        }
    }



    for (i = 0; i < selecao; i++) {
        filhos[i] = avaliarIndivido(filhos[i]);
        escreverArquivo("filhos.txt",filhos[i],i,geracao);
    }

    // Mutação dos Filhos
    for(i=0;i<mutI;i++) {
        filhos[i] = mutacaoIndivido(filhos[i]);
        filhos[i] = avaliarIndivido(filhos[i]);
    }



    for(i=0;i<tNovaPopulacao;i++){
        if(i < qtdI){
            populacao[i] = individuo[i];
        }
        else {
            populacao[i] = filhos[w];
            w++;
        }
    }



    for (i = 0; i < tNovaPopulacao; i++) {
        populacao[i] = avaliarIndivido(populacao[i]);
    }


    ordenaPopulacao(populacao,tNovaPopulacao);

    for(i=0;i<qtdI;i++){
          populacaoRetornada[i] = populacao[i];
          escreverArquivo("novaPopulacao.txt",populacaoRetornada[i],i,geracao);
    }



    return populacaoRetornada;


}

struct individuo torneio3(struct individuo *individuo, int qtdI) {

    int n1, n2, n3;

    n1 = rand() % qtdI;
    do {
        n2 = rand() % qtdI;
    } while (n2 == n1);
    do {
        n3 = rand() % qtdI;
    } while ((n3 == n1) || (n3 == n2));


    if ((individuo[n1].aptidao <= individuo[n2].aptidao) && (individuo[n1].aptidao <= individuo[n3].aptidao)) {
        return individuo[n1];
    }

    else if ((individuo[n2].aptidao <= individuo[n1].aptidao) && (individuo[n2].aptidao <= individuo[n3].aptidao)) {
        return individuo[n2];
    }

    else {
        return individuo[n3];
    }


}

struct individuo mutacaoIndivido(struct individuo individuo) {

    int x, y;
    int aux, aux2;

    x = rand() % 5;
    y = x + 2;

    aux = individuo.v[x].valor;
    aux2 = individuo.v[y].valor;

    individuo.v[x].valor = aux2;
    individuo.v[y].valor = aux;

    return individuo;

}


struct individuo avaliarIndivido(struct individuo individuo) {

    int j, k, aux = 8;
    int res;
    int somaA, somaB, somaC;
    int tamA = 4, tamB = 4, tamC = 5;

    char p1[] = "SEND";
    char p2[] = "MORE";
    char p3[] = "MONEY";


    somaA = 0;
    somaB = 0;
    somaC = 0;

    for (j = 0; j < tamA + tamB + tamC; j++) {

        if (j >= 0 && j < tamA) {
            for (k = 0; k < aux; k++) {
                if (p1[j] == individuo.v[k].label) {
                    somaA = 10 * somaA + individuo.v[k].valor;
                    break;
                }
            }
        }
        else if (j >= tamA && j < tamA + tamB) {
            for (k = 0; k < aux; k++) {
                if (p2[j - tamA] == individuo.v[k].label) {
                    somaB = 10 * somaB + individuo.v[k].valor;
                    break;
                }
            }
        }
        else if (j >= tamA + tamB && j < tamA + tamB + tamC) {
            for (k = 0; k < aux; k++) {
                if (p3[j - tamA - tamB] == individuo.v[k].label) {
                    somaC = 10 * somaC + individuo.v[k].valor;
                    break;
                }
            }
        }
    }

    res = abs((somaA + somaB) - somaC);
    individuo.aptidao = res;

    return individuo;


}

Individuo* melhoresPaisFilhos(struct individuo *individuo, int tNovaPopulacao,int pf){

    struct individuo p[tNovaPopulacao];
    int i;

    ordenaPopulacao(individuo,pf);

    for(i=0;i<tNovaPopulacao;i++){

        p[i] = individuo[i];
    }

    return p;



}


void imprimirIndividuo(Individuo *individuo,int geracao,int indice) {

    int j, k;


    printf("\n");
    printf("\n********* Geracao: %d ***********\n", geracao+1);
    printf("********* Individuo: %d ***********\n", indice);

    for (j = 0; j < 10; j++) {
        if (j < 8) {
            printf("%c ", individuo[indice].v[j].label);
        }
        else if (j >= 8)
            printf("|| %c ", individuo[indice].v[j].label);
    }

    printf("||\n");

    for (j = 0; j < 10; j++) {
        if (j < 8) {
            printf("%d ", individuo[indice].v[j].valor);
        }
        else if (j >= 8)
            printf("|| %d ", individuo[indice].v[j].valor);
    }


    printf("||\n");
    printf("Nota: %d", individuo[indice].aptidao);
    printf("\n");

}

void escreverArquivo(char *nome,Individuo individuo,int indice,int geracao){


    FILE *arq;
    int j;

    arq = fopen(nome,"a");

    if(arq == NULL)
        printf("Erro, nao foi possivel abrir o arquivo\n");
    else

        fprintf(arq,"\n********* Geracao: %d ***********\n", geracao+1);
        fprintf(arq,"********* Individuo: %d ***********\n", indice);

    for (j = 0; j < 10; j++) {
        if (j < 8) {
            fprintf(arq,"%c ", individuo.v[j].label);
        }
        else if (j >= 8)
            fprintf(arq,"|| %c ", individuo.v[j].label);
    }

    fprintf(arq,"||\n");

    for (j = 0; j < 10; j++) {
        if (j < 8) {
            fprintf(arq,"%d ", individuo.v[j].valor);
        }
        else if (j >= 8)
            fprintf(arq,"|| %d ", individuo.v[j].valor);
    }


    fprintf(arq,"||\n");
    fprintf(arq,"Nota: %d", individuo.aptidao);
    fprintf(arq,"\n");


    fclose(arq);


}
