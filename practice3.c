#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct BinaryTree TREE;

struct BinaryTree{
    int data;
    TREE* left_pointer;
    TREE* right_pointer;
};

void CreateFromArray(TREE* current_tree, int current_index, int* inserted_data_array, int length);
void DeleteTree(TREE* current);
void printPreorder(TREE* current);
void printInorder(TREE* current);
void printPostorder(TREE* current);


void DeleteTree(TREE* current){
    if(current->left_pointer!=NULL){//왼쪽 subtree를 따라 NULL이 아닐때까지 내려감
        DeleteTree(current->left_pointer);
    }
    else if(current->right_pointer!=NULL){//왼쪽 subtree가 NULL인 경우 오른쪽 subtree를 따라 NULL이 아닐때까지 내려감
        DeleteTree(current->right_pointer);
    }
    else free(current);//도달한 해당 node 동적할당 해제
}

void printPreorder(TREE* current){
    fprintf(fout,"%d ",current->data);//current의 data 출력하기
    if(current->left_pointer!=NULL){//왼쪽 subtree를 따라 내려가는데 NULL이 아닐 때까지
        printPreorder(current->left_pointer);
    }
    else if(current->right_pointer!=NULL){//왼쪽 subtree가 NULL인 경우 오른쪽 subtree 읽기
        printPreorder(current->right_pointer);
    }
}

void printInorder(TREE* current){
    if(current->left_pointer!=NULL){//왼쪽 subtree를 따라 내려가는데 NULL이 아닐때까지
        printInorder(current->left_pointer);
    }
    fprintf(fout,"%d ",current->data);//current->left_pointer가 NULL이면 그 해당 node의 data값 출력
    if(current->right_pointer!=NULL){//출력후 오른쪽 subtree로 넘어가서 탐색
        printInorder(current->right_pointer);
    }
}

void printPostorder(TREE* current){
    if(current->left_pointer != NULL){//왼쪽 subtree를 따라 NULL이 아닐때까지 내려감
        printPostorder(current->left_pointer);
    }
    else if(current->right_pointer != NULL){//current->left_pointer가 NULL이면 오른쪽 subtree가 NULL이 아닐때까지 탐색
        printPostorder(current->right_pointer);
    }
    else fprintf(fout,"%d ", current->data);//둘다 NULL인 경우 출력 후 종료
}

void CreateFromArray(TREE* current, int current_index, int* inserted_data_array, int length){
    current->data = inserted_data_array[current_index];//current_index의 값을 current data에 넣어줌
    if(current_index==length){//current_index가 끝까지 온 경우 
        return;//재귀 종료
    }

    else if(current->left_pointer = NULL){//왼쪽 노드가 NULL인 경우= 왼쪽 노드 할당 가능
        current->left_pointer=(TREE*)malloc(sizeof(TREE));//왼쪽 노드에 동적 할당
        CreateFromArray(current->left_pointer,2*current_index, inserted_data_array, length);
        //왼쪽 노드에 대한 규칙을 이용하여 재귀
    }
    
    else if(current->right_pointer==NULL){//오른쪽 노드가 NULL인 경우= 오른쪽 노드 할당 가능
        current->right_pointer=(TREE*)malloc(sizeof(TREE));//오른쪽 노드에 동적 할당
        CreateFromArray(current->right_pointer,2*current_index+1,inserted_data_array,length);
        //오른쪽 노드에 대한 규칙을 이용하여 재귀
    }
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int input_data;
	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);
	int* array = malloc(sizeof(int) * (NUM_NODES + 1));
	array[0] = -1;
	for(int i = 0; i < NUM_NODES; i++){
	    fscanf(fin, "%d", &input_data);
	    array[i + 1] = input_data;
	}
	TREE* header = malloc(sizeof(TREE));
	CreateFromArray(header, 1, array, NUM_NODES + 1);
	printPreorder(header);
	fprintf(fout, "\n");
	printInorder(header);
	fprintf(fout, "\n");
	printPostorder(header);
	DeleteTree(header);
	fclose(fin);
	fclose(fout);
	return;
}