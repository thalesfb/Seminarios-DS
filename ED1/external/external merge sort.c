#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Tamanho do buffer de memória
#define MEMORY_SIZE 1000 // Ajuste este valor de acordo com o tamanho da memória disponível
// Tamanho máximo do nome a ser ordenado
#define TAMANHO_MAX_NOME 50

// protótipos das funções
int compare_strings(const void*, const void*);
int divide_file(const char*);
void merge_files(int, const char*);
void external_merge_sort(const char*, const char*);
void remove_temp_files(int);

// Função principal
int main() {
  // Nome do arquivo de entrada
  const char* input_file_name = "nomes.txt";
  // Nome do arquivo de saída
  const char* output_file_name = "sorted_nomes.txt";
  // Chama a função para ordenar o arquivo de entrada
  external_merge_sort(input_file_name, output_file_name);
  // Mostra uma mensagem de sucesso
  printf("Arquivo ordenado com sucesso!\n");
  // Retorna 0 para indicar sucesso
  return 0;
}
// Implementação das funções
// Função para comparar dois nomes compatível com a função qsort
int compare_strings(const void* a, const void* b) {
  // Converte os parâmetros para o tipo compativel com a função
  const char* str_a = *(const char**)a;
  const char* str_b = *(const char**)b;
  // Retorna o resultado da comparação
  return strcmp(str_a, str_b);
}
// Função para ler e dividir o arquivo de entrada em subarquivos
int divide_file(const char* input_file_name) {
  // Abre o arquivo de entrada
  FILE* input_file = fopen(input_file_name, "r");
  // Verifica se o arquivo foi aberto corretamente
  if (input_file == NULL) {
    // Imprime uma mensagem de erro
    perror("Erro ao abrir o arquivo de entrada");
    // Retorna -1 para indicar erro
    return -1;
  }
  // Variável para contar a quantidade de nomes lidos
  int part_memory = 0;
  // Lê os nomes do arquivo de entrada 
  while (!feof(input_file)) {
    // Aloca memória para o buffer de memória
    char** arr = (char**)malloc(MEMORY_SIZE * sizeof(char*));
    // Laço para alocar memória para cada nome lido
    for (int i = 0; i < MEMORY_SIZE; i++) {
      arr[i] = (char*)malloc(TAMANHO_MAX_NOME * sizeof(char));
    }
    int i = 0;
    // Loop para ler os nomes do arquivo de entrada
    while (i < MEMORY_SIZE && fgets(arr[i], TAMANHO_MAX_NOME, input_file) != NULL) {
      i++;
    }
    // Verifica se o laço foi interrompido por um erro
    if (i > 0) {
      // Ordena o subarquivo
      qsort(arr, i, sizeof(char*), compare_strings);
      // Cria o nome do arquivo de saída
      char output_file_name[100];
      // Imprime o nome do arquivo de saída
      sprintf(output_file_name, "part_%d.txt", part_memory);
      // Abre o arquivo de saída
      FILE* output_file = fopen(output_file_name, "w");
      // Verifica se o arquivo foi aberto corretamente
      if (output_file == NULL) {
        // Imprime uma mensagem de erro
        perror("Erro ao abrir o arquivo de saída");
        // Retorna -1 para indicar erro
        return -1;
      }
      // Escreve os nomes ordenados no arquivo de saída
      for (int j = 0; j < i; j++) {
        // Imprime o nome no arquivo de saída
        fputs(arr[j], output_file);
      }
      // Fecha o arquivo de saída
      fclose(output_file);
      // Incrementa o contador de subarquivos
      part_memory++;
    }
    // Libera a memória alocada para os nomes
    for (int i = 0; i < MEMORY_SIZE; i++) {
      free(arr[i]);
    }
    // Libera a memória alocada para o buffer de memória
    free(arr);
  }
  // Fecha o arquivo de entrada
  fclose(input_file);
  // Retorna a quantidade de subarquivos gerados
  return part_memory;
}

// Função para fazer a intercalação (merge) dos subarquivos ordenados
void merge_files(int part_memory, const char* output_file_name) {
  // Aloca memória para os arquivos de entrada
  FILE** input_files = (FILE**)malloc(part_memory * sizeof(FILE*));
  // Laço para abrir os arquivos de entrada
  for (int i = 0; i < part_memory; i++) {
    // Cria o nome do arquivo de entrada
    char input_file_name[100];
    // Imprime o nome do arquivo de entrada
    sprintf(input_file_name, "part_%d.txt", i);
    // Abre o arquivo de entrada para leitura
    input_files[i] = fopen(input_file_name, "r");
    // Verifica se o arquivo foi aberto corretamente
    if (input_files[i] == NULL) {
      // Imprime uma mensagem de erro
      perror("Erro ao abrir o arquivo de entrada");
      // Retorna
      return;
    }
  }
  // Abre o arquivo de saída
  FILE* output_file = fopen(output_file_name, "w");
  // Verifica se o arquivo foi aberto corretamente
  if (output_file == NULL) {
    // Imprime uma mensagem de erro
    perror("Erro ao abrir o arquivo de saída");
    return;
  }
  // Aloca memória para o buffer de memória
  char** buffer = (char**)malloc(part_memory * sizeof(char*));
  // Aloca memória para o vetor de flags de fim de arquivo
  bool* eof_flags = (bool*)malloc(part_memory * sizeof(bool));
  // Laço para alocar memória para cada nome lido
  for (int i = 0; i < part_memory; i++) {
    // Aloca memória para cada nome lido e atribui o valor para o buffer de memória
    buffer[i] = (char*)malloc(TAMANHO_MAX_NOME * sizeof(char));
    // Inicializa buffer das flags de fim de arquivo
    eof_flags[i] = false;
  }
  // Laço para ler os nomes do arquivo de entrada
  for (int i = 0; i < part_memory; i++) {
    // Lê o nome do arquivo de entrada
    if (fgets(buffer[i], TAMANHO_MAX_NOME, input_files[i]) == NULL) {
      // Atribui o valor true para a flag de fim de arquivo
      eof_flags[i] = true;
    }
  }
  // Laço para fazer a intercalação dos subarquivos
  while (true) {
    // Variável para armazenar o índice do menor nome
    int min_index = -1;
    // Laço para encontrar o menor nome
    for (int i = 0; i < part_memory; i++) {
      // Verifica se o nome não chegou ao fim e se é menor que o nome atual
      if (!eof_flags[i] && (min_index == -1 || strcmp(buffer[i], buffer[min_index]) < 0)) {
        // Atribui o índice do menor nome
        min_index = i;
      }
    }
    // Verifica se o laço foi interrompido por um erro
    if (min_index == -1) {
      // Para o laço principal
      break;
    }
    // Imprime o nome menor do buffer no arquivo de saída
    fputs(buffer[min_index], output_file);
    // Lê o próximo nome do arquivo de entrada
    if (fgets(buffer[min_index], TAMANHO_MAX_NOME, input_files[min_index]) == NULL) {
      // Atribui o valor true para a flag de fim de arquivo caso o tenha chegado ao fim
      eof_flags[min_index] = true;
    }
  }
  // Laço para liberar a memória alocada
  for (int i = 0; i < part_memory; i++) {
    // Libera a memória alocada para cada nome
    free(buffer[i]);
    // Fecha o arquivo de entrada
    fclose(input_files[i]);
  }
  // Libera a memória alocada para o buffer de memória
  free(buffer);
  // Libera a memória alocada para o vetor de flags de fim de arquivo
  free(eof_flags);
  // Libera a memória alocada para o arquivo de entrada
  free(input_files);
  // Fecha o arquivo de saída
  fclose(output_file);
}

// Função para ordenar o arquivo de entrada
void external_merge_sort(const char* input_file_name, const char* output_file_name) {
  // Divide o arquivo de entrada em subarquivos atrbuindo o valor para a variável part_memory
  int part_memory = divide_file(input_file_name);
  // Verifica se ocorreu algum erro
  if (part_memory < 0) {
    // Imprime uma mensagem de erro
    fprintf(stderr, "Erro dividindo o arquivo de entrada\n");
    // Retorna
    return;
  }
  // Faz a mesclagem dos subarquivos
  merge_files(part_memory, output_file_name);
  // Remove os arquivos temporários gerados
  remove_temp_files(part_memory);
}

// Função para remover os arquivos temporários gerados
void remove_temp_files(int part_memory) {
  // Laço para remover os arquivos temporários
  for (int i = 0; i < part_memory; i++) {
    // Cria o nome do arquivo de entrada
    char temp_file_name[100];
    // Imprime o nome do arquivo de entrada
    sprintf(temp_file_name, "part_%d.txt", i);
    // Remove o arquivo de entrada
    if (remove(temp_file_name) != 0) {
      // Imprime uma mensagem de erro
      perror("Erro removendo o arquivo temporário");
    }
  }
}