#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double EvaluateInterpolation(int size, double x, double* xs, double* ws, double* wys, double* ys){
  for(int i = 0; i < size; i++){
    if(xs[i] == x){
      return ys[i];
    }
  }
  double num = 0, den = 0;
  for(int i = 0; i < size; i++){
    num += wys[i]/(x-xs[i]);
    den += ws[i]/(x-xs[i]);
  }
  return num/den;
}

int LagrangeWeights(int size, double* ws, double* wys, double* xs, double* os , double* ys) {
  for(int i = 0; i < size; i++){
    double t = 1;
    for(int j = 0; j < i; j++){
      t *= xs[i] - xs[j];
    }
    for(int j = i+1; j < size; j++){
      t *= xs[i] - xs[j];
    }
    ws[i] = 1/t;
    wys[i] = (ys[i]*cos(os[i]))/t;
  }
  return 0;
}

int main(int argc, char *argv[]) {
    
    // Leitura do arquivo de pontos:
    FILE* ptr;
    ptr = fopen("input.txt","r");
    if (NULL == ptr) { //Teste de carga do arquivo
        printf("Erro ao carregar o arquivo\n");
        return 1;
    }
    char words[50];
    fgets(words,50,ptr);
    int points = 0;
    sscanf(words,"%d",&points);
    
    double ws[points], wys[points], ys[points], xs[points], os[points];
    int column = 0;

    
    while (!feof(ptr) && column < points) {
      fgets(words,50,ptr);
      char new_word[50];
      int i = 0;
      int j = 0;
      while(words[i] != ' '){
	new_word[i] = words[j+i];
	i++;
      }
      new_word[j+i] = '\0';
      xs[column] = atof(new_word);
      j = i+1;
      i = 0;
      while(words[i] != ' '){
	new_word[i] = words[j+i];
	i++;
      }
      new_word[j+i] = '\0';
      os[column] = atof(new_word);
      j = j + i + 1;
      i = 0;
      while(words[i] != '\n'){
	new_word[i] = words[j+i];
	i++;
      }
      new_word[j+i] = '\0';
      ys[column] = atof(new_word);
      
      column++;
    }
   
    LagrangeWeights(column,ws,wys,xs,os,ys);

    for(int i = 0; i < column; i++){
      printf("%f %f\n", ws[i], wys[i]);
    }
    
    return 0;
}


