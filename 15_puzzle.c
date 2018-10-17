#include "stdio.h"

void printmatrix(int []);
void DFS(int [],int);
int MoveZero(int [] ,int[],int );
int goal[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
int main(void){

    int initial[16]={5,2,3,4,1,0,15,14,13,12,11,6,7,8,9,10};
     
    printmatrix(initial);
    DFS(initial,1);

    return 0;
}

void DFS(int now[],int depth) {

    
    int terminal=1;
    int next[16];

    for(int i =0;i<16;i++){
        if(now[i]!=((i+1)%15))terminal=0;    
    }

    if(terminal==1){
        printmatrix(now);
        printf("success\n") ;
    } else if(depth>70){
        //printf("exceed\n");
        return;
    } else{
        if(MoveZero(now ,next,8)==0)
            DFS(next,depth+1);
        if(MoveZero(now ,next,4)==0)
            DFS(next,depth+1);
        if(MoveZero(now ,next,6)==0)
            DFS(next,depth+1);
        if(MoveZero(now ,next,2)==0)
            DFS(next,depth+1);
        
       // printf("asdasd\n");
        return;

    }
    //printf("No solution\n");
    


}
//return -1 represent fail to move
int MoveZero(int now[] ,int next[],int flag){//flag=8:up,2:down,4:left,6:right 
    int zeroindex;
    
    for(int i=0;i<16;i++){
        next[i]=now[i];
        
        if(now[i]==0)
            zeroindex=i;
    }

    if(flag==8)
    {   

        //printf("----------------up\n");
        if(zeroindex-4<0)//在上面那排，不能往上
            return -1;
        //printmatrix(next);
        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex-4];
        next[zeroindex-4]=temp;
        //printmatrix(next);
        //printf("----------------\n");
    }
    else if(flag==2){

        //printf("----------------left\n");
        if(zeroindex%4==0)//在左邊那排，不能往左
            return -1;
        //printmatrix(next);

        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex-1];
        next[zeroindex-1]=temp;
        //printmatrix(next);
        //printf("----------------\n");
    }

    else if(flag==4){

       // printf("----------------right\n");
        if((zeroindex+1)%4==0)//在右邊那排，不能往右
            return -1;
       // printmatrix(next);

        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex+1];
        next[zeroindex+1]=temp;
       // printmatrix(next);
       // printf("----------------\n");

    }
    else if(flag==6){
       // printf("----------------down\n");
         if(zeroindex>=12)//在下面那排，不能往下
            return -1;
       // printmatrix(next);

        int temp=next[zeroindex];    
        next[zeroindex]=next[zeroindex+4];
        next[zeroindex+4]=temp;
       // printmatrix(next);
       // printf("----------------\n");

    }
    return 0;
}

void printmatrix(int matrix[]){
    
    printf("\n");
    for(int i=1;i<=16;i++){
        printf("%2d ",matrix[i-1]);
        if(i%4==0)printf("\n");
    }

}