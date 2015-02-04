//
//  Predictor.cpp
//  Football
//
//  Created by Victor Cardozo on 02/12/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//
#include <cmath>

#include "Predictor.h"
#include "Date.h"

using namespace std;

Predictor::Predictor(const string& league, const string& parameters)
    : Engine(league)
{
    //Initialise engine variables.
    init(parameters);
}

void Predictor::setPurse(double val)
{
    m_purse = val;
}

void Predictor::predict(const std::string &home, const std::string &away,
                        const probabilities& odds)
{
    //Get current date
    Date date = currentDateTime();
    
    //Get probabilities
    auto prob = probrability(home, away, date);
    
    //Print probabilities calculated
    cout << home << ": " << prob.home << " | draw: " << prob.draw << " | " << away << ": " << prob.away << endl;
    
    //Make betting decision based on probabilities.
    auto dec = makeDecision(prob);
    
    //Print decision
    cout << "Decision: " << decision(dec) << endl;
    
    //If decided to bet
    if(dec != NONE)
    {
        double stake = 0, probability = 0, odd = 0;
        
        switch(dec)
        {
            case HOME:
                probability = prob.home;
                odd = 1/odds.home;
                break;
            case DRAW:
                probability = prob.draw;
                odd = 1/odds.draw;
                break;
            case AWAY:
                probability = prob.away;
                odd = 1/odds.away;
                break;
            case NONE:;
        }
        
        //Calculate stake to bet. Passing probability of result chosen, its odds and decision.
        stake = calculateStake(probability, odd, dec);
        
        //Print probabilities in question (calculated and by bookies)
        cout << "Bookies: " << odd << " | Calculated: " << probability << endl;
        
        //Deduct from purse.
        m_purse -= stake;
        
        //Print stake and purse
        cout << "Stake: " << stake << " | Purse: " << m_purse << endl;
    }
}

const Date Predictor::currentDateTime()
{
    Date date;
    
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    
    string d = buf;
    
    date.year = std::stoi(d.substr(0, 4));
    date.month = std::stoi(d.substr(5, 2));
    date.day = std::stoi(d.substr(8, 2));
    
    return date;
}










    
    
    
    
    
    
    
    



