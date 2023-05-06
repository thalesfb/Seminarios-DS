#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHUNK_SIZE 1000 // Ajuste este valor de acordo com o tamanho da memória disponível

// Função para comparar dois inteiros (usada no qsort)
int compare(const void* a, const void* b) {
  return (*(int*)a - *(int*)b);
}

// Função para ler e dividir o arquivo de entrada em subarquivos
int divide_file(const char* input_file) {
  int arr[CHUNK_SIZE];
  int num_chunks = 0;

  FILE* file = fopen(input_file, "r");
  if (!file) {
    perror("Error opening input file");
    return 0;
  }

  while (!feof(file)) {
    int i;
    for (i = 0; i < CHUNK_SIZE && fscanf(file, "%d", &arr[i]) == 1; i++);

    qsort(arr, i, sizeof(int), compare); // Ordena o subarquivo

    char output_file[64];
    snprintf(output_file, sizeof(output_file), "chunk_%d.txt", num_chunks);
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
    num_chunks++;
  }

  fclose(file);
  return num_chunks;
}

// Função para fazer a intercalação (merge) dos subarquivos ordenados
void external_merge_sort(const char* output_file, int num_chunks) {
  FILE* files[num_chunks];
  int buffers[num_chunks];
  bool active[num_chunks];

  for (int i = 0; i < num_chunks; i++) {
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

  int num_active = num_chunks;
  while (num_active > 0) {
    int min_index = -1;
    for (int i = 0; i < num_chunks; i++) {
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
}
int main() {
  const char* input_file = "data.txt";
  const char* output_file = "sorted_data.txt";
  int num_chunks = divide_file(input_file);
  if (num_chunks > 0) {
    external_merge_sort(output_file, num_chunks);
    printf("Arquivo ordenado com sucesso: %s\n", output_file);
  }
  else {
    printf("Erro ao dividir o arquivo de entrada.\n");
  }

  return 0;
}