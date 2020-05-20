//
//  Before.h
//  Project1
//
//  Created by Son Dang on 6/26/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#ifndef Before_h
#define Before_h
#include "globals.h"

class Before
{
public:
    Before(int nRows, int nCols);
    bool keepTrack(int r, int c);
    void printWhatWasBefore() const;
    
private:
    int m_row;
    int m_col;
    char m_grid[MAXROWS][MAXCOLS];
};

#endif /* Before_h */
