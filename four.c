#include <stdio.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define DISCOUNT 5
#define ROWS 6
#define COLS 7
#define DIAGS MIN(ROWS,COLS)

int argmax(int *nums, int len){
  int idx=0;
  int max=nums[0];
  int i;
  for (i=1;i<len;i++){
    if (nums[i]>max){
      max=nums[i];
      idx=i;
    }
  }
  return idx;
}

void print_board(int board[ROWS][COLS]){
  int i,j;
  for (i=0;i<COLS;i++){
    printf("%d  ",i+1);
  }
  printf("\n");
  for (i=0;i<ROWS;i++){
    for (j=0;j<COLS;j++){
      char pieces[3]={'-','X','O'};
      printf("%c  ",pieces[board[i][j]]);
    }
    printf("\n");
  }
}

int get_height(int board[ROWS][COLS], int col){
  int h=0;
  int i;
  for (i=ROWS-1;i>=0;i--){
    if (board[i][col]==0){
      break;
    }
    h++;
  }
  return h;
}

int is_full(int board[ROWS][COLS]){
  int i,j;
  for (i=0;i<ROWS;i++){
    for (j=0;j<COLS;j++){
      if (board[i][j]==0){
	return 0;
      }
    }
  }
  return 1;
}

int drop_piece(int board[ROWS][COLS], int col, int piece){
  int h=get_height(board,col);
  if (h==ROWS){
    return -1;
  }
  board[ROWS-h-1][col]=piece;
  return 0;
}

void get_col(int board[ROWS][COLS], int col, int *c, int len){
  int i;
  for (i=0;i<len;i++){
    c[i]=board[i][col];
  }
}

void get_row(int board[ROWS][COLS], int row, int *r, int len){
  int i;
  for (i=0;i<len;i++){
    r[i]=board[row][i];
  }
}

void get_r_diag(int board[ROWS][COLS], int diag, int *d, int len){
  int num=diag<=5?diag+1:12-diag;
  int i=diag<6?diag:5;
  int j=diag<6?0:diag-5;
  int k;
  for (k=0;k<num;k++){
    d[k]=board[i--][j++];
  }
  for (k=num;k<len;k++){
    d[k]=-1;
  }
}

void get_l_diag(int board[ROWS][COLS], int diag, int *d, int len){
  int num=diag<=5?diag+1:12-diag;
  int i=diag<6?diag:5;
  int j=diag<6?6:11-diag;
  int k;
  for (k=0;k<num;k++){
    d[k]=board[i--][j--];
  }
  for (k=num;k<len;k++){
    d[k]=-1;
  }
}

int eq(int *nums1, int *nums2, int len){
  int i;
  for (i=0;i<len;i++){
    if (nums1[i]!=nums2[i]){
      return 0;
    }
  }
  return 1;
}

int find_pattern(int *pattern, int p_len, int *nums, int n_len){
  int count=0;
  int i;
  for (i=0;i<=n_len-p_len;i++){
    if (eq(pattern,&nums[i],p_len)){
      count++;
      i+=p_len;
    }
  }
  return count;
}

int check_four(int nums[], int len){
  int count=0;
  int test=nums[0];
  int i;
  for (i=0;i<len;i++){
    if (test && nums[i]==test){
      count++;
    }
    else {
      count=1;
      test=nums[i];
    }
    if (count==4){
      return test;
    }
  }
  return 0;
}

int check_win(int board[ROWS][COLS]){
  int i,who;
  for (i=0;i<ROWS;i++){
    int r[COLS]={0};
    get_row(board,i,r,COLS);
    who=check_four(r,COLS);
    if (who){return who;}
  }
  for (i=0;i<COLS;i++){
    int c[ROWS]={0};
    get_col(board,i,c,ROWS);
    who=check_four(c,ROWS);
    if (who){return who;}
  }
  for (i=3;i<9;i++){
    int rd[DIAGS]={0};
    get_r_diag(board,i,rd,DIAGS);
    who=check_four(rd,DIAGS);
    if (who){return who;}
  }
  for (i=3;i<9;i++){
    int ld[DIAGS]={0};
    get_l_diag(board,i,ld,DIAGS);
    who=check_four(ld,DIAGS);
    if (who){return who;}
  }
  return 0;
}

void undo(int board[ROWS][COLS], int col){
  int i;
  for (i=0;i<ROWS;i++){
    if (board[i][col]){
      board[i][col]=0;
      return;
    }
  }
}

int count_pattern(int board[ROWS][COLS], int *pattern, int len){
  int count=0;
  int i;
  for (i=0;i<ROWS;i++){
    int r[COLS]={0};
    get_row(board,i,r,COLS);
    count+=find_pattern(pattern,len,r,COLS);
  }
  for (i=0;i<COLS;i++){
    int c[ROWS]={0};
    get_col(board,i,c,ROWS);
    count+=find_pattern(pattern,len,c,ROWS);
  }
  for (i=3;i<9;i++){
    int rd[DIAGS]={0};
    get_r_diag(board,i,rd,DIAGS);
    count+=find_pattern(pattern,len,rd,DIAGS);
  }
  for (i=3;i<9;i++){
    int ld[DIAGS]={0};
    get_l_diag(board,i,ld,DIAGS);
    count+=find_pattern(pattern,len,ld,DIAGS);
  }
  return count;
}

void fill(int *nums, int len, int val){
  int i;
  for (i=0;i<len;i++){
    nums[i]=val;
  }
}

int board_value(int board[ROWS][COLS], int player){
  int t1=0;
  int t2=0;
  int t3=0;
  int pattern[4];
  int i,j;
  for (i=0;i<4;i++){
    fill(pattern,4,0);
    pattern[i]=player;
    t1+=count_pattern(board,pattern,4);
  }
  for (i=0;i<3;i++){
    for (j=i+1;j<4;j++){
      fill(pattern,4,0);
      pattern[i]=player;
      pattern[j]=player;
      t2+=count_pattern(board,pattern,4);
    }
  }
  for (i=0;i<4;i++){
    fill(pattern,4,player);
    pattern[i]=0;
    t3+=count_pattern(board,pattern,4);
  }
  return t1+4*t2+9*t3;
}

int heuristic(int board[ROWS][COLS], int player){
  int who=check_win(board);

  if (who==player){
    return 10000;
  }
  else if (who){
    return -10000;
  }
  else if(is_full(board)){
    return 0;
  }
  int score=0;
  score+=board_value(board,player);
  score-=board_value(board,player%2+1);
  return score;
}

int minimax(int board[ROWS][COLS], int depth, int alpha, int beta, int player, int turn){
  if (depth==0 || check_win(board)){
    //print_board(board);
    //printf("Heuristic: %d\n",heuristic(board,player));
    //getchar();
    return heuristic(board,player)-(DISCOUNT-depth)*DISCOUNT;
  }

  int best=turn==player?-10000:10000;
  int i,val;
  for (i=0;i<COLS;i++){
    int p=drop_piece(board,i,turn);
    if (!p){
      val=minimax(board,depth-1,alpha,beta,player,turn%2+1);
      undo(board,i);
    }
    else {
      val=turn==player?-20000:20000;
    }
    best=turn==player?MAX(best,val):MIN(best,val);
  }
  return best;
}

int ai(int board[ROWS][COLS], int player){
  int depth=4;
  int scores[COLS];
  int i;
  for (i=0;i<COLS;i++){
    int p=drop_piece(board,i,player);
    if (!p){
      scores[i]=minimax(board,depth,-20000,20000,player,player%2+1);
      undo(board,i);
    }
    else {
      scores[i]=-20000;
    }
    //printf("scores[%d]=%d\n",i,scores[i]);
  }
  return argmax(scores,COLS);
}

int main(int argc, char **argv){
  //init board
  int i,j;
  int board[ROWS][COLS];
  for (i=0;i<ROWS;i++){
    for (j=0;j<COLS;j++){
      board[i][j]=0;
    }
  }

  //game loop
  int turn=1;
  while (1){
    print_board(board);
    int where;
    if (turn==1){
      printf("Okay player. Do your thing (1-7): ");
      scanf("%d",&where);
      where-=1;
    }
    else {
      where=ai(board,turn);
      printf("Computer goes in %d\n",where+1);
    }
    int p=drop_piece(board,where,turn);
    int who=check_win(board);
    if (who){
      printf("Player %d won!\n\n",who);
      print_board(board);
      break;
    }
    if (is_full(board)){
      printf("Draw, suckas\n");
      print_board(board);
      break;
    }
    if (p==0){
      turn%=2;
      turn++;
    }
  }

  return 0;
}
