
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define TAM 9

#define VERDE 10
#define BRANCO 15
#define PADRAO 7

int solucao[TAM][TAM];
int tabuleiro[TAM][TAM];
int fixo[TAM][TAM];

int erros = 0;
int dicas = 0;

// =================================
// CORES
// =================================

void cor(int codigo) {
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE), codigo
    );
}

void limparTela(void) {
    system("cls");
}

// =================================
// EMBARALHAR NUMEROS
// =================================

void embaralhar(int v[], int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

// =================================
// VERIFICAR JOGADA
// =================================

int movimentoValido(int grade[9][9],
                    int linha, int coluna, int numero) {

    for (int j = 0; j < 9; j++) {
        if (grade[linha][j] == numero)
            return 0;
    }

    for (int i = 0; i < 9; i++) {
        if (grade[i][coluna] == numero)
            return 0;
    }

    int inicioLinha = (linha / 3) * 3;
    int inicioColuna = (coluna / 3) * 3;

    for (int i = inicioLinha; i < inicioLinha + 3; i++) {
        for (int j = inicioColuna; j < inicioColuna + 3; j++) {
            if (grade[i][j] == numero)
                return 0;
        }
    }

    return 1;
}

// =================================
// ENCONTRAR MELHOR CASA VAZIA
// =================================

int encontrarCasa(int grade[9][9],
                  int *linha, int *coluna,
                  int aleatorio) {

    int menor = 10;
    int candidatos[9];

    *linha = -1;
    *coluna = -1;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (grade[i][j] != 0)
                continue;

            int quantidade = 0;

            for (int n = 1; n <= 9; n++) {
                if (movimentoValido(grade, i, j, n))
                    candidatos[quantidade++] = n;
            }

            if (quantidade < menor) {
                menor = quantidade;
                *linha = i;
                *coluna = j;

                if (menor == 0)
                    return 0;
            }
        }
    }

    if (*linha == -1)
        return 1;

    return 2;
}

// =================================
// GERAR SOLUCAO ALEATORIA
// =================================

int gerarSolucao(int grade[9][9]) {

    int linha, coluna;

    int estado = encontrarCasa(
        grade, &linha, &coluna, 1
    );

    if (estado == 1)
        return 1;

    if (estado == 0)
        return 0;

    int numeros[9];

    for (int i = 0; i < 9; i++)
        numeros[i] = i + 1;

    embaralhar(numeros, 9);

    for (int i = 0; i < 9; i++) {

        int n = numeros[i];

        if (movimentoValido(grade, linha, coluna, n)) {

            grade[linha][coluna] = n;

            if (gerarSolucao(grade))
                return 1;

            grade[linha][coluna] = 0;
        }
    }

    return 0;
}

// =================================
// CONTAR SOLUCOES POSSIVEIS
// =================================

int contarSolucoes(int grade[9][9]) {

    int linha, coluna;

    int estado = encontrarCasa(
        grade, &linha, &coluna, 0
    );

    if (estado == 1)
        return 1;

    if (estado == 0)
        return 0;

    int total = 0;

    for (int n = 1; n <= 9; n++) {

        if (movimentoValido(grade, linha, coluna, n)) {

            grade[linha][coluna] = n;

            total += contarSolucoes(grade);

            grade[linha][coluna] = 0;

            if (total >= 2)
                return 2;
        }
    }

    return total;
}

// =================================
// GERAR NOVO SUDOKU
// =================================

void prepararJogo(int dificuldade) {

    int vazios;

    if (dificuldade == 1)
        vazios = 35;
    else if (dificuldade == 2)
        vazios = 45;
    else
        vazios = 52;

    // Comeca com um tabuleiro vazio

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            solucao[i][j] = 0;

    // Cria uma solucao completamente aleatoria

    gerarSolucao(solucao);

    // Copia a solucao para o tabuleiro

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            tabuleiro[i][j] = solucao[i][j];
            fixo[i][j] = 1;
        }
    }

    // Embaralha as posicoes que serao removidas

    int posicoes[81];

    for (int i = 0; i < 81; i++)
        posicoes[i] = i;

    embaralhar(posicoes, 81);

    int removidos = 0;

    for (int k = 0; k < 81 && removidos < vazios; k++) {

        int linha = posicoes[k] / 9;
        int coluna = posicoes[k] % 9;

        int numero = tabuleiro[linha][coluna];

        tabuleiro[linha][coluna] = 0;

        // Confere se existe somente uma solucao

        int total = contarSolucoes(tabuleiro);

        if (total == 1) {

            fixo[linha][coluna] = 0;
            removidos++;

        } else {

            tabuleiro[linha][coluna] = numero;
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

    for (int i = 0; i < 9; i++) {

        printf("  %d  |", i + 1);

        for (int j = 0; j < 9; j++) {

            if (tabuleiro[i][j] == 0) {

                cor(PADRAO);
                printf(" .");

            } else {

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

    printf("\n  Erros: %d     Dicas: %d\n", erros, dicas);

    printf("\n  1 - Inserir numero");
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

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (tabuleiro[i][j] != solucao[i][j])
                return 0;
        }
    }

    return 1;
}

// =================================
// PEDIR DICA
// =================================

void mostrarDica(void) {

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (tabuleiro[i][j] == 0) {

                tabuleiro[i][j] = solucao[i][j];

                dicas++;

                return;
            }
        }
    }
}

// =================================
// PAUSAR
// =================================

void pausar(void) {
    printf("\n  Pressione ENTER para continuar...");
    getchar();
    getchar();
}

// =================================
// JOGAR
// =================================

void jogar(void) {

    int dificuldade;
    int linha, coluna, numero;
    int opcao;

    limparTela();

    printf("\n  ESCOLHA A DIFICULDADE\n\n");

    printf("  1 - Facil\n");
    printf("  2 - Medio\n");
    printf("  3 - Dificil\n");

    printf("\n  Opcao: ");

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

        printf("\n  Opcao: ");

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

            } else {

                printf("\n  Sudoku ainda nao foi resolvido.\n");

                pausar();
                continue;
            }
        }

        if (opcao != 1 && opcao != 2)
            continue;

        printf("\n  Linha (1-9): ");

        if (scanf("%d", &linha) != 1)
            break;

        printf("  Coluna (1-9): ");

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

        printf("  Numero (1-9): ");

        if (scanf("%d", &numero) != 1)
            break;

        if (numero < 1 || numero > 9)
            continue;

        if (!movimentoValido(tabuleiro, linha, coluna, numero)) {
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

    long long tempo =
        (long long)(time(NULL) - inicio);

    limparTela();

    printf("\n  FIM DE JOGO\n\n");

    printf("  Erros: %d\n", erros);
    printf("  Dicas: %d\n", dicas);
    printf("  Tempo: %lld segundos\n", tempo);

    pausar();
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

                pausar();

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
