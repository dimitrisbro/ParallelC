/*
gcc kmomp4.c -lm -O3 -fopenmp
time ./a.out

pc1         |pc2
real    1m3,477s    |real    0m52,609s
user    3m46,741s   |user    5m3,918s
sys     0m0,270s    |sys     0m0,152s

-fopt-info
kmomp4.c:69:13: note: loop turned into non-loop; it never loops.
kmomp4.c:69:13: note: loop with 17 iterations completely unrolled
kmomp4.c:69:13: note: loop turned into non-loop; it never loops.
kmomp4.c:69:13: note: loop with 17 iterations completely unrolled
kmomp4.c:78:32: note: loop vectorized
kmomp4.c:53:13: note: Loop 3 distributed: split to 0 loops and 1 library calls.
kmomp4.c:132:9: note: Loop 7 distributed: split to 0 loops and 1 library calls.
kmomp4.c:101:9: note: Loop 9 distributed: split to 0 loops and 1 library calls.
kmomp4.c:124:13: note: loop vectorized
kmomp4.c:119:21: note: loop vectorized
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
    
    float mdist=0;

    #pragma omp parallel for reduction(+:mdist) 
    for(int i=0;i<N;i++){
        int th=-1;

        float min=350000;
        for (int j=0;j<Nc;j++){
            float dist=0;
            #pragma omp simd reduction(+:dist) 
            for (int k=0;k<Nv;k++){
                float dd=Vec[i][k]-Center[j][k];
                dd*=dd;
                dist+=dd;//vectorization
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
