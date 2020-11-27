#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 10
#define Nv 2
#define Nc 2
#define THRESHOLD 0.000001


float Vec[10][2]={{1,2},{ 2,3},{3,4},{ 4,5},{ 5,6},{ 6,7},{ 7,8},{ 8,9},{9,10},{10,11}};
float Center[2][2]={{3,4},{8,9}};
float Class[10][2];

float estimateClass( void )
{
    
    float min,mdist=0;
    for(int i=0;i<N;i++){
        int th=-1;

        min=350000;
        for (int j=0;j<Nc;j++){
            float dist=0;
            for (int k=0;k<Nv;k++){
                dist=dist+(Vec[i][k]-Center[j][k])*(Vec[i][k]-Center[j][k]);//parallel here
            }
            dist=sqrt(dist);
            if (min>dist){
                min=dist;
                th=j;
            }
        }
        Class[i][1]=th;
        Class[i][2]=min;
        mdist=mdist+min;
    }

    return mdist;
}


void main(){
    float a=estimateClass();
    for (int i=0;i<N;i++)
        printf("%f %f\n", Class[i][1],Class[i][2]);
}
