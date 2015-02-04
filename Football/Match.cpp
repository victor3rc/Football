//
//  Match.cpp
//  Football
//
//  Created by Victor Cardozo on 03/02/2015.
//  Copyright (c) 2015 Victor Cardozo. All rights reserved.
//

#include "Match.h"

Result Match::result()
{
    if(m_goalsHome > m_goalsAway)
    {
        return Result::HOME;
    }
    else if(m_goalsHome > m_goalsAway)
    {
        return Result::AWAY;
    }
    else
    {
        return Result::DRAW;
    }
}