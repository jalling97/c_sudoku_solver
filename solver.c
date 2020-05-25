#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//returns the block contents of the row col intersection
// pointer to array of shorts
short *get_block(int row, int col, short board[9][9]) {
    
    static short block[9];
    
    //integer arithmetic allows rounding to the nearest block
    int ro = 3*(row/3); //row offset
    int co = 3*(col/3); //col offset

    //set array to corresponding block contents
    for(int ii=0; ii<3; ii++)
    {
        for(int jj=0; jj< 3; jj++)
        {
            *(block + 3*ii + jj) = board[ii+ro][jj+co];
        }
    }
    
    return block;
}

//create another 9x9 matrix for keeping the inital numbers unchanged
void set_locked_nums(short locked_nums[9][9], short board[9][9])
{
    for(int ii=0; ii<9; ii++)
    {
        for(int jj=0; jj<9; jj++)
        {
            if(board[ii][jj] != 0)
                locked_nums[ii][jj] = 1;
        }
    }
}

//Check if the number in a given spot breaks any rules
short valid_entry(short board[9][9], int row, int col)
{
    //simply check if there is a duplicate in the given row and column
    // Then do the same for the block (a little more complex)

    short num = board[row][col];
    if(num == 0) //invalid if the entry in question is zero
        return 0;

    //row check
    for(int jj=0; jj<9; jj++)
    {
        if(board[row][jj] == num && jj != col)  {
            return 0;
        }
    }
    
    //col check
    for(int ii=0; ii<9; ii++)
    {
        if(board[ii][col] == num && ii != row)  {
            return 0;
        }
    }

    //block check
    //  had to use a count of how many times num
    //  appears in the block due to index comparison
    //  between block and board being complicated
    short *block = get_block(row, col, board);
    short match_count = 0;
    for(int kk=0; kk<9; kk++)
    {
        if(*(block+kk) == num)
            match_count++;
    }
    if(match_count >= 2)    {
        return 0;
    }

    //if all checks pass, it must be valid
    return 1;

}

//function for figuring out how far to backtrack
int backtrack(int pos, short locked_nums[9][9])
{
    pos--;
    int row = pos/9;
    int col = pos%9;

    while (locked_nums[row][col] == 1)
    {
        pos--;
        row = pos/9;
        col = pos%9;
    }

    return pos;
}

//read the board from txt file
void create_board(char* filename, short board[9][9])
{
    FILE *fp;
    char line[10];
    fp = fopen(filename, "r"); //open the txt file

    if(fp == NULL) {
        perror("Could not open file");
        return;
    }

    for(int ii=0;ii<9;ii++)
    {
        for(int jj=0;jj<9;jj++)
            fscanf(fp, "%hu", (&board[ii][jj]));
    }

    fclose(fp);
}

//print the board
void print_board(short board[9][9])
{
    printf("-------------------\n");
    for(int ii=0;ii<9;ii++)
    {
        printf("|");
        for(int jj=0;jj<9;jj++)
        {
            if(board[ii][jj] != 0)
                printf("%hu", board[ii][jj]);
            else
                printf(" ");
            if(jj==2 || jj==5 || jj==8)
                printf("|");
            else
                printf(" ");
            
        }
        printf("\n");
        if(ii==2 || ii==5 || ii == 8)
        {
            printf("-------------------\n");
        }
    }

}

/*Solving algorithm
    Brute force approach
      use a while loop to go through each spot on the board
      if not a locked spot, 
        if number is 9, replace with zero and go back one spot
        increment value in the spot
        if valid, continue to next position
        if not valid, increment the number in the spot
        repeat for all spots on the board

    returns 1 if valid solution found, 0 otherwise
*/
int solve_board(short board[9][9])
{
    short locked_nums[9][9] = {0}; //array to lock in starting numbers
    set_locked_nums(locked_nums, board);

    int pos = 0; //position on board

    //81 total positions on a Sudoku board
    while(pos < 81)
    {
        int row = pos/9; //
        int col = pos%9;

        if(pos < 0) {
            printf("\n-----Invalid Sudoku-----\n");
            return 0;
        }

        //if not a protected square
        if(locked_nums[row][col] == 0)
        {
            //if board reaches 9 and is still invalid,
            //must reset and backtrack
            if(board[row][col] == 9)
            {
                board[row][col] = 0;
                pos = backtrack(pos, locked_nums);
                continue;
            }
            
            board[row][col]++;

            if(valid_entry(board, row, col) == 1)
            {
                pos++;
                continue;
            }
        }
        else
        {
            pos++;   
        }
    }

    return 1; //valid solution
}

int main()
{
    short board [9][9] = {0}; //initialize board to all zeroes

    char* filename = "/home/john/coding/c_sudoku_solver/boards/board_1.txt";

    create_board(filename, board); //read starting board from file
    printf("Starting Board:\n");
    print_board(board);
    
    int valid = solve_board(board); //solve using backtracking algorithm
    if(valid)
    {
        printf("\nCompleted Board:\n");
        print_board(board);
    }
    
    return 0;
}