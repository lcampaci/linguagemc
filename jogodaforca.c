#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#define MAX_PALAVRA 50
#define MAX_DICA 120

typedef struct {
    char palavra[MAX_PALAVRA];
    char dica[MAX_DICA];
    char tema[30];
} Pergunta;

Pergunta perguntas[] = {
    {"BATMAN", "O cavaleiro das trevas de Gotham.", "Super-herois"},
    {"HOMEM DE FERRO", "Tony Stark usa uma armadura tecnologica.", "Marvel"},
    {"HARRY POTTER", "Um bruxo famoso com uma cicatriz na testa.", "Filmes"},
    {"HOMEM ARANHA", "Um heroi que escala paredes e lanca teias.", "Marvel"},
    {"DARTH VADER", "Um dos maiores viloes de Star Wars.", "Star Wars"},
    {"PIKACHU", "O Pokemon eletrico mais famoso.", "Games e Anime"},
    {"MARIO", "Encanador famoso dos jogos da Nintendo.", "Games"},
    {"SONIC", "O ourico azul superveloz.", "Games"},
    {"GOKU", "Guerreiro Saiyajin de Dragon Ball.", "Anime"},
    {"NARUTO", "Ninja que sonha em ser Hokage.", "Anime"},
    {"BARBIE", "Boneca que virou estrela de cinema.", "Filmes"},
    {"WEDNESDAY", "Jovem da Familia Addams que ama misterio.", "Series"},
    {"BATATA FRITA", "Comida famosa, mas nao e personagem.", "Comida Pop"},
    {"STAR WARS", "Saga espacial criada por George Lucas.", "Filmes"},
    {"SUPERMAN", "Heroi de Krypton com capa vermelha.", "DC Comics"},
    {"HOMEM DE FERRO", "Vingador interpretado por Robert Downey Jr.", "Marvel"},
    {"FRODO", "Hobbit que precisa destruir um anel.", "Senhor dos Aneis"},
    {"SHREK", "O ogro verde que vive em um pantano.", "Animacao"},
    {"SPONGEBOB", "Esponja do mar que trabalha no Siri Cascudo.", "Desenhos"},
    {"MINECRAFT", "Jogo de blocos com creepers.", "Games"},
    // Filmes
    {"TITANIC", "Navio famoso de um filme romantico.", "Filmes"},
    {"AVATAR", "Filme com seres azuis chamados Na'vi.", "Filmes"},
    {"JURASSIC PARK", "Parque onde dinossauros voltam a vida.", "Filmes"},
    {"MATRIX", "Filme em que Neo descobre uma realidade simulada.", "Filmes"},
    {"PIRATAS DO CARIBE", "Saga de aventuras do capitao Jack Sparrow.", "Filmes"},
    {"FROZEN", "Animacao com Elsa e Anna.", "Disney"},

    // Series
    {"STRANGER THINGS", "Serie com Eleven e o Mundo Invertido.", "Series"},
    {"BREAKING BAD", "Professor de quimica se torna fabricante de drogas.", "Series"},
    {"LA CASA DE PAPEL", "Grupo realiza um grande assalto usando mascaras.", "Series"},
    {"GAME OF THRONES", "Serie de fantasia pela disputa do Trono de Ferro.", "Series"},
    {"THE OFFICE", "Comedia ambientada em um escritorio.", "Series"},
    {"ROUND 6", "Serie sul-coreana de jogos mortais.", "Series"},

    // Marvel e DC
    {"THOR", "Deus do trovao da Marvel.", "Marvel"},
    {"DEADPOOL", "Anti-heroi conhecido pelo humor e regeneracao.", "Marvel"},
    {"WOLVERINE", "Mutante com garras metalicas.", "Marvel"},
    {"CORINGA", "Arqui-inimigo do Batman.", "DC Comics"},
    {"MULHER MARAVILHA", "Heroina amazona com um laco magico.", "DC Comics"},
    {"FLASH", "Super-heroi conhecido pela supervelocidade.", "DC Comics"},

    // Games
    {"FORTNITE", "Jogo de batalha real com construcao.", "Games"},
    {"ROBLOX", "Plataforma de jogos criados pelos usuarios.", "Games"},
    {"POKEMON", "Franquia de treinadores e criaturas especiais.", "Games e Anime"},
    {"ZELDA", "Franquia de aventura protagonizada por Link.", "Games"},
    {"GOD OF WAR", "Kratos enfrenta deuses e criaturas mitologicas.", "Games"},
    {"FIFA", "Franquia de jogos de futebol, hoje chamada EA Sports FC.", "Games"},

    // Animes
    {"ONE PIECE", "Luffy procura o lendario tesouro dos piratas.", "Anime"},
    {"DEATH NOTE", "Caderno capaz de matar quem tem o nome escrito nele.", "Anime"},
    {"SAILOR MOON", "Heroina que luta em nome do amor e da justica.", "Anime"},
    {"DEMON SLAYER", "Tanjiro luta contra demonios para salvar sua irma.", "Anime"},
    {"ATTACK ON TITAN", "Humanidade luta contra gigantes humanoides.", "Anime"},

    // Desenhos e cultura pop
    {"TOM E JERRY", "Gato e rato vivem em uma perseguicao sem fim.", "Desenhos"},
    {"PATRICK ESTRELA", "Melhor amigo rosa do Bob Esponja.", "Desenhos"},

    // Harry Potter e fantasia
    {"HERMIONE GRANGER", "Uma descoberta sobre sua origem escolar muda o destino de um animal condenado.", "Harry Potter"},
    {"VOLDEMORT", "Dividiu sua alma em fragmentos para tentar escapar da morte.", "Harry Potter"},
    {"SEVERUS SNAPE", "Uma lembranca revelada perto do fim muda a interpretacao de suas escolhas.", "Harry Potter"},
    {"ALBUS DUMBLEDORE", "Escondeu informacoes importantes enquanto preparava um jovem para uma tarefa fatal.", "Harry Potter"},
    {"GOLLUM", "Uma obsessao de seculos alterou seu corpo e sua identidade.", "Fantasia"},
    {"GALADRIEL", "Recusou uma oferta de poder que poderia transforma-la em algo terrivel.", "Senhor dos Aneis"},

    // Marvel e DC
    {"DOUTOR ESTRANHO", "Uma decisao aparentemente impossivel depende de enxergar incontaveis futuros.", "Marvel"},
    {"PANTERA NEGRA", "Uma sucessao real depende de uma tradicao ligada a uma planta especial.", "Marvel"},
    {"FEITICEIRA ESCARLATE", "Sua dor pessoal altera as regras da realidade ao seu redor.", "Marvel"},
    {"LOKI", "Sua identidade familiar e o significado de seu destino mudam diversas vezes.", "Marvel"},
    {"THANOS", "Acreditava que uma catastrofe universal poderia ser resolvida por uma divisao radical.", "Marvel"},
    {"VISÃO", "Uma joia em sua testa esta ligada ao conflito sobre sua propria existencia.", "Marvel"},
    {"AQUAMAN", "Seu direito ao trono depende tanto de sua origem quanto de sua relacao com a superficie.", "DC Comics"},
    {"LEX LUTHOR", "Considera que a existencia de um salvador alienigena representa uma ameaca para a humanidade.", "DC Comics"},
    {"ROBIN", "O manto ja foi usado por diferentes pessoas, cada uma com uma historia propria.", "DC Comics"},

    // Star Wars e ficcao cientifica
    {"MANDALORIANO", "Um codigo de conduta entra em conflito com o afeto por um protegido.", "Star Wars"},
    {"BOBA FETT", "Sua reputacao foi construida em torno de rastrear alvos que poucos conseguem capturar.", "Star Wars"},
    {"PADME AMIDALA", "Uma lideranca politica e um romance secreto antecedem uma tragedia galactica.", "Star Wars"},
    {"OBI WAN KENOBI", "Seu maior fracasso esta ligado a um antigo aprendiz.", "Star Wars"},
    {"R2 D2", "Pequeno e incapaz de conversar como os humanos, guarda informacoes decisivas.", "Star Wars"},

    // Series e cinema
    {"SAUL GOODMAN", "Um advogado transforma sua capacidade de contornar regras em uma marca pessoal.", "Breaking Bad"},
    {"TYRION LANNISTER", "Sua maior arma politica costuma ser a palavra, nao a espada.", "Game of Thrones"},
    {"ELEVEN", "Um numero substitui seu nome durante parte de sua infancia em um laboratorio.", "Stranger Things"},
    {"SHELDON COOPER", "Seu conhecimento extraordinario contrasta com dificuldades nas convencoes sociais.", "The Big Bang Theory"},
    {"MICHAEL SCOTT", "Confunde frequentemente sua necessidade de aprovacao com habilidade de lideranca.", "The Office"},
    {"JACK SPARROW", "Sua bussola nao aponta para o norte, mas para seu desejo mais profundo.", "Piratas do Caribe"},
    {"NORMAN BATES", "A relacao com sua mae e central para um dos grandes misterios do suspense.", "Psicose"},
    {"FORREST GUMP", "Participa de momentos historicos sem compreender completamente sua importancia.", "Filmes"},
    {"VITO CORLEONE", "Seu poder se apoia em favores pessoais e em uma nocao particular de lealdade.", "O Poderoso Chefao"},

    // Games
    {"SOLID SNAKE", "Um especialista em infiltracao enfrenta conflitos envolvendo clones e identidade.", "Metal Gear"},
    {"LARA CROFT", "Explora tumbas e ruinas em busca de artefatos que outros desejam controlar.", "Tomb Raider"},
    {"KRATOS", "O passado de um guerreiro e marcado por cinzas e por uma vinganca contra deuses.", "God of War"},
    {"ARTHUR MORGAN", "Um fora da lei questiona sua lealdade quando seu grupo comeca a ruir.", "Red Dead Redemption"},
    {"ELLIE", "Sua imunidade a uma infeccao transforma sua existencia em objeto de disputa.", "The Last of Us"},
    {"SEPHIROTH", "Uma descoberta sobre sua origem desencadeia uma obsessao que ameaca o planeta.", "Final Fantasy"},
    {"ALAN WAKE", "Um escritor tenta compreender acontecimentos que parecem obedecer a uma narrativa.", "Alan Wake"},
    {"GLADOS", "Uma inteligencia artificial transforma testes cientificos em um jogo de sobrevivencia.", "Portal"},
    {"PAC MAN", "Sua jornada por labirintos depende de evitar perseguidores e coletar pequenos objetos.", "Arcade"},
    {"LITTLE NIGHTMARES", "Uma crianca minuscula atravessa ambientes dominados por figuras grotescas.", "Games"},

    // Anime e manga
    {"LIGHT YAGAMI", "Passa a julgar o mundo usando um poder que exige conhecer nomes e rostos.", "Death Note"},
    {"LELOUCH", "Uma habilidade de comando absoluto se torna a base de uma revolucao mascarada.", "Code Geass"},
    {"ITACHI UCHIHA", "Sua reputacao como traidor esconde uma decisao tomada para evitar um conflito maior.", "Naruto"},
    {"KILLUA ZOLDYCK", "Criado para matar, tenta construir uma vida distante das expectativas familiares.", "Hunter x Hunter"},
    {"SATORU GOJO", "Sua tecnica mais conhecida altera a maneira como adversarios conseguem alcanca-lo.", "Jujutsu Kaisen"},
    {"REI AYANAMI", "Sua identidade esta ligada a um projeto que mistura biologia, memoria e misterios cosmicos.", "Evangelion"},
    {"SPIKE SPIEGEL", "Um passado criminoso continua perseguindo um caca-recompensas espacial.", "Cowboy Bebop"},
    {"KEN KANEKI", "Uma cirurgia apos um acidente o coloca entre duas especies em conflito.", "Tokyo Ghoul"},

    // Animacao e cultura pop
    {"RICK SANCHEZ", "Um cientista brilhante trata o multiverso como algo cotidiano e descartavel.", "Rick and Morty"},
    {"BOJACK HORSEMAN", "Um antigo astro tenta lidar com a propria reputacao e com comportamentos autodestrutivos.", "Animacao"},
    {"WALL E", "Uma maquina solitaria continua trabalhando muito depois de a humanidade abandonar o planeta.", "Pixar"},
    {"REMY", "Seu talento culinario desafia uma barreira biologica e a desconfianca de uma profissao.", "Ratatouille"}
};

#define TOTAL (sizeof(perguntas) / sizeof(perguntas[0]))

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarForca(int erros) {
    printf("  +---+\n");
    printf("  |   |\n");

    printf("  |   %c\n", erros >= 1 ? 'O' : ' ');
    printf("  |  %c%c%c\n",
           erros >= 3 ? '/' : ' ',
           erros >= 2 ? '|' : ' ',
           erros >= 4 ? '\\' : ' ');
    printf("  |  %c %c\n",
           erros >= 5 ? '/' : ' ',
           erros >= 6 ? '\\' : ' ');
    printf(" _|_\n\n");
}

int letraUsada(char letra, char usadas[], int total) {
    for (int i = 0; i < total; i++) {
        if (usadas[i] == letra) {
            return 1;
        }
    }
    return 0;
}

int palavraCompleta(const char palavra[], const char descobertas[]) {
    for (int i = 0; palavra[i] != '\0'; i++) {
        if (palavra[i] != ' ' && !descobertas[i]) {
            return 0;
        }
    }
    return 1;
}

void mostrarPalavra(const char palavra[], const char descobertas[]) {
    printf("Palavra: ");

    for (int i = 0; palavra[i] != '\0'; i++) {
        if (palavra[i] == ' ') {
            printf("  ");
        } else if (descobertas[i]) {
            printf("%c ", palavra[i]);
        } else {
            printf("_ ");
        }
    }

    printf("\n");
}

int jogar(Pergunta pergunta) {
    int erros = 0;
    int maxErros = 6;
    char letra;
    char usadas[36];
    int totalUsadas = 0;
    int tamanho = strlen(pergunta.palavra);
    char descobertas[MAX_PALAVRA] = {0};

    while (erros < maxErros &&
           !palavraCompleta(pergunta.palavra, descobertas)) {

        limparTela();

        printf("================================\n");
        printf("    FORCA - MODO ESPECIALISTA\n");
        printf("================================\n\n");

        // A categoria fica escondida
        /*if (erros < 3) {
            printf("Dica bloqueada: erre 3 vezes para desbloquear!\n\n");
        } else {
            printf("DICA: %s\n\n", pergunta.dica);
        }*/

        printf("DICA: %s\n\n", pergunta.dica);

        mostrarForca(erros);
        mostrarPalavra(pergunta.palavra, descobertas);

        printf("\nLetras usadas: ");
        for (int i = 0; i < totalUsadas; i++) {
            printf("%c ", usadas[i]);
        }

        printf("\nErros: %d de %d\n", erros, maxErros);
        printf("Digite uma letra: ");

        if (scanf(" %c", &letra) != 1) {
            return 0;
        }

        letra = toupper((unsigned char)letra);

        if (!isalnum((unsigned char)letra)) {
            printf("Digite uma letra ou numero valido!\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            getchar();
            continue;
        }

        if (letraUsada(letra, usadas, totalUsadas)) {
            printf("Voce ja tentou essa letra!\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            getchar();
            continue;
        }

        usadas[totalUsadas++] = letra;

        int acertou = 0;

        for (int i = 0; i < tamanho; i++) {
            if (pergunta.palavra[i] == letra) {
                descobertas[i] = 1;
                acertou = 1;
            }
        }

        if (!acertou) {
            erros++;
            printf("\nLetra incorreta!\n");
        } else {
            printf("\nBoa! Voce acertou!\n");
        }

        printf("Pressione ENTER para continuar...");
        getchar();
        getchar();
    }

    limparTela();

    printf("================================\n");
    printf("    RESULTADO DA PARTIDA\n");
    printf("================================\n");

    mostrarForca(erros);
    mostrarPalavra(pergunta.palavra, descobertas);

    if (palavraCompleta(pergunta.palavra, descobertas)) {
        int pontos;

        if (erros <= 1) {
            pontos = 20;
        } else if (erros <= 3) {
            pontos = 10;
        } else {
            pontos = 5;
        }

        printf("\nExcelente! Voce descobriu a palavra!\n");
        printf("Resposta: %s\n", pergunta.palavra);
        printf("Pontos conquistados: %d\n", pontos);

        return pontos;
    }

    printf("\nFim de jogo! Voce nao descobriu a palavra.\n");
    printf("Resposta correta: %s\n", pergunta.palavra);

    return 0;
}

int main() {
    setlocale(LC_ALL, "");
    srand((unsigned int)time(NULL));

    int pontos = 0;
    int jogarNovamente = 1;
    int sorteadas[TOTAL] = {0};
    int totalSorteadas = 0;

    while (jogarNovamente) {

        // Reinicia o banco somente quando todas forem usadas
        if (totalSorteadas == TOTAL) {
            for (int i = 0; i < TOTAL; i++) {
                sorteadas[i] = 0;
            }

            totalSorteadas = 0;
        }

        int indice;

        do {
            indice = rand() % TOTAL;
        } while (sorteadas[indice]);

        sorteadas[indice] = 1;
        totalSorteadas++;

        limparTela();

        printf("================================\n");
        printf("    FORCA - CULTURA POP\n");
        printf("      MODO ESPECIALISTA\n");
        printf("================================\n");
        printf("Palavras descobertas: %d de %d\n",
               totalSorteadas - 1, TOTAL);
        printf("Pontuacao acumulada: %d\n\n", pontos);
        printf("Prepare-se para o desafio!\n");
        printf("Pressione ENTER para comecar...");

        getchar();

        pontos += jogar(perguntas[indice]);

        printf("\nPontuacao total: %d pontos\n", pontos);
        printf("\nDeseja jogar outra palavra?\n");
        printf("1 - Sim\n");
        printf("0 - Nao\n");
        printf("Escolha: ");

        if (scanf("%d", &jogarNovamente) != 1) {
            break;
        }

        getchar();

        if (jogarNovamente != 1) {
            jogarNovamente = 0;
        }
    }

    limparTela();

    printf("================================\n");
    printf("          FIM DE JOGO!\n");
    printf("================================\n");
    printf("Pontuacao final: %d pontos\n", pontos);
    printf("Obrigado por jogar!\n");

    return 0;
}
