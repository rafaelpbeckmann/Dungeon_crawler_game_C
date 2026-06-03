#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CELULAS_ATAQUE 12
#define MAX_MONSTROS 10
#define limpar_tela() printf("\033[H\033[J")

#define LINHAS_VILA   10
#define COLUNAS_VILA  10
#define LINHAS_ANDAR1  12
#define COLUNAS_ANDAR1 10

int vidas  = 3;
int chaves = 0;
int arma   = 0;
int fase   = 0;

char vila_original[LINHAS_VILA][COLUNAS_VILA] = {
    {'*','*','*','*','*','*','*','*','*','*'},
    {'*',' ',' ',' ','*',' ',' ',' ',' ','*'},
    {'*','@',' ',' ','*',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ','D',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ','*',' ',' ','*','*','*'},
    {'*',' ','N',' ','*',' ',' ',' ',' ','*'},
    {'*',' ',' ',' ','*',' ',' ','k',' ','*'},
    {'*','*','*','*','*',' ',' ','k',' ','*'},
    {'*',' ',' ',' ',' ',' ',' ','*','L','*'},
    {'*','*','*','*','*','*','*','*','*','*'}
};

char andar1_original[LINHAS_ANDAR1][COLUNAS_ANDAR1] = {
    {'*','*','*','*','*','*','*','*','*','*'},
    {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
    {'*',' ','*','*','*','*',' ',' ',' ','*'},
    {'*',' ',' ',' ',' ','*',' ',' ',' ','*'},
    {'*',' ',' ','@',' ','*',' ',' ',' ','*'},
    {'*','*','D','*','*','*','*','*','*','*'},
    {'*','k','k','k',' ',' ',' ',' ',' ','*'},
    {'*','k','k','k','*','*','*','*','*','*'},
    {'*',' ',' ',' ','*',' ',' ',' ',' ','*'},
    {'*',' ','*','*','*',' ','*','k','k','*'},
    {'*',' ','k','k','k',' ','*','k','L','*'},
    {'*','*','*','*','*','*','*','*','*','*'}
};

char mapa[25][25];

int linhas_atual  = LINHAS_VILA;
int colunas_atual = COLUNAS_VILA;

int jogador_linha  = 1;
int jogador_coluna = 1;
char jogador_dir   = '>';

int monstro_linha[MAX_MONSTROS];
int monstro_coluna[MAX_MONSTROS];
int total_monstros = 0;

void carregar_mapa() {
    total_monstros = 0;

    char (*origem)[25] = NULL;

    if (fase == 0) {
        linhas_atual  = LINHAS_VILA;
        colunas_atual = COLUNAS_VILA;
        for (int i = 0; i < linhas_atual; i++)
            for (int j = 0; j < colunas_atual; j++)
                mapa[i][j] = vila_original[i][j];
    } else if (fase == 1) {
        linhas_atual  = LINHAS_ANDAR1;
        colunas_atual = COLUNAS_ANDAR1;
        for (int i = 0; i < linhas_atual; i++)
            for (int j = 0; j < colunas_atual; j++)
                mapa[i][j] = andar1_original[i][j];
    }

    for (int i = 0; i < linhas_atual; i++) {
        for (int j = 0; j < colunas_atual; j++) {
            if (mapa[i][j] == 'X') {
                monstro_linha[total_monstros]  = i;
                monstro_coluna[total_monstros] = j;
                total_monstros++;
            }
        }
    }
}

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

    if (fase == 0)      printf("[ Vila ]\n\n");
    else if (fase == 1) printf("[ Masmorra - Andar 1 ]\n\n");

    for (int i = 0; i < linhas_atual; i++) {
        for (int j = 0; j < colunas_atual; j++) {
            if (i == jogador_linha && j == jogador_coluna)
                printf("%c", jogador_dir);
            else
                printf("%c", mapa[i][j]);
        }
        printf("\n");
    }
    printf("\nUse wasd para mover, i para interagir, o para atacar. Q para sair.\n");
}

int eh_parede(int linha, int coluna) {
    if (linha < 0 || linha >= linhas_atual || coluna < 0 || coluna >= colunas_atual) return 1;
    return mapa[linha][coluna] == '*';
}

void mover_monstros() {
    int direcoes[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

    for (int m = 0; m < total_monstros; m++) {
        int dir   = rand() % 4;
        int nova_l = monstro_linha[m]  + direcoes[dir][0];
        int nova_c = monstro_coluna[m] + direcoes[dir][1];

        if (nova_l < 0 || nova_l >= linhas_atual || nova_c < 0 || nova_c >= colunas_atual) continue;

        char destino = mapa[nova_l][nova_c];

        if (destino == '*' || destino == '#' || destino == 'k' ||
            destino == 'D' || destino == 'N' || destino == 'X' ||
            destino == 'Y' || destino == 'Z') continue;

        mapa[monstro_linha[m]][monstro_coluna[m]] = ' ';
        monstro_linha[m]  = nova_l;
        monstro_coluna[m] = nova_c;
        mapa[nova_l][nova_c] = 'X';
    }
}

int verificar_morte() {
    char celula = mapa[jogador_linha][jogador_coluna];
    if (celula == '#') return 1;

    for (int m = 0; m < total_monstros; m++) {
        if (monstro_linha[m] == jogador_linha && monstro_coluna[m] == jogador_coluna) return 1;
    }
    return 0;
}

void atacar_celula(int linha, int coluna) {
    if (linha < 0 || linha >= linhas_atual || coluna < 0 || coluna >= colunas_atual) return;

    char alvo = mapa[linha][coluna];

    if (alvo == 'k') {
        mapa[linha][coluna] = ' ';
    } else if (alvo == 'X') {
        mapa[linha][coluna] = ' ';
        for (int m = 0; m < total_monstros; m++) {
            if (monstro_linha[m] == linha && monstro_coluna[m] == coluna) {
                for (int k = m; k < total_monstros - 1; k++) {
                    monstro_linha[k]  = monstro_linha[k+1];
                    monstro_coluna[k] = monstro_coluna[k+1];
                }
                total_monstros--;
                break;
            }
        }
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
            if (l >= 0 && l < linhas_atual && c >= 0 && c < colunas_atual && mapa[l][c] != '*') {
                linhas[total]  = l;
                colunas[total] = c;
                total++;
            }
        }
    }

    mostrar_ataque(linhas, colunas, total);
    for (int i = 0; i < total; i++) atacar_celula(linhas[i], colunas[i]);
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
    for (int i = 0; i < total; i++) atacar_celula(linhas[i], colunas[i]);
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
            if (l >= 0 && l < linhas_atual && c >= 0 && c < colunas_atual && mapa[l][c] != '*') {
                linhas[total]  = l;
                colunas[total] = c;
                total++;
            }
        }
    }

    mostrar_ataque(linhas, colunas, total);
    for (int i = 0; i < total; i++) atacar_celula(linhas[i], colunas[i]);
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

void avancar_fase() {
    fase++;
    chaves = 0;
    jogador_linha  = 1;
    jogador_coluna = 1;
    jogador_dir    = '>';
    carregar_mapa();
    imprimir_mapa();
}

void interagir() {
    int ordem[4][2];
    int n = 0;

    if (jogador_dir == '^') { ordem[n][0] = -1; ordem[n][1] = 0; n++; }
    if (jogador_dir == 'v') { ordem[n][0] =  1; ordem[n][1] = 0; n++; }
    if (jogador_dir == '<') { ordem[n][0] =  0; ordem[n][1] = -1; n++; }
    if (jogador_dir == '>') { ordem[n][0] =  0; ordem[n][1] =  1; n++; }

    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int d = 0; d < 4; d++) {
        int dl = dirs[d][0], dc = dirs[d][1];
        int ja_tem = 0;
        for (int k = 0; k < n; k++)
            if (ordem[k][0] == dl && ordem[k][1] == dc) { ja_tem = 1; break; }
        if (!ja_tem) {
            ordem[n][0] = dl;
            ordem[n][1] = dc;
            n++;
        }
    }

    for (int i = 0; i < 4; i++) {
        int alvo_linha  = jogador_linha  + ordem[i][0];
        int alvo_coluna = jogador_coluna + ordem[i][1];

        if (alvo_linha < 0 || alvo_linha >= linhas_atual ||
            alvo_coluna < 0 || alvo_coluna >= colunas_atual) continue;

        char alvo = mapa[alvo_linha][alvo_coluna];

        if (alvo == '@') {
            chaves++;
            mapa[alvo_linha][alvo_coluna] = ' ';
            printf("\nVoce pegou uma chave! Chaves: %d\n", chaves);
            _getch();
            return;
        } else if (alvo == 'D') {
            if (chaves > 0) {
                chaves--;
                mapa[alvo_linha][alvo_coluna] = '=';
                printf("\nPorta aberta!\n");
            } else {
                printf("\nVoce precisa de uma chave!\n");
            }
            _getch();
            return;
        } else if (alvo == 'N') {
            escolher_arma();
            return;
        } else if (alvo == 'L') {
            avancar_fase();
            return;
        }
    }

    printf("\nNao ha nada para interagir aqui.\n");
    _getch();
}

void mover(int nova_linha, int nova_coluna, char direcao) {
    jogador_dir = direcao;

    char destino = mapa[nova_linha][nova_coluna];

    if (destino != '*' && destino != 'D' && destino != 'k' && destino != 'N') {
        jogador_linha  = nova_linha;
        jogador_coluna = nova_coluna;
    }
}

void resetar_jogador() {
    jogador_linha  = 1;
    jogador_coluna = 1;
    jogador_dir    = '>';
    chaves         = 0;
}

void jogar() {
    srand(time(NULL));
    vidas  = 3;
    arma   = 0;
    fase   = 0;
    carregar_mapa();
    resetar_jogador();
    imprimir_mapa();

    char tecla;
    while (1) {
        tecla = _getch();

        if (tecla == 'q') break;

        if (tecla == 'w') mover(jogador_linha - 1, jogador_coluna, '^');
        if (tecla == 's') mover(jogador_linha + 1, jogador_coluna, 'v');
        if (tecla == 'a') mover(jogador_linha,     jogador_coluna - 1, '<');
        if (tecla == 'd') mover(jogador_linha,     jogador_coluna + 1, '>');
        if (tecla == 'i') interagir();
        if (tecla == 'o') atacar();

        mover_monstros();

        if (verificar_morte()) {
            vidas--;
            if (vidas <= 0) {
                tela_game_over();
                return;
            }
            printf("\nVoce perdeu uma vida! Vidas restantes: %d\n", vidas);
            Sleep(1000);
            carregar_mapa();
            resetar_jogador();
        }

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