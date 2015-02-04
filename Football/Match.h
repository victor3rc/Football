//
//  Match.h
//  Football
//
//  Created by Victor Cardozo on 03/02/2015.
//  Copyright (c) 2015 Victor Cardozo. All rights reserved.
//

#ifndef __Football__Match__
#define __Football__Match__

#include <iostream>
#include <stdio.h>

//enum used to indicate match result.
enum Result { HOME, DRAW, AWAY, NONE };

//Struct used to hold probabilities of possible results.
struct probabilities
{
    double home;
    double draw;
    double away;
    
    Result result;
};

class Match
{
public:
    Match(const std::string& home, const std::string& away, int hg, int ag);
    
    //Returns enum indicating result.
    Result result();
    
private:
    const std::string m_home, m_away;
    const int m_goalsHome, m_goalsAway;
};

#endif /* defined(__Football__Match__) */
