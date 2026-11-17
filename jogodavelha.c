#include <stdio.h>
#include <stdlib.h>

char tabuleiro[3][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

// Limpa a tela
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Mostra o tabuleiro
void mostrarTabuleiro() {
    printf("\n");
    printf(" %c | %c | %c \n",
           tabuleiro[0][0], tabuleiro[0][1], tabuleiro[0][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n",
           tabuleiro[1][0], tabuleiro[1][1], tabuleiro[1][2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n",
           tabuleiro[2][0], tabuleiro[2][1], tabuleiro[2][2]);
    printf("\n");
}

// Verifica se alguem venceu
int verificarVitoria() {
    int i;

    // Verifica linhas e colunas
    for (i = 0; i < 3; i++) {
        if (tabuleiro[i][0] == tabuleiro[i][1] &&
            tabuleiro[i][1] == tabuleiro[i][2])
            return 1;

        if (tabuleiro[0][i] == tabuleiro[1][i] &&
            tabuleiro[1][i] == tabuleiro[2][i])
            return 1;
    }

    // Verifica diagonais
    if (tabuleiro[0][0] == tabuleiro[1][1] &&
        tabuleiro[1][1] == tabuleiro[2][2])
        return 1;

    if (tabuleiro[0][2] == tabuleiro[1][1] &&
        tabuleiro[1][1] == tabuleiro[2][0])
        return 1;

    return 0;
}

int main() {
    int jogada;
    int jogador = 1;
    int linha, coluna;
    int jogadas = 0;
    char simbolo;

    while (1) {
        limparTela();

        printf("===== JOGO DA VELHA =====\n");
        mostrarTabuleiro();

        simbolo = (jogador == 1) ? 'X' : 'O';

        printf("Jogador %d (%c), escolha uma posicao (1-9): ",
               jogador, simbolo);

        if (scanf("%d", &jogada) != 1) {
            printf("\nEntrada invalida!\n");
            return 1;
        }

        // Verifica se a posicao e valida
        if (jogada < 1 || jogada > 9) {
            printf("\nPosicao invalida! Tente novamente.\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            getchar();
            continue;
        }

        linha = (jogada - 1) / 3;
        coluna = (jogada - 1) % 3;

        // Verifica se a posicao esta ocupada
        if (tabuleiro[linha][coluna] == 'X' ||
            tabuleiro[linha][coluna] == 'O') {
            printf("\nEssa posicao ja esta ocupada!\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            getchar();
            continue;
        }

        // Marca a jogada
        tabuleiro[linha][coluna] = simbolo;
        jogadas++;

        // Verifica vitoria
        if (verificarVitoria()) {
            limparTela();
            printf("===== JOGO DA VELHA =====\n");
            mostrarTabuleiro();
            printf("Parabens! Jogador %d (%c) venceu!\n",
                   jogador, simbolo);
            break;
        }

        // Verifica empate
        if (jogadas == 9) {
            limparTela();
            printf("===== JOGO DA VELHA =====\n");
            mostrarTabuleiro();
            printf("Empate! Ninguem venceu.\n");
            break;
        }

        // Alterna o jogador
        jogador = (jogador == 1) ? 2 : 1;
    }

    printf("\nFim de jogo!\n");

    return 0;
}
