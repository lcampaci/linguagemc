
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <errno.h>

#define TAM 9
#define VERDE 10
#define BRANCO 15
#define PADRAO 7
#define CINZA 8

#define ARQUIVO_SAVE "sudoku_save.dat"
#define SAVE_MAGIC "SUDOKU01"
#define SAVE_VERSAO 1

typedef struct {
    char identificacao[9];
    int versao;
    int solucao[TAM][TAM];
    int tabuleiro[TAM][TAM];
    int fixo[TAM][TAM];
    int erros;
    int dicas;
    int dificuldade;
    long long tempoAcumulado;
} DadosJogo;

int solucao[TAM][TAM];
int tabuleiro[TAM][TAM];
int fixo[TAM][TAM];

int erros = 0;
int dicas = 0;
int dificuldade = 1;

long long tempoAcumulado = 0;
time_t inicioSessao;

HANDLE console;

/* =====================================================
   TELA E CORES
   ===================================================== */

void cor(int corTexto) {
    SetConsoleTextAttribute(console, corTexto);
}

void posicionarCursor(short x, short y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(console, pos);
}

void limparTela(void) {
    system("cls");
}

void pausar(void) {
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* =====================================================
   CRONOMETRO
   ===================================================== */

long long obterTempoAtual(void) {
    return tempoAcumulado +
           (long long)(time(NULL) - inicioSessao);
}

void formatarTempo(long long total, char *destino) {
    int horas = (int)(total / 3600);
    int minutos = (int)((total % 3600) / 60);
    int segundos = (int)(total % 60);

    sprintf(destino, "%02d:%02d:%02d",
            horas, minutos, segundos);
}

/* Atualiza somente o cronometro, sem apagar o tabuleiro. */
void atualizarCronometro(void) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD cursorAnterior;

    GetConsoleScreenBufferInfo(console, &info);
    cursorAnterior = info.dwCursorPosition;

    char tempo[20];
    formatarTempo(obterTempoAtual(), tempo);

    /* Posicao exata do unico cronometro */
    posicionarCursor(10, 6);

    cor(VERDE);
    printf("Tempo: %s", tempo);
    cor(PADRAO);

    posicionarCursor(cursorAnterior.X, cursorAnterior.Y);
}

/* =====================================================
   ENTRADA DE DADOS
   ===================================================== */

int lerInteiro(const char *mensagem) {
    char entrada[64];
    char *fim;
    long valor;

    while (1) {
        printf("%s", mensagem);

        if (!fgets(entrada, sizeof(entrada), stdin)) {
            return 0;
        }

        errno = 0;
        valor = strtol(entrada, &fim, 10);

        while (*fim && isspace((unsigned char)*fim)) {
            fim++;
        }

        if (fim != entrada && *fim == '\0' && errno == 0) {
            return (int)valor;
        }

        printf("Entrada invalida. Digite um numero.\n");
    }
}

/*
   Leitura de teclado sem bloqueio.
   O cronometro continua funcionando enquanto o jogador digita.
*/
int lerInteiroComCronometro(const char *mensagem) {
    char entrada[32];
    int tamanho = 0;
    time_t ultimoSegundo = time(NULL);

    entrada[0] = '\0';

    printf("%s", mensagem);
    fflush(stdout);

    while (1) {
        if (_kbhit()) {
            int tecla = _getch();

            if (tecla == 27) {
                printf("\n");
                return -1;
            }

            if (tecla == '\r') {
                entrada[tamanho] = '\0';
                printf("\n");

                if (tamanho == 0) {
                    return 0;
                }

                char *fim;
                long valor = strtol(entrada, &fim, 10);

                if (*fim != '\0') {
                    printf("Digite apenas numeros.\n");
                    return -2;
                }

                return (int)valor;
            }

            if (tecla == '\b') {
                if (tamanho > 0) {
                    tamanho--;
                    entrada[tamanho] = '\0';
                    printf("\b \b");
                    fflush(stdout);
                }
            }
            else if (tecla >= '0' && tecla <= '9') {
                if (tamanho < (int)sizeof(entrada) - 1) {
                    entrada[tamanho++] = (char)tecla;
                    entrada[tamanho] = '\0';
                    putchar(tecla);
                    fflush(stdout);
                }
            }
        }

        time_t agora = time(NULL);

        if (agora != ultimoSegundo) {
            atualizarCronometro();
            ultimoSegundo = agora;
        }

        Sleep(30);
    }
}

/* =====================================================
   REGRAS DO SUDOKU
   ===================================================== */

int movimentoValido(int grade[TAM][TAM],
                    int linha, int coluna, int numero) {
    for (int i = 0; i < TAM; i++) {
        if (grade[linha][i] == numero) return 0;
        if (grade[i][coluna] == numero) return 0;
    }

    int inicioLinha = (linha / 3) * 3;
    int inicioColuna = (coluna / 3) * 3;

    for (int i = inicioLinha; i < inicioLinha + 3; i++) {
        for (int j = inicioColuna; j < inicioColuna + 3; j++) {
            if (grade[i][j] == numero) return 0;
        }
    }

    return 1;
}

void embaralhar(int numeros[], int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }
}

/* =====================================================
   GERACAO ALEATORIA
   ===================================================== */

int gerarSolucao(int grade[TAM][TAM]) {
    int linha = -1, coluna = -1;
    int encontrou = 0;

    for (int i = 0; i < TAM && !encontrou; i++) {
        for (int j = 0; j < TAM; j++) {
            if (grade[i][j] == 0) {
                linha = i;
                coluna = j;
                encontrou = 1;
                break;
            }
        }
    }

    if (!encontrou) return 1;

    int numeros[9] = {1,2,3,4,5,6,7,8,9};
    embaralhar(numeros, 9);

    for (int i = 0; i < 9; i++) {
        int numero = numeros[i];

        if (movimentoValido(grade, linha, coluna, numero)) {
            grade[linha][coluna] = numero;

            if (gerarSolucao(grade)) return 1;

            grade[linha][coluna] = 0;
        }
    }

    return 0;
}

int encontrarCasa(int grade[TAM][TAM],
                  int *melhorLinha, int *melhorColuna) {
    int menorQuantidade = 10;
    int encontrou = 0;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (grade[i][j] == 0) {
                int quantidade = 0;

                for (int n = 1; n <= 9; n++) {
                    if (movimentoValido(grade, i, j, n)) {
                        quantidade++;
                    }
                }

                if (quantidade < menorQuantidade) {
                    menorQuantidade = quantidade;
                    *melhorLinha = i;
                    *melhorColuna = j;
                    encontrou = 1;
                }

                if (quantidade == 0) return 1;
            }
        }
    }

    return encontrou;
}

int contarSolucoes(int grade[TAM][TAM], int limite) {
    int linha = -1, coluna = -1;

    if (!encontrarCasa(grade, &linha, &coluna)) {
        return 1;
    }

    int quantidade = 0;

    for (int numero = 1; numero <= 9; numero++) {
        if (movimentoValido(grade, linha, coluna, numero)) {
            grade[linha][coluna] = numero;

            quantidade += contarSolucoes(grade, limite);

            grade[linha][coluna] = 0;

            if (quantidade >= limite) return quantidade;
        }
    }

    return quantidade;
}

void gerarNovoSudoku(void) {
    memset(solucao, 0, sizeof(solucao));
    memset(tabuleiro, 0, sizeof(tabuleiro));
    memset(fixo, 0, sizeof(fixo));

    gerarSolucao(solucao);
    memcpy(tabuleiro, solucao, sizeof(solucao));

    int quantidadeRemover;

    switch (dificuldade) {
        case 1: quantidadeRemover = 35; break;
        case 2: quantidadeRemover = 45; break;
        default: quantidadeRemover = 52; break;
    }

    int removidas = 0;
    int tentativas = 0;

    while (removidas < quantidadeRemover && tentativas < 500) {
        tentativas++;

        int linha = rand() % 9;
        int coluna = rand() % 9;

        if (tabuleiro[linha][coluna] == 0) continue;

        int valorOriginal = tabuleiro[linha][coluna];
        tabuleiro[linha][coluna] = 0;

        int copia[TAM][TAM];
        memcpy(copia, tabuleiro, sizeof(copia));

        if (contarSolucoes(copia, 2) == 1) {
            removidas++;
        } else {
            tabuleiro[linha][coluna] = valorOriginal;
        }
    }

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            fixo[i][j] = (tabuleiro[i][j] != 0);
        }
    }

    erros = 0;
    dicas = 0;
    tempoAcumulado = 0;
    inicioSessao = time(NULL);
}

/* =====================================================
   TABULEIRO
   ===================================================== */

void exibirTabuleiro(void) {
    limparTela();

    cor(BRANCO);

    printf("\n");
    printf("                  S U D O K U\n");
    printf("             =====================\n\n");

    printf("  Dificuldade: ");

    switch (dificuldade) {
        case 1: printf("Facil"); break;
        case 2: printf("Medio"); break;
        case 3: printf("Dificil"); break;
    }

    printf("  Erros: %d Dicas: %d\n", erros, dicas);

    /*
       UNICO CRONOMETRO DA TELA.
       O tempo inicial e calculado corretamente.
       A funcao atualizarCronometro usa esta mesma posicao.
    */
    char tempo[20];
    formatarTempo(obterTempoAtual(), tempo);

    printf("\n          ");
    cor(VERDE);
    printf("Tempo: %s", tempo);
    cor(PADRAO);
    printf("\n\n");

    cor(BRANCO);

    printf("       1  2  3   4  5  6   7  8  9\n");
    printf("     +---------+---------+---------+\n");

    for (int i = 0; i < TAM; i++) {
        printf("  %d  |", i + 1);

        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == 0) {
                cor(CINZA);
                printf(" . ");
            }
            else if (fixo[i][j]) {
                cor(BRANCO);
                printf(" %d ", tabuleiro[i][j]);
            }
            else {
                cor(VERDE);
                printf(" %d ", tabuleiro[i][j]);
            }

            if (j == 2 || j == 5) {
                cor(BRANCO);
                printf("|");
            }
        }

        cor(BRANCO);
        printf("|\n");

        if (i == 2 || i == 5) {
            printf("     +---------+---------+---------+\n");
        }
    }

    printf("     +---------+---------+---------+\n\n");

    printf("  1 - Inserir numero\n");
    printf("  2 - Apagar numero\n");
    printf("  3 - Receber dica\n");
    printf("  4 - Verificar progresso\n");
    printf("  5 - Salvar partida\n");
    printf("  0 - Salvar e sair\n\n");
}

/* =====================================================
   SALVAR E CARREGAR
   ===================================================== */

int existeJogoSalvo(void) {
    FILE *arquivo = fopen(ARQUIVO_SAVE, "rb");
    if (!arquivo) return 0;

    DadosJogo dados;
    int resultado = fread(&dados, sizeof(DadosJogo), 1, arquivo);

    fclose(arquivo);

    return resultado == 1 &&
           strcmp(dados.identificacao, SAVE_MAGIC) == 0 &&
           dados.versao == SAVE_VERSAO;
}

void salvarJogo(void) {
    DadosJogo dados;
    memset(&dados, 0, sizeof(dados));

    strcpy(dados.identificacao, SAVE_MAGIC);
    dados.versao = SAVE_VERSAO;

    memcpy(dados.solucao, solucao, sizeof(solucao));
    memcpy(dados.tabuleiro, tabuleiro, sizeof(tabuleiro));
    memcpy(dados.fixo, fixo, sizeof(fixo));

    dados.erros = erros;
    dados.dicas = dicas;
    dados.dificuldade = dificuldade;
    dados.tempoAcumulado = obterTempoAtual();

    FILE *arquivo = fopen(ARQUIVO_SAVE, "wb");

    if (!arquivo) {
        printf("\nNao foi possivel salvar a partida.\n");
        return;
    }

    int resultado = fwrite(&dados, sizeof(DadosJogo), 1, arquivo);
    fclose(arquivo);

    if (resultado == 1) {
        tempoAcumulado = dados.tempoAcumulado;
        inicioSessao = time(NULL);
        printf("\nPartida salva com sucesso!\n");
    } else {
        printf("\nErro ao gravar o salvamento.\n");
    }
}

int carregarJogo(void) {
    FILE *arquivo = fopen(ARQUIVO_SAVE, "rb");
    if (!arquivo) return 0;

    DadosJogo dados;

    if (fread(&dados, sizeof(DadosJogo), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    if (strcmp(dados.identificacao, SAVE_MAGIC) != 0 ||
        dados.versao != SAVE_VERSAO ||
        dados.dificuldade < 1 || dados.dificuldade > 3 ||
        dados.erros < 0 || dados.dicas < 0 ||
        dados.tempoAcumulado < 0) {
        return 0;
    }

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (dados.solucao[i][j] < 1 ||
                dados.solucao[i][j] > 9 ||
                dados.tabuleiro[i][j] < 0 ||
                dados.tabuleiro[i][j] > 9 ||
                (dados.fixo[i][j] != 0 &&
                 dados.fixo[i][j] != 1)) {
                return 0;
            }

            if (dados.fixo[i][j] &&
                dados.tabuleiro[i][j] != dados.solucao[i][j]) {
                return 0;
            }
        }
    }

    memcpy(solucao, dados.solucao, sizeof(solucao));
    memcpy(tabuleiro, dados.tabuleiro, sizeof(tabuleiro));
    memcpy(fixo, dados.fixo, sizeof(fixo));

    erros = dados.erros;
    dicas = dados.dicas;
    dificuldade = dados.dificuldade;

    tempoAcumulado = dados.tempoAcumulado;
    inicioSessao = time(NULL);

    return 1;
}

void excluirJogoSalvo(void) {
    remove(ARQUIVO_SAVE);
}

/* =====================================================
   PROGRESSO
   ===================================================== */

int jogoCompleto(void) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] != solucao[i][j]) return 0;
        }
    }

    return 1;
}

int contarCasasVazias(void) {
    int vazias = 0;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == 0) vazias++;
        }
    }

    return vazias;
}

void verificarProgresso(void) {
    printf("\nCasas vazias: %d\n", contarCasasVazias());
    printf("Erros cometidos: %d\n", erros);
    printf("Dicas utilizadas: %d\n", dicas);

    if (contarCasasVazias() == 0 && jogoCompleto()) {
        printf("\nParabens! Sudoku resolvido!\n");
    } else {
        printf("\nA partida ainda nao terminou.\n");
    }

    pausar();
}

/* =====================================================
   INSERIR E APAGAR
   ===================================================== */

void inserirNumero(void) {
    int linha, coluna, numero;

    linha = lerInteiroComCronometro(
        "Digite a linha (1 a 9, ESC cancela): ");
    if (linha < 1 || linha > 9) return;

    coluna = lerInteiroComCronometro(
        "Digite a coluna (1 a 9): ");
    if (coluna < 1 || coluna > 9) return;

    numero = lerInteiroComCronometro(
        "Digite o numero (1 a 9): ");
    if (numero < 1 || numero > 9) return;

    linha--;
    coluna--;

    if (fixo[linha][coluna]) {
        printf("\nEssa posicao pertence ao Sudoku original.\n");
        Sleep(1000);
        return;
    }

    if (tabuleiro[linha][coluna] != 0) {
        printf("\nEssa casa ja esta preenchida. Apague primeiro.\n");
        Sleep(1000);
        return;
    }

    if (numero == solucao[linha][coluna]) {
        tabuleiro[linha][coluna] = numero;
        printf("\nNumero correto!\n");
        Sleep(600);
    } else {
        erros++;
        printf("\nNumero incorreto! Tente novamente.\n");
        Sleep(1000);
    }
}

void apagarNumero(void) {
    int linha, coluna;

    linha = lerInteiroComCronometro("Digite a linha (1 a 9): ");
    if (linha < 1 || linha > 9) return;

    coluna = lerInteiroComCronometro("Digite a coluna (1 a 9): ");
    if (coluna < 1 || coluna > 9) return;

    linha--;
    coluna--;

    if (fixo[linha][coluna]) {
        printf("\nNao e possivel apagar um numero original.\n");
        Sleep(1000);
        return;
    }

    if (tabuleiro[linha][coluna] == 0) {
        printf("\nEssa casa ja esta vazia.\n");
        Sleep(800);
        return;
    }

    tabuleiro[linha][coluna] = 0;
    printf("\nNumero apagado.\n");
    Sleep(600);
}

void receberDica(void) {
    int vazias[TAM * TAM][2];
    int quantidade = 0;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == 0) {
                vazias[quantidade][0] = i;
                vazias[quantidade][1] = j;
                quantidade++;
            }
        }
    }

    if (quantidade == 0) {
        printf("\nNao existem casas vazias.\n");
        Sleep(900);
        return;
    }

    int escolha = rand() % quantidade;
    int linha = vazias[escolha][0];
    int coluna = vazias[escolha][1];

    tabuleiro[linha][coluna] = solucao[linha][coluna];
    dicas++;

    printf("\nDica aplicada na linha %d, coluna %d.\n",
           linha + 1, coluna + 1);

    Sleep(1000);
}

/* =====================================================
   PARTIDA
   ===================================================== */

void jogar(void) {
    int opcao;

    while (1) {
        exibirTabuleiro();

        opcao = lerInteiroComCronometro("Escolha uma opcao: ");

        if (opcao == -1 || opcao == -2) continue;

        switch (opcao) {
            case 1:
                inserirNumero();
                break;

            case 2:
                apagarNumero();
                break;

            case 3:
                receberDica();
                break;

            case 4:
                verificarProgresso();
                break;

            case 5:
                salvarJogo();
                pausar();
                break;

            case 0:
                salvarJogo();
                printf("\nPartida salva. Ate a proxima!\n");
                Sleep(1000);
                return;

            default:
                printf("\nOpcao invalida.\n");
                Sleep(800);
                break;
        }

        if (jogoCompleto()) {
            exibirTabuleiro();

            cor(VERDE);
            printf("\n====================================\n");
            printf("       PARABENS! VOCE VENCEU!\n");
            printf("====================================\n");
            cor(PADRAO);

            char tempo[20];
            formatarTempo(obterTempoAtual(), tempo);

            printf("\nTempo final: %s\n", tempo);
            printf("Erros: %d\n", erros);
            printf("Dicas utilizadas: %d\n", dicas);

            excluirJogoSalvo();
            pausar();
            return;
        }
    }
}

/* =====================================================
   REGRAS
   ===================================================== */

void mostrarRegras(void) {
    limparTela();

    cor(BRANCO);

    printf("\n");
    printf("              REGRAS DO SUDOKU\n");
    printf("          =========================\n\n");

    printf("1. O tabuleiro possui 9 linhas e 9 colunas.\n");
    printf("2. Cada linha deve conter numeros de 1 a 9.\n");
    printf("3. Cada coluna deve conter numeros de 1 a 9.\n");
    printf("4. Cada bloco 3x3 deve conter numeros de 1 a 9.\n");
    printf("5. Nenhum numero pode se repetir na mesma linha,\n");
    printf("   coluna ou bloco 3x3.\n\n");

    printf("CORES:\n");
    cor(BRANCO);
    printf("Branco: numeros originais.\n");
    cor(VERDE);
    printf("Verde: numeros inseridos por voce.\n");
    cor(PADRAO);

    printf("\nCOMANDOS:\n");
    printf("1 - Inserir numero\n");
    printf("2 - Apagar numero\n");
    printf("3 - Receber dica\n");
    printf("4 - Verificar progresso\n");
    printf("5 - Salvar partida\n");
    printf("0 - Salvar e sair\n");

    pausar();
}

/* =====================================================
   MENU PRINCIPAL
   ===================================================== */

int main(void) {
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned int)time(NULL));

    int opcao;

    while (1) {
        limparTela();

        cor(BRANCO);

        printf("\n");
        printf("              S U D O K U\n");
        printf("          =====================\n\n");

        printf("  1 - Novo jogo\n");

        if (existeJogoSalvo()) {
            printf("  2 - Continuar partida salva\n");
        } else {
            printf("  2 - Continuar partida salva (indisponivel)\n");
        }

        printf("  3 - Regras do jogo\n");
        printf("  0 - Sair\n\n");

        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1: {
                if (existeJogoSalvo()) {
                    printf("\nJa existe uma partida salva.\n");
                    printf("Deseja iniciar outro jogo e substituir o salvamento?\n");
                    printf("1 - Sim\n");
                    printf("0 - Nao\n");

                    int confirmar = lerInteiro("Opcao: ");

                    if (confirmar != 1) break;
                }

                limparTela();

                printf("\nEscolha a dificuldade:\n\n");
                printf("1 - Facil\n");
                printf("2 - Medio\n");
                printf("3 - Dificil\n\n");

                dificuldade = lerInteiro("Opcao: ");

                if (dificuldade < 1 || dificuldade > 3) {
                    dificuldade = 1;
                }

                printf("\nGerando um novo Sudoku...\n");
                printf("Aguarde um momento.\n");

                gerarNovoSudoku();
                jogar();

                break;
            }

            case 2:
                if (carregarJogo()) {
                    printf("\nPartida carregada com sucesso!\n");
                    Sleep(800);
                    jogar();
                } else {
                    printf("\nNao foi possivel carregar a partida.\n");
                    printf("O arquivo pode estar ausente ou danificado.\n");
                    Sleep(1800);
                }
                break;

            case 3:
                mostrarRegras();
                break;

            case 0:
                limparTela();
                printf("\nObrigado por jogar Sudoku!\n");
                return 0;

            default:
                printf("\nOpcao invalida.\n");
                Sleep(800);
                break;
        }
    }

    return 0;
}
