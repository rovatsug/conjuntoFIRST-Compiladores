#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITER 10

typedef struct nodo {
	char *x;
	struct nodo *prox;
} nodo;

void printLista(nodo *n) {
	if(n == NULL || n->x == NULL) return;
	printf("%s ", n->x);
	printLista(n->prox);
}

void destroiLista(nodo *n) {
	if(n == NULL) return;
	destroiLista(n->prox);
	if(n->x) free(n->x);
	free(n);
}

void push(nodo *n, char *buffer) {
	nodo *new;
	if(n->x == NULL) {
		n->x = strdup(buffer);
		n->prox = NULL;
		return;
	}
	if(!strcmp(n->x, buffer)) return;
	if(n->prox == NULL) {
		nodo *novo = (nodo *)malloc(sizeof(nodo));
		novo->x = strdup(buffer);
		novo->prox = NULL;
		n->prox = novo;
	} else push(n->prox, buffer);
}

int main() {

	int qtde_terminais, qtde_nao_terminais;
	char buffer[51], espaco;
	char **antecedente; nodo *consequente;

	scanf(" %d %d", &qtde_terminais, &qtde_nao_terminais);
	antecedente = (char **)malloc(qtde_nao_terminais*sizeof(char *));
	consequente = (nodo *)malloc(qtde_nao_terminais*sizeof(nodo));

	for(int i = 0; i < qtde_nao_terminais; i++) {
		
		antecedente[i] = (char *)malloc(50*sizeof(char));
		consequente[i].x = NULL;
		consequente[i].prox = NULL;
	}

	for (int i = 0; i < qtde_nao_terminais; i++) {
    	scanf("%s", antecedente[i]); 
    	scanf("%s", buffer);

		for(int j = 0; j < 50 && buffer[j] != ' '; j++) {
			scanf("%c", &buffer[j]);
			if(buffer[i] == ' ') { 
				buffer[i] = '\0';
			}
			push(&consequente[i], buffer);
		}
	    /*do {
        	if (scanf("%50s%c", buffer, &espaco) != 2) break;
        	push(&consequente[i], buffer);
    	} while (espaco != '\n');*/
	}

	for (int i = 0; i < qtde_nao_terminais; i++) {
    	printf("%s", antecedente[i]); 
    	printf(" = "); 
	    printLista(&consequente[i]);
		printf("\n");
	}

	for(int i = 0; i < qtde_nao_terminais; i++) free(antecedente[i]);
    free(antecedente);

    for(int i = 0; i < qtde_nao_terminais; i++) {
        if (consequente[i].x) free(consequente[i].x);
        destroiLista(consequente[i].prox);           
    }

    free(consequente);
	

	return 0;
}
