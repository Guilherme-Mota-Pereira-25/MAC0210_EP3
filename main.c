#include <math.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char *argv[]) {
    int points = 10;
    double weights[points]; double y[points]; double x[points];
    // Leitura do arquivo de pontos:
    FILE* ptr;
    ptr = fopen("input.txt","r");
    if (NULL == ptr) { 
        printf("erro ao carregar o arquivo.");
        return 1;
    }
        fread(&points, sizeof(int),1,ptr);
        double input_values[3][points];
        int column = 0;
        while (!feof(ptr)) {
            //1 1 1
            //2 2 2
            //...
            fread(&input_values[0][column], sizeof(double),1,ptr);
            fread(&input_values[1][column], sizeof(double),1,ptr);
            fread(&input_values[2][column], sizeof(double),1,ptr);
            column++;
        }
    double ys[points];
    for (int i = 0; i < points; i++)
        ys[i] = input_values[1][i]*cos(input_values[2][i]);
    
   
    LagrangeWeights(input_values[0],weights,ys);
    return 0;
}

int LagrangeWeights(double* xs, double* weights, double* y) {

}