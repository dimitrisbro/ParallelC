
#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline", "unsafe-math-optimizations")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// ***************************************************
#define N 10000
#define Nv 1000
#define Nc 100
#define THRESHOLD 0.000001

// ***************************************************
float Vec[N][Nv] ;
float Center[Nc][Nv] ;
int klash[N] ;

//****************************************************
void initCenter( void )
{       
	int i=0,d,f,p[Nc],j,k;
	srand(1234);
	while(i<Nc) {
		d=rand()%N;
        f=1;
        for(j=0;j<i;j++){
            if (p[j]==d){
                f=0;
                break;
            }
        }
        if (f==1){
            p[i]=d;
            for(k=0;k<Nv;k++){
                Center[i][k]=Vec[d][k];
            }
            i++;
        }
	}
}

// ***************************************************
// returns the total minimum distance
// *************************************************
float estimateklash( void )
{
    
    float mdist=0;
         
    #pragma omp parallel for reduction(+:mdist)                                   
        for(int i=0;i<N;i++){
            int th=-1;

            float min=350000;
            #pragma omp parallel for reduction(min:min)
            for (int j=0;j<Nc;j++){
                float dist=0;
                #pragma omp simd
                for (int k=0;k<Nv;k++){
                    float dd=Vec[i][k]-Center[j][k];
                    dd*=dd;
                    dist+=dd;
                }
                dist=sqrt(dist);
                if (min>dist){
                    min=dist;
                    th=j;
                }
            }
            klash[i]=th;
            mdist=mdist+min;
        }
    
    return mdist;
}


// ***************************************************
void estimateCenter( void )
{   
    float b[Nc][Nv];
    for (int i=0;i<Nc;i++)
        for (int j=0;j<Nv;j++)
            b[i][j]=0;

    int i=0,y=0;
    do{
        int a=klash[i];
        int flag=1;
        for (int j=0;j<i;j++){
            if (a==klash[j]){
                flag=0;
                break;
            }
        }
        if (flag==1){
            int s=0;
            for(int k=i;k<N;k++){
                if (a==klash[k]){
                    s++;
                    for (int m=0;m<Nv;m++){
                        b[y][m]=b[y][m]+Vec[k][m];
                    }
                }
            }
            for (int m=0;m<Nv;m++)
                b[y][m]=b[y][m]/s;
            y++;
        }
        i++;
    }while (i<N);
    
    for (int i=0;i<Nc;i++)
        for (int j=0;j<Nv;j++){
            Center[i][j]=b[i][j];
//            printf(" %f --",Center[i][j]);
        }
//        printf("\n");
}


// ***************************************************
void SetVec( void )
{
    int i, j ;
    srand(1234);
    for( i = 0 ; i< N ; i++ )
        for( j = 0 ; j < Nv ; j++ )
            Vec[i][j] = (1.0*rand())/RAND_MAX ;
}

// ***************************************************
void printVec( void )
{
int i ;

for( i = 0 ; i< Nc ; i++ )
    //for( j = 0 ; j < N ; j++ ){
        printf( "%d\n",i );}

//}

// ***************************************************
int main()
{
float totDist ;
float prevDist ;

SetVec() ;

initCenter() ;

totDist = 1.0e30 ;
do {
prevDist = totDist ;
totDist = estimateklash() ;
estimateCenter() ;
} while((prevDist-totDist)/totDist> THRESHOLD ) ;
printf("%f \n",totDist);
/*
for (int i=0;i<Nc;i++){
    for (int j=0;j<Nv;j++)
        printf(" %f --",Center[i][j]);
    printf("\n");}
*/

return 0 ;
}
