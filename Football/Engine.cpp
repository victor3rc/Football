//
//  Engine.cpp
//  Football
//
//  Created by Victor Cardozo on 06/01/2015.
//  Copyright (c) 2015 Victor Cardozo. All rights reserved.
//
#include <string>
#include <iostream>
#include <cmath>

#include "Engine.h"

using namespace std;

void Engine::init(const string& league, const string& parameters)
{
    //Get stakes in database.
    m_homeStake = m_db.getStakes(league, "home");
    m_awayStake = m_db.getStakes(league, "away");
    m_drawStake = m_db.getStakes(league, "draw");
    
    //Get weights in database.
    m_weights = m_db.weights(parameters);
    
    //Grab parameters in database and set them.
    auto params = m_db.parameters(parameters);
    this->setParameters(params);
}

int Engine::makeDecision(const vector<double>& prob)
{
    //Difference between home win probability and away win probability.
    double difference = prob[0] - prob[2];
    
    if(difference > m_homeThresh)
    {
        return 1;
    }
    else if(abs(difference) <= m_drawThresh/2 && prob[1] > m_drawThresh)
    {
        return 2;
    }
    else if(difference < m_awayThresh)
    {
        return 3;
    }
    
    return 0;
}

double Engine::calculateStake(const double probability, const double odds, int decision)
{
    double bookieOdds = 1/odds;
    
    //If bookie odds aren't high enough, void bet.
    if(bookieOdds > m_maxBookieOdds)
    {
        return 0.0;
    }
    else
    {
        double diff = probability-bookieOdds;
        
        //Use stake limits relevant to betting decision given.
        if(decision == 1)
        {
            return findStake(m_homeStake, diff);
        }
        else if(decision == 3)
        {
            return findStake(m_awayStake, diff);
        }
        else
        {
            return findStake(m_drawStake, diff);
        }
    }
}

vector<double> Engine::getProbrabilities(const std::string& home, const std::string& away, const Date& date)
{
    //Get goal average for different periods of time.
    double homeAverages[4], awayAverages[4];
    
    //"scored?, home?, team, matches" passed as arguments.
    homeAverages[0] = m_db.lastMatchesAverage(true, true, home, m_local, date);
    homeAverages[1] = m_db.lastMatchesAverage(home, date, m_overall);
    homeAverages[2] = m_db.lastMatchesAverage(true, false, home, m_local, date);
    homeAverages[3] = m_db.lastMatchesAverage(false, false, away, m_local, date);
    
    awayAverages[0] = m_db.lastMatchesAverage(true, false, away, m_local, date);
    awayAverages[1] = m_db.lastMatchesAverage(away, date, m_overall);
    awayAverages[2] = m_db.lastMatchesAverage(true, true, away, m_local, date);
    awayAverages[3] = m_db.lastMatchesAverage(false, true, home, m_local, date);
    
    double homeAverage = (homeAverages[0] * m_weights[0]) + (homeAverages[1] * m_weights[1]) +
    (homeAverages[2] * m_weights[2]) + (homeAverages[3] * m_weights[3]);
    
    double awayAverage = (awayAverages[0] * m_weights[0]) + (awayAverages[1] * m_weights[1]) +
    (awayAverages[2] * m_weights[2]) + (awayAverages[3] * m_weights[3]);
    
    //cout << "HOME (H): " << homeAverages[0] << " (All): " << homeAverages[1] << " (A): " << homeAverages[2] << endl;
    //cout << "AWAY (A): " << awayAverages[0] << " (All): " << awayAverages[1] << " (H): " << awayAverages[2] << endl;
    
    return calculateProbabilities(homeAverage, awayAverage);
}

double Engine::findStake(const std::map<double, double> &stakes, double diff)
{
    //Cycle through database stakes.
    for(auto rit = stakes.rbegin(); rit != stakes.rend(); ++rit)
    {
        //If difference is bigger, choose stake for this difference.
        if(diff > rit->first)
        {
            return m_purse*rit->second;
        }
    }
    
    //Use stake for smallest difference.
    return m_purse*stakes.begin()->second;
}

void Engine::setParameters(const std::vector<std::string> &parameters)
{
    try
    {
        //Set variables with parameters found in database.
        m_homeThresh = stod(parameters[1]);
        m_drawThresh = stod(parameters[2]);
        m_awayThresh = stod(parameters[3]);
        
        m_maxBookieOdds = stod(parameters[4]);
        
        m_local = stoi(parameters[5]);
        m_overall = stoi(parameters[6]);
    }
    catch (exception& ex)
    {
        cout << "[+]Engine::setParameters exception: " << ex.what() << endl;
    }
}

vector<double> Engine::calculateProbabilities(double homeAverage, double awayAverage)
{
    //[0] home win, [1] draw, [2] away win
    vector<double> output = {0.0, 0.0, 0.0};
    
    for (int i = 0; i < 10; ++i)
    {
        for (int x = 0; x < 10; ++x)
        {
            double probability;
            
            probability = poisson(homeAverage, i) * poisson(awayAverage, x);
            
            if(i > x)
            {
                output[0] += probability;
            }
            else if (i == x)
            {
                output[1] += probability;
            }
            else if (i < x)
            {
                output[2] += probability;
            }
        }
    }
    
    return output;
}

double Engine::poisson(double average, double rate)
{
    double x = pow(average, rate);
    
    if(rate == 0)
    {
        rate = 1;
    }
    else
    {
        for(int i = rate-1; i > 0; --i)
        {
            rate *= i;
        }
    }
    
    double z = x/rate;
    
    double e = pow(2.71828, -average);
    
    return e*z;
}