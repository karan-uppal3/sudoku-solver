#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool empty_cell(double *arr, int *x, int *y)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (arr[i * 9 + j] == 0)
            {
                *x = i;
                *y = j;
                return true;
            }
        }
    }
    return false;
}

int findc(int c)
{
    switch (c)
    {
    case 0:
    case 1:
    case 2:
        return 1;

    case 3:
    case 4:
    case 5:
        return 4;

    case 6:
    case 7:
    case 8:
        return 7;
    }
}

bool isValid(double *arr, int x, int y, int num)
{
    bool check = true;
    int i, j;

    for (i = 0; i < 9; i++)
    {
        if (i == x)
            continue;

        if ((int)arr[i * 9 + y] == num && check == true)
        {
            check = false;
            return check;
        }
    }

    for (int j = 0; j < 9; j++)
    {
        if (j == y)
            continue;

        if ((int)arr[x * 9 + j] == num && check == true)
        {
            check = false;
            return check;
        }
    }

    i = findc(x);
    j = findc(y);

    for (int k = i - 1; k < i + 2; k++)
    {
        for (int l = j - 1; l < j + 2; l++)
        {
            if (k == x && l == y)
                continue;

            if ((int)arr[k * 9 + l] == num && check == true)
            {
                check = false;
                return check;
            }
        }
    }

    return check;
}

void show(double *arr)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", (int)arr[i * 9 + j]);
        }
        printf("\n");
    }
}

bool solver(double *arr)
{
    int x, y;

    if (empty_cell(arr, &x, &y) == false)
        return true;

    for (int k = 1; k <= 9; k++)
    {
        if (isValid(arr, x, y, k) == true)
        {
            arr[x * 9 + y] = (double)k;

            if (solver(arr) == true)
                return true;
            else
                arr[x * 9 + y] = 0.0;
        }
    }

    return false;
}

double *solve(double *arr)
{
    if (solver(arr) == true)
        return arr;

    return NULL;
}