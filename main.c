#include <stdio.h>
#include <conio.h>

#define LINHAS  10
#define COLUNAS 10
#define limpar_tela() printf("\033[H\033[J")

char mapa[LINHAS][COLUNAS] = {
    {'*','*','*','*','*','*','*','*','*','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ','*','*',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*','*','*','*','*','*','*','*','*','*'}
};

int jogador_linha  = 1;
int jogador_coluna = 1;
char jogador_dir   = '>';

void imprimir_mapa() {
    limpar_tela();
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (i == jogador_linha && j == jogador_coluna)
                printf("%c", jogador_dir);
            else
                printf("%c", mapa[i][j]);
        }
        printf("\n");
    }
    printf("\nUse wasd para mover. Q para sair.\n");
}

void mover(int nova_linha, int nova_coluna, char direcao) {
    jogador_dir = direcao;

    if (mapa[nova_linha][nova_coluna] != '*') {
        jogador_linha  = nova_linha;
        jogador_coluna = nova_coluna;
    }
}

int main() {
    char tecla;

    imprimir_mapa();

    while (1) {
        tecla = _getch();

        if (tecla == 'w') mover(jogador_linha - 1, jogador_coluna, '^');
        if (tecla == 's') mover(jogador_linha + 1, jogador_coluna, 'v');
        if (tecla == 'a') mover(jogador_linha,     jogador_coluna - 1, '<');
        if (tecla == 'd') mover(jogador_linha,     jogador_coluna + 1, '>');
        if (tecla == 'q') break;

        imprimir_mapa();
    }

    return 0;
}