#include <stdio.h>
#include <stdlib.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ---------- PEÇA ----------
typedef struct {
    int id;
} Peca;

// ---------- FILA ----------
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ---------- PILHA ----------
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// ---------- SISTEMA DE UNDO ----------
typedef struct {
    int operacao;   // 1 = jogada, 2 = reserva, 3 = usar reserva, 4 = troca
    Peca p1;        // peça removida/afetada
    Peca p2;        // segunda peça (para troca)
    int valido;
} Undo;

// ---------- GERADOR DE PEÇAS ----------
Peca gerarPeca() {
    static int contador = 1;
    Peca p;
    p.id = contador++;
    return p;
}

// ---------- FILA ----------
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

int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int filaVazia(Fila *f) { return f->quantidade == 0; }

Peca dequeue(Fila *f) {
    Peca removida = f->elementos[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

void enqueue(Fila *f) {
    f->elementos[f->tras] = gerarPeca();
    f->tras = (f->tras + 1) % TAM_FILA;
    f->quantidade++;
}

// ---------- PILHA ----------
void inicializarPilha(Pilha *p) { p->topo = -1; }

int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n⚠️ Pilha cheia!\n");
        return;
    }
    p->topo++;
    p->elementos[p->topo] = x;
    printf("\n✓ Peça ID %d reservada!\n", x.id);
}

Peca pop(Pilha *p) {
    Peca retirada = p->elementos[p->topo];
    p->topo--;
    return retirada;
}

// ---------- EXIBIÇÃO ----------
void exibirFila(Fila *f) {
    printf("\n📌 FILA (5 peças):\nFrente -> ");
    int idx = f->frente;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[ID:%d] ", f->elementos[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("<- Trás\n");
}

void exibirPilha(Pilha *p) {
    printf("\n📌 PILHA (reservas máx 3):\n");
    if (pilhaVazia(p)) {
        printf("[ Pilha vazia ]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[ID:%d]\n", p->elementos[i].id);
    }
}

// ---------- NOVAS FUNÇÕES AVANÇADAS ----------

// 1) Trocar peça do topo da pilha com a da frente da fila
void trocar(Fila *f, Pilha *p, Undo *u) {
    if (pilhaVazia(p)) {
        printf("\n⚠️ Não há peça na pilha para trocar!\n");
        return;
    }

    Peca *fila_frente = &f->elementos[f->frente];
    Peca *pilha_topo = &p->elementos[p->topo];

    // Salvar histórico
    u->operacao = 4;
    u->p1 = *fila_frente;
    u->p2 = *pilha_topo;
    u->valido = 1;

    // Troca
    Peca temp = *fila_frente;
    *fila_frente = *pilha_topo;
    *pilha_topo = temp;

    printf("\n🔄 Troca realizada entre Fila(ID %d) e Pilha(ID %d)\n",
           fila_frente->id, pilha_topo->id);
}

// 2) Desfazer última operação
void desfazer(Fila *f, Pilha *p, Undo *u) {
    if (!u->valido) {
        printf("\n⚠️ Nada a desfazer!\n");
        return;
    }

    switch (u->operacao) {

        case 1: // desfazer jogada
            f->frente = (f->frente - 1 + TAM_FILA) % TAM_FILA;
            f->elementos[f->frente] = u->p1;
            f->quantidade++;
            printf("\n↩️ Jogada desfeita!\n");
            break;

        case 2: // desfazer reserva
            p->topo--;
            f->frente = (f->frente - 1 + TAM_FILA) % TAM_FILA;
            f->elementos[f->frente] = u->p1;
            f->quantidade++;
            printf("\n↩️ Reserva desfeita!\n");
            break;

        case 3: // desfazer uso de peça reservada
            p->topo++;
            p->elementos[p->topo] = u->p1;
            printf("\n↩️ Uso de reserva desfeito!\n");
            break;

        case 4: { // desfazer troca
            Peca *fila_frente = &f->elementos[f->frente];
            Peca *pilha_topo = &p->elementos[p->topo];
            *fila_frente = u->p1;
            *pilha_topo = u->p2;
            printf("\n↩️ Troca desfeita!\n");
        } break;
    }

    u->valido = 0;
}

// 3) Inverter fila com pilha
void inverter(Fila *f, Pilha *p) {
    Pilha temp;
    temp.topo = -1;

    // Transferir fila -> temp
    int idx = f->frente;
    for (int i = 0; i < f->quantidade; i++) {
        push(&temp, f->elementos[idx]);
        idx = (idx + 1) % TAM_FILA;
    }

    // Transferir pilha -> fila
    int count = 0;
    while (!pilhaVazia(p) && count < TAM_FILA) {
        f->elementos[(f->frente + count) % TAM_FILA] = pop(p);
        count++;
    }

    // Transferir temp -> pilha
    while (!pilhaVazia(&temp)) {
        if (!pilhaCheia(p))
            push(p, pop(&temp));
    }

    printf("\n🔁 Fila e pilha invertidas com sucesso!\n");
}

// ---------- MENU ----------
int menu() {
    int op;
    printf("\n===== MENU MESTRE =====\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("4 - Trocar topo da pilha com a frente da fila\n");
    printf("5 - Desfazer última ação\n");
    printf("6 - Inverter fila com pilha\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    return op;
}

// ---------- MAIN ----------
int main() {
    Fila fila;
    Pilha pilha;
    Undo undo = { .valido = 0 };

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        opcao = menu();

        switch (opcao) {

            case 1: { // Jogar peça
                if (filaVazia(&fila)) {
                    printf("\n⚠️ Fila vazia!\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("\n✓ Peça jogada: ID %d\n", jogada.id);

                    undo.operacao = 1;
                    undo.p1 = jogada;
                    undo.valido = 1;

                    enqueue(&fila);
                }
                break;
            }

            case 2: { // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️ Pilha cheia!\n");
                } else {
                    Peca r = dequeue(&fila);
                    push(&pilha, r);

                    undo.operacao = 2;
                    undo.p1 = r;
                    undo.valido = 1;

                    enqueue(&fila);
                }
                break;
            }

            case 3: { // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️ Nada para usar!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\n✓ Peça usada: ID %d\n", usada.id);

                    undo.operacao = 3;
                    undo.p1 = usada;
                    undo.valido = 1;
                }
                break;
            }

            case 4:
                trocar(&fila, &pilha, &undo);
                break;

            case 5:
                desfazer(&fila, &pilha, &undo);
                break;

            case 6:
                inverter(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\n⚠️ Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
