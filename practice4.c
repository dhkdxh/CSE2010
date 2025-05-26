#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;


typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
    if(current==NULL){//current가 NULL인 경우 동적할당 후 key 넣기
        BSTNode* newnode = (BSTNode*)malloc(sizeof(BSTNode));
        newnode->value= key;
        newnode->left=NULL;
        newnode->right=NULL;
        return newnode;//만든 노드 return
    }
    else if(key<current->value){//key가 current->value보다 작은 경우, 왼쪽 subtree로 가게끔 재귀
        current->left= insertNode(current->left,key); 
    }
    else if(key>current->value){//key가 current->value보다 큰 경우, 오른쪽 subtree로 가게끔 재귀
        current->right= insertNode(current->right,key);
    }
    else{//이미 존재하는 key값인 경우 error 메시지 처리하기 위해 NULL return
        return NULL;
    }
    return current;//return current 해주기
}

//오른쪽 자식 기준
BSTNode* deleteNode(BSTNode* current, int key){
    if(current==NULL){//delete를 하려고 하는데 key값이 존재하지 않는 경우 return
        return current;
    }
    else if(key<current->value){//key가 current->value보다 작은 경우 왼쪽 subtree로 재귀
        current->left= deleteNode(current->left,key);
    }
    else if(key>current->value){//key가 current->value보다 큰 경우 오른쪽 subtree로 재귀
        current->right= deleteNode(current->right,key);
    }
    else{
        if(current->left!= NULL && current ->right != NULL){//자식 node가 2개 존재하는 경우
            BSTNode* tmp= current->right;//current->right 저장
            while(tmp->left != NULL){//우측 자식 node의 가장 왼쪽 node까지 도달할 때까지
               tmp= tmp->left;
            }
            current->value= tmp->value;//우측 자식 node의 가장 왼쪽 자식의 값을 current->value로 업뎃
            current->right= deleteNode(current->right,tmp->value);
        }
        else if((current->left != NULL && current->right == NULL)){
            //자식 node가 1개만 존재하는 경우 (왼쪽)
            BSTNode* tmp= current;
            current= current->left; //왼쪽 자식 node 연결              
            free(tmp);//동적할당 해제
            return current;
        }
        else if(current->left == NULL && current->right != NULL){
            //자식 node가 1개만 존재하는 경우(오른쪽)
            BSTNode* tmp= current;
            current= current->right; //오른쪽 자식 node 연결              
            free(tmp);//동적할당 해제
            return current;
        }
        else{//자식 node가 0개 존재하는 경우
            free(current);
            return NULL;
        }
    }
    return current;
}

BSTNode* findNode(BSTNode* current, int key){
    if(current==NULL){//해당 원소가 없는 경우 return
        return NULL;
    }

    if(key<current->value){//key가 current->value보다 작으면 왼쪽 subtree에서 찾기
        return findNode(current->left,key);
    }
    else if(key>current->value){//key가 current->value보다 크면 오른쪽 subtree에서 찾기
        return findNode(current->right,key);
    }
    else{//찾으면 return
        return current;
    }
}
void printInorder(BSTNode* current){
    if(current->left!= NULL){//왼쪽 subtree부터 탐색하여 가장 왼쪽에 도달
        printInorder(current->left);
    }
    
    fprintf(fout,"%d ",current->value);//값 출력

    if(current->right!= NULL){
        printInorder(current->right);//이후 오른쪽 subtree로 가게끔 탐색
    }
}

void deleteTree(BSTNode* current){
    if(current == NULL) {//current가 NULL이 되면 함수 종료
        return;
    }
    deleteTree(current->left);//왼쪽 subtree부터 지우기
    deleteTree(current->right);//이후 오른쪽 subtree 지우기
    free(current);
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
                if(root==NULL){//root가 NULL을 받은 경우 insertion error가 발생했다는 뜻이므로
                    fprintf(fout,"insertion error %d\n",key);//에러문구 처리
                }
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){
					fprintf(fout, "%d is in the tree\n", key);
				}else{
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
    fclose(fin);
    fclose(fout);
}