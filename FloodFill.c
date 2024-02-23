#include <stdio.h>
#include <string.h>
#include "API.h"

#define maxDigits 3
#define size 16

// Have to reduce this
#define queueCapacity 256

void log(char *text)
{
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

char numAsString[maxDigits];
char *numToString(int x)
{
    // Works only for whole numbers
    int i = maxDigits - 1;

    while (i >= 0)
    {
        if (x > 0)
        {
            numAsString[i] = x % 10 + 48;
            x /= 10;
        }
        else
            numAsString[i] = '0';
        i--;
    }

    return numAsString;
}

struct cell
{
    int row;
    int col;
};

struct cell floodQueue[queueCapacity];
int frontFQ = -1, rearFQ = -1;

// Time Complexity - Big O(1)
void insertCell(int row, int col)
{
    struct cell C = {row, col};

    if ((frontFQ == 0 && rearFQ == queueCapacity - 1) || rearFQ + 1 == frontFQ)
    {
        log("Flood Queue overflow");
    }
    else
    {
        if (rearFQ == -1)
            frontFQ = 0, rearFQ = 0;
        else if (rearFQ == queueCapacity - 1)
            rearFQ = 0;
        else
            rearFQ++;
        floodQueue[rearFQ] = C;
    }
}

struct cell removeCell()
{
    struct cell C;

    if (frontFQ == -1)
    {
        log("Flood Queue underflow");
    }
    else
    {
        C = floodQueue[frontFQ];
        if (frontFQ == rearFQ)
            frontFQ = -1, rearFQ = -1;
        else if (frontFQ == queueCapacity - 1)
            frontFQ = 0;
        else
            frontFQ++;
    }

    return C;
}

int hWalls[size - 1][size];
int vWalls[size][size - 1];

int maze[size][size];

void initializeDists()
{
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            maze[r][c] = -1;
        }
    }

    if (size % 2 == 0)
    {
        maze[size / 2 - 1][size / 2 - 1] = 0;
        maze[size / 2][size / 2 - 1] = 0;
        maze[size / 2 - 1][size / 2] = 0;
        maze[size / 2][size / 2] = 0;
    }
    else
    {
        maze[size / 2][size / 2] = 0;
    }
}

void floodfill()
{
    initializeDists();

    if (size % 2 == 0)
    {
        insertCell(size / 2 - 1, size / 2 - 1);
        insertCell(size / 2 - 1, size / 2);
        insertCell(size / 2, size / 2 - 1);
        insertCell(size / 2, size / 2);
    }
    else
    {
        insertCell(size / 2, size / 2);
    }

    while (!(frontFQ == -1 && rearFQ == -1))
    {
        struct cell C = removeCell();
        int r = C.row, c = C.col;
        int val = maze[r][c];

        API_setText(c, 15 - r, numToString(val));

        // Updating North Neighbour
        if (c > 0 && maze[r][c - 1] == -1 && vWalls[r][c - 1] != 1)
        {
            maze[r][c - 1] = val + 1;
            insertCell(r, c - 1);
        }

        // Updating South Neighbour
        if (c < 15 && maze[r][c + 1] == -1 && vWalls[r][c] != 1)
        {
            maze[r][c + 1] = val + 1;
            insertCell(r, c + 1);
        }

        // Updating West Neighbour
        if (r > 0 && maze[r - 1][c] == -1 && hWalls[r - 1][c] != 1)
        {
            maze[r - 1][c] = val + 1;
            insertCell(r - 1, c);
        }

        // Updating East Neighbour
        if (r < 15 && maze[r + 1][c] == -1 && hWalls[r][c] != 1)
        {
            maze[r + 1][c] = val + 1;
            insertCell(r + 1, c);
        }
    }
}

int isGoal(int row, int col)
{
    if (size % 2 == 0)
    {
        return (row == size / 2 || row == size / 2 - 1) && (col == size / 2 || col == size / 2 - 1);
    }
    else
    {
        return row == size / 2 && col == size / 2;
    }
}

int cellExists(int r, int c, int dir)
{
    switch (dir)
    {
    case 0:
        return r > 0;

    case 1:
        return c < size - 1;

    case 2:
        return r < size - 1;

    case 3:
        return c > 0;
    }
}

int getCellVal(int r, int c, int dir)
{
    switch (dir)
    {
    case 0:
        return maze[r - 1][c];

    case 1:
        return maze[r][c + 1];

    case 2:
        return maze[r + 1][c];

    case 3:
        return maze[r][c - 1];
    }
}

int getWall(int r, int c, int dir)
{
    switch (dir)
    {
    case 0:
        return hWalls[r - 1][c];

    case 1:
        return vWalls[r][c];

    case 2:
        return hWalls[r][c];

    case 3:
        return vWalls[r][c - 1];
    }
}

void setWall(int r, int c, int dir, int val)
{
    switch (dir)
    {
    case 0:
        hWalls[r - 1][c] = val;
        break;

    case 1:
        vWalls[r][c] = val;
        break;

    case 2:
        hWalls[r][c] = val;
        break;

    case 3:
        vWalls[r][c - 1] = val;
        break;
    }
}

int wallPresent(int side)
{
    switch (side)
    {
    case 0:
        return API_wallFront();

    case 1:
        return API_wallRight();

    case 3:
        return API_wallLeft();
    }
}

void updateFinalCellCoordinates(int r, int c, int *rfPtr, int *cfPtr, int dir)
{
    switch (dir)
    {
    case 0:
        *rfPtr = r - 1;
        break;

    case 1:
        *cfPtr = c + 1;
        break;

    case 2:
        *rfPtr = r + 1;
        break;

    case 3:
        *cfPtr = c - 1;
        break;
    }
}

char directionAsLetter(int dir) {
    switch (dir)
    {
    case 0:
        return 'n';
        break;
    
    case 1:
        return 'e';
        break;
    
    case 2:
        return 's';
        break;
    
    case 3:
        return 'w';
        break;
    }
}

void checkWall(int r, int c, int val, int orient, int side, int *actionPtr, int *rfPtr, int *cfPtr)
{
    int dir = (orient + side) % 4;

    if (side == 2)
    {
        if (*actionPtr == -1 && getCellVal(r, c, dir) == val - 1)
        {
            *actionPtr = side;
            updateFinalCellCoordinates(r, c, rfPtr, cfPtr, dir);
        }
    }
    else
    {
        if (cellExists(r, c, dir))
        {
            if (getWall(r, c, dir) == 0)
            {
                if (wallPresent(side)) {
                    setWall(r, c, dir, 1);
                    API_setWall(c, 15 - r, directionAsLetter(dir));
                } 
                else
                    setWall(r, c, dir, -1);
            }

            if (*actionPtr == -1 && getWall(r, c, dir) == -1 && getCellVal(r, c, dir) == val - 1)
            {
                *actionPtr = side;
                updateFinalCellCoordinates(r, c, rfPtr, cfPtr, dir);
            }
        }
    }
}

void performTurn(int action)
{
    if (action == 1)
        API_turnRight();
    else if (action == 2)
    {
        API_turnRight();
        API_turnRight();
    }
    else if (action == 3)
        API_turnLeft();
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size; j++)
        {
            hWalls[i][j] = 0;
            vWalls[j][i] = 0;
        }
    }

    int r = 15, c = 0;
    int orientation = 0;

    floodfill();

    while (!isGoal(r, c))
    {
        int val = maze[r][c];
        int action = -1;
        int rf = r, cf = c;

        checkWall(r, c, val, orientation, 0, &action, &rf, &cf);
        checkWall(r, c, val, orientation, 1, &action, &rf, &cf);
        checkWall(r, c, val, orientation, 3, &action, &rf, &cf);
        checkWall(r, c, val, orientation, 2, &action, &rf, &cf);

        if (action == -1)
        {
            floodfill();
        }
        else
        {
            performTurn(action);
            API_moveForward();
            orientation = (orientation + action) % 4;
        }

        r = rf, c = cf;
    }
}
