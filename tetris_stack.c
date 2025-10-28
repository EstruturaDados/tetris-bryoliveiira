#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

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

Fila filaDePecas;      
int proximoId = 0;      

Peca gerarPeca(void);
void inicializarFila(void);
int filaEstaCheia(void);
int filaEstaVazia(void);
void enqueue(void);
void dequeue(void);
void exibirFila(void);
void exibirMenu(void);

int main() {
    srand(time(NULL));

    int opcao;

    inicializarFila();

    do {
        exibirFila();
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1; 
        }

        printf("\n"); 

        switch (opcao) {
            case 1:
                dequeue(); 
                break;
            case 2:
                enqueue(); 
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
    char tiposDePeca[] = {'I', 'O', 'T', 'L'}; 
    
    novaPeca.nome = tiposDePeca[rand() % 4];
    
    novaPeca.id = proximoId;
    proximoId++;
    
    return novaPeca;
}

void inicializarFila() {
    filaDePecas.inicio = 0;
    filaDePecas.fim = -1; 
    filaDePecas.contador = 0;

    printf("Inicializando fila com %d peças...\n", TAM_FILA);

    for (int i = 0; i < TAM_FILA; i++) {
        Peca p = gerarPeca();
        
        filaDePecas.fim = (filaDePecas.fim + 1) % TAM_FILA;
        filaDePecas.pecas[filaDePecas.fim] = p;
        filaDePecas.contador++;
    }
}

int filaEstaCheia() {
    return (filaDePecas.contador == TAM_FILA);
}

int filaEstaVazia() {
    return (filaDePecas.contador == 0);
}

void enqueue() {
    if (filaEstaCheia()) {
        printf("Ação: FALHA. A fila de próximas peças esta cheia!\n");
        printf("É preciso jogar uma peça (op 1) antes de inserir.\n");
    } else {
        Peca novaPeca = gerarPeca();
        
        filaDePecas.fim = (filaDePecas.fim + 1) % TAM_FILA;
        
        filaDePecas.pecas[filaDePecas.fim] = novaPeca;
        filaDePecas.contador++;
        
        printf("Ação: Nova peça [%c %d] inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
    }
}

void dequeue() {
    if (filaEstaVazia()) {
        printf("Ação: FALHA. A fila de peças esta vazia!\n");
        printf("é preciso inserir uma nova peça (op 2).\n");
    } else {
        Peca pecaJogada = filaDePecas.pecas[filaDePecas.inicio];
        
        filaDePecas.inicio = (filaDePecas.inicio + 1) % TAM_FILA;
        
        filaDePecas.contador--;
        
        printf("Ação: Peça [%c %d] jogada (removida da frente).\n", pecaJogada.nome, pecaJogada.id);
    }
}

void exibirFila() {
    printf("\n--- Estado Atual da Fila ---\n");
    
    if (filaEstaVazia()) {
        printf("Fila de peças: [ VAZIA ]\n");
    } else {
        printf("Fila de peças: ");
        
        int i = filaDePecas.inicio;
        
        for (int c = 0; c < filaDePecas.contador; c++) {
            printf("[%c %d] ", filaDePecas.pecas[i].nome, filaDePecas.pecas[i].id);
            
            i = (i + 1) % TAM_FILA;
        }
        printf("\n");
    }
    printf("-----------------------------\n");
}

void exibirMenu() {
    printf("\nOpções de ação:\n");
    printf(" 1 - Jogar peça (dequeue)\n");
    printf(" 2 - Inserir nova peça (enqueue)\n");
    printf(" 0 - Sair\n");
    printf("Escolha sua ação: ");
}