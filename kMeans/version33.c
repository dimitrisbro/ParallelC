#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 6
#define Nv 2
#define Nc 2
#define THRESHOLD 0.0001


float Vec[6][2]={{5,3},{ 2,3},{3,4},{ 13,14},{ 5,6},{ 6,7}/*,{ 1,2},{ 8,9},{9,10},{10,11}*/};
float Center[2][2]={{3,4},{5,6}};
int Class[10];

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
            if (min>dist){
                min=dist;
                th=j;
            }
        }
        Class[i]=th;
        //printf(" %d ^^",Class[i]);
        mdist=mdist+min;
    }

    return mdist;
}

void estimateCenter( void )
{   
    for (int i=0;i<Nc;i++)
        for (int j=0;j<Nv;j++){
            printf(" %f --",Center[i][j]);
        }
        printf("\n");
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
            //printf(" %f --",Center[i][j]);
        }
       // printf("\n");
}

void main()
{
float totDist ;
float prevDist ;

totDist = 1.0e30 ;
do {
prevDist = totDist ;
totDist = estimateClass() ;
estimateCenter() ;
//printf("%f\n",(prevDist-totDist)/totDist);
} while((prevDist-totDist)/totDist> THRESHOLD ) ;
/*
    for (int i=0;i<Nc;i++)
        for (int j=0;j<Nv;j++){
            printf("%f --",Center[i][j]);
        }
    printf("\n");
    for (int j=0;j<N;j++){
            printf("%d xx",Class[j]);
        }
*/
}
