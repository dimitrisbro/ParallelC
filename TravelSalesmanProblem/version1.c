/*
gcc version1.c -lm
./a.out

pc1
5189.448242
996.432800

real    0m11,313s
user    0m11,302s
sys     0m0,008s

pc2
5189.448242
996.432800

real    0m8,529s
user    0m8,519s
sys     0m0,012s

// *********************************
size of city : 80008 
size of diad : 40004 
size of d    : 40004 
RAM needed   : 160016 bytes

*/
// ****************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// ****************
#define N 10000
#define rep 3
// ****************
float city[N+1][2];/*={
        {0,0},
        {1,1},
        {2,2},
        {3,3},
        {4,4},
        {0,0}
    };*/
int diad[N+1];
float d[N+1];

// ***************
void initcity(void){
    
    
    
    srand(1234);
    for (int i=0;i<N;i++){
        city[i][0]=1.0*rand()/RAND_MAX;
        city[i][1]=1.0*rand()/RAND_MAX;
    }
    city[N][0]=city[0][0];
    city[N][1]=city[0][1];
    
}

float initDiad(void){
    
    diad[0]=0;
    diad[N]=0;
    int i=1;
    float dist=0;
    srand(1234);
    while(i<N){
        int flag=1;
        int p=rand()%(N-1);
        p++;
        //float d;
        for (int j=0;j<i;j++){
            if (p==diad[j]){
                flag=0;
                break;
            }
        }
        if (flag==1){
            diad[i]=p;
            d[i-1]=(city[p][0]-city[diad[i-1]][0])*(city[p][0]-city[diad[i-1]][0]);
            d[i-1]=d[i-1]+(city[p][1]-city[diad[i-1]][1])*(city[p][1]-city[diad[i-1]][1]);
            d[i-1]=sqrt(d[i-1]);
            dist=dist+d[i-1];
            i++;
        }
    }
    d[N-1]=(city[N][0]-city[diad[N-1]][0])*(city[N][0]-city[diad[N-1]][0]);
    d[N-1]=d[N-1]+(city[N][1]-city[diad[N-1]][1])*(city[N][1]-city[diad[N-1]][1]);
    d[N-1]=sqrt(d[N-1]);
    dist=dist+d[N-1];
    return dist;
}

float estimateDiad(float dist){
    float ndist=dist;

    int p=rand()%(N-1);
    p++;
    int t;
    do{
    t=rand()%(N-1);
    t++;
    }while(t==p);

    float x=sqrt((city[p][0]-city[diad[t-1]][0])*(city[p][0]-city[diad[t-1]][0])+(city[p][1]-city[diad[t-1]][1])*(city[p][1]-city[diad[t-1]][1]));
    float y=sqrt((city[p][0]-city[diad[t+1]][0])*(city[p][0]-city[diad[t+1]][0])+(city[p][1]-city[diad[t+1]][1])*(city[p][1]-city[diad[t+1]][1]));
    float z=sqrt((city[t][0]-city[diad[p-1]][0])*(city[t][0]-city[diad[p-1]][0])+(city[t][1]-city[diad[p-1]][1])*(city[t][1]-city[diad[p-1]][1]));
    float w=sqrt((city[t][0]-city[diad[p+1]][0])*(city[t][0]-city[diad[p+1]][0])+(city[t][1]-city[diad[p+1]][1])*(city[t][1]-city[diad[p+1]][1]));

    if ((x+y+z+w)<d[p-1]+d[p]+d[t-1]+d[t]){
        //printf("%d %d \n",t,p);
        int a=diad[t];
        diad[t]=diad[p];
        diad[p]=a;
        d[t-1]=x;
        d[t]=y;
        d[p-1]=z;
        d[p]=w;
        ndist=0;
        for (int i=0;i<N;i++){
                ndist=ndist+d[i];
        }
    }
    return ndist;
}


void main(){
    initcity();
    float dist=initDiad();
    printf("%f\n",dist);
    for (int j=0;j<100000000;j++){
        dist=estimateDiad(dist);}
    printf("%f\n",dist);
};
