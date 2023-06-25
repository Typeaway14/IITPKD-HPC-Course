#include <stdio.h>
#include <stdlib.h>

int recurse_fibo(int n, int *cache);

int main(int argc, char *argv[]) {
  int n = 40;
  if (argc == 2) {
    n = atoi(argv[1]);
  }
  int cache[n];
  for (int i = 0; i < n; i++) {
    cache[i] = -1;
  }
  for (int i = 0; i < n; i++)
    printf(" Fibo(%d) = %d \n", i, recurse_fibo(i, cache));
}

int recurse_fibo(int n, int *cache) {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  if (cache[n] != -1) {
    return cache[n];
  }
  cache[n] = recurse_fibo(n - 1, cache) + recurse_fibo((n - 2), cache);
  return cache[n];
  // return recurse_fibo(n - 1, cache) + recurse_fibo(n - 2, cache);
}
