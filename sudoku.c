
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define TAM 9

#define VERDE 10
#define BRANCO 15
#define PADRAO 7

int solucao[TAM][TAM] = {
    {5,3,4,6,7,8,9,1,2},
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};

int tabuleiro[TAM][TAM];
int fixo[TAM][TAM];

int erros = 0;
int dicas = 0;

// =================================
// CONTROLE DE CORES
// =================================

void cor(int codigo) {
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        codigo
    );
}

// =================================
// LIMPAR TELA
// =================================

void limparTela(void) {
    system("cls");
}

// =================================
// PREPARAR JOGO
// =================================

void prepararJogo(int dificuldade) {

    int vazios;

    if (dificuldade == 1)
        vazios = 35;
    else if (dificuldade == 2)
        vazios = 48;
    else
        vazios = 58;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            tabuleiro[i][j] = solucao[i][j];
            fixo[i][j] = 1;

        }
    }

    int removidos = 0;

    while (removidos < vazios) {

        int i = rand() % 9;
        int j = rand() % 9;

        if (tabuleiro[i][j] != 0) {

            tabuleiro[i][j] = 0;
            fixo[i][j] = 0;

            removidos++;
        }
    }
}

// =================================
// EXIBIR TABULEIRO
// =================================

void exibirTabuleiro(void) {

    limparTela();

    cor(BRANCO);

    printf("\n");
    printf("          S U D O K U\n");
    printf("       =================\n\n");

    printf("       1 2 3   4 5 6   7 8 9\n");
    printf("     +-------+-------+-------+\n");

    for (int i = 0; i < TAM; i++) {

        printf("  %d  |", i + 1);

        for (int j = 0; j < TAM; j++) {

            if (tabuleiro[i][j] == 0) {

                cor(PADRAO);
                printf(" .");

            }
            else {

                if (fixo[i][j])
                    cor(BRANCO);
                else
                    cor(VERDE);

                printf(" %d", tabuleiro[i][j]);

                cor(PADRAO);
            }

            if (j == 2 || j == 5)
                printf(" |");
        }

        printf(" |\n");

        if (i == 2 || i == 5)
            printf("     +-------+-------+-------+\n");
    }

    printf("     +-------+-------+-------+\n");

    cor(BRANCO);

    printf("\n  Erros: %d", erros);
    printf("    Dicas: %d\n", dicas);

    printf("\n");
    printf("  1 - Inserir numero");
    printf("    2 - Apagar numero\n");

    printf("  3 - Pedir dica");
    printf("        4 - Verificar resultado\n");

    printf("  0 - Voltar ao menu\n");

    cor(PADRAO);
}

// =================================
// VERIFICAR VITORIA
// =================================

int verificarVitoria(void) {

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (tabuleiro[i][j] != solucao[i][j])
                return 0;

        }
    }

    return 1;
}

// =================================
// MOVIMENTO VALIDO
// =================================

int movimentoValido(int linha, int coluna, int numero) {

    for (int j = 0; j < TAM; j++) {

        if (j != coluna &&
            tabuleiro[linha][j] == numero)
            return 0;
    }

    for (int i = 0; i < TAM; i++) {

        if (i != linha &&
            tabuleiro[i][coluna] == numero)
            return 0;
    }

    int inicioLinha = (linha / 3) * 3;
    int inicioColuna = (coluna / 3) * 3;

    for (int i = inicioLinha; i < inicioLinha + 3; i++) {

        for (int j = inicioColuna; j < inicioColuna + 3; j++) {

            if ((i != linha || j != coluna) &&
                tabuleiro[i][j] == numero)
                return 0;
        }
    }

    return 1;
}

// =================================
// PEDIR DICA
// =================================

void mostrarDica(void) {

    for (int i = 0; i < TAM; i++) {

        for (int j = 0; j < TAM; j++) {

            if (tabuleiro[i][j] == 0) {

                tabuleiro[i][j] = solucao[i][j];

                dicas++;

                return;
            }
        }
    }
}

// =================================
// JOGO
// =================================

void jogar(void) {

    int dificuldade;
    int linha, coluna, numero;
    int opcao;

    limparTela();

    printf("\nESCOLHA A DIFICULDADE\n\n");

    printf("  1 - Facil\n");
    printf("  2 - Medio\n");
    printf("  3 - Dificil\n");

    printf("\nOpcao: ");

    if (scanf("%d", &dificuldade) != 1)
        return;

    if (dificuldade < 1 || dificuldade > 3)
        return;

    prepararJogo(dificuldade);

    erros = 0;
    dicas = 0;

    time_t inicio = time(NULL);

    while (1) {

        exibirTabuleiro();

        printf("\nOpcao: ");

        if (scanf("%d", &opcao) != 1)
            break;

        if (opcao == 0)
            break;

        if (opcao == 3) {

            mostrarDica();
            continue;
        }

        if (opcao == 4) {

            limparTela();

            if (verificarVitoria()) {

                cor(VERDE);
                printf("\n  PARABENS! SUDOKU RESOLVIDO!\n");
                cor(PADRAO);

                break;

            }
            else {

                printf("\n  Sudoku ainda nao foi resolvido.\n");

                printf("\n  Pressione ENTER para continuar.");

                getchar();
                getchar();

                continue;
            }
        }

        if (opcao != 1 && opcao != 2)
            continue;

        printf("\nLinha (1-9): ");

        if (scanf("%d", &linha) != 1)
            break;

        printf("Coluna (1-9): ");

        if (scanf("%d", &coluna) != 1)
            break;

        if (linha < 1 || linha > 9 ||
            coluna < 1 || coluna > 9)
            continue;

        linha--;
        coluna--;

        if (fixo[linha][coluna])
            continue;

        if (opcao == 2) {

            tabuleiro[linha][coluna] = 0;

            continue;
        }

        printf("Numero (1-9): ");

        if (scanf("%d", &numero) != 1)
            break;

        if (numero < 1 || numero > 9)
            continue;

        if (!movimentoValido(linha, coluna, numero)) {

            erros++;
            continue;
        }

        if (numero != solucao[linha][coluna]) {

            erros++;
            continue;
        }

        tabuleiro[linha][coluna] = numero;

        if (verificarVitoria()) {

            exibirTabuleiro();

            cor(VERDE);
            printf("\n  PARABENS! VOCE VENCEU!\n");
            cor(PADRAO);

            break;
        }
    }

    limparTela();

    printf("\n  FIM DE JOGO\n\n");

    printf("  Erros: %d\n", erros);
    printf("  Dicas: %d\n", dicas);

    printf("  Tempo: %lld segundos\n",
        (long long)(time(NULL) - inicio));

    printf("\n  Pressione ENTER para continuar.");

    getchar();
    getchar();
}

// =================================
// MENU PRINCIPAL
// =================================

int main(void) {

    int opcao;

    srand((unsigned)time(NULL));

    do {

        limparTela();

        cor(BRANCO);

        printf("\n");
        printf("  ==============================\n");
        printf("            S U D O K U         \n");
        printf("  ==============================\n\n");

        printf("       1 - NOVO JOGO\n");
        printf("       2 - REGRAS\n");
        printf("       0 - SAIR\n");

        printf("\n  ==============================\n");

        printf("\n  Escolha: ");

        if (scanf("%d", &opcao) != 1)
            break;

        switch (opcao) {

            case 1:
                jogar();
                break;

            case 2:

                limparTela();

                printf("\n  REGRAS DO SUDOKU\n\n");

                printf("  - Cada linha deve conter numeros de 1 a 9.\n");
                printf("  - Cada coluna deve conter numeros de 1 a 9.\n");
                printf("  - Cada bloco 3x3 deve conter numeros de 1 a 9.\n");
                printf("  - Nao pode repetir numeros.\n");
                printf("  - Casas originais nao podem ser alteradas.\n");

                printf("\n  Pressione ENTER para voltar.");

                getchar();
                getchar();

                break;

            case 0:
                limparTela();
                printf("\n  Obrigado por jogar!\n\n");
                break;

            default:
                break;
        }

    } while (opcao != 0);

    cor(PADRAO);

    return 0;
}
