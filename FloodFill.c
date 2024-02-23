#include <stdio.h>

#include "API.h"

void log(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

struct cell {
    int row;
    int col;
};

struct cell goals[4] = {
    {7, 7}, {7, 8}, {8, 7}, {8, 8}
};

struct cell floodQueue[32];
int frontFQ = -1, rearFQ = -1;

// Time Complexity - Big O(1)
void insertCell(int row, int col) {
    struct cell C = {row, col};

    if ((frontFQ == 0 && rearFQ == 31) || rearFQ + 1 == frontFQ) {
        log("Flood Queue overflow");
    } else {
        if (rearFQ == -1) 
            frontFQ = 0, rearFQ = 0;
        else if (rearFQ == 31)    
            rearFQ = 0;
        else
            rearFQ++;
        floodQueue[rearFQ] = C;
    }
}

struct cell removeCell() {
    struct cell C;

    if (frontFQ == -1) {
        log("Flood Queue underflow");
    } else {
        C = floodQueue[frontFQ];
        if (frontFQ == rearFQ) 
            frontFQ = -1, rearFQ = -1;
        else if (frontFQ == 31)
            frontFQ = 0;
        else
            frontFQ++;
    }

    return C;
}

int hWalls[15][16];
int vWalls[16][15];

int maze[16][16];

void initializeDists() {
    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            maze[r][c] = -1;
        }
    }

    for (int i = 0; i < 4; i++) {
        maze[goals[i].row][goals[i].col] = 0;
    }
}

void floodfill() {
    initializeDists();

    for (int i = 0; i < 4; i++) {
        insertCell(goals[i].row, goals[i].col);
    }

    while (!(frontFQ == -1 && rearFQ == -1))
    {
        struct cell C = removeCell();
        int r = C.row, c = C.col;
        int val = maze[r][c];

        // Updating North Neighbour
        if (c > 0 && maze[r][c - 1] == -1 && vWalls[r][c - 1] != 1) {
            maze[r][c - 1] = val + 1;
            insertCell(r, c - 1);
        }

        // Updating South Neighbour
        if (c < 15 && maze[r][c + 1] == -1 && vWalls[r][c] != 1) {
            maze[r][c + 1] = val + 1;
            insertCell(r, c + 1);
        }

        // Updating West Neighbour
        if (r > 0 && maze[r - 1][c] == -1 && hWalls[r - 1][c] != 1) {
            maze[r - 1][c] = val + 1;
            insertCell(r - 1, c);
        }

        // Updating East Neighbour
        if (r < 15 && maze[r + 1][c] == -1 && hWalls[r][c] != 1) {
            maze[r + 1][c] = val + 1;
            insertCell(r + 1, c);
        }
    }   
}

int isGoal(int row, int col) {
    for (int i = 0; i < 4; i++) {
        if (goals[i].row == row && goals[i].col == col) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 16; j++) {
            hWalls[i][j] = 0;
            vWalls[j][i] = 0;
        }
    }

    int r = 15, c = 0;
    int orientation = 0;

    floodfill();

    while (!isGoal(r, c)) {
        int val = maze[r][c];
        int action = -1;
        int rf = r, cf = c;

        // North
        if (orientation == 0) {
            // Check Front (North)
            if (r > 0) {
                if (hWalls[r - 1][c] == 0) {
                    if (API_wallFront()) hWalls[r - 1][c] = 1;
                    else hWalls[r - 1][c] = -1;
                }

                if (hWalls[r - 1][c] == -1 && maze[r - 1][c] == val - 1) {
                    action = 0;
                    rf = r - 1;
                }
            }

            // Check Right (East)
            if (c < 15) {
                if (vWalls[r][c] == 0) {
                    if (API_wallRight()) vWalls[r][c] = 1;
                    else vWalls[r][c] = -1;
                }

                if (action == -1 && vWalls[r][c] == -1 && maze[r][c + 1] == val - 1) {
                    action = 1;
                    cf = c + 1;
                }
            }

            // Check Left (West)
            if (c > 0) {
                if (vWalls[r][c - 1] == 0) {
                    if (API_wallLeft()) vWalls[r][c - 1] = 1;
                    else vWalls[r][c - 1] = -1;
                }

                if (action == -1 && vWalls[r][c - 1] == -1 && maze[r][c - 1] == val - 1) {
                    action = 3;
                    cf = c - 1;
                }
            }

            // Check Backtrack (South)
            if (action == -1 && maze[r + 1][c] == val - 1) {
                action = 2;
                rf = r + 1;
            }
        }

        // East
        else if (orientation == 1) {
            // Check Front (East)
            if (c < 15) {
                if (vWalls[r][c] == 0) {
                    if (API_wallFront()) vWalls[r][c] = 1;
                    else vWalls[r][c] = -1;
                }

                if (vWalls[r][c] == -1 && maze[r][c + 1] == val - 1) {
                    action = 0;
                    cf = c + 1;
                }
            }

            // Check Right (South)
            if (r  < 15) {
                if (hWalls[r][c] == 0) {
                    if (API_wallRight()) hWalls[r][c] = 1;
                    else hWalls[r][c] = -1;
                }

                if (action == -1 && hWalls[r][c] == -1 && maze[r + 1][c] == val - 1) {
                    action = 1;
                    rf = r + 1;
                }
            }

            // Check Left (North)
            if (r > 0) {
                if (hWalls[r - 1][c] == 0) {
                    if (API_wallLeft()) hWalls[r - 1][c] = 1;
                    else hWalls[r - 1][c] = -1;
                }

                if (action == -1 && hWalls[r - 1][c] == -1 && maze[r - 1][c] == val - 1) {
                    action = 3;
                    rf = r - 1;
                }
            }

            // Check Backtrack (West)
            if (action == -1 && maze[r][c - 1] == val - 1) {
                action = 2;
                cf = c - 1;
            }
        }

        // South
        else if (orientation == 2) {
            // Check Front (South)
            if (r  < 15) {
                if (hWalls[r][c] == 0) {
                    if (API_wallFront()) hWalls[r][c] = 1;
                    else hWalls[r][c] = -1;
                }

                if (hWalls[r][c] == -1 && maze[r + 1][c] == val - 1) {
                    action = 0;
                    rf = r + 1;
                }
            }

            // Check Right (West)
            if (c > 0) {
                if (vWalls[r][c - 1] == 0) {
                    if (API_wallRight()) vWalls[r][c - 1] = 1;
                    else vWalls[r][c - 1] = -1;
                }

                if (action == -1 && vWalls[r][c - 1] == -1 && maze[r][c - 1] == val - 1) {
                    action = 1;
                    cf = c - 1;
                }
            }

            // Check Left (East)
            if (c < 15) {
                if (vWalls[r][c] == 0) {
                    if (API_wallLeft()) vWalls[r][c] = 1;
                    else vWalls[r][c] = -1;
                }

                if (action == -1 && vWalls[r][c] == -1 && maze[r][c + 1] == val - 1) {
                    action = 3;
                    cf = c + 1;
                }
            }

            // Check Backtrack (North)
            if (action == -1 && maze[r - 1][c] == val - 1) {
                action = 2;
                rf = r - 1;
            }
        }

        // West
        else {
            // Check Front (West)
            if (c > 0) {
                if (vWalls[r][c - 1] == 0) {
                    if (API_wallFront()) vWalls[r][c - 1] = 1;
                    else vWalls[r][c - 1] = -1;
                }

                if (vWalls[r][c - 1] == -1 && maze[r][c - 1] == val - 1) {
                    action = 0;
                    cf = c - 1;
                }
            }

            // Check Right (North)
            if (r > 0) {
                if (hWalls[r - 1][c] == 0) {
                    if (API_wallRight()) hWalls[r - 1][c] = 1;
                    else hWalls[r - 1][c] = -1;
                }

                if (action == -1 && hWalls[r - 1][c] == -1 && maze[r - 1][c] == val - 1) {
                    action = 1;
                    rf = r - 1;
                }
            }

            // Check Left (South)
            if (r  < 15) {
                if (hWalls[r][c] == 0) {
                    if (API_wallLeft()) hWalls[r][c] = 1;
                    else hWalls[r][c] = -1;
                }

                if (action == -1 && hWalls[r][c] == -1 && maze[r + 1][c] == val - 1) {
                    action = 3;
                    rf = r + 1;
                }
            }
    
            // Check Backtrack (East)
            if (action == -1 && maze[r][c + 1] == val - 1) {
                action = 2;
                cf = c + 1;
            }
        }

        if (action == -1) {
            floodfill();
        } else {
            if (action == 1) {
                API_turnRight();
                orientation += 1;
                if (orientation > 3) orientation = 0;
            } else if (action == 2) {
                API_turnRight(); API_turnRight();
                if (orientation < 2) orientation += 2;
                else orientation -= 2;
            } else if (action == 3) {
                API_turnLeft();
                orientation -= 1;
                if (orientation < 0) orientation = 3;
            }

            API_moveForward();
        }

        r = rf, c = cf;
    }
}
