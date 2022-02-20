//
//  main.c
//  melisayilmaz
//
//  Created by Melisa Yilmaz on 12.11.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // for the random generator
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int N;
char** matrix;
bool game_turn = true;
bool gameEnd = false;
char winner = 't';
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

int randomRow()
{
    int row = rand() % N;
    return row;
}

int randomColumn(){
    int col= rand() % N;
    return col;
}

bool isCellEmpty(int row, int col)
{
    bool status=true;
    if(matrix[row][col] != ' ')
        status = false;
    return status;
    
}

void puttingThePlayer(int row, int col, char player)
{
    matrix[row][col] = player;
}

bool checkingRow(int row, int col,char player)
{
    bool status = true;
    for(int i = 0; i < N; i++)
    {
        if(matrix[i][col] != player)
            status=false;
    }
    return status;
}
bool checkingColumn(int row, int col,char player)
{
    bool status = true;
    for(int i = 0; i < N; i++)
    {
        if(matrix[row][i] != player)
            status=false;
    }
    return status;
}

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

bool checkingLeftCross(int row, int col,char player)
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

bool checkingWinner(int row, int col, char player)
{
    bool winner = false;
    if(checkingRow(row, col, player) == true || checkingColumn(row, col, player) == true || checkingRightCross(row, col, player) == true ||checkingLeftCross(row, col, player) == true )
    {
        winner = true;
    }
    return winner;
}


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
        pthread_mutex_lock(&mutex);
        if(isFull() == false && winner =='t')
        {
            if( (game_turn == true && player=='X' )|| (game_turn == false && player=='O'))
            {
                int row = randomRow();
                int col = randomColumn();
                while(isCellEmpty(row, col) == false)
                {
                    row = randomRow();
                    col = randomColumn();
                }
                puttingThePlayer(row, col, player);
                printf("Player %c played on: (%d,%d)\n",player,row,col);
                if(checkingWinner(row, col, player))
                {
                    winner = player;
                    gameEnd = true;
                }
                else
                {
                    if (player == 'X')
                        game_turn=false;
                    else
                        game_turn = true;
                }
            }
        }
        if (isFull() == true && winner == 't')
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
    else {
        printf("It is a tie\n");
    }
    
    printMatrix();
    deallocateMatrix();
   
    return 0;
}

