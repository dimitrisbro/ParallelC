/*


pc1

gcc version3.c -lm -O3
time ./a.out

157.217926 

real    1m50,937s
user    1m50,801s
sys     0m0,120s

// *************************
Ram

size of city : 80000 
size of diad : 40000 
size of d    : 400000000 
RAM needed   : 400120000 bytes


*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define N 10000

float city[N][2];/*={
        {0,0},
        {1,1},
        {2,2},
        {3,3},
        {4,4}
    };*/

int diad[N];
float dist[N][N];

void initcity(void){
    

    
    srand(1234);
    for (int i=0;i<N;i++){
        city[i][0]=1.0*rand()/RAND_MAX;
        city[i][1]=1.0*rand()/RAND_MAX;
    }
    
}
// ****************

void initDist(void){
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            dist[i][j]=sqrt((city[i][0]-city[j][0])*(city[i][0]-city[j][0])+(city[i][1]-city[j][1])*(city[i][1]-city[j][1]));
        }
    }
}

// *********************

float minDist(void){
    diad[0]=0;
    float tdist=0;
    for (int i=0;i<N-1;i++){
        float min=1000000;
        float minn[2];
        int th=-1;
        int thh[2];
        for (int j=0;j<N;j++){
            int flag=1;
            for (int k=0;k<i;k++){
                if (j==diad[k]){
                    flag=0;
                    break;
                }
            }
            if (dist[diad[i]][j]!=0 && flag==1 && min>dist[diad[i]][j]){
                min=dist[diad[i]][j];
                minn[1]=minn[0]; 
                minn[0]=min;
                th=j;
                thh[1]=thh[0];
                thh[0]=th;
            }
        }
        int d=rand()%2; 
        diad[i+1]=thh[d];
        tdist=tdist+minn[d];
        //printf("distance : %f  %d -> %d \n",min,diad[i],th);
    }
    tdist=tdist+dist[diad[N-1]][0];
    return tdist;
}

void main(void){
    initcity();
    initDist();
    /*for (int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%f ",dist[i][j]);}
        printf("%d \n",i);}*/
    float tdist=minDist();
    printf("%f \n",tdist);
    /*for(int k=0;k<N;k++)
        printf ("%d ** ",diad[k]);*/

    /*printf("size of city : %ld \nsize of diad : %ld \nsize of d    : %ld \n",sizeof(city),sizeof(diad),sizeof(dist));
    printf("RAM needed   : %ld bytes\n",sizeof(city)+sizeof(diad)+sizeof(dist));*/
};
