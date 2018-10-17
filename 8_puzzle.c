#include "stdio.h"

void printmatrix(int []);
void DFS(int [],int);
int MoveZero(int [] ,int[],int );
int goal[9]={1,2,3,4,5,6,7,8,0};
int terminal=1;
int main(void){

    int initial[9]={1,2,3,4,5,0,7,8,6};
     
    printmatrix(initial);
    DFS(initial,1);

    return 0;
}

void DFS(int now[],int depth) {

    
    
    int next[9];
    for(int i =0;i<9;i++){
        if(now[i]!=((i+1)%9))terminal=0;    
    }

    if(terminal==1){
        printmatrix(now);
        printf("success\n") ;
        return;
    } 
    else if(depth>25){
        //printf("exceed\n");
        return;
    } 
    else{
        if(MoveZero(now ,next,2)==0)
            DFS(next,depth+1);
        if(MoveZero(now ,next,4)==0)
            DFS(next,depth+1);
        if(MoveZero(now ,next,6)==0)
            DFS(next,depth+1);
        if(MoveZero(now ,next,8)==0)
            DFS(next,depth+1);
        
       // printf("asdasd\n");
        return;

    }
    //printf("No solution\n");
    


}
//return -1 represent fail to move
int MoveZero(int now[] ,int next[],int flag){//flag=8:up,2:down,4:left,6:right 
    int zeroindex;
    
    for(int i=0;i<9;i++){
        next[i]=now[i];
        
        if(now[i]==0)
            zeroindex=i;
    }

    if(flag==8)
    {   

       // printf("----------------up\n");
        if(zeroindex<=2)//在上面那排，不能往上
            return -1;
      //  printmatrix(next);
        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex-3];
        next[zeroindex-3]=temp;
       // printmatrix(next);
      //  printf("----------------\n");
    }
    else if(flag==4){

        //printf("----------------left\n");
        if(zeroindex%3==0)//在左邊那排，不能往左
            return -1;
       // printmatrix(next);

        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex-1];
        next[zeroindex-1]=temp;
        //printmatrix(next);
        //printf("----------------\n");
    }

    else if(flag==6){

      //  printf("----------------right\n");
        if((zeroindex+1)%3==0)//在右邊那排，不能往右
            return -1;
      //  printmatrix(next);

        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex+1];
        next[zeroindex+1]=temp;
       // printmatrix(next);
       // printf("----------------\n");

    }
    else if(flag==2){
        //printf("----------------down\n");
         if(zeroindex>=6)//在下面那排，不能往下
            return -1;
        //printmatrix(next);

        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex+3];
        next[zeroindex+3]=temp;
        //printmatrix(next);
        //printf("----------------\n");

    }
    return 0;
}

void printmatrix(int matrix[]){
    
    printf("\n");
    for(int i=1;i<=9;i++){
        printf("%2d ",matrix[i-1]);
        if(i%3==0)printf("\n");
    }

}