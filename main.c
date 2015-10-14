#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
//#include <sys/resource.h>

//gameplay functions
void printBoard ();
char checkWin();

//finding the best move functions
int minimax (int depth, char turn, int a, int B, bool maxPlaya);
int findLongest (char player);
int window (char player);

//global variables;
char **board;
int n;
const int startDepth=2;
bool debug = false;

int main (void)
{
    //declaring variables
    char YorN='N';
    int row, col, dummy, playrow, playcol, moveNumber=0;
    char humanColour, turn='B', computerColour;
    double maxTime=0;
    bool cpuVScpu = false;
    char horc;
    //creating the board
    printf("Enter board dimensions (n): ");
    scanf("%d", &n);
    printf("Will the computer play a human ('h') or itself ('c'): ");
    scanf(" %c", &horc);
    cpuVScpu = (horc == 'c');
    printf("cpu or human? %d", cpuVScpu);
    board = (char**) malloc(n*sizeof(char*));
    for (int i = 0; i<n; i++)
    {
        board[i] = (char*) malloc (n*sizeof(char));
    }
    //filling the board with 'U's
    for (int i = 0; i<n; i++)
    {
        for (int j = 0; j<n; j++)
        {
            board[i][j]='U';
        }
    }
    printBoard ();

    //getting computerColour color
    printf("Computer playing B or W?: ");
    scanf(" %c", &computerColour);
    if(computerColour=='B')
        humanColour='W';
    else
        humanColour='B';

    //start play
    do
    {
        //computer's turn
//        struct rusage usage; // a structure to hold "resource usage" (including time)
//        struct timeval start, end; // will hold the start and end times
//        getrusage(RUSAGE_SELF, &usage);
//        start = usage.ru_utime;
//        double time_start = start.tv_sec + start.tv_usec/1000000.0; // in seconds
        if (turn==computerColour)
        {
            if (row==(n-1)/2 && col==(n-1)/2 && moveNumber==1)
            {
                playrow=row-1;
                playcol=col-1;
                board[playrow][playcol]=computerColour;
            }
            else
            {
                dummy = minimax(startDepth, computerColour, -99999999, 99999999, true);
                for (int i = 0; i<n; i++)
                {
                    for (int j = 0; j<n; j++)
                    {
                        if(board[i][j]=='T')
                        {
                            board[i][j]=computerColour;
                            playrow=i;
                            playcol=j;
                        }
                    }
                }
            }
            printf("Computer lays a stone at ROW %d COL %d.\n", playrow, playcol);
            turn=humanColour;
            moveNumber++;
            //checking for win and printing board
            printBoard();
            YorN=checkWin();
            if(YorN!='N')
                break;
        }
//        getrusage(RUSAGE_SELF, &usage);
//        end = usage.ru_utime;
//        double time_end = end.tv_sec + end.tv_usec/1000000.0; // in seconds
//        double total_time = time_end - time_start;  // total_time now holds the time (in seconds) it takes to run your code
//        printf("time: %lf, max time: %lf\n", total_time, maxTime);
//        maxTime=fmax(total_time, maxTime);


        if (turn==humanColour)
        {
            //playing itself
            if (cpuVScpu) {
                if (row==(n-1)/2 && col==(n-1)/2 && moveNumber==1)
                {
                    playrow=row-1;
                    playcol=col-1;
                    board[playrow][playcol]=humanColour;
                }
                else
                {
                    dummy = minimax(startDepth, humanColour, -99999999, 99999999, true);
                    for (int i = 0; i<n; i++)
                    {
                        for (int j = 0; j<n; j++)
                        {
                            if(board[i][j]=='T')
                            {
                                board[i][j]=humanColour;
                                playrow=i;
                                playcol=j;
                            }
                        }
                    }
                }
                printf("Computer lays a stone at ROW %d COL %d.\n", playrow, playcol);
                turn=computerColour;
                moveNumber++;
                //checking for win and printing board
                printBoard();
                YorN=checkWin();
                if(YorN!='N')
                    break;
            } else {
                //human's turn
                do
                {
                    printf("Lay down a stone (ROW COL): ");
                    scanf("%d %d", &row, &col);
                    if (board[row][col] == 'U')
                    {
                        board[row][col]=humanColour;
                        turn=computerColour;
                        moveNumber++;
                    }
                    else
                        printf("That square is occupied.\n");
                }
                while(turn==humanColour);
                printBoard();
                YorN=checkWin();
            }
        }
    }
    while(YorN=='N');
    //freeing the board;
    for (int i=0; i<n; i++)
    {
        free(board[i]);
    }
    free(board);
    return 0;

}

void printBoard()
{
    printf("  ");
    for (int i = 0; i<n; i++)
        printf("%3d", i);
    printf("\n");
    for (int i = 0; i<n; i++)
    {
        printf("%2d  ", i);
        for(int j=0; j<n; j++)
        {
            if (board[i][j]=='U')
                printf("   ");
            else
                printf("%c  ", board[i][j]);
        }
        printf("\n");
    }
}

//window score function
int window (char player)
{
    int temp=0, score=0, counter;

    char otherPlayer;
    if (player=='B')
        otherPlayer='W';
    if (player=='W')
        otherPlayer='B';

    int extraPoints=0;
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if (board[i][j]==player)
            {
                //starting 6 back, checking each series of 6 vertically
                for (int t = -5; t<=0; t++)
                {
                    extraPoints=0;
                    counter=0;
                    for (int a = 0; a<6 && i+t+5<n && i+t>=0; a++)
                    {
                        //extra points are awarded for consecutive tiles
                        if (board[i+t+a][j]=='U')
                        {
                            extraPoints=0;
                        }
                        if (board[i+t+a][j]==player)
                        {
                            temp+=(10+extraPoints);
                            extraPoints=(extraPoints+1)*2;
                            counter++;
                        }
                        if (board[i+t+a][j]==otherPlayer)
                        {
                            temp=0;
                            break;
                        }
                    }
                    if (counter==6){
                        temp*=81;
                    }
                    if (counter==5){
                        temp*=9;
                    }
                    if (counter==4){
                        temp*=3;
                    }
                    score+=temp;
                    temp=0;
                }

                //starting 6 back, checking each series of 6 horizontally
                for (int t = -5; t<=0; t++)
                {
                    extraPoints=0;
                    counter=0;
                    for (int a = 0; a<6 && j+5+t<n && j+t>=0; a++)
                    {
                        if (board[i][j+t+a]=='U')
                        {
                            extraPoints=0;
                        }
                        if (board[i][j+t+a]==player)
                        {
                            temp+=(10+extraPoints);
                            extraPoints=(extraPoints+1)*2;
                            counter++;
                        }
                        if (board[i][j+t+a]==otherPlayer)
                        {
                            temp=0;
                            break;
                        }
                    }
                    if (counter==6){
                        temp*=81;
                    }
                    if (counter==5){
                        temp*=9;
                    }
                    if (counter==4){
                        temp*=3;
                    }
                    score+=temp;
                    temp=0;
                }


                //starting 6 back, checking each series of 6 diagonally downward
                for (int t = -5; t<=0; t++)
                {
                    extraPoints=0;
                    counter=0;
                    for (int a = 0; a<6 && j+5+t<n && i+5+t<n && i+t>=0 && j+t>=0; a++)
                    {
                        if (board[i+t+a][j+t+a]=='U')
                        {
                            extraPoints=0;
                        }
                        if (board[i+t+a][j+t+a]==player)
                        {
                            temp+=(10+extraPoints);
                            extraPoints=(extraPoints+1)*2;
                            counter++;
                        }
                        if (board[i+t+a][j+t+a]==otherPlayer)
                        {
                            temp=0;
                            break;
                        }
                    }
                    if (counter==6){
                        temp*=81;
                    }
                    if (counter==5){
                        temp*=9;
                    }
                    if (counter==4){
                        temp*=3;
                    }
                    score+=temp;
                    temp=0;
                }


                //starting 6 back, checking each series of 6 diagonally upward
                for (int t = -5; t<=0; t++)
                {
                    extraPoints=0;
                    counter=0;
                    for (int a = 0; a<6 && j+5+t<n && i-t<n && j+t>=0 && i-5-t>=0; a++)
                    {
                        if (board[i-t-a][j+t+a]=='U')
                        {
                            extraPoints=0;
                        }
                        if (board[i-t-a][j+t+a]==player)
                        {
                            temp+=(10+extraPoints);
                            extraPoints=(extraPoints+1)*2;
                            counter++;
                        }
                        if (board[i-t-a][j+t+a]==otherPlayer)
                        {
                            temp=0;
                            break;
                        }
                    }
                    if (counter==6){
                        temp*=81;
                    }
                    if (counter==5){
                        temp*=9;
                    }
                    if (counter==4){
                        temp*=3;
                    }
                    score+=temp;
                    temp=0;
                }
            }
        }
    }
    return score;
}



//find longest score function
int findLongest (char player)
{
    //declaring variables
    int score=0, temp=0, potential=0;
    bool continuous;
    char otherPlayer;
    if (player=='B')
        otherPlayer='W';
    if (player=='W')
        otherPlayer='B';

    //finding longest streak that could potentially form 6
    for(int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if (board[i][j] != 'U')
            {
                //checking for longest streak vertically
                temp=0;
                potential=0;
                continuous=true;
                for (int k = 0; k<=5 && i+k<n; k++)
                {
                    if (board[i+k][j]==player && continuous)
                        temp++;
                    if (board[i+k][j]=='U')
                        continuous=false;
                    if (board[i+k][j]!=otherPlayer)
                        potential++;
                    if (board[i+k][j]==otherPlayer)
                        break;
                }
                continuous=true;
                for (int k = -1; k>=-5 && i+k>=0; k--)
                {
                    if (board[i+k][j]==player && continuous)
                        temp++;
                    if (board[i+k][j]=='U')
                        continuous=false;
                    if (board[i+k][j]!=otherPlayer)
                        potential++;
                    if (board[i+k][j]==otherPlayer)
                        break;
                }
                if (potential<6)
                {
                    temp=0;
                }
                if (temp>=6)
                {
                    score=999999;
                    goto bottom;
                }
                score+=temp;

                //checking for longest streak horizontally
                temp=0;
                potential=0;
                continuous=true;
                for (int k = 0; k<=5 && j+k<n; k++)
                {
                    if (board[i][j+k]==player && continuous)
                        temp++;
                    if (board[i][j+k]=='U')
                        continuous=false;
                    if (board[i][j+k]!=otherPlayer)
                        potential++;
                    if (board[i][j+k]==otherPlayer)
                        break;
                }
                continuous=true;
                for (int k = -1; k>=-5 && j+k>=0; k--)
                {
                    if (board[i][j+k]==player && continuous)
                        temp++;
                    if (board[i][j+k]=='U')
                        continuous=false;
                    if (board[i][j+k]!=otherPlayer)
                        potential++;
                    if (board[i][j+k]==otherPlayer)
                        break;
                }
                if (potential<6)
                {
                    temp=0;
                }
                if (temp>=6)
                {
                    score=999999;
                    goto bottom;
                }
                score+=temp;

                //checking for longest streak downward diagonally
                temp=0;
                potential=0;
                continuous=true;
                for (int k = 0; k<=5 && j+k<n && i+k<n; k++)
                {
                    if (board[i+k][j+k]==player && continuous)
                        temp++;
                    if (board[i+k][j+k]=='U')
                        continuous=false;
                    if (board[i+k][j+k]!=otherPlayer)
                        potential++;
                    if (board[i+k][j+k]==otherPlayer)
                        break;
                }
                continuous=true;
                for (int k = -1; k>=-5 && j+k>=0 && i+k>=0; k--)
                {
                    if (board[i+k][j+k]==player && continuous)
                        temp++;
                    if (board[i+k][j+k]=='U')
                        continuous=false;
                    if (board[i+k][j+k]!=otherPlayer)
                        potential++;
                    if (board[i+k][j+k]==otherPlayer)
                        break;
                }
                if (potential<6)
                {
                    temp=0;
                }
                if (temp>=6)
                {
                    score=999999;
                    goto bottom;
                }
                score+=temp;

                //checking for longest streak upward diagonally
                temp=0;
                potential=0;
                continuous=true;
                for (int k = 0; k<=5 && j-k>=0 && i+k<n; k++)
                {
                    if (board[i+k][j-k]==player && continuous)
                        temp++;
                    if (board[i+k][j-k]=='U')
                        continuous=false;
                    if (board[i+k][j-k]!=otherPlayer)
                        potential++;
                    if (board[i+k][j-k]==otherPlayer)
                        break;
                }
                continuous=true;
                for (int k = -1; k>=-5 && j-k<n && i+k>=0; k--)
                {
                    if (board[i+k][j-k]==player && continuous)
                        temp++;
                    if (board[i+k][j-k]=='U')
                        continuous=false;
                    if (board[i+k][j-k]!=otherPlayer)
                        potential++;
                    if (board[i+k][j-k]==otherPlayer)
                        break;
                }
                if (potential<6)
                {
                    temp=0;
                }
                if (temp>=6)
                {
                    score=999999;
                    goto bottom;
                }
                score+=temp;
            }
        }
    }
bottom:
    return score;
}

//function to evaluate the score of the board for both players, then find the difference
int eval(char comp)
{
    char human;
    if (comp=='B')
        human='W';
    if (comp=='W')
        human='B';

    int scoreComp, score1, score2;
    int scoreHuman, score3, score4;
    int difference;
    //if (debug)
    //    printBoard();

    scoreComp = window(comp);
    scoreHuman = window(human);
    difference = scoreComp-scoreHuman;
    return difference;
}

//recursive function
int minimax (int depth, char turn, int a, int B, bool maxPlaya)
{
    //setting and declaring variables
    bool stop;
    int positive;
    int i, j;
    int score, sum1, sum2;
    bool possible=false;
    int row=(int)(n-1)/2, col=(int)(n-1)/2;

    char nextTurn;
    if(turn=='B')
        nextTurn='W';
    else
        nextTurn='B';
    //terminating case
    if (depth==0)
    {
        if (maxPlaya)
            score=eval(turn);
        else
            score=eval(nextTurn);
        return score;
    }

    //Maximizing case
    else if (maxPlaya)
    {
        score=-9999999;
        stop=false;
        positive=-1;
        i=(n-1)/2, j=(n-1)/2;

        for (int m=1; m<n+1; m++)
        {
            for (int iorj=1; iorj<=2 && !stop; iorj++)
            {
                for (int k=1; k<=m && !stop; k++)
                {

                    //primary pruning - set the tile as a possible tile if and only if the tile is 6 from a placed tile
                    for(int u=-5; u<6; u++)
                    {
                        if(i+u>=0 && i+u<n && board[i+u][j]!='U')
                        {
                            possible=true;
                            break;
                        }
                        if(j+u>=0 && j+u<n && board[i][j+u]!='U')
                        {
                            possible=true;
                            break;
                        }
                        if(i+u>=0 && i+u<n && j+u>=0 && j+u<n && board[i+u][j+u]!='U')
                        {
                            possible=true;
                            break;
                        }
                        if(i+u>=0 && i+u<n && j-u>=0 && j-u<n && board[i+u][j-u]!='U')
                        {
                            possible=true;
                            break;
                        }
                    }

                    //if the tile is empty and is a possible tile (see above), place the potential tile, find the score, then return the tile to empty
                    if(board[i][j]=='U' && possible)
                    {
                        board[i][j]=turn;
                        score = (minimax(depth-1, nextTurn, a, B, !maxPlaya));
                        if (score>a)
                        {
                            a=score;
                            if (depth==startDepth)
                            {
                                row=i;
                                col=j;
                            }
                        }

                        //printf("row: %d, col: %d, depth: %d\n", row, col, depth);//debug
                        board[i][j]='U';
                        if (B<=a)
                        {
                            goto alpha;
                        }
                    }
                    if (iorj==1)
                    {
                        i+= 1*positive;
                    }
                    else
                    {
                        j+= 1*positive;
                    }
                    if(i>=n||j>=n||i<0||j<0)
                    {
                        stop=true;
                        break;
                    }
                }
            }
            positive*=(-1);
        }
alpha:
        if (depth==startDepth)
            board[row][col]='T';
        return a;
    }

    else if (!maxPlaya)
    {
        stop=false;
        positive=-1;
        i=(n-1)/2, j=(n-1)/2;
        score=999999;

        for (int m=1; m<n+1; m++)
        {
            for (int iorj=1; iorj<=2 && !stop; iorj++)
            {
                for (int k=1; k<=m && !stop; k++)
                {
                    //primary pruning - set the tile as a possible tile if and only if the tile is 6 from a placed tile
                    for(int u=-5; u<6; u++)
                    {
                        if(i+u>=0 && i+u<n && board[i+u][j]!='U')
                        {
                            possible=true;
                            break;
                        }
                        if(j+u>=0 && j+u<n && board[i][j+u]!='U')
                        {
                            possible=true;
                            break;
                        }
                        if(i+u>=0 && i+u<n && j+u>=0 && j+u<n && board[i+u][j+u]!='U')
                        {
                            possible=true;
                            break;
                        }
                        if(i+u>=0 && i+u<n && j-u>=0 && j-u<n && board[i+u][j-u]!='U')
                        {
                            possible=true;
                            break;
                        }
                    }

                    //if the tile is empty and is a possible tile (see above), place the potential tile, find the score, then return the tile to empty
                    if(board[i][j]=='U' && possible)
                    {
                        board[i][j]=turn;
                        B=fmin(minimax(depth-1, nextTurn, a, B, !maxPlaya), B);
                        board[i][j]='U';
                        if(B<=a)
                        {
                            goto beta;
                        }
                    }
                    if (iorj==1)
                    {
                        i+= 1*positive;
                    }
                    else
                    {
                        j+= 1*positive;
                    }
                    if(i>=n||j>=n||i<0||j<0)
                    {
                        stop=true;
                        break;
                    }
                }
            }
            positive*=(-1);
        }
beta:
        return B;
    }
}

//function to check if there is a winner
char checkWin()
{
    int counter=0;
    bool win=false;
    char winningColour, won='N';
    for (int i=0; i<n && !win; i++)
    {
        for (int j=0; j<n && !win; j++)
        {
            if(board[i][j]=='U')
                counter++;
        }
    }

    if (findLongest('W')>=99999)
    {
        win=true;
        winningColour='W';
    }
    if (findLongest('B')>=99999)
    {
        win=true;
        winningColour='B';
    }

    if (win)
    {
        won='Y';
        if (winningColour == 'W')
            printf("White player wins.\n");
        if (winningColour == 'B')
            printf("Black player wins.\n");
    }
    else if(counter==0)
    {
        won='D';
        printf("Draw!\n");
    }
    else
        won='N';
    return won;
}
