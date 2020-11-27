/*

gcc version2.c -lm -pg
./a.out
gprof

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 85.74      4.87     4.87 100000000     0.00     0.00  estimateDiad  <---- needs to be "paralleled"
 13.22      5.62     0.75        1   750.79   750.79  initDiad
  1.06      5.68     0.06                             main
  0.00      5.68     0.00        1     0.00     0.00  initcity


// ******************
no changes
gcc version2.c -lm -O3
time ./a.out

5189.448242
996.432800

real    0m3,633s
user    0m3,633s
sys     0m0,004s

// ******************
parallel for and schedule in estimateDiad
gcc version2.c -lm -O3 -fopenmp
time ./a.out

5189.448242
996.430664

real    0m6,703s <---- delay
user    0m25,871s
sys     0m0,024s

// *********************
simd in estiamateDiad
gcc version2.c -lm -O3 -fopenmp
time ./a.out

5189.448242
996.430664

real    0m3,425s <----- slight improvement
user    0m3,424s
sys     0m0,004s

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
        #pragma omp /*simd reduction(+:ndist)*/ /*parallel for reduction(+:ndist) schedule(static)*/
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
    printf("size of city : %ld \nsize of diad : %ld \nsize of d    : %ld \n",sizeof(city),sizeof(diad),sizeof(d));
    printf("RAM needed   : %ld bytes\n",sizeof(city)+sizeof(diad)+sizeof(d));
};
