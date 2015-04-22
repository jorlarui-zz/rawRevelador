#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HI(num) (((num) & 0x0000FF00) >> 8) 
#define LO(num) ((num) & 0x000000FF) 

#define BUFSIZE 64

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

/* read PGM */
PGMData* readPGM(const char *file_name, PGMData *data) {
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;

    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(3);
    }

    fgets(version, sizeof(version), pgmFile);
    if(strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(4);
    }

    //Look up the pgm file format to understand this
    skipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->col);
    skipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->row);
    skipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);
   
    data->matrix = allocate_dynamic_matrix(data->row, data->col);
    if (data->max_gray > 255)
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j) {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data->matrix[i][j] = (hi << 8) + lo;
            }
    else
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j) {
                lo = fgetc(pgmFile);
                data->matrix[i][j] = lo;
            }
 
    fclose(pgmFile);
    return data;
}

void writePGM(const char *filename, const PGMData *data)
{
    FILE *pgmFile;
    int i, j;
    int hi, lo;
 
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(5);
    }
 
    fprintf(pgmFile, "P5 ");
    fprintf(pgmFile, "%d %d ", data->col, data->row);
    fprintf(pgmFile, "%d ", data->max_gray);
 
    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = HI(data->matrix[i][j]);
                lo = LO(data->matrix[i][j]);
                fputc(hi, pgmFile);
                fputc(lo, pgmFile);
            }
 
        }
    } else {
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j) {
                lo = LO(data->matrix[i][j]);
                fputc(lo, pgmFile);
            }
    }
 
    fclose(pgmFile);
    deallocate_dynamic_matrix(data->matrix, data->row);
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("./separateChannel <pgmFile> <outputFilename>\n");
    exit(6);
  }

  char r[BUFSIZE];
  char g1[BUFSIZE];
  char g2[BUFSIZE];
  char b[BUFSIZE];

  memset(r, 0, BUFSIZE);
  memset(g1, 0, BUFSIZE);
  memset(g2, 0, BUFSIZE);
  memset(b, 0, BUFSIZE);

  strcpy(r, "r_");
  strcpy(g1, "g1_");
  strcpy(g2, "g2_");
  strcpy(b, "b_");

  strcpy(r+2, argv[2]);
  strcpy(g1+3, argv[2]);
  strcpy(g2+3, argv[2]);
  strcpy(b+2, argv[2]);

  PGMData *picData;
  picData = (PGMData *) malloc(sizeof(PGMData));
  picData = readPGM(argv[1], picData);

  FILE *fp_r;
  FILE *fp_g1;
  FILE *fp_g2;
  FILE *fp_b;

  fp_r = fopen(r, "wb");
  if (fp_r == NULL) {
    perror("Cannot open file for writing");
    exit(7);
  }
  fp_g1 = fopen(g1, "wb");
  if (fp_g1 == NULL) {
    perror("Cannot open file for writing");
    exit(7);
  }
  fp_g2 = fopen(g2, "wb");
  if (fp_g2 == NULL) {
    perror("Cannot open file for writing");
    exit(7);
  }
  fp_b = fopen(b, "wb");
  if (fp_b == NULL) {
    perror("Cannot open file for writing");
    exit(7);
  }

  int i, j;
  for(i = 0; i < picData->row; i = i + 1) {
    for(j = 0; j < picData->col; j = j + 1) {
      //red channel
      if ((i%2 == 0) && (j%2 == 0)) { 
        fprintf(fp_r, "%d \n", picData->matrix[i][j]);
      }
      //green1 channel
      else if ((i%2 == 0) && (j%2 == 1)) { 
        fprintf(fp_g1, "%d \n", picData->matrix[i][j]);
      }
      //green2 channel
      else if ((i%2 == 1) && (j%2 == 0)) { 
        fprintf(fp_g2, "%d \n", picData->matrix[i][j]);
      }
      //blue channel
      else {
        fprintf(fp_b, "%d \n", picData->matrix[i][j]);
      }
    }
  }

  fclose(fp_r);
  fclose(fp_g1);
  fclose(fp_g2);
  fclose(fp_b);
}
