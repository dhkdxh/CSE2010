#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode{
	Element element;
	AVLNode* left;
	AVLNode* right;
	int height;
};

//구현하면 좋은 함수
//getHeight, max, updateHeight, rebalance

AVLNode* RotateRight(AVLNode* node){
    //우회전
    AVLNode* L= node->left;//L에 node->left를 담음
    node->left= L->right;
    L->right= node;
    node->height= max(node->left->height, node->right->height) + 1;
    L->height = max(node->left->height, node->height)+1;

    //node->left를 L이라 할 때
    //p->left를 L->right로
    //L->right를 p로
    //p와 L의 height update
    //return L
    return L;
}

AVLNode* RotateLeft(AVLNode* node){
    //좌회전
    AVLNode* R= node->right;
    node->right = R->left;
    R->left = node;
    node->height= max(node->left->height, node->right->height) + 1;
    R->height = max(node->right->height, node->height)+1;
    //node->right를 R이라 할 때
    //p->right를 R->left로
    //R->left를 p로
    //p와 R의 height update
    //return R
    //rotateRight에서 전부 반대방향
    return R;
}

AVLNode* rebalance(AVLNode* T){
    if(T->left->height > T->right->height + 1){
        if(T->left->left->height < T->left->right->height){
            T= RotateLeft(T);
        }
        T= RotateRight(T);
    }
    else if(T->left->height +1 < T->right->height){
        if(T->right->left->height > T->right->right->height){
            T= RotateRight(T);
        }
        T= RotateLeft(T);
    }
    // if 왼쪽 높이 > 오른쪽 높이 + 1 일 경우
    //     만약 LR 케이스면 좌회전
    //     우회전
    // elif 왼쪽 높이 + 1 < 오른쪽 높이 일 경우 
    //     만약 RL 케이스면 우회전
    //     좌회전
    // 높이 갱신
    return T;
}


AVLNode* Insert(Element X, AVLNode* T){
    if(T==NULL){
        AVLNode* A= (AVLNode*)malloc(sizeof(AVLNode));
        A->height = 1;
        A->element = X;
        return A;
    }
    else if(T->element==X){
        return NULL;
    }
    else{
        if(X<T->element){
            T->left= Insert(X,T->left);
        }
        else if(X>T->element){
            T->right= Insert(X,T->right);
        }
        T->height += 1;
        T=rebalance(T);
    }
    return T;
    //NULL이면 AVLNode 생성 후 반환
    //T->element != X면 recursive하게 찾고,
    //T->left = Insert(X, T->left) 형태로 포인터 갱신
    //T->element == X면 삽입 오류
    //rebalance 해줘야 함(삽입 시 균형 망가질 수 있음)
}

AVLNode* Delete(Element X, AVLNode* T){
    if(T==NULL){//X 원소가 tree에 없는 경우 NULL return
        return NULL;
    }
    else if(X < T->element){//X원소가 T의 element보다 작은 경우
        T->left= Delete(X, T->left);//왼쪽 subtree recursive
    }
    else if(X > T->element){//X원소가 T의 element보다 큰 경우
        T->right= Delete(X, T->right);//오른쪽 subtree recursive
    }
    else{//X원소를 찾은 경우
        if(T->left != NULL && T->right != NULL){//T의 오른쪽과 왼쪽 subtree가 다 있는 경우
            AVLNode* tmp= T->right;//T의 오른쪽 자식을 tmp에 설정
            while(tmp->left != NULL){//가장 왼쪽 자식에 도달할 때까지
                tmp = tmp->left;
            }
            T->element= tmp->element;//가장 왼쪽 자식의 element를 T->element로 업뎃
            T->right= Delete(tmp->element,T->right);
        }
        else if(T->left == NULL && T->right != NULL){//T의 오른쪽 자식만 있는 경우
            AVLNode* tmp= T;//지울 노드 tmp에 담기
            T= T->right;//지우기 전에 먼저 자식 tree pointer 연결
            free(tmp);//동적할당 해제
            T=rebalance(T);
            return T;
        }
        else if(T->left != NULL && T->right == NULL){//T의 왼쪽 자식만 있는 경우
            AVLNode* tmp= T;//지울노드 tmp에 담기
            T= T->left;//지우기 전에 먼저 자식 tree pointer 연결
            free(tmp);//동적 할당 해제
            T=rebalance(T);
            return T;
        }
        else{//T에 자식이 없는 경우
            free(T);//바로 동적할당 해제
            return NULL;
        }
    }
    T = rebalance(T);//재귀적으로 rebalance를 해줘야 AVLTree 조건 만족(균형 깨질 수 있음)
    return T;
}

void PrintInorder(AVLNode* T){
    if(T==NULL){//Tree가 비어있는 경우
        fprintf(fout,"Tree is empty\n");
    }
    else{//inorder 형식으로 출력
        PrintInorder(T->left);
        fprintf(fout,"%d(%d) ",T->element,T->height);
        PrintInorder(T->right);
    }
    fprintf(fout,"\n");
}

void DeleteTree(AVLNode* T){
    if(T==NULL){//Tree가 비어있는 경우
        fprintf(fout,"Tree is already empty\n");
    }
    else{//postorder로 free
        DeleteTree(T->left);
        DeleteTree(T->right);
        free(T);//동적할당 해제
    }
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLNode* Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
                if(Tree==NULL){//insertion error 난 경우
                    fprintf(fout,"insertion error : %d is already in the tree\n",key);
                }
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
                if(Tree==NULL){//deletion error 난 경우
                    fprintf(fout,"deletion error : %d is not in the tree\n",key);
                }
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}
