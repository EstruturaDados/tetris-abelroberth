#include <stdio.h>
#include <stdlib.h>

#define TAM_FILA 5      // Fila sempre terá 5 peças
#define TAM_PILHA 3     // Pilha comporta no máximo 3 peças

// Struct da peça
typedef struct {
    int id;   // apenas um identificador numérico
} Peca;

// -------- FILA CIRCULAR ---------
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// -------- PILHA LINEAR ----------
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

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

    for (int i = 0; i < TAM_FILA; i++) {
        f->elementos[f->tras] = gerarPeca();
        f->tras = (f->tras + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// Inicializa pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica fila
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int filaVazia(Fila *f) { return f->quantidade == 0; }

// Verifica pilha
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

// Remove peça da fila (DEQUEUE)
Peca dequeue(Fila *f) {
    Peca removida = f->elementos[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// Insere peça nova na fila (mantém sempre cheia)
void enqueue(Fila *f) {
    if (filaCheia(f)) return; // nunca deveria acontecer
    f->elementos[f->tras] = gerarPeca();
    f->tras = (f->tras + 1) % TAM_FILA;
    f->quantidade++;
}

// PUSH na pilha
void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n⚠️ A pilha está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->elementos[p->topo] = x;
    printf("\n✓ Peça ID %d reservada com sucesso!\n", x.id);
}

// POP da pilha
Peca pop(Pilha *p) {
    Peca retirada = p->elementos[p->topo];
    p->topo--;
    return retirada;
}

// Exibir fila
void exibirFila(Fila *f) {
    printf("\n📌 FILA (sempre cheia com 5 peças):\n");
    printf("Frente -> ");

    int idx = f->frente;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[ID:%d] ", f->elementos[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }

    printf("<- Trás\n");
}

// Exibir pilha
void exibirPilha(Pilha *p) {
    printf("\n📌 PILHA DE RESERVA (capacidade 3):\n");

    if (pilhaVazia(p)) {
        printf("[ Pilha vazia ]\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[ID:%d]\n", p->elementos[i].id);
    }
}

// Menu
int menu() {
    int op;
    printf("\n===== MENU =====\n");
    printf("1 - Jogar peça (remove da fila)\n");
    printf("2 - Reservar peça (push na pilha)\n");
    printf("3 - Usar peça reservada (pop da pilha)\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    return op;
}

int main() {
    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        opcao = menu();

        switch (opcao) {

            case 1: {
                // Jogar peça da frente
                if (filaVazia(&fila)) {
                    printf("\n⚠️ A fila está vazia!\n");
                } else {
                    Peca removida = dequeue(&fila);
                    printf("\n✓ Peça jogada: ID %d\n", removida.id);
                    enqueue(&fila); // mantém a fila sempre cheia
                }
                break;
            }

            case 2: {
                // Reservar peça
                if (filaVazia(&fila)) {
                    printf("\n⚠️ Fila vazia! Nada para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\n⚠️ Pilha cheia! Não é possível reservar.\n");
                } else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    enqueue(&fila); // mantém fila cheia
                }
                break;
            }

            case 3: {
                // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️ Nenhuma peça reservada!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\n✓ Peça usada: ID %d\n", usada.id);
                }
                break;
            }

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
