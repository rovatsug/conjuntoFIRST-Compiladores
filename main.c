#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITER 25

#define MAX_X 20
#define MAX_Y 20

#define MAX_ANT 10
#define MAX_CONSEQ 10

typedef struct block{
	char antecedente[MAX_ANT], consequente[MAX_CONSEQ];
} block;

int isTerminal(char x) {
	if('a' <= x && x <= 'z') return 1;
	return 0;
}

int isVoid(char x) {
	if(x == '$') return 1;
	return 0;
}

int isNotTerminal(int x) { 
	if('A' <= x && x <= 'Z') return 1;
	return 0;
}

int getIndice(block first[MAX_Y][MAX_X], char simboloNaoTerminal) {
	int indice = -1;
	for(int i = 0; i < MAX_Y; i++) {
		if(first[i][0].antecedente[0] == simboloNaoTerminal) indice = i;
	}
	return indice;
}

int HaTransicaoVazia(char simbolo, block table[MAX_Y][MAX_X]) {
	int indice = getIndice(table, simbolo);
	if(indice == -1) return 0;
	for(int k = 0; k < MAX_X; k++) if(table[indice][k].consequente[0] == '$') return 1;
	return 0;
}

void adicionaTerminalAoFirst(block first[MAX_Y][MAX_X], char simboloTerminal, char simboloNaoTerminal) {
	int indice = getIndice(first, simboloNaoTerminal);
	if(indice == -1 || !isTerminal(simboloTerminal) || !isNotTerminal(simboloNaoTerminal)) {
		printf("ERRO!\n"); return;	
	}
	for(int i = 0; i < MAX_X; i++) {
		if(first[indice][i].consequente[0] == simboloTerminal) return;
		if(first[indice][i].consequente[0] == '*') { 
			first[indice][i].consequente[0] = simboloTerminal;
			return;
		}
	}
}

void adicionaVazio(block first[MAX_Y][MAX_X], char naoTerminal) {
	int indice = getIndice(first, naoTerminal);
	if(indice == -1) return;
	for(int i = 0; i < MAX_X; i++) {
		if(first[indice][i].consequente[0] == '$') return;
		if(first[indice][i].consequente[0] == '*') {
			first[indice][i].consequente[0] = '$';
			break;
		}
	}
}

void inicialize(block table[MAX_Y][MAX_X]) {
	for(int i = 0; i < MAX_X; i++) {
		for(int j = 0; j < MAX_Y; j++) {
			table[i][j].antecedente[0] = table[i][j].consequente[0] = '*';
			table[i][j].antecedente[1] = table[i][j].consequente[1] = '\0';

		}
	}
}

void printTable(block table[MAX_Y][MAX_X]) {
	for(int i = 0; i < MAX_X; i++) {
		for(int j = 0; j < MAX_Y; j++) printf("%s=%s, ", table[i][j].antecedente, table[i][j].consequente);
		printf("\n");
	}	
}

void buildTable(block table[MAX_Y][MAX_X]) {
	char ant[MAX_ANT];
	char buffer = '$';
	ant[0] = '$';
	
	for(int i = 0; i < MAX_Y; i++) {
		for(int k = 0; k < MAX_ANT; k++) {
			scanf("%c", &buffer);
			if(buffer == '=') break;
			ant[k] = buffer;
			ant[k+1] = '\0';	
		}
		for(int j = 0; j < MAX_X; j++) {
			for(int k = 0; k < MAX_ANT && ant[k] != '\0'; k++) {
				table[i][j].antecedente[k] = ant[k];
				if(k+1 < MAX_ANT) table[i][j].antecedente[k+1] = '\0';
			}
			for(int k = 0; k < MAX_CONSEQ; k++) {
				scanf("%c", &buffer);
				if(buffer == '|' || buffer == '\n') break;
				if(buffer == ';') break;
				table[i][j].consequente[k] = buffer;
			}
			if(buffer == ';' || buffer == '\n') break;
		}
		if(buffer == ';') break;
	}
}

void inicializeFirst(block table[MAX_Y][MAX_X], block first[MAX_Y][MAX_X]) {
	for(int i = 0; i < MAX_Y; i++) {
		for(int j = 0; j < MAX_X; j++) {
			for(int k = 0; k < MAX_ANT; k++) {
				first[i][j].antecedente[k] = table[i][0].antecedente[k];
				if(k+1 < MAX_ANT) first[i][j].antecedente[k+1] = '\0'; 
				if(table[i][j].antecedente[k] == '\0') break;
			}
			first[i][j].consequente[0] = '*';
			first[i][j].consequente[1] = '\0';
		}
	}
}

void printFirst(block first[MAX_Y][MAX_X]) {
	for(int i = 0; i < MAX_Y; i++) {
		for(int j = 0; j < MAX_X; j++) {
			if(first[i][0].antecedente[0] == '*') break;
			if(j == 0 && first[i][j].antecedente[0] != '*') { 
				printf("FIRST(%s) = {", &first[i][j].antecedente);
			}
			if(first[i][j].consequente[0] != '*')  printf("%s", first[i][j].consequente);
			if(j+1 < MAX_CONSEQ && first[i][j+1].consequente[0] != '*' && first[i][j].consequente[0] != '*') printf(", ");
			if(j+1 == MAX_Y) printf("}\n");
		}
	}
}

int TemVazioNoFirst(char simbolo, block first[MAX_Y][MAX_X]) {
    int indice = getIndice(first, simbolo);
    if(indice == -1) return 0;
    for(int i = 0; i < MAX_X; i++) {
        if(first[indice][i].consequente[0] == '$') return 1;
        if(first[indice][i].consequente[0] == '*') break;
    }
    return 0;
}

void inicioTerminais(block table[MAX_Y][MAX_X], block first[MAX_Y][MAX_X]) {
	int boole = 1, exitt = 1;
	for(int i = 0; i < MAX_Y; i++) {
		for(int j = 0; j < MAX_X; j++) {
			if(isTerminal(table[i][j].consequente[0]) || isVoid(table[i][j].consequente[0])) {
				for(int k = 0; k < MAX_Y; k++) {
					if(first[i][k].consequente[0] == table[i][j].consequente[0]) boole = 0;
				}	
				if(boole) {
					for(int k = 0; k < MAX_X && exitt; k++) {
						if(first[i][k].consequente[0] == '*') {
							first[i][k].consequente[0] = table[i][j].consequente[0];
							first[i][k].consequente[1] = '\0';
							exitt = 0;
						}
					
					}
					exitt = 1;
				}
			}
			boole = 1;
		}
	}
}

void addFirstWithNoVoid(char antecedente, char consequente, block first[MAX_Y][MAX_X]) {
	int ant_indice = -1, conseq_indice = -1, naoHa = 1, position = -1;
	char x;
	for(int i = 0; i < MAX_Y; i++) {
		if(first[i][0].antecedente[0] == antecedente) ant_indice = i;	
		if(first[i][0].antecedente[0] == consequente) conseq_indice = i;
	}
	for(int i = 0; i < MAX_X; i++) {
		position = -1;
		naoHa = 1;
		x = first[conseq_indice][i].consequente[0];
		if(x == '*' || isVoid(x)) continue;
		//ja existe?
		for(int k = 0; k < MAX_Y; k++) {
			if(x == first[ant_indice][k].consequente[0]) {
				naoHa = 0;
				break;
			}
			if(position == -1 && first[ant_indice][k].consequente[0] == '*') {
				position = k;
			}
		}
		if(naoHa && position != -1) {
			first[ant_indice][position].consequente[0] = x;
                	first[ant_indice][position].consequente[1] = '\0';
		}
	}
}	

void inicioNaoTerminais(block table[MAX_Y][MAX_X], block first[MAX_Y][MAX_X]) {
   int podeSerVazio = 1; char ant, atual; 
   for(int i = 0; i < MAX_Y; i++) {
        ant = table[i][0].antecedente[0];
        if(ant == '*') continue;
        for(int j = 0; j < MAX_X; j++) {
            if(table[i][j].consequente[0] == '*') break;
            podeSerVazio = 1;
            for(int k = 0; table[i][j].consequente[k] != '\0'; k++) {
                atual = table[i][j].consequente[k];
                if(isTerminal(atual)) {
                    adicionaTerminalAoFirst(first, atual, ant);
                    podeSerVazio = 0;
                    break;
                } else if(isNotTerminal(atual)) {
                    addFirstWithNoVoid(ant, atual, first);
                    if(!TemVazioNoFirst(atual, first)) {
                        podeSerVazio = 0;
                        break;
                    }
                } else if(isVoid(atual)) break;
            }
            if(podeSerVazio) adicionaVazio(first, ant);
        }
    }
}

void buildingFirst(block table[MAX_Y][MAX_X], block first[MAX_Y][MAX_X]) {
	inicioTerminais(table, first);
	for(int i = 0; i < ITER; i++) inicioNaoTerminais(table, first);
}

void orderFirst(block first[MAX_Y][MAX_X]) {
	char aux1, aux2; int indice = -1, indice_final = -1;
	for(int k = 0; k < MAX_Y; k++)
	        for(int i = 0; i < MAX_X; i++)
			for(int j = i+1; j < MAX_X; j++) {
				if(first[k][i].consequente[0] > first[k][j].consequente[0]) {
					if(first[k][j].consequente[0] == '*') continue;
					aux1 = first[k][i].consequente[0];
					first[k][i].consequente[0] = first[k][j].consequente[0];
					first[k][j].consequente[0] = aux1;
				}
			}
}

int main() {
	block table[MAX_Y][MAX_X];
	block first[MAX_Y][MAX_X];
	inicialize(table);
	buildTable(table);
	inicializeFirst(table, first);
	buildingFirst(table, first);
	orderFirst(first);
	printFirst(first);
	return 0;
}
