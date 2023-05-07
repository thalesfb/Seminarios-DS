#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000 // Ajuste este valor de acordo com o tamanho da memória disponível

// Função que mescla dois arrays
void merge(int arr[], int left[], int left_size, int right[], int right_size) {
  // Variáveis para controlar os índices dos arrays
  int i = 0, j = 0, k = 0;
  // Laço que vai até o final de um dos arrays
  while (i < left_size && j < right_size) {
    // Verifica se o elemento do array da esquerda é menor que o elemento do array da direita
    if (left[i] < right[j]) {
      // Copia o elemento do array da esquerda para o array principal
      arr[k++] = left[i++];
    }
    else {
      // Copia o elemento do array da direita para o array principal
      arr[k++] = right[j++];
    }
  }
  // Laço que vai até o final do array da esquerda caso sobre algum elemento
  while (i < left_size) {
    // Copia o elemento do array da esquerda para o array principal
    arr[k++] = left[i++];
  }
  // Laço que vai até o final do array da direita caso sobre algum elemento
  while (j < right_size) {
    // Copia o elemento do array da direita para o array principal
    arr[k++] = right[j++];
  }
}

// Função que ordena o array
void merge_sort(int arr[], int size) {
  // Verifica se o tamanho do array é menor que 2
  if (size < 2) {
    // Retorna
    return;
  }
  // Calcula o meio do array
  int middle = size / 2;
  // Cria array da esquerda com o tamanho do meio
  int left[middle];
  // Cria array da direita com o tamanho do array menos o meio
  int right[size - middle];
  // Laço que vai do início até o meio do array e copia os elementos para o array da esquerda
  for (int i = 0; i < middle; i++) {
    left[i] = arr[i];
  }
  // Laço que vai do meio até o final do array e copia os elementos para o array da direita
  for (int i = middle; i < size; i++) {
    right[i - middle] = arr[i];
  }
  // Chama a função merge_sort para o array da esquerda
  merge_sort(left, middle);
  // Chama a função merge_sort para o array da direita
  merge_sort(right, size - middle);
  // Chama a função merge para mesclar os arrays
  merge(arr, left, middle, right, size - middle);
}

// Função para gerar dados aleatórios
void generate_random_data(const char* filename, int count) {
  // Abre o arquivo para escrita
  FILE* file = fopen(filename, "w");
  // Verifica se o arquivo foi aberto corretamente
  if (!file) {
    // Imprime uma mensagem de erro
    perror("Error opening file");
    // Retorna
    return;
  }
  // Gera uma semente para o gerador de números aleatórios
  srand(time(NULL));
  // Gera os números aleatórios e escreve no arquivo
  for (int i = 0; i < count; i++) {
    // Gera um número aleatório entre 0 e 99999
    int random_number = rand() % 100000;
    // Escreve o número no arquivo
    fprintf(file, "%d\n", random_number);
  }
  // Fecha o arquivo
  fclose(file);
}

// Função principal
int main() {
  // Gera os dados aleatórios
  generate_random_data("data.txt", N);
  // Abre o arquivo para leitura
  FILE* file = fopen("data.txt", "r");
  // Verifica se o arquivo foi aberto corretamente
  if (!file) {
    // Imprime uma mensagem de erro
    perror("Error opening file");
    // Retorna 1 para indicar erro
    return 1;
  }
  // Cria um array para armazenar os dados
  int data[N]; // Ajuste o tamanho do array conforme necessário
  // Variável para contar a quantidade de dados lidos
  int count = 0;
  // Lê os dados do arquivo
  while (fscanf(file, "%d", &data[count]) != EOF) {
    // Incrementa o contador
    count++;
  }
  // Fecha o arquivo
  fclose(file);
  // Chama a função merge_sort para ordenar os dados
  merge_sort(data, count);
  // Abre o arquivo para escrita
  file = fopen("sorted_data.txt", "w");
  // Verifica se o arquivo foi aberto corretamente
  if (!file) {
    // Imprime uma mensagem de erro
    perror("Error opening file");
    // Retorna 1 para indicar erro
    return 1;
  }
  // Escreve os dados ordenados no arquivo
  for (int i = 0; i < count; i++) {
    // Escreve o número no arquivo
    fprintf(file, "%d\n", data[i]);
  }
  // Fecha o arquivo
  fclose(file);
  // Retorna 0 para indicar sucesso
  return 0;
}
