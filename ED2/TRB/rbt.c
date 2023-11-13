#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Cor;

typedef struct sNo {
  int valor;
  Cor cor;
  struct sNo* pai, * esquerda, * direita;
} No;

typedef struct {
  No* raiz;
} RedBlackTree;

//prototipos de funcões
No* alocaMemoriaNo(); //aloca memoria para um sNo
RedBlackTree* alocaMemoriaRBT(); //aloca memoria para uma RedBlackTree
void liberaNo(No*); //libera memoria de um sNo
void liberaArvore(RedBlackTree*); //libera memoria de uma RedBlackTree
No* criaNo(int); //cria um novo sNo
RedBlackTree* criaArvoreRBT(); //cria uma nova RedBlackTree
void inOrder(No*); // percorre a arvore em ordem
void transplant(RedBlackTree*, No*, No*); //mudar de cor
void inserirNo(RedBlackTree*, int); //insere um sNo na RedBlackTree
void balancearInsercao(RedBlackTree*, No*); // balanceia a inserção
No* rotacaoEsquerda(RedBlackTree*, No*); // rotaciona para a esquerda
No* rotacaoDireita(RedBlackTree*, No*); // rotaciona para a direita
No* minimoNode(No*); //encontra o no com o menor valor
No* encontrarNo(RedBlackTree*, int); //encontra um no
void removerNo(RedBlackTree*, int); //remove um no
void balancearRemocao(RedBlackTree*, No*); //balanceia a remoção
void testRBT(); //testa todos os aspectos da arvore rubro-negra

// função para alocar memória para um nó
No* alocaMemoriaNo() {
  No* sNo = (No*)malloc(sizeof(No));
  if (sNo == NULL) {
    fprintf(stderr, "Erro ao alocar memória para o sNo!\n");
    return NULL;
  }
  return sNo;
}

// função para alocar memória para uma árvore
RedBlackTree* alocaMemoriaRBT() {
  RedBlackTree* arvore = (RedBlackTree*)malloc(sizeof(RedBlackTree));
  if (arvore == NULL) {
    printf("Erro ao alocar memória para a RedBlackTree!\n");
    return NULL;
  }
  return arvore;
}

// função para liberar memória de um nó
void liberaNo(No* no) {
  if (no != NULL) {
    liberaNo(no->esquerda);
    liberaNo(no->direita);
    free(no);
  }
}

// função para liberar memória de uma árvore
void liberaArvore(RedBlackTree* arvore) {
  if (arvore != NULL) {
    liberaNo(arvore->raiz);
    free(arvore);
  }
}

// função para criar um novo nó
No* criaNo(int valor) {
  No* no = alocaMemoriaNo();
  if (no == NULL) {
    fprintf(stderr, "Erro ao criar novo no!\n");
    return NULL;
  }
  no->valor = valor;
  no->cor = RED;
  no->pai = NULL;
  no->esquerda = NULL;
  no->direita = NULL;
  return no;
}

// função para criar uma nova árvore
RedBlackTree* criaArvoreRBT() {
  RedBlackTree* novo = alocaMemoriaRBT();
  if (novo == NULL) {
    fprintf(stderr, "Erro ao criar nova RedBlackTree!\n");
    exit(EXIT_FAILURE);
  }
  novo->raiz = NULL;
  return novo;
}

// função para percorrer a árvore em ordem
void inOrder(No* sNo) {
  if (sNo != NULL) {
    inOrder(sNo->esquerda);
    printf("%d (%s)\n", sNo->valor, sNo->cor == RED ? "RED" : "BLACK");
    inOrder(sNo->direita);
  }
}

// função para mudar de cor
void transplant(RedBlackTree* RedBlackTree, No* no1, No* no2) {
  if (no1->pai == NULL) {
    RedBlackTree->raiz = no2;
  }
  else if (no1 == no1->pai->esquerda) {
    no1->pai->esquerda = no2;
  }
  else {
    no1->pai->direita = no2;
  }
  if (no2 != NULL) {
    no2->pai = no1->pai;
  }
}

// função para inserir um nó na árvore
void inserirNo(RedBlackTree* RedBlackTree, int valor) {
  No* novoNo = criaNo(valor);
  if (novoNo == NULL) {
    fprintf(stderr, "Erro ao criar novo nó!\n");
    return;
  }

  No* pai = NULL;
  No* atual = RedBlackTree->raiz;

  // Encontra o nó pai do novo nó
  while (atual != NULL) {
    pai = atual;
    if (novoNo->valor < atual->valor) {
      atual = atual->esquerda;
    }
    else {
      atual = atual->direita;
    }
  }

  // o pai do novo nó é nulo
  novoNo->pai = pai;

  if (pai == NULL) { // arvore vazia
    RedBlackTree->raiz = novoNo;
  } // se o valor do novo nó é menor que o valor do pai, o novo nó é filho esquerdo
  else if (novoNo->valor < pai->valor) {
    pai->esquerda = novoNo;
  } // se o valor do novo nó é maior que o valor do pai, o novo nó é filho direito
  else {
    pai->direita = novoNo;
  }
  // balancear a inserção com as propriedades da árvore rubro-negra
  balancearInsercao(RedBlackTree, novoNo);
}

// função para balancear a inserção
void balancearInsercao(RedBlackTree* RedBlackTree, No* no) {
  while (no != RedBlackTree->raiz && no->pai->cor == RED) {
    if (no->pai == no->pai->pai->esquerda) {
      No* tioNode = no->pai->pai->direita; // tio do nó

      //caso 1: tio é vermelho
      if (tioNode != NULL && tioNode->cor == RED) {
        no->pai->cor = BLACK;
        tioNode->cor = BLACK;
        no->pai->pai->cor = RED;
        no = no->pai->pai;
      }
      else {
        //caso 2: tio é preto e nó é filho direito
        if (no == no->pai->direita) {
          no = no->pai;
          no = rotacaoEsquerda(RedBlackTree, no);
        }
        //caso 3: tio é preto e nó é filho esquerdo
        no->pai->cor = BLACK;
        no->pai->pai->cor = RED;
        no = rotacaoDireita(RedBlackTree, no->pai->pai);
      }
    }
    else {
      No* tioNode = no->pai->pai->esquerda; // caso simétrico: tio é filho esquerdo

      if (tioNode != NULL && tioNode->cor == RED) {
        no->pai->cor = BLACK;
        tioNode->cor = BLACK;
        no->pai->pai->cor = RED;
        no = no->pai->pai;
      }
      else {
        if (no == no->pai->esquerda) {
          no = no->pai;
          no = rotacaoDireita(RedBlackTree, no);
        }
        no->pai->cor = BLACK;
        no->pai->pai->cor = RED;
        no = rotacaoEsquerda(RedBlackTree, no->pai->pai);
      }
    }
  }
  RedBlackTree->raiz->cor = BLACK;
}

// função para rotacionar para a esquerda
No* rotacaoEsquerda(RedBlackTree* RedBlackTree, No* no) {
  No* filhoDireito = no->direita;
  no->direita = filhoDireito->esquerda;

  if (filhoDireito->esquerda != NULL) {
    filhoDireito->esquerda->pai = no;
  }

  filhoDireito->pai = no->pai;

  if (no->pai == NULL) { // Se o nó é a raiz
    RedBlackTree->raiz = filhoDireito;
  }
  else if (no == no->pai->esquerda) { // Se o nó é filho esquerdo
    no->pai->esquerda = filhoDireito;
  }
  else { // Se o nó é filho direito
    no->pai->direita = filhoDireito;
  }

  filhoDireito->esquerda = no;
  no->pai = filhoDireito;

  return filhoDireito; // novo nó raiz da subárvore rotacionada
}

No* rotacaoDireita(RedBlackTree* RedBlackTree, No* no) {
  No* filhoEsquerdo = no->esquerda;
  no->esquerda = filhoEsquerdo->direita;

  if (filhoEsquerdo->direita != NULL) {
    filhoEsquerdo->direita->pai = no;
  }

  filhoEsquerdo->pai = no->pai;

  if (no->pai == NULL) { // Se o nó é a raiz
    RedBlackTree->raiz = filhoEsquerdo;
  }
  else if (no == no->pai->direita) { // Se o nó é filho direito
    no->pai->direita = filhoEsquerdo;
  }
  else { // Se o nó é filho esquerdo
    no->pai->esquerda = filhoEsquerdo;
  }

  filhoEsquerdo->direita = no;
  no->pai = filhoEsquerdo;

  return filhoEsquerdo; // novo nó raiz da subárvore rotacionada
}

// função para encontrar o nó com o menor valor
No* minimoNode(No* no) {
  while (no->esquerda != NULL) {
    no = no->esquerda;
  }
  return no; // retorna o nó com o menor valor
}

// função para encontrar um nó
No* encontrarNo(RedBlackTree* RedBlackTree, int valor) {
  No* atual = RedBlackTree->raiz;
  while (atual != NULL) {
    if (valor < atual->valor) {
      atual = atual->esquerda;
    }
    else if (valor > atual->valor) {
      atual = atual->direita;
    }
    else {
      return atual;
    }
  }
  return NULL;
}

// função para remover um nó
void removerNo(RedBlackTree* RedBlackTree, int valor) {
  No* no = encontrarNo(RedBlackTree, valor);

  if (no == NULL) {
    fprintf(stderr, "O nó com valor %d não existe na árvore.\n", valor);
    return;
  }

  No* noRemovido = no;
  Cor corOriginal = noRemovido->cor;
  No* substituto;

  if (no->esquerda == NULL) { // Se o nó não tem filho esquerdo
    substituto = no->direita;
    transplant(RedBlackTree, no, no->direita);
  }
  else if (no->direita == NULL) { // Se o nó não tem filho direito
    substituto = no->esquerda;
    transplant(RedBlackTree, no, no->esquerda);
  }
  else { // Se o nó tem dois filhos
    noRemovido = minimoNode(no->direita); // encontra o nó com o menor valor na subárvore direita
    corOriginal = noRemovido->cor;
    substituto = noRemovido->direita;

    if (noRemovido->pai == no) {
      if (substituto != NULL) {
        substituto->pai = noRemovido;
      }
    }
    else {
      transplant(RedBlackTree, noRemovido, noRemovido->direita);
      noRemovido->direita = no->direita;
      noRemovido->direita->pai = noRemovido;
    }

    transplant(RedBlackTree, no, noRemovido);
    noRemovido->esquerda = no->esquerda;
    noRemovido->esquerda->pai = noRemovido;
    noRemovido->cor = no->cor;
  }

  free(no);

  if (corOriginal == BLACK) {
    balancearRemocao(RedBlackTree, substituto);
  }
}

// função para balancear a remoção
void balancearRemocao(RedBlackTree* RedBlackTree, No* no) {
  while (no != RedBlackTree->raiz && no->cor == BLACK) {
    if (no == no->pai->esquerda) { // Se o nó é filho esquerdo
      No* irmao = no->pai->direita; // irmão do nó
      if (irmao->cor == RED) { // caso 1: irmão é vermelho
        irmao->cor = BLACK;
        no->pai->cor = RED;
        rotacaoEsquerda(RedBlackTree, no->pai);
        irmao = no->pai->direita;
      }
      if (irmao->esquerda->cor == BLACK && irmao->direita->cor == BLACK) { // caso 2: ambos os filhos do irmão são pretos
        irmao->cor = RED;
        no = no->pai;
      }
      else { // caso 3: o filho direito do irmão é preto e o filho esquerdo do irmão é vermelho
        if (irmao->direita->cor == BLACK) {
          irmao->esquerda->cor = BLACK;
          irmao->cor = RED;
          irmao = rotacaoDireita(RedBlackTree, irmao);
        } // caso 4: o filho direito do irmão é vermelho
        irmao->cor = no->pai->cor;
        no->pai->cor = BLACK;
        irmao->direita->cor = BLACK;
        rotacaoEsquerda(RedBlackTree, no->pai);
        no = RedBlackTree->raiz;
      }
    } // caso simétrico: o nó é filho direito
    else {
      No* irmao = no->pai->esquerda;
      if (irmao->cor == RED) { // caso 1: irmão é vermelho
        irmao->cor = BLACK;
        no->pai->cor = RED;
        rotacaoDireita(RedBlackTree, no->pai);
        irmao = no->pai->esquerda;
      } // caso 2: ambos os filhos do irmão são pretos
      if (irmao->direita->cor == BLACK && irmao->esquerda->cor == BLACK) {
        irmao->cor = RED;
        no = no->pai;
      }
      else { // caso 3: o filho esquerdo do irmão é preto e o filho direito do irmão é vermelho
        if (irmao->esquerda->cor == BLACK) {
          irmao->direita->cor = BLACK;
          irmao->cor = RED;
          irmao = rotacaoEsquerda(RedBlackTree, irmao);
        }
        // caso 4: o filho esquerdo do irmão é vermelho
        irmao->cor = no->pai->cor;
        no->pai->cor = BLACK;
        irmao->esquerda->cor = BLACK;
        rotacaoDireita(RedBlackTree, no->pai);
        no = RedBlackTree->raiz;
      }
    }
  }
  no->cor = BLACK;
}

// função para testar todos os apectos da árvore rubro-negra
void testRBT() {

  RedBlackTree* RedBlackTree = criaArvoreRBT();
  inserirNo(RedBlackTree, 10);
  inserirNo(RedBlackTree, 20);
  inserirNo(RedBlackTree, 30);


  printf("Árvore Rubro-Negra:\n");
  inOrder(RedBlackTree->raiz);
  printf("\n\n");
  inserirNo(RedBlackTree, 34);
  inserirNo(RedBlackTree, 36);

  printf("Árvore Rubro-Negra:\n");
  inOrder(RedBlackTree->raiz);
  printf("\n\n");

  inserirNo(RedBlackTree, 25);
  inserirNo(RedBlackTree, 40);
  inserirNo(RedBlackTree, 35);
  inserirNo(RedBlackTree, 45);
  printf("\n\n");

  printf("Árvore Rubro-Negra:\n");
  inOrder(RedBlackTree->raiz);

  // Remover um nó
  removerNo(RedBlackTree, 30);
  printf("\n\n");

  printf("Árvore Rubro-Negra após remoção:\n");
  inOrder(RedBlackTree->raiz);

  inserirNo(RedBlackTree, 26);

  printf("\n\n");

  printf("Árvore Rubro-Negra:\n");
  inOrder(RedBlackTree->raiz);

  // Liberar memória alocada
  liberaArvore(RedBlackTree);
}

// menu principal
void menu() {
  printf("0. Sair\n");
  printf("1. Inserir um nó\n");
  printf("2. Remover um nó\n");
  printf("3. Imprimir a árvore\n");
  printf("4. Teste\n");
  printf("Escolha uma opção: ");
}

int main() {
  RedBlackTree* RedBlackTree = criaArvoreRBT();
  int opcao, valor;
  do {
    printf("\n");
    menu();
    scanf("%d", &opcao);
    switch (opcao) {
    case 0:
      liberaArvore(RedBlackTree);
      return 0;
      break;
    case 1:
      printf("Digite o valor do nó: ");
      scanf("%d", &valor);
      inserirNo(RedBlackTree, valor);
      break;
    case 2:
      printf("Digite o valor do nó: ");
      scanf("%d", &valor);
      removerNo(RedBlackTree, valor);
      break;
    case 3:
      printf("Árvore Rubro-Negra:\n");
      inOrder(RedBlackTree->raiz);
      printf("\n\n");
      break;
    case 4:
      testRBT();
      break;
    default:
      printf("Opção inválida!\n");
      break;
    }
  } while (opcao != 0);
}