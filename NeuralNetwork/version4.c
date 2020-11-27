/*

For this version the dataset fashion nist from https://www.kaggle.com/zalando-research/fashionmnist is needed

gcc version4.c -lm -O3             ->seirial
time ./a.out

real    15m2,891s
user    15m2,305s
sys     0m0,437s

gcc version4.c -lm -fopenmp -O3    ->parallel
time ./a.out

real    6m24,408s
user    24m54,988s
sys     0m12,696s

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// *****************************

#define N  754//2
#define N1 100//2
#define N2 10//2
#define a  0.25

//double Vector[N];//={0.6,0.4};

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
double Vector[60000][784] ;
unsigned char Class[60000] ;
double Test[10000][784] ;
unsigned char ClassT[60000] ;

// ***************************************
void readData(int f){
    FILE *fp ;
        char B[20001], *p ;
        int j ;

        if (f==0){
            fp = fopen( "fashion-mnist_train.csv", "r" ) ;
            
            if ( fp == NULL )
                    printf("error 1");

            if ( fgets( B, 20000, fp ) != B ) 
                    printf("error 2") ;

            for( j = 0 ; j < 60000 ; j++ ){
                if ( fgets( B, 20000, fp ) != B ) 
                    printf("error 2")    ;

                p = strtok(B,",") ;
                if ( p == NULL )
                    printf("error 3") ;
                Class[j] = atoi(p) ;

                for( int i = 0 ; i < 784 ; i++ )  {
                        p = strtok(NULL,",\n" ) ;
                        Vector[j][i] = atof(p)/255.0 ;
                        }
            }
        }
        else{
            fp = fopen( "fashion-mnist_test.csv", "r" ) ;
            if ( fp == NULL )
                    printf("error 1");

            if ( fgets( B, 20000, fp ) != B ) 
                    printf("error 2") ;

            for( j = 0 ; j < 10000 ; j++ ){
                if ( fgets( B, 20000, fp ) != B ) 
                    printf("error 2")    ;

                p = strtok(B,",") ;
                if ( p == NULL )
                    printf("error 3") ;
                ClassT[j] = atoi(p) ;

                for( int i = 0 ; i < 784 ; i++ )  {
                        p = strtok(NULL,",\n" ) ;
                        Test[j][i] = atof(p)/255.0 ;
                        }
            }
        }
        
        

        fclose(fp) ;
}


// ***************************************
//energopoihsh tou neurwnikou
void activateN( double *Vector){
    #pragma omp parallel for
    for(int i=0;i<N1;i++){//kryfo epipedo
        double y1=0;
        DL1[i]=0;
        OL1[i]=0;
        
        #pragma omp simd
        for(int j=0;j<N;j++){
            y1=y1+WL1[i][j]*Vector[j];
        }
        DL1[i]=y1+WL1[i][N];
        OL1[i]=1/(1+exp(-DL1[i]));
        //printf("y(%d)=%f \n",i,DL1[i]);
        //printf("O(%d)=%f \n",i,OL1[i]);
    }
    #pragma omp parallel for
    for(int i=0;i<N2;i++){
        double y2=0;
        DL2[i]=0;
        OL2[i]=0;
        #pragma omp simd
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
    double d2[N2],d1[N1];
    #pragma omp simd 
    for(int i=0;i<N2;i++){
        d2[i]=(-2)*(exp(DL2[i])/(1+exp(DL2[i]))/(1+exp(DL2[i])))*(OL2[i]-desired[i]);   
        //d2[i]=(-2)*DL2[i]*(1-DL2[i])*(OL2[i]-desired[i]);   
    }                                                       
                                                            
    #pragma omp parallel for
    for(int i=0;i<N1;i++){
        double x=0;
        #pragma omp simd reduction(+:x)
        for(int j=0;j<N2;j++){
            x=x+d2[j]*WL2[j][i];        
        } 
        d1[i]=x*(exp(DL1[i])/(1+exp(DL1[i]))/(1+exp(DL1[i])));                              
        //d1[i]=x*DL1[i]*(1-DL1[i]);      
    }                                   
    #pragma omp parallel for
    for(int i=0;i<N2;i++){
        #pragma omp simd
        for (int j=0;j<N1;j++){
            float dw=a*d2[i]*OL1[j];    
            WL2[i][j]=WL2[i][j]+dw;     
            //printf("%f \n",WL2[i][j]);
        }

    }

    #pragma omp parallel for
    for(int i=0;i<N1;i++){
        for (int j=0;j<N;j++){
            float dw=a*d1[i]*input[j]; 
            WL1[i][j]=WL1[i][j]+dw;     
            //printf("%f \n",WL1[i][j]);
        }

    }

};


// *****************************************************
void main(void){
    double y;
    //double Vector[N];
    srand(1234);
    
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
    int f=0;
    readData(f);                                 
    double x[N];
    int s=rand()%764;
    for (int i=0;i<N;i++){
        x[i]=Vector[s][i];
    }
    for(int i=0;i<N2;i++){
        if (Class[s]==i){
            b[i]=1;
        }
        else{
            b[i]=0;
        }   
    }
    activateN(x);                          
    for (int t = 0; t < 6000000; t++){
        trainN(x,b);                       
            int s=rand()%764;

        for (int i=0;i<N;i++){
            x[i]=Vector[s][i];
        }
        for(int i=0;i<N2;i++){
            if (Class[s]==i){
                b[i]=1;
            }
            else{
                b[i]=0;
            }   
        }
        activateN(x);                      
    }
    f=1;
    readData(f);
    float q=0;
    
    for(int i=0;i<10000;i++){
        for (int j=0;j<N;j++){
            x[j]=Test[i][j];
        }
        for(int j=0;j<N2;j++){
            if (ClassT[i]==j){
                b[j]=1;
            }
            else{
                b[j]=0;
            }   
        }
        activateN(x);
        y=0;
        for (int k=0;k<N2;k++){
            y=y+((OL2[k]-b[k])*(OL2[k]-b[k]));
            }
        q=q+y;
        //printf("%f\n",sqrt(y));
    }
    //printf("apostash: %f\n",sqrt(q));
    
};
