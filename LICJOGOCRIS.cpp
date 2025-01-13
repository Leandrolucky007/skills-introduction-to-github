#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

#define MAX_RODADAS 20
#define NUM_DADOS 5
#define ARQUIVO_SALVAMENTO "salvamento.dat"

void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Estrutura para salvar o estado do jogo
typedef struct {
    char tabela[20];
    int rodada;
} EstadoJogo;

// Função para salvar o estado do jogo
void salvar_jogo(EstadoJogo* estado) {
    FILE* arquivo = fopen(ARQUIVO_SALVAMENTO, "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar o jogo.\n");
        return;
    }
    fwrite(estado, sizeof(EstadoJogo), 1, arquivo);
    fclose(arquivo);
    printf("Jogo salvo com sucesso!\n");
}

// Função para carregar o estado do jogo
int carregar_jogo(EstadoJogo* estado) {
    FILE* arquivo = fopen(ARQUIVO_SALVAMENTO, "rb");
    if (arquivo == NULL) {
        printf("Nenhum jogo salvo encontrado.\n");
        return 0; // Falha ao carregar o jogo
    }
    fread(estado, sizeof(EstadoJogo), 1, arquivo);
    fclose(arquivo);
    printf("Jogo carregado com sucesso!\n");
    return 1; // Jogo carregado com sucesso
}

// Regras do jogo
void instrucoes() {
    printf("Este jogo de dados tem uma estrutura semelhante à estrutura do jogo Yahtzee:\n");
    printf("O jogador lança os dados (5 dados de 6 faces pontuados de 1 a 6) e pode escolher\n");
    printf("e guardar quantos dados quiser podendo relançar os restantes; no máximo\n");
    printf("mais duas vezes por turno. Entre lançamentos, o jogador pode de novo\n");
    printf("escolher e guardar quaisquer dos dados obtidos ou previamente guardados.\n");
    printf("Pressione ENTER para continuar...");
    getchar();
    getchar();
    limpar_tela();
}

// Função para exibir dados graficamente
void mostrar_dados_graficos(int dados[]) {
    printf("\n=== Dados Atuais ===\n");
    for (int i = 0; i < NUM_DADOS; i++) {
        printf("[%d]", dados[i]);
    }
    printf("\n");
}

// Função para exibir a tabela de pontuação
void mostrar_tabela(char tabela[]) {
    printf("Jogador: |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |\n");
    printf("  Tu   : |");
    for (int i = 0; i < 20; i++) {
        printf("  %c |", tabela[i]);
    }
    printf("\n");
    printf("         |        50 pontos       |                50 pontos                   |          50 pontos          |\n");
}

// Função para rolar os dados
void rolar_dados(int dados[], int manter[]) {
    for (int i = 0; i < NUM_DADOS; i++) {
        if (!manter[i]) { // Apenas rola dados que não foram mantidos
            dados[i] = rand() % 6 + 1;
        }
    }
}

// Calcula a pontuação com base nos dados
int calcular_pontuacao(int dados[]) {
    int soma = 0;
    int frequencias[7] = {0}; // Frequências dos valores 1 a 6
    for (int i = 0; i < NUM_DADOS; i++) {
        frequencias[dados[i]]++;
    }
    // Soma os valores que não aparecem nos dados
    for (int i = 1; i <= 6; i++) {
        if (frequencias[i] == 0) {
            soma += i;
        }
    }
    return soma;
}

// Novo jogo
void novo_jogo(EstadoJogo* estado) {
    while (estado->rodada < MAX_RODADAS) {
        printf("Rodada %d de %d\n", estado->rodada + 1, MAX_RODADAS);
        mostrar_tabela(estado->tabela);

        int dados[NUM_DADOS] = {0};
        int manter[NUM_DADOS] = {0};

        for (int lancamento = 1; lancamento <= 3; lancamento++) {
            printf("Lancamento %d\n", lancamento);
            rolar_dados(dados, manter);
            mostrar_dados_graficos(dados);

            if (lancamento <= 3) {
                printf("Quais dados deseja manter? (1 para manter, 0 para rolar novamente)\n");
                for (int i = 0; i < NUM_DADOS; i++) {
                    printf("Dado %d: ", i + 1);
                    scanf("%d", &manter[i]);
                }
            }
        }

        int pontuacao = calcular_pontuacao(dados);
        printf("Pontuacao desta rodada: %d\n", pontuacao);

        if (estado->tabela[pontuacao - 1] == ' ') {
            estado->tabela[pontuacao - 1] = 'X'; // Marca a pontuação na tabela
        } else {
            printf("Pontuacao %d ja foi usada! Escolha outra opcao para inutilizar.\n", pontuacao);
            for (int i = 0; i < 20; i++) {
                if (estado->tabela[i] == ' ') {
                    estado->tabela[i] = '-'; // Inutiliza um espaço vazio
                    break;
                }
            }
        }

        estado->rodada++;
        limpar_tela();

        printf("Deseja salvar o jogo? (1-Sim, 0-Não): ");
        int salvar;
        scanf("%d", &salvar);
        if (salvar == 1) {
            salvar_jogo(estado);
        }
    }

    printf("Jogo terminado! Pontuacoes finais:\n");
    mostrar_tabela(estado->tabela);
}

// Menu principal
void menu_principal() {
    EstadoJogo estado;
    estado.rodada = 0;
    memset(estado.tabela, ' ', sizeof(estado.tabela));

    int opcao_menu;

    while (1) {
        printf("Escolha uma opcao:\n");
        printf("1-Regras do jogo\n");
        printf("2-Novo jogo\n");
        printf("3-Carregar jogo salvo\n");
        printf("4-Sair\n");
        scanf("%d", &opcao_menu);

        limpar_tela();

        if (opcao_menu == 1) {
            instrucoes();
        } else if (opcao_menu == 2) {
            novo_jogo(&estado);
        } else if (opcao_menu == 3) {
            if (carregar_jogo(&estado)) {
                novo_jogo(&estado);
            }
        } else if (opcao_menu == 4) {
            printf("Saindo do programa...\n");
            exit(0);
        } else {
            printf("Opcao invalida! Tente novamente.\n");
        }
    }
}

// Programa principal
int main() {
	setlocale(LC_ALL,"Portuguese");
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    limpar_tela();
    menu_principal();
    return 0;
}
	
