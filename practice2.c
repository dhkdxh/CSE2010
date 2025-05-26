#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
bool IsEmpty(Stack* S);
bool IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i] != '#'){
		if('1'<=input_str[i] && input_str[i]<='9'){//input 받은 것이 숫자라면
			Push(stack,input_str[i]-'0');
		}
		else{//연산자의 경우
			int b = Pop(stack);
			int a = Pop(stack);
			switch (input_str[i]){

				case '+' : Push(stack, a+b); break;
				case '-' : Push(stack, a-b); break;
				case '*' : Push(stack, a*b); break;

				case '/' : 
                if(b==0) {fprintf(fout,"\nerror : invalid postfix expression, divide by zero");
				fclose(fin); fclose(fout);DeleteStack(stack); return;}//divide by 0의 경우 예외문구 출력후 종료+동적할당 해제
                else {Push(stack, a/b);} break;

				case '%' : 
                if(b==0) {fprintf(fout,"\nerror : invalid postfix expression, divide by zero");
				fclose(fin); fclose(fout);DeleteStack(stack); return;}//divide by 0의 경우 예외문구 출력후 종료+동적할당 해제
                else {Push(stack, a%b);} break;

				default : //연산자, 숫자, #을 제외한 빈칸의 경우
                break;
			}
		}
		fprintf(fout, "%d ", Top(stack));
		i++;
	}
	if(input_str[i]=='#'){//#으로 끝나는 경우
		if(stack->top>0) {fprintf(fout,"\nerror : invalid postfix expression, %d elements are left", (stack->top)+1);}
		//만약 top index가 0보다 큰경우(stack에 element가 2개 이상 있는 경우), 예외 문구 처리 후 종료
		else {fprintf(fout, "\nevaluation result : %d", Top(stack));}//아닌 경우 결과값 출력 후 종료
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
    Stack* S= (Stack*)malloc(sizeof(Stack));//Stack pointer S 할당
	if (S == NULL) {//S의 할당이 안된경우(예외)
        fprintf(fout, "Error: Memory allocation failed.\n");
		fclose(fin);
		fclose(fout);
        exit(1);//프로그램 종료
    }
    S->key= (int*)malloc(sizeof(int)*max);//S->key에 int형 array pointer를 할당
	if (S->key == NULL) {
        fprintf(fout, "Error: Memory allocation failed.\n");
        free(S);//stack 할당된거 할당 해제
        fclose(fin);
		fclose(fout);
        exit(1);//프로그램 종료
    }
    S->max_stack_size= max;//max값을 S->max_stack_size에 넣어줌
    S->top=-1;// S->top 을 -1로 설정
	return S;
}

void DeleteStack(Stack* S){
    free(S->key);// S->key를 동적할당 해제 
    free(S);//이후 S를 동적할당 해제
	return;
}

void Push(Stack* S, int X){
    if(IsFull(S)){//S가 가득차있는 경우라면(IsFull함수가 true를 return한다면)
        fprintf(fout,"\nerror : invalid postfix expression, stack is full");//에러메시지 출력
		fclose(fin);
		fclose(fout);
		DeleteStack(S);
		exit(1);//예외처리 후 종료
    }
    else{
        S->top += 1;//S의 top index를 1증가 시켜줌
        S->key[S->top]=X;// update된 S의 top에 X 대입
	    return;//예외처리 후 종료
    }
}

int Pop(Stack* S){
    if(IsEmpty(S)){//S가 빈 경우에라면(IsEmpty함수가 true를 return 한다면)
        fprintf(fout,"\nerror : invalid postfix expression, stack is empty");
		fclose(fin);
		fclose(fout);
		DeleteStack(S);
		exit(1);
    }
    else{
        int currentTop= S->key[S->top];//currentTop이라는 변수에 현재 top index의 원소를 할당
        S->top -= 1;//pop을 해줬으니 top index 옮겨주기
	    return currentTop;
    }
}

int Top(Stack* S){
	return S->key[S->top];// S->key의 top을 반환시켜줌
}

bool IsFull(Stack* S){
    if(S->top==(S->max_stack_size)-1){//S의 top index가 stack size보다 1작은 경우가 full이므로
        return true;//이때 true return
    }
	else return false;
}

bool IsEmpty(Stack* S){
    if(S->top==-1){//S의 top index가 -1인 경우 S가 비어있으므로
        return true;//이때 true return
    }
	else return false;
}