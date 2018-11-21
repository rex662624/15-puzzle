#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#define N 4

typedef struct board{
	int b[16], misplaced_tiles;
	struct board * parent;
	int depth;
} BOARD;	

typedef struct node{
	BOARD B;
	struct node *next;

} NODE;

BOARD goal_state, best_possible_state;
int puzzle[16];
NODE * final;
NODE *open_list, *close_list;
//NODE *StepList;
int limit =70;//一開始限制65步以內到
int goal;

void PrintBoard(int *arr, int misplaced_tiles){
	int i;
	//printf("Misplaced tiles: %d", misplaced_tiles);
	printf("-------------------------\n");
	for(i=0; i<16; i++){ 
		if(i%4==0) printf("\n");
		if(arr[i]==0)	printf("   ");
		else	printf("%2d ", arr[i]);
	}
	printf("\n");
}



BOARD GenerateGoal(){//設定goal state 的盤面
	BOARD goal_state;
	int i;
	for(i=0; i<16; i++){
		goal_state.b[i] = i+1;
		if(i==15) goal_state.b[i] = 0;
	}
	return goal_state;
}

BOARD GenerateStart(){//隨機產生start state
	BOARD start = GenerateGoal();
	int i, j, temp;
	
	srand(time(NULL));
	for(i=15; i>0; i--){
		j=rand()%(i+1);
		temp=start.b[i];
		start.b[i] = start.b[j];
		start.b[j] = temp;
	}
	start.depth=0;
	return start;
}	

BOARD FewStepStart(int choice){//產生預先設定的start state（步數較少）
	//int data1[16]={5,2,12,8,1,3,9,7,4,14,15,11,13,6,10,0};
	//int data1[16]={1,2,3,4,5,6,7,8,9,10,11,13,12,14,15,0};
	int data1[16]={2,6,4,12,1,5,3,8,0,9,11,7,13,14,15,10};
	//int data2[16]={2,8,13,14,3,0,15,4,6,5,7,11,9,1,10,12};
	int data2[16]={1,0,8,4,5,2,3,7,9,10,6,12,13,14,15,11};
	BOARD start;
	int i, j, temp;
	if(choice==1)
		for(i=15; i>=0; i--){
			start.b[i]=data1[i];
		}
	else
		for(i=15; i>=0; i--){
			start.b[i]=data2[i];
		}
	start.depth=0;
	
	return start;
}	

int ZeroPos(int A[]){// find zero position
	int i;
	for(i=0;i<16;i++) if(A[i]==0) return i;
}



int CompareBoard(int a[], int b[]){//check if a and b is the same
	int count =0, i, j;
	for(i=0;i<16;i++) if(a[i]==b[i]) count++;
	if(count == 16) return 1;
	else return 0;
}

//======================check if a Board is solvable

int getInvCount(int board[])
{
    int inv_count = 0;
    for (int i = 0; i < N * N - 1; i++){
        for (int j = i + 1; j < N * N; j++){
            // count pairs(i, j) such that i appears
            // before j, but i > j.
            if (board[j] && board[i] && board[i] > board[j]) inv_count++;
        }
    }
    return inv_count;
}
int findXPosition(int board[])
{
    // start from bottom-right corner of matrix
    for (int i = N - 1; i >= 0; i--)
        for (int j = N - 1; j >= 0; j--)
            if (board[i*4+j] == 0) return N - i;
}
bool isSolvable(int board[])
{
    // Count inversions in given puzzle
    int invCount = getInvCount((int*)board);

    // If grid is odd, return true if inversion
    // count is even.
    if (N & 1) return !(invCount & 1);

    else     // grid is even
    {
        int pos = findXPosition(board);
        if (pos & 1) return !(invCount & 1);
        else return invCount & 1;
    }
}
//==============================================
int IsGoal(int board[]){
	int i;
	for(i=0; i<15; i++){
		if(i+1 != board[i]) return 0;
	
	}
	goal=1;
	return 1;

}

int find_misplaced_tiles(int A[]){//計算有幾個人放錯位置
	int i, WrongPlace=0;
	if(A[15]!=0) WrongPlace++;

	for(i=0; i<15; i++){
		if(A[i] != (i+1)){
			WrongPlace++;
		}
	}
	return WrongPlace++;
}

int find_misplaced_tiles2(int A[]){//曼哈頓距離
	int a[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};

	int i, WrongPlace=0;
	//if(A[15]!=0) WrongPlace++;

	for(i=0; i<15; i++){
		WrongPlace+= abs(((A[i]-1)/4)-i/4)+abs(((A[i]-1)%4)-i%4);
	}
	return WrongPlace++;
}

void Add_To_Open_List(BOARD* now,int depth){//把目前的加入open list
	int *A =now->b;
	NODE *op_list_l = open_list, *clo_list_l = close_list;
	int i, flag=0;
	while(op_list_l!=NULL){//看看是不是已經在openlist中
		if(CompareBoard(op_list_l->B.b,A)){
			flag = 1;
			break;
		}
		op_list_l = op_list_l->next;
	}
	while(clo_list_l!=NULL && flag==0){//看看是不是已經在closelist中
		if(CompareBoard(clo_list_l->B.b,A)){
			flag = 1;
			break;
		}
		clo_list_l = clo_list_l->next;
	}

	//如果都不在openlist和close list才可以加入openlist
	if(flag==0){

		NODE *NewBoard = malloc(sizeof(NODE));
		NewBoard->next = NULL;
		NewBoard->B.misplaced_tiles = find_misplaced_tiles2(A);
		NewBoard->B.parent = &final->B;//parent=上一個走的人
		//if(final!=NULL)
		NewBoard->B.depth = depth+1;//更新depth=上一個人+1
		//else NewBoard->B.depth=0;
		// updating board
		for(i=0;i<16;i++)NewBoard->B.b[i] = A[i];
		// insert to the open_list
		if(open_list==NULL) open_list = NewBoard;
		else{
			NODE *current = open_list;
			//openlist是根據 misplaced_tiles 排下來
			while (current->next!=NULL && current->B.misplaced_tiles < NewBoard->B.misplaced_tiles){
	            current = current->next;
	        }
	        NewBoard->next = current->next;
	        current->next = NewBoard;
    	}

	//printf("sdsd\n");
	}
}

void NextPossibleState(BOARD* now){
	
	int *A = now->b;
	int ZeroPosition = ZeroPos(A);//找到0的位置
	int Zero_row = ZeroPosition/4;
	int Zero_col = ZeroPosition%4;
	printf("\ndepth%d,limit%d\n",now->depth,limit);
	if(now->depth>limit)return;//超過這輪的limit	

	if(Zero_row-1>=0){//最上面那排
		//把 0 移到上面
		A[ZeroPosition] = A[ZeroPosition-4];
		A[ZeroPosition-4] = 0;
		//加入openlist
		Add_To_Open_List(now,now->depth);
		//恢復移動前的盤面，因為下面還有動作
		A[ZeroPosition-4] = A[ZeroPosition] ;
		A[ZeroPosition] = 0;
	}
	if(Zero_col-1>=0){//左邊
		A[ZeroPosition] = A[ZeroPosition-1];
		A[ZeroPosition-1] = 0;
		Add_To_Open_List(now,now->depth);
		A[ZeroPosition-1] = A[ZeroPosition] ;
		A[ZeroPosition] = 0;
	}
	if(Zero_row+1<=3){//最下面
		A[ZeroPosition] = A[ZeroPosition+4];
		A[ZeroPosition+4] = 0;
		Add_To_Open_List(now,now->depth);
		A[ZeroPosition+4] = A[ZeroPosition] ;
		A[ZeroPosition] = 0;
	}
	if(Zero_col+1<=3){//最右邊
		A[ZeroPosition] = A[ZeroPosition+1];
		A[ZeroPosition+1] = 0;
		Add_To_Open_List(now,now->depth);
		A[ZeroPosition+1] = A[ZeroPosition] ;
		A[ZeroPosition] = 0;
	}
}


int ChooseNext(){// 找出最好一步的 heuristic方法：選出盤面錯最少的
	
	int  i;
	NODE *temp = open_list, *curr=open_list;

	open_list = open_list->next;//因為開頭要移出去了
	//最好的state在openlist開頭
	best_possible_state = curr->B;
	PrintBoard(best_possible_state.b, curr->B.misplaced_tiles);//印出

	
	int isgoal=0;
	isgoal = IsGoal(best_possible_state.b);
	if(isgoal){//如果下一步是goal
		//插進close list
		if(close_list==NULL) close_list = curr;
		else{
			// inserting at the head
			curr->next = close_list;
			close_list = curr;
		}
		final = curr;//紀錄這步走了什麼，後面方便回朔
		return 1;
	}
	
	else{ // 如果下一步不是goal 加入 close list
		if(close_list==NULL) close_list = curr;
		else{
			// inserting at the head
			curr->next = close_list;
			close_list = curr;
			final = curr;//紀錄這步走了什麼，後面方便回朔
		}
		NODE *temp = close_list;
		return 0;
	}
}

void solve_board(BOARD* start_state){//課本p35 a* algorithm

	//先Enqueue Start開始可以一步到的盤面
	NextPossibleState(start_state);
	int isgoal=0;
	isgoal = ChooseNext();

	//如果走一步就到終點
	if(isgoal) 
		return;
	else{//否則開始走A*流程
		while(open_list!=NULL){//如果還有 state 可以走
			//找出所有可以到的下一個盤面
			NextPossibleState(&best_possible_state);
			isgoal = 0;
			isgoal = ChooseNext();
			if(isgoal) return;
		}
	}
}



int main(int argc, char *argv[]){

while(limit+=5){//每次多搜尋5層
	printf("*********************************************depth++\n");
	//if(limit>30)return 0 ;
	NODE *StepList;
	goal=0;

	srand((unsigned int)time(NULL));
	BOARD start_state;
	int i;

	//產生初始盤面和終局盤面
	goal_state = GenerateGoal();
	if(argc<2)
		start_state = GenerateStart();
	else
		start_state = FewStepStart(atoi(argv[1]));
	int m = find_misplaced_tiles2(start_state.b);
	start_state.misplaced_tiles = m;
	start_state.parent = NULL;

	goal_state.misplaced_tiles = 0;

	//初始化盤面的openlist和closelist
	open_list = (NODE*)malloc(sizeof(NODE));
	close_list = (NODE*)malloc(sizeof(NODE));
	open_list->next= NULL;
	close_list->next = NULL;

	// adding start_state to close_list(已經走過了)
	close_list->B = start_state;
	printf("Starting board:\n");
	PrintBoard(start_state.b, start_state.misplaced_tiles);
	
	//看state是否可走
	int solvable = isSolvable(start_state.b);
	if(solvable) printf("Solvable\n");
	else {printf("Unsolvable board state\n");return 0;}

	//開始解
	if(solvable){
		if(IsGoal(start_state.b)){//如果一開始就是終盤
			printf("\nstart state:\n");
			PrintBoard(start_state.b, start_state.misplaced_tiles);
		}
		else{
			solve_board(&start_state);
			printf("\nstart state:\n");
			PrintBoard(start_state.b, find_misplaced_tiles2(start_state.b));
		}
	}
	if(goal==0)continue;
	//else return 0;
	//if(best_possible_state.parent==NULL)printf("sadasd\n");
	//printf("%d\n" ,best_possible_state.parent->b[8]);
	int Step=0;
	//FILE * pFile;
  	//pFile =	fopen ( "out.txt", "w" );
	
	BOARD* tmp = &final->B;
	while(tmp!=NULL){
		if(StepList==NULL) 
		{
			NODE * new = malloc(sizeof(NODE));
			new->B=*tmp;
			new->next=NULL;
			StepList = new;
		}
		else{
			NODE * new = malloc(sizeof(NODE));
			new->B=*tmp;
			new->next=StepList;
			StepList=new;
		}
		/*
		fprintf(pFile,"\n--------------------------\n");
		for(i=0; i<16; i++){ 
			if(i%4==0) fprintf(pFile,"\n");
			if(tmp->b[i]==0) fprintf(pFile,"  \t");
			else fprintf(pFile,"%2d\t", tmp->b[i]);
		}
		//print(tmp->parent->b, 1, find_misplaced_tiles(tmp->parent->b));
		*/
		tmp=tmp->parent;
		Step++;
	}


	//加入start state
	if(StepList==NULL) {
			NODE * new = malloc(sizeof(NODE));
			new->B=start_state;
			new->next=NULL;
			StepList = new;
	}
	else {
			NODE * new = malloc(sizeof(NODE));
			new->B=start_state;
			new->next=StepList;
			StepList=new;
	}

	printf("Total Step:%d\n",Step);	


	char n ;
	int count=0;
	int printOneTime=1;
	if(argc>2)printOneTime=3;
	while(StepList)
	{
		printf("Step%d of Total %d-------------------------\n",count,Step);
		for(i=0; i<16; i++){ 
			if(i%4==0) printf("\n");
			if(StepList->B.b[i]==0)	printf("   ");
			else	printf("%2d ", StepList->B.b[i]);
		}
		printf("\n");
		StepList=StepList->next;
		
		count++;
		if(count%printOneTime==0){scanf("%c",&n);}
	}
	

	//fprintf(pFile,"\n--------------------------\n");
	//印出start state
	/*
	for(i=0; i<16; i++){ 
			if(i%4==0) fprintf(pFile,"\n");
			if(start_state.b[i]==0) fprintf(pFile,"  \t");
			else fprintf(pFile,"%2d\t", start_state.b[i]);
		}
	*/
	return 0;
}
	return 0;
}
