#include <stdio.h>
#include <stdlib.h>
int board[4][4];
int move_left();

void rotate90() {
    for (int i = 0; i < 2; i++) {
        for (int j = i; j < 3 - i; j++) {
            int temp = board[i][j];
            board[i][j] = board[3 - j][i];
            board[3 - j][i] = board[3 - i][3 - j];
            board[3 - i][3 - j] = board[j][3 - i];
            board[j][3 - i] = temp;
        }
    }
}

int move_up() {
	/* student implementation */
	/* Return 0 if the board is unchanged; otherwise, return 1. */
    rotate90();
    rotate90();
    rotate90();
    int check = move_left();
    rotate90();
    return check;
}

int move_down() {
	/* student implementation */
	/* Return 0 if the board is unchanged; otherwise, return 1. */
	rotate90();
    int check = move_left();
    rotate90();
    rotate90();
    rotate90();
    return check;
}

int move_left() {
    /* student implementation */ 
	/* Return 0 if the board is unchanged; otherwise, return 1. */
    int check = 0;
    for (int i = 0; i < 4; i++){
        int temp[4] = {0};
        int idx = 0;

        for (int j = 0; j < 4; j++)
            if (board[i][j] != 0) temp[idx++] = board[i][j];
        
        for (int j = 0; j < 3; j++){
            if (temp[j] != 0 && temp[j] == temp[j+1]){
                temp[j] *= 2;
                temp[j+1] = 0;
            }
        }

        int res[4] = {0};
        idx = 0;
        for (int j = 0; j < 4; j++){
            if (temp[j] != 0){
                res[idx++] = temp[j];
            }
        }

        for (int j = 0; j < 4; j++){
            if (res[j] != board[i][j]){
                board[i][j] = res[j];
                check = 1;
            }
        }
    }

    return check;
}

int move_right() {
    /* student implementation */
	/* Return 0 if the board is unchanged; otherwise, return 1. */
    rotate90();
    rotate90();
    int check = move_left();
    rotate90();
    rotate90();
    return check;
}

int game_over() {
	/* student implementation */
	/* return 0 if the game is not over, else return 1 */
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] == 0) return 0;
        }
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j] == board[i][j+1]) return 0;
        }
    }

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] == board[i+1][j]) return 0;
        }
    }

	return 1;
}

void print_board() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d", board[i][j]);
            if (j < 3) printf(" ");
        }
        printf("\n");
    }
}

int main() {
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) scanf("%d", &board[i][j]);
    }

    int Q;
    scanf("%d", &Q);

    for (int turn = 1; turn <= Q; turn++) {
        char S;
        int y, x, v;
        scanf(" %c %d %d %d", &S, &y, &x, &v);

        int changed = 0;
		if (S == 'U') changed = move_up();
		else if (S == 'D') changed = move_down();
		else if (S == 'L') changed = move_left();
		else if (S == 'R') changed = move_right();

		if (changed) board[y][x] = v;
		if (game_over()) {
			print_board();
			exit(0);
		}
    }

    print_board();
    return 0;
}