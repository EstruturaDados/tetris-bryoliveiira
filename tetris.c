#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int contador;
} Fila;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

Fila filaDePecas;
Pilha pilhaDeReserva;
int proximoId = 0;

Peca gerarPeca(void);
void inicializarFila(void);
void inicializarPilha(void);

int filaEstaCheia(void);
int filaEstaVazia(void);
int pilhaEstaCheia(void);
int pilhaEstaVazia(void);

Peca helper_dequeue(void);
void helper_enqueue(Peca p);

void acao_jogarPeca(void);
void acao_reservarPeca(void);
void acao_usarPecaReservada(void);

void exibirEstado(void);
void exibirFila(void);
void exibirPilha(void);
void exibirMenu(void);

int main() {
    srand(time(NULL));
    int opcao;

    inicializarFila();
    inicializarPilha();

    do {
        exibirEstado();
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }
        printf("\n");

        switch (opcao) {
            case 1:
                acao_jogarPeca();
                break;
            case 2:
                acao_reservarPeca();
                break;
            case 3:
                acao_usarPecaReservada();
                break;
            case 0:
                printf("Saindo do Tetris Stack...\n");
                break;
            default:
                printf("Opção invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

Peca gerarPeca() {
    Peca novaPeca;
    char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    
    novaPeca.nome = tiposDePeca[rand() % 7];
    novaPeca.id = proximoId++;
    return novaPeca;
}

void inicializarFila() {
    filaDePecas.inicio = 0;
    filaDePecas.fim = -1;
    filaDePecas.contador = 0;

    printf("Inicializando fila com %d peças...\n", TAM_FILA);
    for (int i = 0; i < TAM_FILA; i++) {
        helper_enqueue(gerarPeca());
    }
}

void inicializarPilha() {
    pilhaDeReserva.topo = -1;
}

int filaEstaCheia() {
    return (filaDePecas.contador == TAM_FILA);
}

int filaEstaVazia() {
    return (filaDePecas.contador == 0);
}

int pilhaEstaCheia() {
    return (pilhaDeReserva.topo == TAM_PILHA - 1);
}

int pilhaEstaVazia() {
    return (pilhaDeReserva.topo == -1);
}

Peca helper_dequeue(void) {
    Peca pecaRemovida = filaDePecas.pecas[filaDePecas.inicio];
    filaDePecas.inicio = (filaDePecas.inicio + 1) % TAM_FILA;
    filaDePecas.contador--;
    return pecaRemovida;
}

void helper_enqueue(Peca p) {
    if (filaEstaCheia()) {
        printf("ERRO INTERNO: Tentativa de enfileirar em fila cheia.\n");
        return;
    }
    filaDePecas.fim = (filaDePecas.fim + 1) % TAM_FILA;
    filaDePecas.pecas[filaDePecas.fim] = p;
    filaDePecas.contador++;
}

void acao_jogarPeca(void) {
    if (filaEstaVazia()) {
        printf("Ação: FALHA. A fila de peças esta vazia!\n");
        return;
    }

    Peca pecaJogada = helper_dequeue();
    printf("Ação: Peça [%c %d] jogada (removida da frente).\n", pecaJogada.nome, pecaJogada.id);
    
    helper_enqueue(gerarPeca());
}

void acao_reservarPeca(void) {
    if (pilhaEstaCheia()) {
        printf("Ação: FALHA. A pilha de reserva esta cheia! (Max: %d)\n", TAM_PILHA);
        return;
    }
    if (filaEstaVazia()) {
        printf("Ação: FALHA. A fila de peças esta vazia!\n");
        return;
    }

    Peca pecaReservada = helper_dequeue();
    pilhaDeReserva.pecas[++pilhaDeReserva.topo] = pecaReservada;
    
    printf("Ação: Peça [%c %d] movida da fila para a reserva.\n", pecaReservada.nome, pecaReservada.id);

    helper_enqueue(gerarPeca());
}

void acao_usarPecaReservada(void) {
    if (pilhaEstaVazia()) {
        printf("Ação: FALHA. A pilha de reserva esta vazia!\n");
        return;
    }

    Peca pecaUsada = pilhaDeReserva.pecas[pilhaDeReserva.topo--];
    printf("Ação: Peça reservada [%c %d] usada (removida da pilha).\n", pecaUsada.nome, pecaUsada.id);

    helper_enqueue(gerarPeca());
}

void exibirEstado() {
    printf("\n--- Estado Atual do Jogo ---\n");
    exibirFila();
    exibirPilha();
    printf("----------------------------\n");
}

void exibirFila() {
    printf("Fila de peças: ");
    if (filaEstaVazia()) {
        printf("[ VAZIA ]\n");
    } else {
        int i = filaDePecas.inicio;
        for (int c = 0; c < filaDePecas.contador; c++) {
            printf("[%c %d] ", filaDePecas.pecas[i].nome, filaDePecas.pecas[i].id);
            i = (i + 1) % TAM_FILA;
        }
        printf("\n");
    }
}

void exibirPilha() {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaEstaVazia()) {
        printf("[ VAZIA ]\n");
    } else {
        for (int i = pilhaDeReserva.topo; i >= 0; i--) {
            printf("[%c %d] ", pilhaDeReserva.pecas[i].nome, pilhaDeReserva.pecas[i].id);
        }
        printf("\n");
    }
}

void exibirMenu() {
    printf("\nOpções de ação:\n");
    printf(" 1 - Jogar peça (da fila)\n");
    printf(" 2 - Reservar peça (fila -> pilha)\n");
    printf(" 3 - Usar peça reservada (da pilha)\n");
    printf(" 0 - Sair\n");
    printf("Escolha sua ação: ");
}
