//
//  Engine.h
//  Football
//
//  Created by Victor Cardozo on 06/01/2015.
//  Copyright (c) 2015 Victor Cardozo. All rights reserved.
//

#ifndef __Football__Engine__
#define __Football__Engine__

#include <stdio.h>
#include <vector>
#include <map>

#include "database.h"
#include "Date.h"

class Engine
{
public:
    Engine(const std::string& league);
    
    //Initialise parameters.
    //'parameters' parameters name to load.
    void init(const std::string& parameters);
    
    //Function to decide bet to be made.
    //'prob' calculated home probability[0], draw probability[1] and away probability[2] in vector.
    int makeDecision(const std::vector<double>& prob);
    
    //Calculate stake based on betting decision given.
    //'probability' is probability calculated for bet chosen.
    //'odds' are the odds given by the bookies.
    //'decision' is the bet chosen: '1' for home, '2' for a draw and '3' for away.
    //Returns bet stake.
    double calculateStake(const double probability, const double odds, int decision);
   
    //Calculates probabilities of home win, draw and away win, using poisson distribution.
    //'home' is team playing at home.
    //'away' is team playing away.
    //'date' is date game is taking place.
    //'local' is number of games played locally to be used to calculate average of goals.
    //'overall' is number of games played everywhere to be used to calculate average of goals.
    std::vector<double> getProbrabilities(const std::string& home, const std::string& away,
                                          const struct Date& date);
    
protected:
    //Variables to hold parameters found in database.
    double m_purse, m_homeThresh, m_awayThresh, m_drawThresh, m_maxBookieOdds;
    int m_local, m_overall;
    std::vector<double> m_weights;
    
    //Stakes in database.
    std::map<double,double> m_homeStake, m_drawStake, m_awayStake;
    
    //Object used to access database.
    Database m_db;
    
private:
    //Find stake according to limits found in database.
    //'stakes' is map of a limit and stake to use for that limit.
    //The limit is calculated from difference between bookies and algorithm's probability for a result.
    //'diff' is the current difference between bookies and algorithm's probability for a result.
    //Returns bet stake.
    double findStake(const std::map<double,double>& stakes, double diff);

    //Set parameters found in database.
    //'parameters' is vector containing all parameters in database.
    void setParameters(const std::vector<std::string>& parameters);
    
    //Calculate probabilities using poisson distribution, for 0 to 10 goals happening.
    //'homeAverage' average of goals scored by home team.
    //'awayAverage' average of goals scored by away team.
    //Returns vector with probabilities. [0] home win, [1] draw, [2] away win.
    std::vector<double> calculateProbabilities(double homeAverage, double awayAverage);
    
    //Calculate the probabilities of an event happening using the poisson distribution.
    //'average' is average of goals.
    //'goals' is the number of goals happening.
    //returns probability of the given number of goals happening.
    double poisson(double average, double goals);
};

#endif /* defined(__Football__Engine__) */
