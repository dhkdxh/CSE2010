#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);

Heap* CreateHeap(int heapSize){
    Heap* h= (Heap*)malloc(sizeof(Heap));//heap 동적할당으로 생성
    h->Elements= (int*)malloc(sizeof(int)*heapSize);//array 동적할당으로 생성
    h->Capacity = heapSize;
    h->Size=-1;
    return h;//만든 heap return
}

void DeleteHeap(Heap* heap){
    free(heap->Elements);//array 동적할당 해제
    free(heap);//heap 동적할당도 해제
}

void Push(Heap *heap, int value){
    if(heap->Size==(heap->Capacity-1)){//index가 array크기보다 1작은 경우가 full인 경우이므로
        fprintf(fout,"push error: heap is full\n");//에러메시지 출력
    }
    else if(Find(heap,value)){//만약 넣으려는 값이 이미 array 안에 있다면
        fprintf(fout,"push error: %d is already in the heap\n",value);//에러메시지 출력
    }
    else{
        heap->Elements[++(heap->Size)]= value;//heap->Elements 끝에 value를 삽입
        int index= heap->Size;//현재 가장 끝에 넣은 값의 index 저장
        int pindex= (heap->Size-1)/2;//그 부모노드의 index를 pindex에 저장

        while(true){//Maxheap이 될때까지 돌림
            if(heap->Elements[index] > heap->Elements[pindex]){//자식 node가 부모 node보다 값이 큰 경우
                int temp= heap->Elements[index];//부모 노드와 자식 노드의 값 바꿔주기
                heap->Elements[index]= heap->Elements[pindex];
                heap->Elements[pindex]=temp;
                
                index=pindex;//현재 노드를 부모 노드로 변경
                pindex=(index-1)/2;//부모노드의 부모노드로 pindex 변경
            }
            else{
                break;//if문이 성립되지 않으면 maxheap을 만족했으므로 멈추기
            }
        }
    }
}

bool Find(Heap *heap, int value){
    int flag = 0;//겹치는 값이 있는지 확인하기 위한 변수 생성
    for(int i=0; i<=heap->Size; i++){//반복문을 돌려서
        if(heap->Elements[i]==value){//찾으려는 값이 heap->Elements안에 있으면면
            flag=1;//flag값을 1로 바꿈
        }
    }
    if(flag){//flag=1이면
        return true;//값이 있으므로 true return
    }
    return false;//아니면 false return
}

int Pop(Heap* heap){
    if(heap->Size==-1){//heap이 비어있는 경우는 -1 return
        return -1;
    }

    int temp= heap->Elements[0];
    heap->Elements[0] = heap->Elements[heap->Size];//heap->Elements의 첫번쨰 원소에 가장 마지막 index 원소 넣기
    heap->Size--;//size 변경

    int index= 0;
    while(true){
        int childleft = -INF;//왼쪽 자식 변수 생성
        int childright = -INF;//오른쪽 자식 변수 생성
        if((index*2)+1 < heap->Size){//왼쪽 자식이 존재하는 경우
            childleft= heap->Elements[(index*2)+1];//childleft 변수에 왼쪽 자식 key 넣기
        }
        if((index+1)*2 < heap->Size){//오른쪽 자식이 존재하는 경우
            childright= heap->Elements[(index+1)*2];//childright 변수에 오른쪽 자식 key 넣기
        }

        if(childleft<childright && heap->Elements[index]<childright){//오른쪽 자식이 왼쪽 자식보다 크고, 현재 index의 key보다 큰 경우
            int t= heap->Elements[index];//오른쪽 자식과 현재 자식의 key 바꾸기
            heap->Elements[index]= childright;
            heap->Elements[(index+1)*2]= t;
            index=(index+1)*2;//현재 index를 오른쪽 자식으로 바꿔주기
        }
        else if(childright<childleft && heap->Elements[index]<childleft){//왼쪽 자식이 오른쪽 자식보다 크고, 현재 index의 key보다 큰 경우
            int t= heap->Elements[index];//왼쪽 자식과 현재 자식의 key 바꾸기
            heap->Elements[index]= childleft;
            heap->Elements[(index*2)+1]= t;
            index= (index*2)+1;//현재 index를 왼쪽 자식으로 바꿔주기
        }
        else{//위의 2경우다 아닌경우 maxheap이 만족되었으므로 while문 끝내기
            break;
        }
    }
    return temp;//삭제한 node return
}

void PrintHeap(Heap* heap){
    for(int i=1; i<=heap->Size; i++){//heap->Elements의 원소를 하나씩 출력함
        fprintf(fout,"%d ",heap->Elements[i]);
    }
    fprintf(fout,"\n");
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Push(maxHeap, key);
				break;
			case 'd':
				max_element = Pop(maxHeap);
                if(max_element=-1){
                    fprintf(fout,"pop error: heap is empty\n");
                }
				else if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "%d is not in the heap\n", key);
				break;
		}
	}
	DeleteHeap(maxHeap);
    fclose(fin);
    fclose(fout);
	return 0;
}