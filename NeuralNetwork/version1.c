/*
gcc version1.c
time ./a.out

O(0)=0.109484 
O(1)=0.500659 
O(2)=0.979413 
O(3)=0.348445 
O(4)=0.625606 
O(5)=0.796999 
O(6)=0.637712 
O(7)=0.455059 
O(8)=0.401492 
O(9)=0.343085 

real    0m0,006s
user    0m0,000s
sys     0m0,006s
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// *****************************
/*
definition of the size of the NN
*/
#define N  12 //2
#define N1 100 //2
#define N2 10 //2

//double Vector[N];//={0.2,0.4};
double WL1[N1][N+1];/*={
    {0.5,0.6,0.3},
    {0.2,0.4,0.7}
};*/
double WL2[N2][N1+1];/*={
    {0.3,0.5,-0.7},
    {-0.7,0.5,0.3}
};*/
double DL1[N1];
double DL2[N2];
double OL1[N1]; 
double OL2[N2];

// ***************************************
//activation of the NN
void activateN( double *Vector){
    for(int i=0;i<N1;i++){//hidden layer
        double y1=0;
        for(int j=0;j<N;j++){
            y1=y1+WL1[i][j]*Vector[j];//internal state of the neuron
        }
        DL1[i]=y1+WL1[i][N];
        OL1[i]=1/(1+exp(-DL1[i]));//result of each neuron
        //printf("y(%d)=%f \n",i,DL1[i]);
        //printf("O(%d)=%f \n",i,OL1[i]);
    }
    for(int i=0;i<N2;i++){//exit layer
        double y2=0;
        for(int j=0;j<N1;j++){
            y2=y2+WL2[i][j]*OL1[j];
        }
        DL2[i]=y2+WL2[i][N1];
        OL2[i]=1/(1+exp(-DL2[i]));
        //printf("y(%d)=%f \n",i,DL2[i]);
        printf("O(%d)=%f \n",i,OL2[i]);
    }
};
void trainN( double *input, double *desired ) ;

void main(void){
    double Vector[N];
    srand(1234);
    for(int i=0;i<N;i++){
        Vector[i]=1.0*rand()/RAND_MAX;//random values from 0 to 1 for the input
        //printf("%f \n",Vector[i]);
    }
    for(int i=0;i<N1;i++){
        for(int j=0;j<N+1;j++){
            WL1[i][j]=1.0*(rand()-rand())/RAND_MAX;//random values from -1 to 1 for the weights 
                                                    
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
};
