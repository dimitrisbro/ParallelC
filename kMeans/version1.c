#include <stdio.h>
#include <stdlib.h>

// ***************************************************
#define N 10
#define Nv 2
#define Nc 2
#define THRESHOLD 0.000001

// ***************************************************
float Vec[N][Nv] ;// array with vectors
float Center[Nc][Nv] ; //array with centers
int Class[N] ; //array with the class of each center

// ***************************************************
void initCenter( void )
{
//centers initialization
}

// ***************************************************
// returns the total minimum distance
// ***************************************************
float estimateClass( void )
{
//finds class and returns the distance from the center
}

// ***************************************************
void estimateCenter( void )
{
//finds the centers
}


// ***************************************************
void SetVec( void )
//vectors initialization 
{
int i, j ;
for( i = 0 ; i< N ; i++ )
for( j = 0 ; j < Nv ; j++ )
Vec[i][j] = (1.0*rand())/RAND_MAX ;
}

// ***************************************************
void printVec( void )
{
int i, j ;

for( i = 0 ; i< N ; i++ )
for( j = 0 ; j < Nv ; j++ )
printf( "%f\n", Vec[i][j] );
}

// ***************************************************
int main()
{
float totDist ;
float prevDist ;

SetVec() ;
printVec() ;
initCenter() ;

totDist = 1.0e30 ;
do {
prevDist = totDist ;
totDist = estimateClass() ;
estimateCenter() ;
} while((prevDist-totDist)/totDist> THRESHOLD ) ;

return 0 ;
}
