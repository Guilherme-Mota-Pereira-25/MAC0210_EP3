#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Avalia um ponto x com base no polinomio de lagrange
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

//Constroi o polinomio de lagrange
int LagrangeWeights(int size, double* ws, double* wys, double* xs, double* ys) {
  for(int i = 0; i < size; i++){
    double t = 1;
    for(int j = 0; j < i; j++){
      t *= xs[i] - xs[j];
    }
    for(int j = i+1; j < size; j++){
      t *= xs[i] - xs[j];
    }
    ws[i] = 1/t;
    wys[i] = ys[i]/t;
  }
  return 0;
}

//Calcula a integral com base no metodo do trapezio
double TrapIntegral(double x1, double x2, int size, double* xs, double* ws, double* wys, double* ys, double* wos, double* wyos, double* os){
  double fx1 = EvaluateInterpolation(size,x1,xs,ws,wys,ys)*EvaluateInterpolation(size,x1,xs,wos,wyos,os);
  double fx2 = EvaluateInterpolation(size,x2,xs,ws,wys,ys)*EvaluateInterpolation(size,x2,xs,wos,wyos,os);
  return ((x2-x1)/2.0)*(fx1+fx2);
}

//Calcula a integral por meio do metodo de integracao composta com o metodo do trapezio
double CompTrapIntegral(double x1, double x2, int r, int size, double* xs, double* ws, double* wys, double* ys, double* wos, double* wyos, double* os){
  double area = 0;
  double start = 0;
  double interval = 0.0;
  //Ordena x1 e x2
  if(x1>x2){
    start = x2;
    interval = (x1-x2)/r;
  }
  else{
    start = x1;
    interval = (x2-x1)/r;
  }
  double next = start + interval;
  for(int i = 0; i < r; i++){
    area = area + TrapIntegral(start,next, size, xs, ws, wys, ys, wos, wyos, os);
    start = next;
    next = next + interval;
  }
  return area;
}

//Calcula a integral com base no metodo de Simpson
double SimpIntegral(double x1, double x2, int size, double* xs, double* ws, double* wys, double* ys, double* wos, double* wyos, double* os){
  double fx1 = EvaluateInterpolation(size,x1,xs,ws,wys,ys)*EvaluateInterpolation(size,x1,xs,wos,wyos,os);
  double fxm = EvaluateInterpolation(size,(x2+x1)/2.0,xs,ws,wys,ys)*EvaluateInterpolation(size,(x2+x1)/2,xs,wos,wyos,os);
  double fx2 = EvaluateInterpolation(size,x2,xs,ws,wys,ys)*EvaluateInterpolation(size,x2,xs,wos,wyos,os);
  return ((x2-x1)/6.0)*(fx1+4*fxm+fx2);
}

//Calcula a integral por meio do metodo de integracao composta com o metodo de Simpson
double CompSimpIntegral(double x1, double x2, int r, int size, double* xs, double* ws, double* wys, double* ys, double* wos, double* wyos, double* os){
  double area = 0;
  double start = 0;
  double interval = 0.0;
  //Ordena x1 e x2
  if(x1>x2){
    start = x2;
    interval = (x1-x2)/r;
  }
  else{
    start = x1;
    interval = (x2-x1)/r;
  }
  double next = start + interval;
  for(int i = 0; i < r; i++){
    area = area + SimpIntegral(start,next, size, xs, ws, wys, ys, wos, wyos, os);
    start = next;
    next = next + interval;
  }
  return area;
}

//Gera um numero aleatorio
double rng(){
  return ((double) rand())/((double) RAND_MAX);
}

//Funcao estipulada para ser avaliada pelo metodo de Monte Carlo
double MonteCarloFunction(double x, double y){
  if(x*x + y*y <= 1){
    return 1;
  }
  else{
    return 0;
  }
}

//Avalia o metodo de Monte Carlo para uma dimensao
double MonteCarlo(double a, double b,int n){
  double area = 0.0;
  double u = 0.0;
  for(int i = 0; i < n; i++){
    u = rng(); //Gera um valor aleatorio
    area = area + MonteCarloFunction((b-a)*u+a,0.0); //Avalia a funcao em (b-a)*u+a
  }
  return ((b-a)*area)/n; //Area/n multiplicada pelo fator de correcao de du/dx 
}

//Avalia o metodo de Monte Carlo para uma dimensao
double TwoDimMonteCarlo(double a, double b, double c, double d, int n){
  double area = 0.0;
  double u = 0.0;
  double v = 0.0;
  for(int i = 0; i < n; i++){
    u = rng(); //Gera um valor aleatorio
    v = rng(); //Gera um valor aleatorio
    area = area + MonteCarloFunction((d-c)*u+c,(b-a)*v+a); //Avalia a funcao em (d-c)*u+c,(b-a)*v+a
  }
  return (area/n)*((d-c)*(b-a));//Area/n multiplicada pelo fator de correcao de dudv/dxdy 
}

int main(int argc, char *argv[]) {
    
    // Leitura do arquivo de pontos:
    FILE* ptr;
    ptr = fopen("input.txt","r");
    if (NULL == ptr) { //Teste de carga do arquivo
        printf("Erro ao carregar o arquivo\n");
        return 1;
    }

    srand(time(NULL)); //Muda o valor do gerador de numeros com base no tempo
    
    char words[50]; //Array de caracteres que armazenara as linhas do arquivo
    fgets(words,50,ptr);//Captura primeira linha com o valor da quantidade de linhas
    int points = 0; //Armazena a quantidade de linhas
    sscanf(words,"%d",&points);

    double xs[points]; //Array de valores de x
    double ys[points]; //Array de valores de y
    double ws[points], wys[points]; //Arrays de pesos da interpolacao de y
    double os[points]; //Array de valores de angulo
    double wos[points], wyos[points]; //Arrays de pesos da interpolacao do angulo


    //Processa o arquivo, armazenando os dados nos arrays
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
      while(words[j+i] != ' '){
	new_word[i] = words[j+i];
	i++;
      }
      new_word[j+i] = '\0';
      os[column] = cos(atof(new_word));
      j = j + i + 1;
      i = 0;
      while(words[j+i] != '\n'){
	new_word[i] = words[j+i];
	i++;
      }
      new_word[j+i] = '\0';
      ys[column] = atof(new_word);
      
      column++;
    }

    //Gera os pesos de Lagrange
    LagrangeWeights(points,ws,wys,xs,ys); //Gera os pesos de y
    LagrangeWeights(points,wos,wyos,xs,os); //Gera os pesos dos angulos




    //Parte de testes

    
    //printf("%f\n",CompTrapIntegral(0, 5, 10000, points, xs, ws, wys, ys, wos, wyos, os));
    //printf("%f\n",CompSimpIntegral(0, 5, 100, points, xs, ws, wys, ys, wos, wyos, os));
    
    /*for(int i = 0; i < points; i++){
      printf("%f %f %f %f %f\n",xs[i],os[i],ys[i],ws[i], wys[i]);
      }*/
    printf("%f\n",4*TwoDimMonteCarlo(0.0,1.0,0.0,1.0,1000000000));
    
    //printf("%f\n",EvaluateInterpolation(points, 16, xs,ws,wys,ys));
    return 0;
}


