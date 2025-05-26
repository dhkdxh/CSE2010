#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct LinkedListElement LE;

struct LinkedListElement{
    int key;
    LE* next_pointer;
};

void PrintPrevious(LE* header, int target_key);
void PrintList(LE* header);
void Insert(LE* header, int inserted_key, int target_key);
void Delete(LE* header, int target_key);
LE* Find(LE* header, int target_key);
LE* FindPrevious(LE* header, int target_key);

void Insert(LE* header, int inserted_key, int target_key){

    if(Find(header,inserted_key)!=NULL){//insert data가 이미 list에 존재하는지를 Find함수를 이용하여 판단하기
        fprintf(fout,"insertion %d failed: the key already exists\n", inserted_key);
    }
    else if(Find(header,target_key)==NULL){//insert 위치가 list에 존재하지 않는 경우 판단하기
        fprintf(fout,"insertion %d failed : can not find location\n", target_key);
    }
    else{
        if(target_key==-1){
            LE* inserted_node = (LE*)malloc(sizeof(LE));// inserted_node를 동적할당해준다.
            inserted_node->key=inserted_key;//할당받은 node에 key를 넣어주고 linked list의 맨 앞에 넣어준다.
            inserted_node->next_pointer=header->next_pointer;
            header->next_pointer=inserted_node;
        }
        else{
            LE* P=Find(header,target_key);// Find함수를 이용하여 target_key가 있는 노드를 P에 넣어준다.

            LE* inserted_node = (LE*)malloc(sizeof(LE));// inserted_node를 동적할당해준다.
            inserted_node->key = inserted_key;//할당받은 node에 key를 넣어주고 P의 next_pointer가 inserted_node를 가리키도록 한다.
            inserted_node->next_pointer = P->next_pointer;
            P->next_pointer=inserted_node;
        }
    }
}

void Delete(LE* header, int target_key){
    if(Find(header,target_key)==NULL){//target_key가 list에 존재하지 않는 경우 판단하기
        fprintf(fout,"deletion %d failed : node is not in the list\n", target_key);
    }
    else{
        LE* P= FindPrevious(header,target_key);//FindPrevious 함수를 이용하여 target_key 노드의 직전 노드를 P에 할당
        LE* tmp;//delete할 node 저장할 tmp 만들어주기

        tmp=P->next_pointer;//tmp node에 delete할 node 주소를 담아주고 
        P->next_pointer=tmp->next_pointer;//P의 next_pointer를 delete할 node의 next_pointer로 연결 후
        free(tmp);//동적 할당 해제
    }
}

LE* Find(LE* header, int target_key){
    LE* current=header;
    while(current->next_pointer!=NULL){//current의 next_pointer가 NULL이 되기 전까지 돌림
        if(current->key==target_key){//current의 key가 target_key가 되면
            return current;//target_key가 있는 노드를 return 한다.
        }
        current=current->next_pointer;
    }
    return NULL;//못찾을 경우 NULL을 return한다.    
}

LE* FindPrevious(LE* header, int target_key){
    LE* current=header;
    while(current->next_pointer->next_pointer!=NULL){//current의 next_pointer의 next_pointer가 NULL이 되기 전까지 돌림
        if(current->next_pointer->key==target_key){//current의 next_pointer의 key가 target_key가 되면 
            return current;//target_key가 있는 노드의 전 노드를 return 한다.
        }
        current=current->next_pointer;
    }
    return NULL; // 못 찾을 경우 NULL을 return한다.
}

void PrintPrevious(LE* header, int target_key){
    LE* P= FindPrevious(header,target_key);//찾으려는 node의 이전 node의 주소를 P에 할당
    if(P==NULL){//target_key가 있는 node의 이전노드가 존재하지 않을 경우
        fprintf(fout,"finding %d failed: node is not in the list\n", target_key); 
    }
    else if(P==header){//target_key가 있는 node의 이전노드가 header인 경우
        fprintf(fout,"previous node of %d is head\n", target_key);
    }
    else{//이전 노드를 잘 찾은 경우
        fprintf("previous node of %d is %d\n",target_key,P->key);
    }
}

void PrintList(LE* header){
    if(header->next_pointer==NULL){//list가 비었는지를 header->next_pointer가 NULL일 경우로 판단
        fprintf(fout,"empty list\n");
    }
    else{
        LE* P = header;//현재 위치를 header로 설정
        while(P->next_pointer!=NULL){//list의 끝까지 이동하면서
            fprintf(fout,"%d ",P->key);//node의 key 출력하기
            P=P->next_pointer;//현재 위치 update
        }
        fprintf(fout,"\n");
    }
}

LE* CreateList(){
    LE* L=(LE*)malloc(sizeof(LE));//동적할당을 이용하여 header node L 생성하기
    L->key=0;
    L->next_pointer=NULL;
    return L;
}

void DeleteList(LE* header){
    LE* P = header;
    LE* current = NULL;
    while (P != NULL) {//P를 이용하여 node memory 해제시키기
        current = P->next_pointer;//current에 P의 next_pointer 주소 저장
        free(P);//P 해제하고
        P = current;//P에 P->next_pointer 주소 할당
    }
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int key1, key2;

    LE* header = CreateList();
	
	while(1){
		command = fgetc(fin);
		if(feof(fin)) break;
		switch(command){
		    case 'i':
		        fscanf(fin, "%d %d", &key1, &key2);
		        Insert(header, key1, key2);
		        break;
		    case 'd':
		        fscanf(fin, "%d", &key1);
		        Delete(header, key1);
		        break;
		    case 'f':
		        fscanf(fin, "%d", &key1);
		        PrintPrevious(header, key1);
		        break;
		    case 'p':
		        PrintList(header);
		        break;
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return;
}