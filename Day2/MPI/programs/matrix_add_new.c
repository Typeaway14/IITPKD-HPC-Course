#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define MAT_SIZE 32

void Matrix_add(int A[MAT_SIZE][MAT_SIZE], int B[MAT_SIZE][MAT_SIZE],
                int start_row, int end_row) {
  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < MAT_SIZE; j++) {
      B[i][j] = A[i][j] + B[i][j];
    }
  }
}

int main(int argc, char *argv[]) {
  int rank, size;
  int limit;
  double start_time = 0.0, end_time = 0.0;
  double elapsed = 0.0;
  while (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    MPI_Init(&argc, &argv);
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int A[MAT_SIZE][MAT_SIZE];
  int B[MAT_SIZE][MAT_SIZE];
  limit = MAT_SIZE / size;
  int start_row = rank * limit;
  int end_row = start_row + limit;
  for (int i = 0; i < MAT_SIZE; i++) {
    for (int j = 0; j < MAT_SIZE; j++) {
      A[i][j] = 2;
      B[i][j] = 3;
    }
  }
  start_time = MPI_Wtime();
  MPI_Scatter(A, limit * MAT_SIZE, MPI_INT, A[start_row], limit, MPI_INT, 0,
              MPI_COMM_WORLD);
  MPI_Scatter(B, limit * MAT_SIZE, MPI_INT, B[start_row], limit, MPI_INT, 0,
              MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  Matrix_add(A, B, start_row, end_row);

  MPI_Gather(B[start_row], limit * MAT_SIZE, MPI_INT, B, MAT_SIZE * limit,
             MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  end_time = MPI_Wtime();
  if (rank == 0) {
    elapsed = end_time - start_time;
    printf("Elapsed time = %4.2lf seconds. \n", elapsed);
    for (int i = 0; i < MAT_SIZE; i++) {
      for (int j = 0; j < MAT_SIZE; j++) {
        printf(" %d ", B[i][j]);
      }
      printf("\n");
    }
  }

  return 0;
}
