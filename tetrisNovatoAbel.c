#include <stdio.h>
#include <stdlib.h>

#define TAM 5   // tamanho fixo da fila

// Struct da peça
typedef struct {
    int id;   // apenas um número identificador para simplificar
} Peca;

// Struct da fila circular
typedef struct {
    Peca elementos[TAM];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Gera automaticamente uma nova peça
Peca gerarPeca() {
    static int contador = 1;
    Peca nova;
    nova.id = contador++;
    return nova;
}

// Inicializa a fila com 5 peças
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;

    for (int i = 0; i < TAM; i++) {
        f->elementos[f->tras] = gerarPeca();
        f->tras = (f->tras + 1) % TAM;
        f->quantidade++;
    }
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAM;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Enfileirar peça
void enqueue(Fila *f) {
    if (filaCheia(f)) {
        printf("\n⚠️ A fila está cheia! Não é possível inserir.\n");
        return;
    }
    f->elementos[f->tras] = gerarPeca();
    f->tras = (f->tras + 1) % TAM;
    f->quantidade++;
    printf("\n✓ Peça inserida com sucesso!\n");
}

// Desenfileirar peça
void dequeue(Fila *f) {
    if (filaVazia(f)) {
        printf("\n⚠️ A fila está vazia! Nada para remover.\n");
        return;
    }

    Peca removida = f->elementos[f->frente];
    printf("\n✓ Peça removida: ID = %d\n", removida.id);

    f->frente = (f->frente + 1) % TAM;
    f->quantidade--;
}

// Exibir a fila atual
void exibirFila(Fila *f) {
    printf("\n📌 ESTADO ATUAL DA FILA:\n");

    if (filaVazia(f)) {
        printf("[ Fila vazia ]\n");
        return;
    }

    printf("Frente -> ");
    int idx = f->frente;

    for (int i = 0; i < f->quantidade; i++) {
        printf("[ID:%d] ", f->elementos[idx].id);
        idx = (idx + 1) % TAM;
    }

    printf("<- Trás\n");
}

// Menu principal
int menu() {
    int op;
    printf("\n===== MENU FILA CIRCULAR =====\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Inserir nova peça (enqueue)\n");
    printf("3 - Visualizar fila\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    return op;
}

int main() {
    Fila fila;
    inicializarFila(&fila);

    int opcao;

    do {
        exibirFila(&fila);
        opcao = menu();

        switch (opcao) {
            case 1:
                dequeue(&fila);
                break;
            case 2:
                enqueue(&fila);
                break;
            case 3:
                exibirFila(&fila);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
