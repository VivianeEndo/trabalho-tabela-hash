//Estrutura programa Tabela Hash - Viviane Endo Mori
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TAMANHO_TABELA 100

struct Contato {
    char nome[100];
    char telefone[15];
    struct Contato* prox;
};

struct TabelaHash {
    struct Contato* tabela[MAX_TAMANHO_TABELA];
    double tempoInsercaoTotal;
    double tempoBuscaTotal;
    int contadorInsercao;
    int contadorBusca;
};

int funcaoHash(char* nome) {
    int hash = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        hash += nome[i];
    }
    return hash % MAX_TAMANHO_TABELA;
}

void inicializarTabela(struct TabelaHash* t) {
    for (int i = 0; i < MAX_TAMANHO_TABELA; i++) {
        t->tabela[i] = NULL;
    }
    t->tempoInsercaoTotal = 0;
    t->tempoBuscaTotal = 0;
    t->contadorInsercao = 0;
    t->contadorBusca = 0;
}

void adicionarContato(struct TabelaHash* t, char* nome, char* telefone) {
    clock_t start = clock();
    int index = funcaoHash(nome);
    struct Contato* novoContato = (struct Contato*)malloc(sizeof(struct Contato));
    strcpy(novoContato->nome, nome);
    strcpy(novoContato->telefone, telefone);
    novoContato->prox = t->tabela[index];
    t->tabela[index] = novoContato;
    clock_t end = clock();
    double tempoInsercao = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    t->tempoInsercaoTotal += tempoInsercao;
    t->contadorInsercao++;
    printf("Contato de %s adicionado em %.2f ms.\n", nome, tempoInsercao);
}

char* buscarContato(struct TabelaHash* t, char* nome) {
    clock_t start = clock();
    int index = funcaoHash(nome);
    struct Contato* atual = t->tabela[index];
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            clock_t end = clock();
            double tempoBusca = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            t->tempoBuscaTotal += tempoBusca;
            t->contadorBusca++;
            printf("Telefone de %s: %s (tempo de busca: %.2f ms)\n", nome, atual->telefone, tempoBusca);
            return atual->telefone;
        }
        atual = atual->prox;
    }
    clock_t end = clock();
    double tempoBusca = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    t->tempoBuscaTotal += tempoBusca;
    t->contadorBusca++;
    printf("Contato %s não encontrado. Tempo de busca: %.2f ms\n", nome, tempoBusca);
    return NULL;
}

void removerContato(struct TabelaHash* t, char* nome) {
    clock_t start = clock();
    int index = funcaoHash(nome);
    struct Contato* atual = t->tabela[index];
    struct Contato* anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (anterior == NULL) {
                t->tabela[index] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            clock_t end = clock();
            double tempoRemocao = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            printf("Contato %s removido em %.2f ms.\n", nome, tempoRemocao);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    clock_t end = clock();
    double tempoRemocao = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("Contato %s não encontrado. Tempo de remoção: %.2f ms\n", nome, tempoRemocao);
}

void exibirContatos(struct TabelaHash* t) {
    for (int i = 0; i < MAX_TAMANHO_TABELA; i++) {
        struct Contato* atual = t->tabela[i];
        while (atual != NULL) {
            printf("Nome: %s, Telefone: %s\n", atual->nome, atual->telefone);
            atual = atual->prox;
        }
    }
}

void exibirTempos(struct TabelaHash* t) {
    if (t->contadorInsercao > 0) {
        printf("Tempo médio de inserção: %.2f ms.\n", t->tempoInsercaoTotal / t->contadorInsercao);
    }
    if (t->contadorBusca > 0) {
        printf("Tempo médio de busca: %.2f ms.\n", t->tempoBuscaTotal / t->contadorBusca);
    }
}

int main() {
    struct TabelaHash listaTelefonica;
    inicializarTabela(&listaTelefonica);

    int opcao;
    char nome[100], telefone[15];

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Nome: ");
                getchar();
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Telefone: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = 0;
                adicionarContato(&listaTelefonica, nome, telefone);
                break;
            case 2:
                printf("Digite um nome para buscar: ");
                getchar();
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                buscarContato(&listaTelefonica, nome);
                break;
            case 3:
                printf("Digite um nome para remover: ");
                getchar();
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerContato(&listaTelefonica, nome);
                break;
            case 4:
                exibirContatos(&listaTelefonica);
                break;
            case 0:
                exibirTempos(&listaTelefonica);
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while(opcao != 0);

    return 0;
}
