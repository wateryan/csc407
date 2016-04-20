#include <stdio.h>  // to use printf, scanf, FILE, fopen, fgets, fclose, sscanf
#include <stdlib.h> // to use malloc, free
#include <string.h>

typedef struct { // define new struct type for storing
  char c;        // a char and an int
  int n;
} entry_t;

typedef entry_t *row_t; // define row type
typedef row_t *table_t; // define table type

table_t t;

void usage(int argc);
void allocate_table(int width, int height);
void print_table(table_t t, int num_rows, int num_cols);
void update_table(int num_rows, int num_cols, FILE *f);

int main(int argc, char *argv[]) {
  int num_rows; // number of rows
  int num_cols; // number of columns

  usage(argc);

  // if a command line argument is passed,
  //       open the file indicated by the first argument (argv[1])
  //       if not, read the input from stdin
  // create and initialize 2-D table
  // INPUT FORMAT : two positive integers num_rows num_cols
  //                list of num_rows x num_cols entries of the format
  //                (c,i) where c is a char(acter) and i is an int(eger)

  // INPUT EXAMPLE:
  // 3 4
  // (f,4)( ,0)( ,0)( ,0)( ,0)(x,7)( ,0)( ,0)( ,0)( ,0)( ,0)(d,9)
  char dataBuffer[BUFSIZ];

  FILE *f;
  if (argc == 2) {
    f = fopen(argv[1], "r");
    printf("Reading from %s\n", argv[1]);
  } else {
    f = stdin;
    printf("Reading from stdin\n");
  }
  fgets(dataBuffer, BUFSIZ, f);
  sscanf(dataBuffer, "%d %d", &num_rows, &num_cols);
  printf("Creating %d by %d table\n", num_rows, num_cols);
  allocate_table(num_rows, num_cols);

  //  print initial table in table format
  //       after reading the first two integers num_rows and num_cols
  //       (printing in list format would be printing nothing, see below)
  //
  // ( ,0) ( ,0) ( ,0) ( ,0)
  // ( ,0) ( ,0) ( ,0) ( ,0)
  // ( ,0) ( ,0) ( ,0) ( ,0)
  printf("Initial table: \n");
  print_table(t, num_rows, num_cols);

  // read either the input file or stdin and update table entries
  // close input file when done (nothing to do if reading from stdin).
  update_table(num_rows, num_cols, f);
  fclose(f);
  //  print updated table in table format
  // TABLE FORMAT EXAMPLE (space separated colums for each of the rows)
  //
  // (f,4) ( ,0) ( ,0) ( ,0)
  // ( ,0) (x,7) ( ,0) ( ,0)
  // ( ,0) ( ,0) ( ,0) (d,9)
  printf("Update table:\n");
  print_table(t, num_rows, num_cols);

  // print updated table in list format
  // LIST FORMAT EXAMPLE (only those entries that are not default: "( ,0)")
  // Each line should be formatted as "(i,j):(d,m)"
  // where i and j are the row and column of such non-default entry "(d,m)"
  //
  // (0,0):(f,4)
  // (1,1):(x,7)
  // (2,3):(d,9)
  printf("List view: \n");
  int i, j;
  for (i = 0; i < num_rows; i++) {
    for (j = 0; j < num_cols; j++) {
      entry_t entry = t[i][j];
      printf("(%d,%d):(%c,%d)\n", i, j, entry.c, entry.n);
    }
  }

  // free memory space allocated to table
  for (i = 0; i < num_rows; i++) {
    free(t[i]);
  }
  free(t);

  return 0;
}

void allocate_table(int num_rows, int num_cols) {
  int i, j;
  t = (row_t *)malloc(sizeof(row_t) * num_rows);
  for (i = 0; i < num_rows; i++) {
    t[i] = (entry_t *)malloc(sizeof(entry_t) * num_cols);
    for (j = 0; j < num_cols; j++) {
      t[i][j].c = ' ';
      t[i][j].n = 0;
    }
  }
}

void print_table(table_t t, int num_rows, int num_cols) {
  int i, j;
  for (i = 0; i < num_rows; i++) {
    for (j = 0; j < num_cols; j++) {
      entry_t entry = t[i][j];;
      printf("(%c,%d)", entry.c, entry.n);
    }
    printf("\n");
  }
  printf("\n");
}

void update_table(int num_rows, int num_cols, FILE *f) {
  int i, j, n, l;
  char buf[BUFSIZ];
  char c, *p;
  if(fgets(buf, BUFSIZ, f) != NULL) {
    p = buf;
    for (i = 0; i < num_rows; i++) {
        for(j = 0; j < num_cols; j++) {
          if(sscanf(p, "(%c,%d)%n", &c, &n, &l) > 0) {
            t[i][j].c = c;
            t[i][j].n = n;
            p += l;
          }
        }
    }
  }

  // if (fgets(buf, BUFSIZ, f) != NULL) {
  //   printf("%s", buf);
  //   p = buf;
  //   for (i = 0; i < num_rows; i++) {
  //     printf("%d %d\n", i, j);
  //     for (j = 0; j < num_cols; j++) {
  //       sscanf(p, "(%c,%d)%n", &c, &n, &l);
  //       printf("%c, %d : %c : %d %d : %d\n", c, n, *p, i, j, l);
  //       t[i][j]->c = c;
  //       t[i][j]->n = n;
  //       p += l;
  //     }
  //   }
  // }
  //
  //
  // if(fgets(buf, BUFSIZ, f) != NULL) {
  //   printf("%s\n", buf);
  //   char *p = buf;
  //   while(sscanf(p, "(%c,%d)%n", &c, &n, &i) > 0) {
  //     t[i][j]->c = c;
  //     t[i][j]->n = n;
  //     printf("%c, %d\n", c ,n);
  //     p += i;
  //   }
  // }
  // // t[i][j]->c = c;
  // // t[i][j]->n = n;
}

void usage(int argc) {
  if (argc != 1 && argc != 2) {
    printf("Usage: hw1 [file]\n");
  }
}
