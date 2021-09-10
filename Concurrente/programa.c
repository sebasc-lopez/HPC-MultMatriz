#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>


int n, hilos;

int numHilos;
int row, col;
double **matA;
double **matB;
double **matC;



void llenarMatriz(int col, int row)
{
  int i, j;

  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      matA[i][j] = rand() % 9;
    }
  }

  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      matB[i][j] = rand() % 9;
    }
  }
}

void print_result(int col, int row)
{
  int i, j;

  printf("Matriz A : \n");
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      printf("%.0lf ", matA[i][j]);
    }
    printf("\n");
  }

  printf("Matriz B : \n");
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      printf("%.0lf ", matB[i][j]);
    }
    printf("\n");
  }

  printf("Matriz C : \n");
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      printf("%.0lf ", matC[i][j]);
    }
    printf("\n");
  }
}


void *matmul(void *id_arg)
{
  int i, j, k;
  long id = (long)id_arg;
  int filasxHilo= col / numHilos;
  int inicio = id * filasxHilo;
  int final;
  if (id + 1 == numHilos)
  {
    final = row;
  }
  else
  {
    final = (id + 1) * filasxHilo;
  }
  for (i = inicio; i < final; i++)
  {
    for (j = 0; j < col; j++)
    {
      for (k = 0; k < row; k++)
      {
        matC[i][j] += matA[i][k] * matB[k][j];
      }
    }
  }
}

int main(int argc, char *argv[])
{

  numHilos = strtol(argv[2], NULL, 10);
  row = strtol(argv[1], NULL, 10), col = strtol(argv[1], NULL, 10);
  srand(time(NULL));

  pthread_t tid[numHilos];
  int i, j;
  long rank;

  matA = malloc(row * sizeof(double *));
  for (i = 0; i < row; i++)
    matA[i] = malloc(col * sizeof(double));

  matB = malloc(row * sizeof(double *));
  for (i = 0; i < row; i++)
    matB[i] = malloc(col * sizeof(double));

  matC = malloc(row * sizeof(double *));
  for (i = 0; i < row; i++)
    matC[i] = malloc(col * sizeof(double));


  llenarMatriz(col, row);

  struct timeval inicio;
  struct timeval fin;
  double milisegundosFinal;
  long segundos, milisegundos;

  gettimeofday(&inicio, 0);


  for (rank = 0; rank < numHilos; rank++)
    pthread_create(&tid[rank], NULL, matmul, (void *)rank);


  for (rank = 0; rank < numHilos; rank++)
    pthread_join(tid[rank], NULL);

  gettimeofday(&fin, 0);


  segundos = fin.tv_sec - inicio.tv_sec;
  milisegundos = fin.tv_usec - inicio.tv_usec;
  milisegundosFinal = ((segundos)*1000 + milisegundos / 1000.0);

  printf("Tiempo de ejecucion:\t");
  printf("%.16g milisegundos\n", milisegundosFinal);

  FILE *archivo;                         
  archivo = fopen("documento.txt", "a");
  fprintf(archivo, "%d\n", row);
  fprintf(archivo, "%.16g\n", milisegundosFinal);

  /*print_result(col,row);*/

  
  free(matA);
  free(matB);
  free(matC);

 
  pthread_exit(NULL);

  return 0;
}


