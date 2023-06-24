#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int merge(int *, int, int,
          int); // function to merge 2 arrays in sorted order
int mergesort(int *, int,
              int); // recursively called function that splits an
                    // input array into two and pass to merge
void init_array_random(int *, int);
void print_array(int *, int);

int main(int argc, char *argv[]) {
  int n;
  int l, m, r; // left, middle, right
  int rank, size;
  n = 1000;
  int guinea_array[n];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    init_array_random(
        guinea_array,
        n); // initializes array of size n with random values between 0 and 2n
  }

  // print_array(guinea_array, n);

  int local_size = n / size;
  int guinea_array_local[local_size];
  double start_time = MPI_Wtime();
  MPI_Scatter(guinea_array, local_size, MPI_INT, guinea_array_local, local_size,
              MPI_INT, 0, MPI_COMM_WORLD);

  mergesort(guinea_array_local, 0, local_size - 1);

  MPI_Gather(guinea_array_local, local_size, MPI_INT, guinea_array, n, MPI_INT,
             0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  l = 0;
  r = n - 1;
  mergesort(guinea_array, l, r);
  double end_time = MPI_Wtime();
  print_array(guinea_array, n);
  printf("\nTIME ELAPSED: %lf",end_time-start_time);
  return 1;
}

void print_array(int *guinea_array, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d, ", guinea_array[i]);
  }
}

int mergesort(int *guinea_array, int l, int r) {
  if (l >= r) {
    return 1;
  }
  int m = l + (r - l) / 2; // left subarray >= right subarray
  mergesort(guinea_array, l, m);
  mergesort(guinea_array, m + 1, r);

  merge(guinea_array, l, m, r);

  return 1;
}

int merge(int *guinea_array, int l, int m, int r) {
  int n_left = m - l + 1;
  int n_right = r - m;
  int left_array[n_left];
  int right_array[n_right];

  int i_left = 0;
  int i_right = 0;
  int j = l;

  for (int i = 0; i < n_left; i++) {
    left_array[i] = guinea_array[i + l];
  }
  for (int i = 0; i < n_right; i++) {
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

void init_array_random(int *guinea_array, int n) {
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    guinea_array[i] = rand() % (n);
  }
}
