#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 50
#define MAXFILA 14

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
typedef struct Sala
{
    char nome[MAXSTRING];
    struct Sala* esquerda;
    struct Sala* direita;
    int depth;
}Sala;

typedef struct Fila{
    Sala* comodos[MAXFILA];
    int inicio;
    int fim;
    int total;
}Fila;

void conectarSalas(Sala *raiz);
Sala* criarSala(char* nome);
Sala* explorarSalas(Sala *s, char direction);
Sala* dequeue(Fila *f);
void enqueue(Fila *f, Sala *s);
void iniciarFila(Fila *f);
void freeArvore(Sala *s);

char* const comodos[] = {"Corredor","Sala","Quarto","Banheiro","Suite","Cozinha","Garagem","Varanda","Sotao","Biblioteca","Sauna","Churrasqueira","Quato Visita"};
int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.
    Sala* raiz =  criarSala("Entrada");
    conectarSalas(raiz);
    char direction;
    Sala *salaAtual = raiz;
    do{
        printf("Voce esta na comodo %s\n", salaAtual->nome);
        printf("d -> ir para a Direita\n");
        printf("e -> ir para a Esquerda\n");
        printf("s -> Sair\n");
        printf("Escolha uma Direcao: ");
        scanf("%c", &direction);
        getchar();

        if(direction == 'd')
        {
            salaAtual = explorarSalas(salaAtual, 'r');
            if(salaAtual == NULL)
            {
                printf("Voce atingiu a ultima sala\n");
                break;
            }
        }
        else if(direction == 'e')
        {
            salaAtual = explorarSalas(salaAtual, 'l');
            if(salaAtual == NULL)
            {
                printf("Voce atingiu a ultima sala\n");
                break;
            }
        }

    }while(direction != 's');
    freeArvore(raiz);

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}
void iniciarFila(Fila *f)
{
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}
void enqueue(Fila *f, Sala *s){
    f->comodos[f->fim] = s;
    f->fim = (f->fim +1);
    f->total++;
}

Sala* dequeue(Fila *f)
{
    Sala* deqSala = f->comodos[f->inicio + 1];
    f->inicio = (f->inicio + 1);
    f->total--;
    return deqSala;
}


Sala* criarSala(char* nome)
{
    Sala* novaSala = malloc(sizeof(struct Sala));
    if(novaSala == NULL) return NULL;
    strcpy(novaSala->nome, nome);
    novaSala->direita = NULL;
    novaSala->esquerda = NULL;
    return novaSala;    
}
/**
 * Esta função conectarSalas() percorre a lista nomes de cômodos e cria novas salas para cada um deles.
 * Para manter a organização em forma de árvore binária, é utilizada uma Fila (FIFO) que garante
 * que as salas sejam conectadas em ordem de nível (level-order), semelhante ao preenchimento de
 * uma árvore binária completa.
 *
 * Funcionamento:
 * - A sala raiz é colocada na fila.
 * - Para cada novo cômodo, cria-se uma nova Sala e ela é enfileirada.
 * - O elemento no início da fila (atual) é preenchido primeiro à esquerda, depois à direita.
 * - Quando ambos os filhos de 'atual' estão ocupados, retira-se o próximo elemento da fila,
 *   garantindo a expansão em largura.
 *
 * O uso da Fila é essencial para manter a estrutura organizada e evitar conexões incorretas,
 * permitindo que a árvore de salas seja construída de forma ordenada e previsível.
 *
 */

void conectarSalas(Sala *raiz)
{
    int total = sizeof(comodos) / sizeof(comodos[0]);
    Sala* atual = raiz;
    Fila f;
    iniciarFila(&f);
    enqueue(&f, raiz);
    for(int i = 0; i < total; i++)
    {
        Sala* novoComodo = criarSala(comodos[i]);
        enqueue(&f, novoComodo);
        if(atual->esquerda == NULL)
        {
            atual->esquerda = novoComodo;
        }
        else if(atual->direita == NULL)
        {
            atual->direita = novoComodo;
        }
        
        else {
            atual = dequeue(&f);
            atual->esquerda = novoComodo;
        }
    }
}

Sala* explorarSalas(Sala *s, char direction)
{
    if(direction == 'l' && s->esquerda != NULL)
    {
        return s->esquerda;
    }
    if(direction == 'r' && s->direita != NULL)
    {
        return s->direita;
    }
    
    return NULL;
    
}

void freeArvore(Sala *s)
{
    if(s->esquerda)
    {
        freeArvore(s->esquerda);
    }
    if(s->direita)
    {
        freeArvore(s->direita);
    }
    free(s);
    return;
}