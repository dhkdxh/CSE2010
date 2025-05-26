#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define INF 1e7

FILE *fin;
FILE *fout;

typedef struct Node {
	int index; //index of this node
	int dist;	//distance for heap
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** distMatrix;
	Node** nodes;
}Graph;

typedef struct Heap {
	int capacity;
	int size;
	Node** element;
}Heap;

//구현해야 하는 함수
Graph* CreateGraph(int size);
void DeleteGraph(Graph* g);
void PrintShortestPath(Graph* g);
Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap* heap, Node* node);
Node* Pop(Heap* heap);

bool isEmpty(Heap* heap){//heap이 비었는지 확인하기 위해 isEmpty 함수 구현
	if(heap->size= heap->capacity-1){
		return true;
	}
	else{
		return false;
	}
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = CreateGraph(size);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->distMatrix[node1 - 1][node2 - 1] = weight;
		tmp = fgetc(fin);
	}

	PrintShortestPath(g);

	DeleteGraph(g);
}

Graph* CreateGraph(int size) {
	Graph* g= (Graph*)malloc(sizeof(Graph));//Graph g 동적 할당으로 만들기
	g->nodes= (Node**)malloc(sizeof(Node*)*size);//nodes의 경우 Node*형의 array이므로 동적할당하기
	for(int i=0; i<size; i++){//nodes의 size 만큼 index는 i로, dis와 prev는 INF로 초기화
		g->nodes[i]->index = i;
		g->nodes[i]->dist = INF;
		g->nodes[i]->prev= INF;
	}
	g->size= size;//g의 size에 size 할당
	g->distMatrix=(int**)malloc(sizeof(int*)*size);//g->distMatrix의 경우 2차원 배열이므로 동적할당
	for(int i=0; i<size; i++){
		g->distMatrix[i]=(int*)malloc(sizeof(int)*size);
	}
    return g;
}

void DeleteGraph(Graph* graph){
	for(int i=0; i<graph->size; i++){//2차원 배열 동적할당 해제 하는 방식으로 free
		free(graph->distMatrix[i]);
	}
	free(graph->distMatrix);
	free(graph->nodes);//nodes 배열도 free
	free(graph);//마지막으로 graph free
}

Heap* CreateHeap(int heapSize) {
	Heap* h= (Heap*)malloc(sizeof(Heap));//Heap* h 동적할당
	h->size= 0;//현재 element의 size는 0이므로 0 할당
	h->capacity= heapSize;//element 배열의 크기는 heapSize가 됨
	h->element= (Node**)malloc(sizeof(Node*)*heapSize);//Node*를 return 값으로 하는 배열을 할당해주기
    return h;
}

void DeleteHeap(Heap* heap){
	free(heap->element);//array 먼저 동적할당 해제
	free(heap);//heap 동적할당 해제
}

void PrintShortestPath(Graph* graph) {
	graph->nodes[0]->dist = 0;//0번 node의 dist를 0으로 설정함
	Heap* heap= CreateHeap(graph->size);//heap 생성 후
	Push(heap, graph->nodes[0]);//0번 노드를 heap에 넣기
    
	while(!isEmpty(heap)){//heap이 빌때까지
		Node* a= Pop(heap);//가장 dist가 작은 node를 꺼냄
		int distOA = a->dist;//그 dist를 distOA 변수에 저장
		for(int i=0; i<graph->size; i++){//graph의 크기 만큼
			Node* b= graph->nodes[i];//해당 node를 b라고 두기
			if(a==b){//꺼낸 node와 해당 node가 같은건 의미 없으므로 continue
				continue;
			}
			int distOB = b->dist;//같지 않은 경우 꺼낸 node의 dist 저장
			if(distOA + graph->distMatrix[a->index][b->index] < distOB){//다엑스트라 알고리즘으로 비교한 뒤 업뎃
				b->dist= distOA + graph->distMatrix[a->index][b->index];
				b->prev= a->index;
				Push(heap,b);
			}
		}
	}
    
    for(int i=0; i<graph->size; i++){//for문으로 graph->size만큼 돌려서
		Node* b= graph->nodes[i];//해당 node를 b로 저장
		int* printarr= (int*)malloc(sizeof(int)*graph->size);//최단 거리를 출력하기 위해 array 동적할당
		int currentindex = 0;//array의 현재 index 저장
		printarr[0]= b->index;//일단 현재 node를 printarr의 0번째 index에 넣음

		while(b->prev != INF){//최단 거리를 찾기 위해 b->prev를 따라감
			currentindex++;//index 업뎃
			b= graph->nodes[b->prev];//b를 b->prev 인덱스의 node로 업뎃
			printarr[currentindex]= b->index;//currentindex의 위치에 업뎃된 node의 인덱스 넣음
		}
		
		b= graph->nodes[i];//출력을 위해 b에 다시 graph->nodes[i]를 넣어줌

		if(currentindex==0){//currentindex가 0 = b->prev가 없음을 뜻하므로
			fprintf(fout,"can not reach to node %d\n",b->index + 1);//에러문구 출력
		}
		else{//b->prev가 있어 도달할 수 있는 경우
			for(int j=currentindex; j>=0; j--){
				fprintf(fout,"%d->",printarr[j]+1);
				if(j==0){
					fprintf(fout,"%d (cost: %d)\n",printarr[j]+1, b->dist);
				}
			}
		}
	}
	DeleteHeap(heap);//만든 heap 삭제
}

void Push(Heap* heap, Node* node) {
	heap->size++;
	heap->element[heap->size]= node;
	int index= heap->size;//현재 가장 끝의 값의 index 저장
	int pindex= (heap->size -1)/2;//가장 끝의 값의 부모의 index를 pindex라 저장
    
	while(true){//minheap을 만족할 때까지 반복문
		if(heap->element[index]->dist< heap->element[pindex]->dist){//자식이 부모보다 값이 작은 경우
			Node* temp= heap->element[index];//부모와 자식 node 바꾸기
			heap->element[index]= heap->element[pindex];
			heap->element[pindex]= temp;

			index= pindex;//현재 부모 node를 자식 node로 업뎃
			pindex=(index-1)/2;//부모 node를 현재의 부모의 부모 노드로 없뎃
		}
		else{//이 경우 minheap을 만족했으므로 반복문 멈추기
			break;
		}
	}
}

Node* Pop(Heap *heap) {
    Node* temp= heap->element[0];//지울 node temp에 저장
	heap->element[0]= heap->element[heap->size];//그리고 heap의 root에 가장 끝 원소를 올림
	heap->size--;//size 변경

	int index=0;//minheap을 만족시키기 위한 index 값 0으로 설정
	while(true){
		Node* childleft= NULL;//왼쪽 자식 변수 생성
		Node* childright= NULL; //오른쪽 자식 변수 생성
		if((index*2) +1 <= heap->size){//왼쪽 자식이 존재하는 경우
			childleft= heap->element[(index*2)+1];
		}
		if(((index+1)*2) <= heap->size){//오른쪽 자식이 존재하는 경우
			childright= heap->element[(index+1)*2];
		}

		if(childleft->dist < childright->dist && heap->element[index]->dist > childleft->dist){
			//왼쪽 자식이 오른쪽 자식보다 작고, 현재 index의 dist보다 작은 경우
			Node* temp= heap->element[index];//부모와 왼쪽 자식 바꾸기
			heap->element[index]=childleft;
			heap->element[(index*2)+1]= temp;
			index= (index*2)+1;
		}
		else if(childright->dist < childleft->dist && heap->element[index]->dist > childleft->dist){
			//오른쪽 자식이 왼쪽 자식보다 작고 현재 index의 dist보다 작은 경우
			Node* temp= heap->element[index];//부모와 오른쪽 자식 바꾸기
			heap->element[index]= childright;
			heap->element[(index+1)*2]= temp;
			index= (index+1)*2;
		}
		else{//이 경우 minheap이 만족되었으므로 while문 끝내기
			break;
		}
	}
	return temp;//삭제할 node return(가장 dist가 작은 node)
}
