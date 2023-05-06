#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000 // Ajuste este valor de acordo com o tamanho da memória disponível

void merge(int arr[], int left[], int left_size, int right[], int right_size) {
  int i = 0, j = 0, k = 0;

  while (i < left_size && j < right_size) {
    if (left[i] < right[j]) {
      arr[k++] = left[i++];
    }
    else {
      arr[k++] = right[j++];
    }
  }

  while (i < left_size) {
    arr[k++] = left[i++];
  }

  while (j < right_size) {
    arr[k++] = right[j++];
  }
}

void merge_sort(int arr[], int size) {
  if (size < 2) {
    return;
  }

  int middle = size / 2;
  int left[middle];
  int right[size - middle];

  for (int i = 0; i < middle; i++) {
    left[i] = arr[i];
  }
  for (int i = middle; i < size; i++) {
    right[i - middle] = arr[i];
  }

  merge_sort(left, middle);
  merge_sort(right, size - middle);
  merge(arr, left, middle, right, size - middle);
}

void generate_random_data(const char* filename, int count) {
  FILE* file = fopen(filename, "w");
  if (!file) {
    perror("Error opening file");
    return;
  }

  srand(time(NULL));
  for (int i = 0; i < count; i++) {
    int random_number = rand() % 100000; // Gera números aleatórios entre 0 e 99999
    fprintf(file, "%d\n", random_number);
  }

  fclose(file);
}

int main() {
  generate_random_data("data.txt", N);
  FILE* file = fopen("data.txt", "r");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  int data[N]; // Ajuste o tamanho do array conforme necessário
  int count = 0;

  while (fscanf(file, "%d", &data[count]) != EOF) {
    count++;
  }
  fclose(file);

  merge_sort(data, count);

  file = fopen("sorted_data.txt", "w");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  for (int i = 0; i < count; i++) {
    fprintf(file, "%d\n", data[i]);
  }
  fclose(file);

  return 0;
}
