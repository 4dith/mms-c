#include <stdio.h>

int maze[3][3] = {
    {1, 2, 3}, 
    {-4, -5, -6},
    {-1, 1, 0}
};

int main() {
    FILE *pF = fopen("hWalls.csv", "w");
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(pF, "%d, ", maze[i][j]);
        }
        fprintf(pF, "\n");
    }

    fclose(pF);
    return 0;
}
