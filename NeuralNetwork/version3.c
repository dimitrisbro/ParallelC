/*
gcc version3.c -lm
time ./a.out

distance: 0.000040

real    0m0,016s    ->serial is faster
user    0m0,010s
sys     0m0,006s

// #########################################

gcc version3.c -lm -pg
time ./a.out 
gprof

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
100.11      0.03     0.03      300   100.11   100.11  trainN        -> need to be "paralleled"
  0.00      0.03     0.00      301     0.00     0.00  activateN

// ########################################

gcc version3.c -lm -fopenmp -O3 
time ./a.out

apostash: 0.000040

real    0m0,007s    -> faster
user    0m0,007s
sys     0m0,001s

//###########################################
adding #pragma omp simd in the loops of train

gcc version3.c -lm -fopenmp -O3 
time ./a.out

apostash: 0.000040

real    0m0,067s    -> faster
user    0m0,006s
sys     0m0,000s
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// *****************************

#define N  12//2
#define N1 100//2
#define N2 10//2
#define a  0.25

double Vector[N];//={0.6,0.4};

double WL1[N1][N+1];/*={
    {0.5,0.6,0.3},
    {0.2,0.4,0.2}
};*/
double WL2[N2][N1+1];/*={
    {0.3,0.5,-0.1},
    {-0.7,0.5,0.3}
};*/
double DL1[N1];
double DL2[N2];
double OL1[N1]; 
double OL2[N2];
double b[N2];//={0.05,0.95};

// ***************************************

void activateN( double *Vector){
    for(int i=0;i<N1;i++){
        double y1=0;
        DL1[i]=0;
        OL1[i]=0;
        
        for(int j=0;j<N;j++){
            y1=y1+WL1[i][j]*Vector[j];
        }
        DL1[i]=y1+WL1[i][N];
        OL1[i]=1/(1+exp(-DL1[i]));
        //printf("y(%d)=%f \n",i,DL1[i]);
        //printf("O(%d)=%f \n",i,OL1[i]);
    }
    for(int i=0;i<N2;i++){
        double y2=0;
        DL2[i]=0;
        OL2[i]=0;
        for(int j=0;j<N1;j++){
            y2=y2+WL2[i][j]*OL1[j];
        }
        DL2[i]=y2+WL2[i][N1];
        OL2[i]=1/(1+exp(-DL2[i]));
        //printf("y(%d)=%f \n",i,DL2[i]);
        //printf("O(%d)=%f \n",i,OL2[i]);
    }
};

// ***********************************************
void trainN(double *input, double *desired ){
    double d2[N2],d1[N1],y;
    //#pragma omp simd
    for(int i=0;i<N2;i++){
        d2[i]=(-2)*(exp(DL2[i])/(1+exp(DL2[i]))/(1+exp(DL2[i])))*(OL2[i]-desired[i]);   
        //d2[i]=(-2)*DL2[i]*(1-DL2[i])*(OL2[i]-desired[i]);   
    }                                                       
                                                            
    
    //#pragma pragma omp parallel for
    for(int i=0;i<N1;i++){
        double x=0;
        #pragma omp simd
        for(int j=0;j<N2;j++){
            x=x+d2[j]*WL2[j][i];        
        } 
        d1[i]=x*(exp(DL1[i])/(1+exp(DL1[i]))/(1+exp(DL1[i])));                              
        //d1[i]=x*DL1[i]*(1-DL1[i]);      
    }                                   

    //#pragma pragma omp parallel for
    for(int i=0;i<N2;i++){
        #pragma omp simd
        for (int j=0;j<N1;j++){
            float dw=a*d2[i]*OL1[j];    
            WL2[i][j]=WL2[i][j]+dw;     
            //printf("%f \n",WL2[i][j]);
        }

    }

    //#pragma pragma omp parallel for
    for(int i=0;i<N1;i++){
        #pragma omp simd
        for (int j=0;j<N;j++){
            float dw=a*d1[i]*Vector[j]; 
            WL1[i][j]=WL1[i][j]+dw;     
            //printf("%f \n",WL1[i][j]);
        }

    }

};


// *****************************************************
void main(void){
    double y;
    double Vector[N];
    srand(1234);
    for(int i=0;i<N;i++){
        Vector[i]=1.0*rand()/RAND_MAX;
        //printf("%f * ",Vector[i]);
    }
    printf("\n");
    
    for(int i=0;i<N2;i++){
        b[i]=1.0*rand()/RAND_MAX;
        //printf("%f * ",b[i]);
    }
    //printf("\n");
    
    for(int i=0;i<N1;i++){
        for(int j=0;j<N+1;j++){
            WL1[i][j]=1.0*(rand()-rand())/RAND_MAX;
                                                    
            //printf("%f \n",WL1[i][j]);
        }
        //printf("\n");
    }
    for(int i=0;i<N2;i++){
        for(int j=0;j<N1+1;j++){
            WL2[i][j]=1.0*(rand()-rand())/RAND_MAX;
            //printf("%f \n",WL2[i][j]);
        }
        //printf("\n");
    }
    activateN(Vector);                          
    for (int t = 0; t < 300; t++){
        trainN(Vector,b);                       
        activateN(Vector);                      
        y=0;
        for (int i=0;i<N2;i++){
            y=y+((OL2[i]-b[i])*(OL2[i]-b[i]));
            }
        //printf("%f\n",sqrt(y));
        }
    printf("apostash: %f\n",sqrt(y));
    /*
    for (int k=0;k<N2;k++)    
        printf("%f *",OL2[k]);
        printf("\n");*/
        /*
    printf("%f",OL2[0]);
    printf("%f\n",OL2[1]);
    printf("%f",c);
    printf("%f",f);*/

    /*activateN(Vector);
    for (int i=0;i<N2;i++){
        y=sqrt((Vector[i]-b[i])*(Vector[i]-b[i]));
    }
    printf("%f\n",y);}*/
};
