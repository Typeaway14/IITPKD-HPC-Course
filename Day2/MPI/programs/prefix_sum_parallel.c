#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 40

int main(int argc, char *argv[]) {

  int rank, size;
  int *prefix = NULL;
  int *input = NULL;
  int *thread_inp = NULL, *thread_prefix = NULL;
  int local_size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    input = (int *)malloc(MAX_SIZE * sizeof(int));
    prefix = (int *)malloc(MAX_SIZE * sizeof(int));
    for (int i = 0; i < MAX_SIZE; i++) {
      input[i] = i + 1;
    }
  }

  local_size = MAX_SIZE / size;

  thread_inp = (int *)malloc(local_size * sizeof(int));
  thread_prefix = (int *)malloc(local_size * sizeof(int));

  MPI_Scatter(input, local_size, MPI_INT, thread_inp, local_size, MPI_INT, 0,
              MPI_COMM_WORLD);
  thread_prefix[0] = thread_inp[0];
  for (int i = 1; i < local_size; i++) {
    thread_prefix[i] = thread_prefix[i - 1] + thread_inp[i];
  }

  MPI_Gather(thread_prefix, local_size, MPI_INT, prefix, local_size, MPI_INT, 0,
             MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 0; i < MAX_SIZE; i++) {
      printf("%d,", prefix[i]);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    free(input);
    free(prefix);
  }
  free(thread_inp);
  free(thread_prefix);
  MPI_Finalize();
  return 0;
}
