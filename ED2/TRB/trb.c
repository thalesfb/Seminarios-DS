#include <stdio.h>
#include <stdlib.h>

#define VERMELHO 0
#define PRETO 1

typedef struct Node {
  int valor;
  int cor;  // VERMELHO (0) ou PRETO (1)
  struct Node* pai;
  struct Node* esquerda;
  struct Node* direita;
} Node;

typedef struct Arvore {
  Node* raiz;
} Arvore;

Node* criarNo(int valor) {
  Node* no = (Node*)malloc(sizeof(Node));
  no->valor = valor;
  no->cor = VERMELHO;
  no->pai = NULL;
  no->esquerda = NULL;
  no->direita = NULL;
  return no;
}

Arvore* criarArvore() {
  Arvore* arvore = (Arvore*)malloc(sizeof(Arvore));
  arvore->raiz = NULL;
  return arvore;
}

//void balancearInsercao(Arvore* arvore, Node* no);
//void rotacaoEsquerda(Arvore* arvore, Node* no);
//void rotacaoDireita(Arvore* arvore, Node* no);

Node* rotacaoEsquerda(Arvore* arvore, Node* no) {
  Node* filhoDireito = no->direita;
  no->direita = filhoDireito->esquerda;

  if (filhoDireito->esquerda != NULL) {
    filhoDireito->esquerda->pai = no;
  }

  filhoDireito->pai = no->pai;

  if (no->pai == NULL) {
    arvore->raiz = filhoDireito;
  }
  else if (no == no->pai->esquerda) {
    no->pai->esquerda = filhoDireito;
  }
  else {
    no->pai->direita = filhoDireito;
  }

  filhoDireito->esquerda = no;
  no->pai = filhoDireito;

  return filhoDireito; // Retorna o novo nó raiz da subárvore
}

Node* rotacaoDireita(Arvore* arvore, Node* no) {
  Node* filhoEsquerdo = no->esquerda;
  no->esquerda = filhoEsquerdo->direita;

  if (filhoEsquerdo->direita != NULL) {
    filhoEsquerdo->direita->pai = no;
  }

  filhoEsquerdo->pai = no->pai;

  if (no->pai == NULL) {
    arvore->raiz = filhoEsquerdo;
  }
  else if (no == no->pai->direita) {
    no->pai->direita = filhoEsquerdo;
  }
  else {
    no->pai->esquerda = filhoEsquerdo;
  }

  filhoEsquerdo->direita = no;
  no->pai = filhoEsquerdo;

  return filhoEsquerdo; // Retorna o novo nó raiz da subárvore
}

void balancearInsercao(Arvore* arvore, Node* no) {
  while (no != arvore->raiz && no->pai->cor == VERMELHO) {
    if (no->pai == no->pai->pai->esquerda) {
      Node* tioNode = no->pai->pai->direita;
      if (tioNode != NULL && tioNode->cor == VERMELHO) {
        no->pai->cor = PRETO;
        tioNode->cor = PRETO;
        no->pai->pai->cor = VERMELHO;
        no = no->pai->pai;
      }
      else {
        if (no == no->pai->direita) {
          no = no->pai;
          no = rotacaoEsquerda(arvore, no);
        }
        no->pai->cor = PRETO;
        no->pai->pai->cor = VERMELHO;
        no = rotacaoDireita(arvore, no->pai->pai);
      }
    }
    else {
      Node* tioNode = no->pai->pai->esquerda;
      if (tioNode != NULL && tioNode->cor == VERMELHO) {
        no->pai->cor = PRETO;
        tioNode->cor = PRETO;
        no->pai->pai->cor = VERMELHO;
        no = no->pai->pai;
      }
      else {
        if (no == no->pai->esquerda) {
          no = no->pai;
          no = rotacaoDireita(arvore, no);
        }
        no->pai->cor = PRETO;
        no->pai->pai->cor = VERMELHO;
        no = rotacaoEsquerda(arvore, no->pai->pai);
      }
    }
  }
  arvore->raiz->cor = PRETO;
}

void inserirNo(Arvore* arvore, int valor) {
  Node* novoNo = criarNo(valor);
  Node* pai = NULL;
  Node* atual = arvore->raiz;

  while (atual != NULL) {
    pai = atual;
    if (novoNo->valor < atual->valor) {
      atual = atual->esquerda;
    }
    else {
      atual = atual->direita;
    }
  }

  novoNo->pai = pai;

  if (pai == NULL) {
    arvore->raiz = novoNo;
  }
  else if (novoNo->valor < pai->valor) {
    pai->esquerda = novoNo;
  }
  else {
    pai->direita = novoNo;
  }

  balancearInsercao(arvore, novoNo);
}

Node* encontrarNo(Arvore* arvore, int valor) {
  Node* atual = arvore->raiz;
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

Node* minimoNode(Node* no) {
  while (no->esquerda != NULL) {
    no = no->esquerda;
  }
  return no;
}

void transplant(Arvore* arvore, Node* no1, Node* no2) {
  if (no1->pai == NULL) {
    arvore->raiz = no2;
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

void balancearRemocao(Arvore* arvore, Node* no) {
  while (no != arvore->raiz && no->cor == PRETO) {
    if (no == no->pai->esquerda) {
      Node* irmao = no->pai->direita;
      if (irmao->cor == VERMELHO) {
        irmao->cor = PRETO;
        no->pai->cor = VERMELHO;
        rotacaoEsquerda(arvore, no->pai);
        irmao = no->pai->direita;
      }
      if (irmao->esquerda->cor == PRETO && irmao->direita->cor == PRETO) {
        irmao->cor = VERMELHO;
        no = no->pai;
      }
      else {
        if (irmao->direita->cor == PRETO) {
          irmao->esquerda->cor = PRETO;
          irmao->cor = VERMELHO;
          irmao = rotacaoDireita(arvore, irmao);
        }
        irmao->cor = no->pai->cor;
        no->pai->cor = PRETO;
        irmao->direita->cor = PRETO;
        rotacaoEsquerda(arvore, no->pai);
        no = arvore->raiz;
      }
    }
    else {
      Node* irmao = no->pai->esquerda;
      if (irmao->cor == VERMELHO) {
        irmao->cor = PRETO;
        no->pai->cor = VERMELHO;
        rotacaoDireita(arvore, no->pai);
        irmao = no->pai->esquerda;
      }
      if (irmao->direita->cor == PRETO && irmao->esquerda->cor == PRETO) {
        irmao->cor = VERMELHO;
        no = no->pai;
      }
      else {
        if (irmao->esquerda->cor == PRETO) {
          irmao->direita->cor = PRETO;
          irmao->cor = VERMELHO;
          irmao = rotacaoEsquerda(arvore, irmao);
        }
        irmao->cor = no->pai->cor;
        no->pai->cor = PRETO;
        irmao->esquerda->cor = PRETO;
        rotacaoDireita(arvore, no->pai);
        no = arvore->raiz;
      }
    }
  }
  no->cor = PRETO;
}

void removerNo(Arvore* arvore, int valor) {
  Node* no = encontrarNo(arvore, valor);

  if (no == NULL) {
    printf("O nó com valor %d não existe na árvore.\n", valor);
    return;
  }

  Node* noRemovido = no;
  int corOriginal = noRemovido->cor;
  Node* substituto;

  if (no->esquerda == NULL) {
    substituto = no->direita;
    transplant(arvore, no, no->direita);
  }
  else if (no->direita == NULL) {
    substituto = no->esquerda;
    transplant(arvore, no, no->esquerda);
  }
  else {
    noRemovido = minimoNode(no->direita);
    corOriginal = noRemovido->cor;
    substituto = noRemovido->direita;
    if (noRemovido->pai == no) {
      if (substituto != NULL) {
        substituto->pai = noRemovido;
      }
    }
    else {
      transplant(arvore, noRemovido, noRemovido->direita);
      noRemovido->direita = no->direita;
      noRemovido->direita->pai = noRemovido;
    }
    transplant(arvore, no, noRemovido);
    noRemovido->esquerda = no->esquerda;
    noRemovido->esquerda->pai = noRemovido;
    noRemovido->cor = no->cor;
  }

  free(no);

  if (corOriginal == PRETO) {
    balancearRemocao(arvore, substituto);
  }
}

void inOrder(Node* no) {
  if (no != NULL) {
    inOrder(no->esquerda);
    printf("%d %s\n", no->valor, no->cor == VERMELHO ? "VERMELHO" : "PRETO");
    inOrder(no->direita);
  }
}

void liberarMemoria(Node* no) {
  if (no != NULL) {
    liberarMemoria(no->esquerda);
    liberarMemoria(no->direita);
    free(no);
  }
}

int main() {
  Arvore* arvore = criarArvore();
  inserirNo(arvore, 10);
  inserirNo(arvore, 20);
  inserirNo(arvore, 30);


  printf("Árvore Rubro-Negra:\n");
  inOrder(arvore->raiz);
  printf("\n\n");
  inserirNo(arvore, 34);
  inserirNo(arvore, 36);

  printf("Árvore Rubro-Negra:\n");
  inOrder(arvore->raiz);
  printf("\n\n");

  inserirNo(arvore, 25);
  inserirNo(arvore, 40);
  inserirNo(arvore, 35);
  inserirNo(arvore, 45);
  printf("\n\n");

  printf("Árvore Rubro-Negra:\n");
  inOrder(arvore->raiz);

  // Remover um nó
  removerNo(arvore, 30);
  printf("\n\n");

  printf("Árvore Rubro-Negra após remoção:\n");
  inOrder(arvore->raiz);

  inserirNo(arvore, 26);

  printf("\n\n");

  printf("Árvore Rubro-Negra:\n");
  inOrder(arvore->raiz);

  // Liberar memória alocada
  liberarMemoria(arvore->raiz);
  free(arvore);

  return 0;
}