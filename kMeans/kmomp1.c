/*
gcc kmomp1.c -lm -O2
time ./a.out

pc1		     |pc2
real    5m0,372s    |real    5m11,525s
user    5m0,179s    |user    5m11,213s
sys     0m0,160s    |sys     0m0,184s

Profiler:
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
 99.35     20.52    20.52                             estimateClass
  0.53     20.63     0.11                             estimateCenter
  0.29     20.69     0.06                             __libc_csu_init
  0.05     20.70     0.01                             SetVec

estimateClass is the one that is going to be "paralled"
*/

//#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline", "unsafe-math-optimizations")
//#pragma GCC option("arch=native","tune=native","no-zero-upper")
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
totDist = estimateClass() ;
estimateCenter() ;
} while((prevDist-totDist)/totDist> THRESHOLD ) ;
/*
for (int i=0;i<Nc;i++){
    for (int j=0;j<Nv;j++)
        printf(" %f --",Center[i][j]);
    printf("\n");}
*/
printf("%f \n",totDist);

return 0 ;
}
