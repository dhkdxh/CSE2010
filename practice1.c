#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

long long sum_between_two(int a, int b){
    int min,max;
    if(a>=b){min=b; max=a;}
    else{min=a; max=b;}

    long long result=0;
    for(int i=min; i<=max; i++){
        result += i;
    }
    return result;
}

void main(int arge, char* argv[]){
    fin= fopen(argv[1],"r");
    fout=fopen(argv[2],"w");
    int a,b;
    long long sum;

    while(fscanf(fin, "%d %d",&a,&b)!= EOF){
        sum=sum_between_two(a,b);
        fprintf(fout,"sum of %d and %d is %lld\n", a,b,sum);
    }
    fclose(fin);
    fclose(fout);
    return;
}

