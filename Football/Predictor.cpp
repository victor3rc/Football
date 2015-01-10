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
{
    //Initialise engine variables.
    init(league, parameters);
}

void Predictor::setPurse(double val)
{
    m_purse = val;
}

void Predictor::predict(const std::string &home, const std::string &away,
                        const vector<double>& odds)
{
    //Get current date
    Date date = currentDateTime();
    
    //Get probabilities
    auto prob = getProbrabilities(home, away, date);
    
    //Print probabilities calculated
    cout << home << ": " << prob[0] << " | draw: " << prob[1] << " | " << away << ": " << prob[2] << endl;
    
    //Make betting decision based on probabilities.
    int dec = makeDecision(prob);
    
    //Print decision
    cout << "Decision: " << decision(dec) << endl;
    
    //If decided to bet
    if(dec != 0)
    {
        //Calculate stake to bet. Passing probability of result chosen, its odds and decision.
        double stake = calculateStake(prob[dec-1], odds[dec-1], dec);
        
        //Print probabilities in question (calculated and by bookies)
        cout << "Bookies: " << 1/odds[dec-1] << " | Calculated: " << prob[dec-1] << endl;
        
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

const std::string Predictor::decision(int d)
{
    if(d == 1)
    {
        return "Home";
    }
    else if(d == 2)
    {
        return "Draw";
    }
    else if(d == 3)
    {
        return "Away";
    }
    else
    {
        return "No bet";
    }
}










    
    
    
    
    
    
    
    



