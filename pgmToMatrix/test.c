#include <stdlib.h>
#include <stdio.h>

#define HI(num) (((num) & 0x0000FF00) >> 8) 
#define LO(num) ((num) & 0x000000FF) 

typedef struct PGMData {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

int **allocate_dynamic_matrix(int row, int col) {
    int **ret_val;
    int i;

    ret_val = (int **) malloc(sizeof(int *) * row);
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(1);
    }
 
    for (i = 0; i < row; ++i) {
         ret_val[i] = (int *)malloc(sizeof(int) * col);
         if (ret_val[i] == NULL) {
             perror("memory allocation failure");
             exit(2);
         }
    }

    return ret_val;
}

void deallocate_dynamic_matrix(int **matrix, int row) {
    
    int i;

    for(i = 0; i < row; i = i + 1)
        free(matrix[i]);
    free(matrix);
}

void skipComments(FILE *fp) {
    int ch;
    char line[100];

    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;

    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        skipComments(fp);
    }
    else {
        fseek(fp, -1, SEEK_CUR);
    }
}

int main() {
  
  PGMData *stuff;
  
  stuff = (PGMData *) malloc(sizeof(PGMData));

  printf("%x\n", stuff);
  printf("%x\n", &stuff);

}
