#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define LINHAS  10
#define COLUNAS 10
#define limpar_tela() printf("\033[H\033[J")
#define MAX_CELULAS_ATAQUE 12

int vidas  = 3;
int chaves = 0;
int arma   = 0;

char mapa[LINHAS][COLUNAS] = {
    {'*','*','*','*','*','*','*','*','*','*'},
    {'*',' ',' ',' ','@',' ',' ',' ',' ','*'},
    {'*',' ','*','*','*','D','*',' ',' ','*'},
    {'*',' ',' ',' ',' ','=',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ','k',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ','N',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','L'},
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
    printf("  N  = NPC\n");
    printf("  X  = monstro tipo 1\n");
    printf("  Y  = monstro tipo 2\n");
    printf("  Z  = boss final\n\n");
    printf("ARMAS:\n");
    printf("  Espada    = area 3x2 a frente\n");
    printf("  Arco      = linha reta, 4 celulas\n");
    printf("  Cajado    = 8 celulas ao redor\n\n");
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
    printf("Vidas: %d  |  Chaves: %d  |  Arma: ", vidas, chaves);
    if (arma == 1)      printf("Espada\n\n");
    else if (arma == 2) printf("Arco\n\n");
    else if (arma == 3) printf("Cajado\n\n");
    else                printf("Nenhuma\n\n");

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (i == jogador_linha && j == jogador_coluna)
                printf("%c", jogador_dir);
            else
                printf("%c", mapa[i][j]);
        }
        printf("\n");
    }
    printf("\nUse wasd para mover, i para interagir, o para atacar. Q para sair.\n");
}

void mover(int nova_linha, int nova_coluna, char direcao) {
    jogador_dir = direcao;

    char destino = mapa[nova_linha][nova_coluna];

    if (destino != '*' && destino != 'D' && destino != 'k' && destino != 'N') {
        jogador_linha  = nova_linha;
        jogador_coluna = nova_coluna;
    }
}

int eh_parede(int linha, int coluna) {
    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS) return 1;
    return mapa[linha][coluna] == '*';
}

void atacar_celula(int linha, int coluna) {
    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS) return;
    if (mapa[linha][coluna] == 'k') {
        mapa[linha][coluna] = ' ';
    }
}

void mostrar_ataque(int linhas[], int colunas[], int total) {
    char backup[MAX_CELULAS_ATAQUE];

    for (int i = 0; i < total; i++) {
        backup[i] = mapa[linhas[i]][colunas[i]];
        mapa[linhas[i]][colunas[i]] = '!';
    }

    imprimir_mapa();
    Sleep(150);

    for (int i = 0; i < total; i++) {
        mapa[linhas[i]][colunas[i]] = backup[i];
    }
}

void atacar_espada() {
    int dl = 0, dc = 0, el = 0, ec = 0;

    if (jogador_dir == '^') { dl = -1; dc =  0; el = 0; ec = 1; }
    if (jogador_dir == 'v') { dl =  1; dc =  0; el = 0; ec = 1; }
    if (jogador_dir == '<') { dl =  0; dc = -1; el = 1; ec = 0; }
    if (jogador_dir == '>') { dl =  0; dc =  1; el = 1; ec = 0; }

    int linhas[MAX_CELULAS_ATAQUE];
    int colunas[MAX_CELULAS_ATAQUE];
    int total = 0;

    for (int prof = 1; prof <= 2; prof++) {
        int base_l = jogador_linha  + dl * prof;
        int base_c = jogador_coluna + dc * prof;

        if (eh_parede(base_l, base_c)) break;

        int candidatos_l[3] = { base_l, base_l + el, base_l - el };
        int candidatos_c[3] = { base_c, base_c + ec, base_c - ec };

        for (int k = 0; k < 3; k++) {
            int l = candidatos_l[k];
            int c = candidatos_c[k];
            if (l >= 0 && l < LINHAS && c >= 0 && c < COLUNAS && mapa[l][c] != '*') {
                linhas[total]  = l;
                colunas[total] = c;
                total++;
            }
        }
    }

    mostrar_ataque(linhas, colunas, total);

    for (int i = 0; i < total; i++) {
        atacar_celula(linhas[i], colunas[i]);
    }
}

void atacar_arco() {
    int dl = 0, dc = 0;

    if (jogador_dir == '^') { dl = -1; dc =  0; }
    if (jogador_dir == 'v') { dl =  1; dc =  0; }
    if (jogador_dir == '<') { dl =  0; dc = -1; }
    if (jogador_dir == '>') { dl =  0; dc =  1; }

    int linhas[MAX_CELULAS_ATAQUE];
    int colunas[MAX_CELULAS_ATAQUE];
    int total = 0;

    for (int i = 1; i <= 4; i++) {
        int l = jogador_linha  + dl * i;
        int c = jogador_coluna + dc * i;

        if (eh_parede(l, c)) break;

        linhas[total]  = l;
        colunas[total] = c;
        total++;
    }

    mostrar_ataque(linhas, colunas, total);

    for (int i = 0; i < total; i++) {
        atacar_celula(linhas[i], colunas[i]);
    }
}

void atacar_cajado() {
    int linhas[MAX_CELULAS_ATAQUE];
    int colunas[MAX_CELULAS_ATAQUE];
    int total = 0;

    for (int dl = -1; dl <= 1; dl++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dl == 0 && dc == 0) continue;
            int l = jogador_linha  + dl;
            int c = jogador_coluna + dc;
            if (l >= 0 && l < LINHAS && c >= 0 && c < COLUNAS && mapa[l][c] != '*') {
                linhas[total]  = l;
                colunas[total] = c;
                total++;
            }
        }
    }

    mostrar_ataque(linhas, colunas, total);

    for (int i = 0; i < total; i++) {
        atacar_celula(linhas[i], colunas[i]);
    }
}

void escolher_arma() {
    limpar_tela();
    printf("=== NPC ===\n\n");
    printf("Aventureiro, escolha sua arma antes de entrar na masmorra:\n\n");
    printf("1. Espada  - area 3x2 a frente\n");
    printf("2. Arco    - linha reta, 4 celulas\n");
    printf("3. Cajado  - 8 celulas ao redor\n\n");
    printf("Escolha: ");

    char opcao;
    while (1) {
        opcao = _getch();
        if (opcao == '1') { arma = 1; printf("Espada escolhida!\n");  break; }
        if (opcao == '2') { arma = 2; printf("Arco escolhido!\n");    break; }
        if (opcao == '3') { arma = 3; printf("Cajado escolhido!\n");  break; }
    }
    _getch();
}

void atacar() {
    if (arma == 0) {
        printf("\nVoce nao tem uma arma! Fale com o NPC primeiro.\n");
        _getch();
        return;
    }

    if (arma == 1) atacar_espada();
    if (arma == 2) atacar_arco();
    if (arma == 3) atacar_cajado();
}

void interagir() {
    int alvo_linha  = jogador_linha;
    int alvo_coluna = jogador_coluna;

    if (jogador_dir == '^') alvo_linha--;
    if (jogador_dir == 'v') alvo_linha++;
    if (jogador_dir == '<') alvo_coluna--;
    if (jogador_dir == '>') alvo_coluna++;

    char alvo = mapa[alvo_linha][alvo_coluna];

    if (alvo == '@') {
        chaves++;
        mapa[alvo_linha][alvo_coluna] = ' ';
        printf("\nVoce pegou uma chave! Chaves: %d\n", chaves);
        _getch();
    } else if (alvo == 'D') {
        if (chaves > 0) {
            chaves--;
            mapa[alvo_linha][alvo_coluna] = '=';
            printf("\nPorta aberta!\n");
        } else {
            printf("\nVoce precisa de uma chave!\n");
        }
        _getch();
    } else if (alvo == 'N') {
        escolher_arma();
    } else if (alvo == 'L') {
        printf("\nVoce subiu a escada! (proxima fase em breve)\n");
        _getch();
    } else {
        printf("\nNao ha nada para interagir aqui.\n");
        _getch();
    }
}

void resetar_jogador() {
    jogador_linha  = 1;
    jogador_coluna = 1;
    jogador_dir    = '>';
    chaves         = 0;
}

void jogar() {
    vidas = 3;
    arma  = 0;
    resetar_jogador();
    imprimir_mapa();

    char tecla;
    while (1) {
        tecla = _getch();

        if (tecla == 'w') mover(jogador_linha - 1, jogador_coluna, '^');
        if (tecla == 's') mover(jogador_linha + 1, jogador_coluna, 'v');
        if (tecla == 'a') mover(jogador_linha,     jogador_coluna - 1, '<');
        if (tecla == 'd') mover(jogador_linha,     jogador_coluna + 1, '>');
        if (tecla == 'i') interagir();
        if (tecla == 'o') atacar();
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