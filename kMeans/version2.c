/*
gcc version2.c -lm


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
	int i=0,d,f,p[Nc],j,k;// p array for the old coordinates of the centers
	srand(time(NULL));
	while(i<Nc) {
		d=rand()%N;//random value for the centers initialization
        f=1;//flag in order not to appear centers with the same values 
        for(j=0;j<i;j++){
            if (p[j]==d){ //if rand value is the same with p, the search continues
                f=0;
                break;
            }
        }
        if (f==1){
            p[i]=d;// new value for p array
            for(k=0;k<Nv;k++){
                Center[i][k]=Vec[d][k]; // construction of the vector center
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
    for(int i=0;i<N;i++){//for each vec we find the dist from each center
        int th=-1;//for the min dist we mark the corresponding center

        min=350000;
        for (int j=0;j<Nc;j++){
            float dist=0;
            for (int k=0;k<Nv;k++){
                dist=dist+(Vec[i][k]-Center[j][k])*(Vec[i][k]-Center[j][k]);//Euclidean distance
            }
            dist=sqrt(dist);
            if (min>dist){
                min=dist;
                th=j;
            }
        }
        Class[i]=th;
        mdist=mdist+min;//total distance which is returned for the check 
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
        int a=Class[i];//takes one value from the class
        int flag=1;
        for (int j=0;j<i;j++){//checking if it was taken before
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
                b[y][m]=b[y][m]/s;//mean value
            y++;
        }
        i++;
    }while (i<N);
    
    for (int i=0;i<Nc;i++)
        for (int j=0;j<Nv;j++){
            Center[i][j]=b[i][j];//replacing with the new centers
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
