#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{
    QueueNode* next;
    int data;
};

typedef struct _Queue {
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	bool** matrix;
}Graph;

/*
구현해야 하는 함수들
*/
Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void TopologicalSort(Graph* G);
Queue* CreateQueue();
bool IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
/*
구현하면 좋은 함수들
*/
// Indegree 배열 설정
void SetIndegree(Graph* G, int* idg);
// Bubble sort도 무방
void SortNode(int* arr, int n);



void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);
	
    SortNode(nodes, n);
	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}


Graph* CreateGraph(int* nodes, int n){
    Graph* g= (Graph*)malloc(sizeof(Graph));//graph malloc 해준뒤
    g->matrix = (bool**)malloc(sizeof(bool*)*n);//이차원 bool 배열 malloc
    for(int i=0; i<n; i++){
        g->matrix[i]= (bool*)malloc(sizeof(bool)*n);
    }
    g->size= n;//size에 n 할당 
    g->node= nodes;
    return g;
}

void InsertEdge(Graph* G, int a, int b){
    int pa,pb;//index를 찾기 위해 변수 declare
    for(int i=0; i<G->size; i++){//for문을 이용하여 index 찾기
        if(a == G->node[i]){
            pa= i;
        }
        if(b == G->node[i]){
            pb=i;
        }
    }
    G->matrix[pa][pb]=true;//찾은 index를 true로 바꾸면 edge가 있음을 나타냄
}

void PrintGraph(Graph* G){
    fprintf(fout,"%d ",0);//0은 matrix 형태를 맞추기 위해 형식상 넣음
    for(int i=0; i<G->size; i++){//첫번째 행에 node의 element를 나타내기 위해 출력
        fprintf(fout,"%d ",G->node[i]);
    }
    fprintf(fout,"\n");
    for(int i=0; i<G->size; i++){//첫번째 열에는 node의 element를, 이후에는 matrix의 값 나타내게
        for(int j=0; j<=G->size; j++){//for문 돌리기
            if(j==0){
                fprintf(fout,"%d ",G->node[i]);
            }
            else{
                fprintf(fout,"%d ",G->matrix[i][j-1]);
            }
        }
        fprintf(fout,"\n");
    }
}

void DeleteGraph(Graph* G){
    for(int i=0; i<G->size; i++){//G->matrix free 해주기
        free(G->matrix[i]);
    }
    free(G->matrix);//이중 array이므로 단계별로 free 해주기
    free(G->node);//node free 해주기
    free(G);
}

void TopologicalSort(Graph* G){
    Queue* q= (Queue*)malloc(sizeof(Queue));//queue 생성
    int* printorder= (int*)malloc(sizeof(int)*(G->size));//topologicalSort 후 출력할 값을 저장할 array 할당
    int* idg= (int*)malloc(sizeof(int)*(G->size));//indegree 저장할 array 할당
    int printorder_size= 0;//printorder array의 원소 개수를 저장하기 위해 할당
    SetIndegree(G,idg);//SetIndegree로 indegree 저장

    for(int i=0; i<printorder_size; i++){
        if(idg[i]==0){//indegree가 0인 node queue에 넣어주기
            Enqueue(q,G->node[i]);
        }
    }
    while(!IsEmpty(q)){//q가 빌 때까지
        int currentNode= Dequeue(q);//dequeue 먼저해서
        printorder[printorder_size] = currentNode;//해당 원소 printorder array에 넣어주고
        printorder_size++;//array size update
        int index=0;//G->matrix에서 currentNode를 선행으로 할 노드를 찾기 위해 index 변수 설정
        for(int i=0; i<G->size; i++){//currentNode의 index를 찾아 index 변수에 넣어주기
            if(G->node[i]==currentNode){
                index=i;
            }
        }
        for(int i=0; i<G->size; i++){
            if(G->matrix[index][i]){//currentNode를 선행으로 하는 node가 있으면
                idg[i]--;//그 노드의 indegree를 1 빼주기
                if(idg[i]==0){//이후 indegree가 0이 될경우
                    Enqueue(q,G->node[i]);//queue에 삽입
                }
            }
        }
    }

    if(printorder_size==G->size){//사이클 없이 topologicalSort가 잘 된 경우
        for(int i=0; i<G->size; i++){//printorder array의 원소들 출력하기
            fprintf(fout,"%d ",printorder[i]);
        }
    }
    else{//사이클이 있는 경우 남은 정점들이 있으므로 printorder_size와 G->size가 맞지 않음
        fprintf(fout,"sorting error : cycle\n");//에러메시지 출력
    }

    free(idg);//idg,printorder,q free 해주기
    free(printorder);
    DeleteQueue(q);
}

Queue* CreateQueue(){
    Queue* q= (Queue*)malloc(sizeof(Queue));//queue 동적할당 해주기
    //front와 rear NULL로 initialize 해주기
    q->front=NULL;
    q->rear=NULL;
    return q;//만든 queue를 return
}

bool IsEmpty(Queue* Q){
    if(Q->size == 0){//size가 0이면 true return
        return true;
    }
    return false;//아니면 false return
}

int Dequeue(Queue* Q){
    QueueNode* temp= Q->front;//가장 앞 node를 지우기 위해 temp에 저장
    Q->size--;//Q size update
    if(Q->front == Q->rear){//빼게 될 경우 빈 queue인 경우== front와 rear이 같은 node를 가리키는 경우
        int X= Q->front->data;//X에 뺄 node의 data 저장
        free(temp);
        Q->front=NULL;//queue가 비었으므로 front와 rear NULL로 저장
        Q->rear=NULL;
        return X;//뺀 원소 return
    }
    else{//node를 제거해도 비지 않는 경우
        Q->front=Q->front->next;//front를 front->next 주소로 업뎃
        int X= temp->data; //뺄 node의 data 저장
        free(temp);
        return X;//뺀 원소 return
    }
}

void Enqueue(Queue* Q, int X){
    QueueNode* temp= (QueueNode*)malloc(sizeof(QueueNode));//새로 삽입할 node 만들기
    temp->data= X;//data에 값 넣어주기
    temp->next= NULL;
    Q->size++;//size update
    if(Q->front==NULL && Q->rear==NULL){//빈 queue 일경우
        Q->front=temp;//front와 rear 둘다 temp로 설정
        Q->rear=temp;
    }
    else if(Q->rear!=NULL){//빈 queue가 아닐경우
        Q->rear->next= temp;//Q->rear->next에 temp를 넣어주고
        Q->rear= Q->rear->next;//Q->rear에 Q->rear->next 넣어주기
    }
}

void DeleteQueue(Queue* Q){
    while(!IsEmpty(Q)){//IsEmpty가 아닐때까지
        int check = Dequeue(Q);
    }
    free(Q);
    //IsEmpty일 때까지 Dequeue 후 free
}

void SetIndegree(Graph* G, int* idg){
    for(int i=0; i<G->size; i++){//idg array를 0으로 초기화해줌
        idg[i]=0;
    }
    for(int i=0; i<G->size; i++){//G->matrix[i][j]가 true이면 idg[j]의 수를 1증가
        for(int j=0; j<G->size; j++){
            if(G->matrix[i][j]==true){
                idg[j]++;
            }
        }
    }
}

void SortNode(int* arr, int n){
    int min;//가장 작은 값을 지정하기 위해 변수 설정
    for(int i=0; i<n; i++){//array 정렬
        min= arr[i];
        for(int j=i+1; j<n; j++){
            if(min > arr[j]){
                arr[i]=arr[j];
                arr[j]=min;
            }
        }
    }
}

