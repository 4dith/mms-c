#include <stdio.h>
#include "API.h"
#include <string.h>

#define maxDigits 3

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

void logWalls() {
    int front = API_wallFront();
    int left = API_wallLeft();
    int right = API_wallRight();
    if (front) {
        if (left) {
            if (right) log("Front, left and right"); else log("Front and left");
        } else {
            if (right) log("Front and right"); else log("Front");
        }
    } else {
        if (left) {
            if (right) log("Left and right"); else log("Left");
        } else {
            if (right) log("Right"); else log("No walls");
        }
    }
}

int main() {
    logWalls();
    API_moveForwardHalf();
    API_turnRight45();
    logWalls();
    API_moveForward();
    API_moveForward();
    API_moveForward();
    logWalls();
}