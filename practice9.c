#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    fprintf(fout,"left: %d, right: %d, pivot: %d\n", 0, size-1, array->values[size-1]);
    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);

    fclose(fin);
    fclose(fout);
	return 0;
}

Array* CreateArray(int size){//array 동적할당하기
    Array* arr= (Array*)malloc(sizeof(Array));
    arr->size= size;
    arr->values= (int*)malloc(sizeof(int)*size);//배열 동적할당하기
}

void QuickSort(Array* array, int left, int right, int depth){
    if(left < right){
        int pivot= Partition(array,left,right,depth);
        fprintf(fout,"left: %d, right: %d, pivot: %d\n", left,right,pivot);
        QuickSort(array, left, pivot-1, depth+1);
        QuickSort(array,pivot+1,right, depth+1);
    }
    //pivot 찾기
    //pivot 기준 왼쪽 Quicksort
    //pivot 기준 오른쪽 Quicksort
}

int Partition(Array* array, int left, int right, int depth){
    int pivot = right;
    //pivot 설정
    int num1=-1, num2=-1;
    int pleft= left, pright= right;
    //반복 (왼쪽 >= 오른쪽)
    PrintArray(array,left,right);
    while(1){
        if(pleft > pright){
            break;
        }

        if(array->values[pleft] > pivot){
            num1= pleft;
        }
        else{
            pleft++;
        }

        if(array->values[pright] < pivot){
            num2= pright;
        }
        else{
            pright--;
        }

        if(num1==pleft && num2==pright){
            swap(array->values[pleft],array->values[pright]);
            PrintArray(array,left,right);
            pleft++;
            pright--;
        }
    }
    swap(array->values[pleft],array->values[pivot]);
    PrintArray(array,left,right);
    pivot= pleft;
    return pivot;
    //왼쪽에서부터 pivot보다 큰 값 찾기
    //오른쪽에서부터 pivot보다 작은 값 찾기
    //위 두 값 교환
    //return pivot
}

void PrintArray(Array* array, int left, int right){
    int i;
    for(i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-");
        }
        else{
            fprintf(fout, "%-3d", array->values[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    free(array->values);//values가 있는 array free하기
    free(array);//array free하기
}

void swap(int* a, int* b) {
    int temp= *a;
    *a = *b;
    *b= temp;
}
