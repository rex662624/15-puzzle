#include <stdio.h>
#include <stdlib.h>

int check_inversion(int board[3][3])
{
    int inversion = 0;
    int a,b,i,j,ii,jj;
    for(a=0;a<9;++a){
        for(b=0;b<a;++b){
            i = a/3, j = a%3;
            ii= b/3, jj= b%3;
            if(board[i][j]&&board[ii][jj]&&board[i][j]<board[ii][jj])
                ++inversion;
            if(board[i][j]==0){
                inversion += i+1;
            }
        }
    }
    return (inversion&1);
}

int check_ans(int board[][3])
{
    int cost = 0;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(board[i][j] && board[i][j] != i*3 + j + 1)
                cost++;
    //printf("cost = %d\n",cost);
    return cost;
}

int onboard(int x, int y)
{
    return (0<=x && x<3 && 0<=y && y<3);
}

const int dirX[4] = {1,-1,0,0}, dirY[4] = {0,0,1,-1};
const int reverse_dir[4] = {1,0,3,2};
int solution[40],stack[40];

void swap(int *A,int *B)
{
    int tmp;
    tmp = *A;
    *A = *B;
    *B = tmp;
}

int bound = 32;
void DFS(int spaceX, int spaceY, int step, int prev_dir, int board[][3])
{
    if(check_ans(board) == 0)
    {
        if(step < bound){                                      /* 找最佳解 */
            bound = step;
            for(int i=0;i<bound;i++)
                solution[i] = stack[i];
        }
    }
    if(step >= bound)return;

    for(int i=0; i<4; i++)                                     /* 四種推動方向 */
    {
        int nextX = spaceX + dirX[i], nextY = spaceY + dirY[i];/* 空格的新位置 */

        if(reverse_dir[i] == prev_dir)continue;                /* 避免來回推動 */
        if(!onboard(nextX, nextY))continue;                    /* 避免出界 */

        stack[step] = i;                                       /* 記錄方向 */

        swap(&board[spaceX][spaceY], &board[nextX][nextY]);
        DFS(nextX, nextY, step+1, i, board);
        swap(&board[spaceX][spaceY], &board[nextX][nextY]);
    }
}

void print(int *X,int *Y,int board[][3], int oper)
{
    system("cls");
    //printf("X=%d,Y=%d,i = %d\n",X,Y,oper);
    swap(&board[*X][*Y],&board[*X+dirX[oper]][*Y+dirY[oper]]);
    *X = *X+dirX[oper], *Y = *Y+dirY[oper];
    int i,j;
    for(i=0; i<3; i++){
        for(j=0; j<3; j++)
            printf(" %d",board[i][j]);
        printf("\n");
    }
}

void solve(int board[][3])
{
   
    int source[3][3];
    int i,j,spaceX,spaceY;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
        {
            scanf("%d",&board[i][j]);
            source[i][j] = board[i][j];
            if(board[i][j] == 0)
                spaceX = i, spaceY = j;
        }
         if(check_inversion(board)){
        printf("This board can't be solve.\n");
        return;
    }

    DFS(spaceX, spaceY, 0, -1, board);
    //system("PAUSE");
    system("cls");
    for(i=0; i<3; i++){
        for(j=0; j<3; j++)
            printf(" %d",source[i][j]);
        printf("\n");
    }
    system("PAUSE");
    for(i=0; i<bound; i++)
        print(&spaceX,&spaceY,source,solution[i]),printf(" step = %d\n",i+1),system("pause");
    return;
}

int main()
{
    int board[3][3];
    solve(board);
    return 0;
}