// Melisa Yilmaz 26777
// Date: 12.11.2021
// Programming Assignment 2
// Tic-Tac-Toe with Threads


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int N; // NXN matrix
char** matrix; // create matrix for XOX game
bool game_turn = true;
bool gameEnd = false;
char winner = 't';


// create empty matrix with given size (N)
void createMatrix()
{
    int i;
    matrix = malloc(sizeof(char*)*N);
    for (i = 0; i < N; i++)
    {
        matrix[i] = malloc(sizeof(char)*N);
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = ' ';
        }
    }
}

// deallocate all the rows and columns of matrix which is created by using malloc
void deallocateMatrix(){
    int i;
    for (i = 0; i < N; i++)
    {
        free(matrix[i]);
        matrix[i] = NULL;
        
    }
    free(matrix);
    matrix = NULL;
}

// create random row
int randomRow()
{
    int row = rand() % N;
    return row;
}

// create random column
int randomColumn(){
    int col= rand() % N;
    return col;
}

// check whether place of given row and column is occupided or not
bool isCellEmpty(int row, int col)
{
    bool status=true;
    if(matrix[row][col] != ' ')
        status = false;
    return status;
    
}

// put player to the occupided place by row and column in the matrix
void puttingThePlayer(int row, int col, char player)
{
    matrix[row][col] = player;
}

//checking whether given column is occupided or not
bool checkingColumn(int col,char player)
{
    bool status = true;
    for(int i = 0; i < N; i++)
    {
        if(matrix[i][col] != player)
            status=false;
    }
    return status;
}

//checking whether given row is occupided or not
bool checkingRow(int row,char player)
{
    bool status = true;
    for(int i = 0; i < N; i++)
    {
        if(matrix[row][i] != player)
            status=false;
    }
    return status;
}

//checking the right cross in the matrix
bool checkingRightCross(int row, int col,char player)
{
    bool status = true;
    if (row == col)
    {
        for(int i = 0; i < N; i++)
        {
            if(matrix[i][i] != player)
                status=false;
        }
    }
    else
        status=false;
    
    return status;
}

//checking the left cross in the matrix
bool checkingLeftCross(char player)
{
    bool status = true;
   
    for (int j = 0; j < N; j++)
    {
        if(matrix[j][N-j-1] != player)
        {
            status=false;
        }
    }
    return status;
    
}

// check whether matrix is full or not
bool isFull()
{
    bool status=false;
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (int k = 0; k < N; k++)
        {
            if(matrix[i][k] == ' ')
            {
                count++;
                status = false;
            }
        }
    }
    if (count==0)
    {
        status=true;
    }
    return status;
    
}

// check whether the winner is X or O
bool checkingWinner(int row, int col, char player)
{
    bool winner = false;
    if(checkingRow(row, player) == true || checkingColumn(col, player) == true || checkingRightCross(row, col, player) == true ||checkingLeftCross(player) == true )
    {
        winner = true;
    }
    return winner;
}

// print all the rows and columns of matrix
void printMatrix()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("[%c]",matrix[i][j]);
        }
        printf("\n");
    }
}

void* TicTacToe(void* arg)
{
    char player = (char*) arg;
    
    while(gameEnd == false)
    {
        pthread_mutex_lock(&mutex); // only one player should access to the matrix
        
        if(isFull() == false && winner =='t') // if matrix is not full and there haven't been a winner yet
        {
            if( (game_turn == true && player=='X') || (game_turn == false && player=='O'))
            {
                int row = randomRow(); // create random row
                int col = randomColumn(); // create random column
                
                while(isCellEmpty(row, col) == false) // create new row and column value until created place of newly created row and column have been already taken
                {
                    row = randomRow();
                    col = randomColumn();
                }
                
                puttingThePlayer(row, col, player); // put the player to the created row and column if that palce is empty
                printf("Player %c played on: (%d,%d)\n",player,row,col);
                
                if(checkingWinner(row, col, player)) // check whether player is win or not
                {
                    winner = player;
                    gameEnd = true;
                }
                else
                {   // give next turn to the other player
                    if (player == 'X')
                        game_turn=false;
                    else
                        game_turn = true;
                }
            }
        }
        
        if (isFull() == true && winner == 't') //if matrix is full and winner is "t" so winner is not X or O
        {
            winner = 't';
            gameEnd =true;
        }
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
    
}

int main(int argc, const char * argv[]) {
    
    srand(time(NULL)); // to generate random variable to choose empty index in the matrix
    
    N = atoi(argv[1]); // to take size of the matrix as an argument from console
    createMatrix();
    printf("Board Size: %dX%d\n",N,N);

    pthread_t playerX,playerO;
    
    //X & O threads are created
    if (pthread_create(&playerX, NULL, TicTacToe, (void*) (char) 'X') != 0)
    {
        perror("Failed to create thread of player X");
    }
    if (pthread_create(&playerO, NULL, TicTacToe, (void*) (char) 'O') != 0)
    {
        perror("Failed to create thread of player O");
    }
    
    if (pthread_join(playerX,NULL) != 0)
    {
        return 3;
    }
    if (pthread_join(playerO,NULL) != 0)
    {
        return 4;
    }
    printf("\n");
    printf("Game end\n");
    if (winner != 't')
    {
        printf("Winner is %c\n", winner);
    }
    else { // if winner is equal to the "t", it means that it is a tie
        printf("It is a tie\n");
    }
    
    printMatrix();
    deallocateMatrix();
   
    return 0;
}

