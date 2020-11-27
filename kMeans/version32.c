#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 10
#define Nv 2
#define Nc 2
#define THRESHOLD 0.000001


float Vec[10][2]={{35,22},{ 2,3},{3,4},{ 13,14},{ 5,6},{ 6,7},{ 1,2},{ 8,9},{9,10},{10,11}};
float Center[2][2]={{3,4},{8,9}};
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
        mdist=mdist+min;
    }

    return mdist;
}

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
            printf("%d **",s);
            for (int m=0;m<Nv;m++)
                b[y][m]=b[y][m]/s;
            y++;
        }
        i++;
    }while (i<N);
    
    for (int i=0;i<Nc;i++)
        for (int j=0;j<Nv;j++){
            Center[i][j]=b[i][j];
            printf("%f\n",Center[i][j]);
        }
}


void main(){
    float a=estimateClass();
    for(int i=0;i<N;i++)
        printf("%d --",Class[i]);
    printf("\n");
    estimateCenter();
    printf("\n");
}
