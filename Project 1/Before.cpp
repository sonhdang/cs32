//
//  Before.cpp
//  Project1
//
//  Created by Son Dang on 6/26/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#include <stdio.h>
#include "Before.h"
#include <iostream>
using namespace std;

Before::Before(int nRows, int nCols)
{
    m_row = nRows;
    m_col = nCols;
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            m_grid[i][j] = '.';
        }
    }
}

bool Before::keepTrack(int r, int c)
{
    if (r <= 0  ||  c <= 0  ||  r > m_row  ||  r > m_col)
        return false;
    r--;
    c--;
    if(m_grid[r][c] == 'Z')
        return true;
    else if(m_grid[r][c] == '.')
    {
        m_grid[r][c] = 'A';
    }
    else
        m_grid[r][c]++;
    return true;
}

void Before::printWhatWasBefore() const
{
    clearScreen();
    for (int i = 0; i < m_row; i++)
    {
        for (int j = 0; j < m_col; j++)
        {
            cout << m_grid[i][j];
        }
        cout << endl;
    }
}
