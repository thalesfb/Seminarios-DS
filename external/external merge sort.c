#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MEMORY_SIZE 1000 // Ajuste este valor de acordo com o tamanho da memória disponível
#define TAMANHO_MAX_NOME 50 // Tamanho maximo do nome a ser ordenado

// Função para comparar dois nomes
int compare(const void* a, const void* b) {
  return strcmp((const char*)a, (const char*)b);
}

// Função para ler e dividir o arquivo de entrada em subarquivos
int divide_file(const char* input_file_name) {
  FILE* input_file = fopen(input_file_name, "r");
  if (input_file == NULL) {
    perror("Error opening input file");
    return -1;
  }

  int part_memory = 0;

  while (!feof(input_file)) {
    char** arr = (char**)malloc(MEMORY_SIZE * sizeof(char*));
    for (int i = 0; i < MEMORY_SIZE; i++) {
      arr[i] = (char*)malloc(TAMANHO_MAX_NOME * sizeof(char));
    }

    int i = 0;
    while (i < MEMORY_SIZE && fgets(arr[i], TAMANHO_MAX_NOME, input_file) != NULL) {
      i++;
    }

    if (i > 0) {
      qsort(arr, i, TAMANHO_MAX_NOME * sizeof(char), compare);

      char output_file_name[100];
      sprintf(output_file_name, "part_%d.txt", part_memory);
      FILE* output_file = fopen(output_file_name, "w");
      if (output_file == NULL) {
        perror("Error opening output file");
        return -1;
      }

      for (int j = 0; j < i; j++) {
        fputs(arr[j], output_file);
      }

      fclose(output_file);
      part_memory++;
    }

    for (int i = 0; i < MEMORY_SIZE; i++) {
      free(arr[i]);
    }
    free(arr);
  }

  fclose(input_file);
  return part_memory;
}

/* int divide_file(const char* input_file) {

  char** arr = (char**)malloc(MEMORY_SIZE * sizeof(char*));

  int arr[MEMORY_SIZE];
  int part_memory = 0;

  FILE* file = fopen(input_file, "r");
  if (!file) {
    perror("Error opening input file");
    return 0;
  }

  while (!feof(file)) {
    int i;
    for (i = 0; i < MEMORY_SIZE && fscanf(file, "%d", &arr[i]) == 1; i++);

    qsort(arr, i, sizeof(int), compare); // Ordena o subarquivo

    char output_file[64];
    snprintf(output_file, sizeof(output_file), "pert_%d.txt", part_memory);
    FILE* out = fopen(output_file, "w");
    if (!out) {
      perror("Error opening output file");
      fclose(file);
      return 0;
    }

    for (int j = 0; j < i; j++) {
      fprintf(out, "%d\n", arr[j]);
    }

    fclose(out);
    part_memory++;
  }

  fclose(file);
  return part_memory;
} */

// Função para fazer a intercalação (merge) dos subarquivos ordenados
void merge_files(int part_memory, const char* output_file_name) {
  FILE** input_files = (FILE**)malloc(part_memory * sizeof(FILE*));
  for (int i = 0; i < part_memory; i++) {
    char input_file_name[100];
    sprintf(input_file_name, "part_%d.txt", i);
    input_files[i] = fopen(input_file_name, "r");
    if (input_files[i] == NULL) {
      perror("Error opening input file");
      return;
    }
  }

  FILE* output_file = fopen(output_file_name, "w");
  if (output_file == NULL) {
    perror("Error opening output file");
    return;
  }

  char** buffer = (char**)malloc(part_memory * sizeof(char*));
  bool* eof_flags = (bool*)malloc(part_memory * sizeof(bool));
  for (int i = 0; i < part_memory; i++) {
    buffer[i] = (char*)malloc(TAMANHO_MAX_NOME * sizeof(char));
    eof_flags[i] = false;
  }

  for (int i = 0; i < part_memory; i++) {
    if (fgets(buffer[i], TAMANHO_MAX_NOME, input_files[i]) == NULL) {
      eof_flags[i] = true;
    }
  }

  while (true) {
    int min_index = -1;
    for (int i = 0; i < part_memory; i++) {
      if (!eof_flags[i] && (min_index == -1 || strcmp(buffer[i], buffer[min_index]) < 0)) {
        min_index = i;
      }
    }

    if (min_index == -1) {
      break;
    }

    fputs(buffer[min_index], output_file);

    if (fgets(buffer[min_index], TAMANHO_MAX_NOME, input_files[min_index]) == NULL) {
      eof_flags[min_index] = true;
    }
  }

  for (int i = 0; i < part_memory; i++) {
    free(buffer[i]);
    fclose(input_files[i]);
  }
  free(buffer);
  free(eof_flags);
  free(input_files);

  fclose(output_file);
}

// Função para ordenar o arquivo de entrada
void external_merge_sort(const char* input_file_name, const char* output_file_name) {
  int part_memory = divide_file(input_file_name);
  if (part_memory < 0) {
    fprintf(stderr, "Erro dividindo o arquivo de entrada\n");
  }
  merge_files(part_memory, output_file_name);
}

/* void external_merge_sort(const char* output_file, int part_memory) {
  FILE* files[part_memory];
  int buffers[part_memory];
  bool active[part_memory];

  for (int i = 0; i < part_memory; i++) {
    char input_file[64];
    snprintf(input_file, sizeof(input_file), "chunk_%d.txt", i);
    files[i] = fopen(input_file, "r");
    if (!files[i]) {
      perror("Error opening input file");
      return;
    }

    if (fscanf(files[i], "%d", &buffers[i]) != 1) {
      perror("Error reading input file");
      fclose(files[i]);
      return;
    }
    active[i] = true;
  }

  FILE* out = fopen(output_file, "w");
  if (!out) {
    perror("Error opening output file");
    return;
  }

  int num_active = part_memory;
  while (num_active > 0) {
    int min_index = -1;
    for (int i = 0; i < part_memory; i++) {
      if (!active[i]) {
        continue;
      }

      if (min_index == -1 || buffers[i] < buffers[min_index]) {
        min_index = i;
      }
    }

    fprintf(out, "%d\n", buffers[min_index]);
    if (fscanf(files[min_index], "%d", &buffers[min_index]) != 1) {
      active[min_index] = false;
      fclose(files[min_index]);
      num_active--;
    }
  }
  fclose(out);
} */
int main() {
  const char* input_file_name = "nomes.txt";
  const char* output_file_name = "sorted_nomes.txt";

  external_merge_sort(input_file_name, output_file_name);

  /*   int part_memory = divide_file(input_file);
    if (part_memory > 0) {
      external_merge_sort(output_file, part_memory);
      printf("Arquivo ordenado com sucesso: %s\n", output_file);
    }
    else {
      printf("Erro ao dividir o arquivo de entrada.\n");
    } */
  return 0;
}