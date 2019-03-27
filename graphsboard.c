#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

void size_prompt();
int play_game(char computer_active);
int replay_query();
char computer_query();

void printBoard(char *board);
int takeTurn(char *board, int player, char*);

int comp_best_move(char *board, int, char*); 
int calc_move(char *board, int, char*, int);
void computer_takeTurn(char *board, int, char*);
int evaluation(int evaluate[], int);

int checkWin(char *board);
int checkFour(char *board, int, int, int, int);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);

int num_rows = 0;
int num_cols = 0;

int main(int argc, char *argv[]){
	
	int finished = 0;
	int one_score = 0;
	int two_score = 0;
	int result = 0;
	char computer_active = '0';
	
	printf("Welcome to Connect 4! \nYou can play as two humans, or vs a computer going first or second. \nYou may resize the board before each game as small as 4x4 and as large as you wish, though sizes larger than 40 may have visual errors. \nScore will be kept track throughout the session by a Player 1 or 2 basis, where Player 1 went first.\n"); 
	
	while(finished != 1){ 
		computer_active = computer_query();
		size_prompt();
		result = play_game(computer_active);
		if(result == 1) one_score++;
		if(result == 2) two_score++;
		
		printf("Current scores: \nPlayer 1 - %d\nPlayer 2 - %d\n",one_score,two_score);
		printf("Play again? Y/N \n");
		finished = replay_query();
	}
	printf("Thanks for playing, shutting down.\n");
	return 0;
}

char computer_query(){
	char computer_mode = '3';
	printf("Please pick a mode:\nPlayer 1 V Player 2\n0: Human V Human\n1: Human V Computer\n2: Computer V Human\n");
	while( !(computer_mode == '0' || computer_mode == '1' || computer_mode == '2') ){
		computer_mode = getchar();
		if( !(computer_mode == '0' || computer_mode == '1' || computer_mode == '2' || computer_mode == '\n') ){
			printf("Please enter mode: 0,1,2\n");
		}
	}
	return computer_mode;
}

int replay_query(){
	char play_again='x';
	char buffer[50];
	int correct,error,finished = 0;
	while( !(play_again == 'Y' || play_again == 'y' || play_again == 'n' || play_again == 'N') ){
		play_again = getchar();
		if( !(play_again == 'Y' || play_again == 'y' || play_again == 'n' || play_again == 'N' || play_again == '\n') ){
			printf("Please enter Y to continue or N to quit\n");
		}
	}
	if(play_again == 'n' || play_again == 'N') finished = 1;
	gets(buffer);
return finished;
}

int play_game(char computer_active){
	char *piece_select = "RB";
	char board[num_rows * num_cols];
	memset(board, ' ', num_rows * num_cols);

	int turn, done = 0;

	for(turn = 0; turn < num_rows * num_cols && !done; turn++){
		printBoard(board);   
		if( (computer_active == '1' && turn%2 == 1) || (computer_active == '2' && turn%2 == 0) ){
			computer_takeTurn(board, turn%2, piece_select);
		}else{
			while(!takeTurn(board, turn % 2, piece_select)){
				printBoard(board);   
				puts("**Column full!**\n");
			}
		}
		done = checkWin(board);
	} 
   printBoard(board);

   if(turn == num_rows * num_cols && !done){
      puts("Tie Game!");
	  return 0;
   } else {
		turn--;
		if(piece_select[turn % 2] == 'R'){
			printf("Player %d (\x1b[31m%c\x1b[0m) wins!\n", turn % 2 + 1, piece_select[turn % 2]);
			return 1;
		}
		if(piece_select[turn % 2] == 'B'){
			printf("Player %d (\x1b[34m%c\x1b[0m) wins!\n", turn % 2 + 1, piece_select[turn % 2]);
			return 2;
		}
   }
}

void size_prompt(){
	int correct = 0;
	char buffer[50];
	num_rows = 0;
	num_cols = 0;
	while(correct != 1){
		printf("What number of rows do you want to play with?\n");
		scanf("%d",&num_rows);
		if(num_rows < 4){
			printf("Please choose rows equal to or greater than 4\n");
		}
		if(num_rows > 40){
			printf("There may be formatting errors with too large of dimensions\n");
		}
		if (num_rows >= 4){
			correct = 1;
		}
		gets(buffer);
	}
		correct = 0;
	while(correct != 1){
		printf("What number of columns do you want to play with?\n");
		scanf("%d",&num_cols);
		if(num_cols < 4){
			printf("Please choose columns equal to or greater than 4\n");
		}
		if(num_cols > 40){
			printf("There may be formatting errors with too large of dimensions\n");
		}
		if (num_cols >= 4){
			correct = 1;
		}
		gets(buffer);
	}
	return;
}

void printBoard(char *board){
	int row, col;

	puts("\n    ****Connect Four****\n");
	for(row = 0; row < num_rows; row++){
		printf("\n");
		for(col = 0; col < num_cols; col++){
			if(board[num_cols * row + col] == 'R'){
				printf("|\x1b[31m%c\x1b[0m ",  board[num_cols * row + col]);
			}
			else{
				if(board[num_cols * row + col] == 'B'){
					printf("|\x1b[34m%c\x1b[0m ",  board[num_cols * row + col]);
				}else{ 
				printf("|%c ",  board[num_cols * row + col]);
				}
			}
		}
		puts("|");
		for(col = 0; col < num_cols; col++){
			printf("---");
		}
		printf("-");
	}
	printf("\n");
	for(col = 1; col < num_cols+1; col++){
		printf(" %02d",col);
	}
	printf("\n");
}

int takeTurn(char *board, int player, char *piece_select){
	int row, col = 0;
	printf("Player %d (%c):\nEnter column number to place your piece: ", player + 1, piece_select[player]);

	while(1){ 
		if(1 != scanf("%d", &col) || col < 1 || col > num_cols ){
			while(getchar() != '\n');
			puts("Number out of bounds! Try again.");
		} else { 
			break;
		}
	}
	col--;

	for(row = num_rows - 1; row >= 0; row--){
		if(board[num_cols * row + col] == ' '){
			board[num_cols * row + col] = piece_select[player];
			return 1;
		}
	}
	return 0;
}

int checkWin(char *board){
	return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));
}

int checkFour(char *board, int a, int b, int c, int d){
	return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');
}

int horizontalCheck(char *board){
	int row, col, index;

	for(row = 0; row < num_rows; row++){
		for(col = 0; col < num_cols - 3; col++){
			index = num_cols * row + col;
			if(checkFour(board,index,index+1,index+2,index+3)){
				return 1;
			}
		}
    }
    return 0;
}

int verticalCheck(char *board){
	int row, col, index;
    int HEIGHT = num_cols;

	for(row = 0; row < num_rows - 3; row++){
		for(col = 0; col < num_cols; col++){
			index = num_cols * row + col;
			if(checkFour(board,index,index+HEIGHT,index+2*HEIGHT,index+3*HEIGHT)){
				return 1;
			}
		}
	}
   return 0;
}

int diagonalCheck(char *board){
	int row, col, index, count = 0;
	const int DIAG_RGT = num_cols-1, DIAG_LFT = num_cols+1;

	for(row = 0; row < num_rows - 3; row++){	
		for(col = 0; col < num_cols; col++){
			index = num_cols * row + col;
			if(count <= 3 && checkFour(board,index,index+DIAG_LFT,index+DIAG_LFT*2,index+DIAG_LFT*3) || count >= 3 && checkFour(board,index,index+DIAG_RGT,index+DIAG_RGT*2,index+DIAG_RGT*3)){
				return 1;
			}
			count++;
		}
		count = 0;
	}
	return 0;
}

void computer_takeTurn(char *board, int player, char *piece_select){
	printf("I made it to computer_takeTurn");
	board[comp_best_move(board, player, piece_select)] = piece_select[player];
}

int comp_best_move (char *board, int player, char *piece_select){
	printf("I made it to comp_best_move");
	int x = 0;
	int y = 0;
	int row = 0;
	int best_score=0;
	int this_score=0;
	int move=num_cols/2;
	
	while(x<num_cols){
		for(row = num_rows; row >= 0; row--){
			if(board[num_cols * row + x] == ' '){
				y = num_cols * row + x;
			}
		}
		this_score = calc_move(board,player,piece_select,y);
		if(this_score > best_score){ //-calc_move(board,(player+1)%2,piece_select,y+num_cols
			move = y;
			best_score = this_score;
			this_score = 0;
			y=0;
		}
		x=x+1;
	}
	printf("\x1b[34mcomp_best_move returns:%d\x1b[0m",move);
	return move;
}

int calc_move(char *board, int player, char *piece_select, int position){
	printf("\x1b[31m\ncalc_move received:%d\x1b[0m",position);
	int score = 0;
	int x;
	int add;
	int evaluate[6];
	
	x = 0;      //setup for horizontal evaluation
	while (x<7){
		if ( (position+x-3) > num_rows*num_cols || (position+x-3) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position+x-3]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("could not determine contents");
					break;
			}
			if((position+x-3)%num_cols == num_cols-1 || (position+x-3)%num_cols == 0){
			evaluate[x] = evaluate[x] + 3;
			}
		}	
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add;

	
	x = 0;      //setup for vertical evaluation
	while (x<7){
		if ( (position+(x-3)*num_cols) > num_rows*num_cols || (position+(x-3)*num_cols) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position+(x-3)*num_cols]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					evaluate[x] = 10;
					printf("could not determine contents");
					break;
			}
			if( (position+(x-3)*num_cols)%num_cols == num_cols-1 || (position+(x-3)*num_cols)%num_cols == 0){
				evaluate[x] = evaluate[x] + 3;
			}
		}	
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add;
	
	x = 0;      //setup for lower left to upper right evaluation
	while (x<7){
		if( ( (position+(x-3)+(x-3)*num_cols)*num_cols) > num_rows*num_cols || (position+(x-3)+(x-3)*num_cols) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position+(x-3)+(x-3)*num_cols]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("could not determine contents");
					break;
			}
			if( (position+(x-3)+(x-3)*num_cols)%num_cols == num_cols-1 || (position+(x-3)+(x-3)*num_cols)%num_cols == 0){
			evaluate[x] = evaluate[x] + 3;
			}
		}
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add;
	
	x = 0;      //setup for lower right to upper left evaluation
	while (x<7){
		if ( ( (position-(x-3)+(x-3)*num_cols)*num_cols) > num_rows*num_cols || (position-(x-3)+(x-3)*num_cols) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position-(x-3)+(x-3)*num_cols]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("could not determine contents");
					break;
			}
			if( (position-(x-3)+(x-3)*num_cols)%num_cols == num_cols-1 || (position-(x-3)+(x-3)*num_cols)%num_cols == 0){
			evaluate[x] = evaluate[x] + 3;
			}
		}
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add;
	
	printf("\x1b[33mcalc_move returns:%d\x1b[0m",score);
	return score;
}

int evaluation (int evaluate[],int player){
//	printf("I made it to evaluation");
	
	int score = 0;
	if(evaluate[3] == -1){  //if the move itself is invalid, score 0;
		return score;
	}
	
	int x = 0;
	int y = 7;
	while(x<7){				//find the wrap around, restore its value, eliminate things past the wrap around
		if(evaluate[x] > 2){
			evaluate[x] = evaluate[x]-3;
			if (x>3){
				y = x;
				x = x + 1;
				while(x<7){
					evaluate[x] = -1;
					x = x + 1;
				}
				x = y;
				y = 0;
			}	
			if (x<3){
				y = x;
				x = x - 1;
				while(x > -1){
				evaluate[x] = -1;
				x = x - 1;
				}
				x = y;
				y = 0;
			}
		}
	x = x + 1;
	}
	
	int e0 = evaluate[0];
	int e1 = evaluate[1];
	int e2 = evaluate[2];
	int e4 = evaluate[4];
	int e5 = evaluate[5];
	int e6 = evaluate[6];
	
	//is the move next to a group of 3 in a row?
	if( ( ( (e0==e1&&e1==e2) || (e1==e2&&e2==e4) || (e2==e4&&e4==e5) ) && e2>0 ) || ( (e4==e5&&e5==e6)&& e4>0 ) ){
		score = score + 1000; //it's better than anything other than winning
		if(e2==(player+4)%3||e4==(player+4)%3){
			score = score + 1000; //it's winning
		}
	}
	//is the move next to a group of 2 in a row? (with growth potential due to 0's)
	if( (e1==e2 && (e1>0) && (e0==0 || e4==0) ) || (e2==e4 && (e2>0) && (e1==0 || e5==0) ) || (e4==e5 && (e4>0) && (e2==0||e6==0) ) ){
		score == score + 100; //not as important as winning, next best thing, color less important
	}
	//does the move make a 2 chain?
	if( (e2>0 && ( (e0==e1 && e0==0) || (e1==e4 && e1==0) || (e4==e5 && e4==0) ) )||(e4>0  && ( (e0==e1 && e0==0) || (e1==e4 && e1==0) || (e4==e5 && e4==0) ) ) ){
		score = score + 10; //not as good as making a 3 chain
	}
	
	x = 0;
	while(x<7){
		if(evaluate[x] != -1){
			score = score + 1; //being central is not as good as starting a chain, but it's better to avoid walls 
		}
		x=x+1;
	}	
	printf("eval:%d ",score);
	return score;
}