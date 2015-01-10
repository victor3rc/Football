//
//  Simulation.h
//  Football
//
//  Created by Victor Cardozo on 11/11/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//

#ifndef __Football__Simulation__
#define __Football__Simulation__

#include <iostream>
#include <vector>
#include <map>

#include "Engine.h"
#include "Date.h"

class Simulation : public Engine
{
public:
    //Constructor
    //'day' is day to start simulation.
    //'month' is month to start simulation.
    //'year' is year to start simulation.
    //'league' is league to simulate.
    //'parameters' are the name of the parameters to be used in sumulation.
    //'research' indicates if research is to be carried out or not.
    Simulation(int day, int month, int year, const std::string& league, const std::string& parameters, bool research);
    
    //Function to run simulation.
    //'Date' is the end date of simulation.
    void simulate(Date limit);
    
private:
    //Function used to simulate taking bets.
    //'bets' is the container indicating which bets are to be made.
    //It contains the home team as a key and a vector of the probabilities calculated [0-2] and the decision [3].
    void makeBets(const std::map<std::string, std::vector<double>>& bets);
    
    //Function used to check if date given is before m_date.
    //Returns 'true' if it is and 'false' if it isn't.
    bool isBefore(Date d);
    
    //Function used to increment m_date.
    void incrementDate();
    
    //Function used to ask user for research parameters.
    void setResearchParameters();
    
    //Function used to print research results.
    void printResearch();
    
    //Function used to account for a result in research.
    //'decision' is the betting decision made( '1' home,'2' draw or '3' away)
    //'odds' are the odds relevant to the decision made.
    //'container' indicates which container the research is relevant to.
    void addResultToResearch(int decision, double odds, std::map<double,int>& container);
    
    //Date object used for simulations.
    Date m_date;
    
    //Indicates whether to carry out research and whether to use bookies probability or the calculated one in research.
    bool m_research, m_useBookiesProbability;
    
    //Containers used in research for correct bets and wrong ones.
    std::map<double,int> m_correct, m_wrong;
    
    //Probability gap to study in research.
    double m_probabilityGap;
    
    //Indicates which result is to be studied in research. '1' home, '2' draw or '3' away'
    int m_result;
};

#endif /* defined(__Football__Simulation__) */
