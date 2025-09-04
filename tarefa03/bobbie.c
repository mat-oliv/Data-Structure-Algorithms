#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct goals {
    int *v;
    int qtd;
    char name[20];
};

void liberar(struct goals *p) {
    free(p);

}

int sum(struct goals *p) {
    int r = 0;
    for (int i = 0; i < p->qtd; i++) {
        r += p->v[i];
    }
    return r;
}

int main() {
    char name[20];
    struct goals *players[2];
    players[0] = malloc(sizeof(struct goals));

    // Player 0
    scanf("%s", name);
    strcpy(players[0]->name, name);
    players[0]->v = malloc(3 * sizeof(int));
    for (int i = 0; i < 3; i++) {
        scanf("%d", &players[0]->v[i]);
    }

    // Player 1:
    scanf("%s", name);
    players[1] = malloc(sizeof(struct goals));
    strcpy(players[1]->name, name);
    players[0]-> qtd = 3;
    players[1]->qtd = 3;
    // Dados do player 1 são iguais aos dados do player 0
    players[1]->v = players[0]->v;

    if (players[0]->qtd > 3 || players[1]->qtd > 3) {
        printf("Melhor conferir esse Morango do Amor!\n");
    }

    printf("Placar inicial: %d vs %d\n", sum(players[0]), sum(players[1]));

    free(players[0]->v); 
    liberar(players[0]);
    liberar(players[1]);

    

    return 0;
}
