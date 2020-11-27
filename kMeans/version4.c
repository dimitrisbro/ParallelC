/*
gcc version4.c -lm -pg
./a.out
gprof

Profiler:
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 99.26   1000.35  1000.35       25    40.01    40.01  estimateClass
  0.84   1008.85     8.49       25     0.34     0.34  estimateCenter
  0.05   1009.34     0.49        1     0.49     0.49  SetVec
  0.00   1009.34     0.00        1     0.00     0.00  initCenter

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// ***************************************************
#define N 100000
#define Nv 1000
#define Nc 100
#define THRESHOLD 0.000001

// ***************************************************
float Vec[N][Nv] ;
float Center[Nc][Nv] ;
int Class[N] ;

//****************************************************
void initCenter( void )
{       
	int i=0,d,f,p[Nc],j,k;
	srand(time(NULL));
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
float estimateClass( void )
{
    
    float min,mdist=0;
    for(int i=0;i<N;i++){
        int th=-1;

        min=350000;
        for (int j=0;j<Nc;j++){
            float dist=0;
            for (int k=0;k<Nv;k++){
                dist=dist+(Vec[i][k]-Center[j][k])*(Vec[i][k]-Center[j][k]);
            }
            dist=sqrt(dist);
            if (min>dist){
                min=dist;
                th=j;
            }
        }
        Class[i]=th;
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
        int a=Class[i];
        int flag=1;
        for (int j=0;j<i;j++){
            if (a==Class[j]){
                flag=0;
                break;
            }
        }
        if (flag==1){
            int s=0;
            for(int k=i;k<N;k++){
                if (a==Class[k]){
                    s++;
                    for (int m=0;m<Nv;m++){
                        b[y][m]=b[y][m]+Vec[k][m];
                    }
                }
            }
            for (int m=0;m<Nv;m++)
                b[y][m]=b[y][m]/s;/
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
totDist = estimateClass() ;
estimateCenter() ;
} while((prevDist-totDist)/totDist> THRESHOLD ) ;

for (int i=0;i<Nc;i++){
    for (int j=0;j<Nv;j++)
        printf(" %f --",Center[i][j]);
    printf("\n");}


return 0 ;
}
