#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include<mpi.h>

unsigned short int
merge(unsigned short int *, unsigned short int, unsigned short int,
      unsigned short int); // function to merge 2 arrays in sorted order
unsigned short int
mergesort(unsigned short int *, unsigned short int,
          unsigned short int); // recursively called function that splits an
                               // input array into two and pass to merge
void init_array_random(unsigned short int *, unsigned short int);
void print_array(unsigned short int *, unsigned short int);

int main(int argc, char *argv[]) {
  unsigned short int n;
  unsigned short int l, m, r; // left, middle, right
  if (argc != 2) {
    n = 1000;
  } else {
    n = atoi(argv[1]);
  }
  unsigned short int guinea_array[n];
  init_array_random(
      guinea_array,
      n); // initializes array of size n with random values between 0 and 2n
  print_array(guinea_array, n);
  l = 0;
  r = n - 1;
  mergesort(guinea_array, l, r);
  // print_array(guinea_array, n);
  return 1;
}

void print_array(unsigned short int *guinea_array, unsigned short int n) {
  for (unsigned short int i = 0; i < n; i++) {
    printf("%d, ", guinea_array[i]);
  }
}

unsigned short int mergesort(unsigned short int *guinea_array,
                             unsigned short int l, unsigned short int r) {
  if (l >= r) {
    return 1;
  }
  unsigned short int m = l + (r - l) / 2; // left subarray >= right subarray
  mergesort(guinea_array, l, m);
  mergesort(guinea_array, m + 1, r);

  merge(guinea_array, l, m, r);

  return 1;
}

unsigned short int merge(unsigned short int *guinea_array, unsigned short int l,
                         unsigned short int m, unsigned short int r) {
  unsigned short int n_left = m - l + 1;
  unsigned short int n_right = r - m;
  unsigned short int left_array[n_left];
  unsigned short int right_array[n_right];

  unsigned short int i_left = 0;
  unsigned short int i_right = 0;
  unsigned short int j = l;

  for (unsigned short int i = 0; i < n_left; i++) {
    left_array[i] = guinea_array[i + l];
  }
  for (unsigned short int i = 0; i < n_right; i++) {
    right_array[i] = guinea_array[i + m + 1];
  }

  while (i_left < n_left && i_right < n_right) {
    if (left_array[i_left] <= right_array[i_right]) {
      guinea_array[j] = left_array[i_left];
      i_left++;
      j++;
    } else {
      guinea_array[j] = right_array[i_right];
      i_right++;
      j++;
    }
  }
  while (i_left < n_left) {
    guinea_array[j] = left_array[i_left];
    j++;
    i_left++;
  }
  while (i_right < n_right) {
    guinea_array[j] = right_array[i_right];
    j++;
    i_right++;
  }
  return 1;
}

void init_array_random(unsigned short int *guinea_array, unsigned short int n) {
  srand(time(NULL));
  for (unsigned short int i = 0; i < n; i++) {
    guinea_array[i] = rand() % (n);
  }
}
