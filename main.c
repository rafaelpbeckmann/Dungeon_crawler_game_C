#include <stdio.h>
#include <conio.h>

#define LINHAS  10
#define COLUNAS 10
#define limpar_tela() printf("\033[H\033[J")


int vidas = 3;

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



void tela_tutorial() {
    limpar_tela();
    printf("=== TUTORIAL ===\n\n");
    printf("CONTROLES:\n");
    printf("  w = mover para cima\n");
    printf("  s = mover para baixo\n");
    printf("  a = mover para esquerda\n");
    printf("  d = mover para direita\n");
    printf("  i = interagir com objeto a frente\n");
    printf("  o = atacar a frente\n\n");
    printf("SIMBOLOS:\n");
    printf("  *  = parede\n");
    printf("  #  = espinho (mata ao pisar)\n");
    printf("  k  = caixa (destruivel com ataque)\n");
    printf("  @  = chave\n");
    printf("  D  = porta fechada\n");
    printf("  =  = porta aberta\n");
    printf("  L  = escada (proxima fase)\n");
    printf("  X  = monstro tipo 1\n");
    printf("  Y  = monstro tipo 2\n");
    printf("  Z  = boss final\n\n");
    printf("Pressione qualquer tecla para voltar...\n");
    _getch();
}

void tela_creditos() {
    limpar_tela();
    printf("=== CREDITOS ===\n\n");
    printf("Desenvolvido por:\n");
    printf("  - Rafael Beckmann\n\n");
    printf("Obrigado por jogar!\n\n");
    printf("Pressione qualquer tecla para sair...\n");
    _getch();
}

void tela_game_over() {
    limpar_tela();
    printf("=== GAME OVER ===\n\n");
    printf("Voce perdeu todas as suas vidas!\n\n");
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    _getch();
}


void imprimir_mapa() {
    limpar_tela();
    printf("Vidas: %d\n\n", vidas);
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

void resetar_jogador() {
    jogador_linha  = 1;
    jogador_coluna = 1;
    jogador_dir    = '>';
}

void jogar() {
    vidas = 3;
    resetar_jogador();
    imprimir_mapa();

    char tecla;
    while (1) {
        tecla = _getch();

        if (tecla == 'w') mover(jogador_linha - 1, jogador_coluna, '^');
        if (tecla == 's') mover(jogador_linha + 1, jogador_coluna, 'v');
        if (tecla == 'a') mover(jogador_linha,     jogador_coluna - 1, '<');
        if (tecla == 'd') mover(jogador_linha,     jogador_coluna + 1, '>');
        if (tecla == 'q') break;

        

        imprimir_mapa();
    }
}


void menu() {
    char opcao;
    while (1) {
        limpar_tela();
        printf("=== DUNGEON CRAWLER ===\n\n");
        printf("1. Jogar\n");
        printf("2. Tutorial\n");
        printf("3. Sair\n\n");
        printf("Escolha uma opcao: ");
        opcao = _getch();

        if (opcao == '1') {
            jogar();
        } else if (opcao == '2') {
            tela_tutorial();
        } else if (opcao == '3') {
            tela_creditos();
            break;
        }
    }
}

int main() {
    menu();
    return 0;
}