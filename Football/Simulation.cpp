//
//  Simulation.cpp
//  Football
//
//  Created by Victor Cardozo on 11/11/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//
#include <cmath>

#include "Simulation.h"

using namespace std;

Simulation::Simulation(int day, int month, int year, const string& parameters, const string& league, bool research)
    : m_research(research),
      Engine(league)
{
    //Set research parameters.
    if(m_research)
    {
        setResearchParameters();
    }
    
    //Initialise date for simulations
    m_date.day = day;
    m_date.month = month;
    m_date.year = year;
    
    //Set purse to 100.
    m_purse = 100.0;
    
    //initialise engine variables.
    init(parameters);
}

void Simulation::simulate(Date limit)
{
    //Clear research containers.
    m_correct.clear();
    m_wrong.clear();
    
    //If current date before limit date.
    while(isBefore(limit))
    {
        //Find matches being played on current date.
        auto matches = m_db.getMatches(m_date);
        
        //If found matches on database
        if(!matches.empty())
        {
            //Container to hold team playing at home, probabilities calculated and betting decision.
            map<string, probabilities> bets;
            
            //Cycle through all matches found for this date in database.
            for(auto& m : matches)
            {
                //get poisson probabilities for given match.
                auto prob = probrability(m.first, m.second, m_date);
                
                //Make betting decision.
                prob.result =  makeDecision(prob);
                
                //If decided to bet, add it to map holding the bets to be made.
                if(prob.result != NONE)
                {
                    bets.insert(pair<string,probabilities>(m.first, prob));
                }
                
                //Print probabilities and betting decision.
                cout << m.first << ": " << prob.home << " | draw: " << prob.draw <<
                " | " << m.second << ": " << prob.away << "---> " << decision(prob.result) << endl;
            }
            
            //Make bets.
            makeBets(bets);
        }
        
        //Move on to next day.
        incrementDate();
    }
    
    if(m_research)
    {
        printResearch();
    }
}

void Simulation::makeBets(const map<string, probabilities>& bets)
{
    //Get odds for matches taking place in date given. Gets map with home team paired with vector containing odds + result.
    auto odds = m_db.getOdds(m_date);
    
    //For each match:
    for(auto& o : odds)
    {
        //Search to see if bet was made for home team.
        auto it = bets.find(o.first);
        int decision;
        
        //If bet was made, grab decision.
        if(it == bets.end())
        {
            continue;
        }
        
        //Calculate stake to bet. Passing probability of result chosen, its odds and decision.
        double stake = 0, probability = 0, odd = 0;
        
        switch(decision)
        {
            case HOME:
                probability = it->second.home;
                odd = o.second.home;
                break;
            case DRAW:
                probability = it->second.draw;
                odd = o.second.draw;
                break;
            case AWAY:
                probability = it->second.away;
                odd = o.second.away;
                break;
            case NONE:;
        }
        
        //Calculate stake to bet. Passing probability of result chosen, its odds and decision.
        stake = calculateStake(probability, 1/odd, it->second.result);
        
        if(m_research && m_useBookiesProbability)
        {
            probability = odd;
        }
        
        //if decision equals outcome adjust purse by looking at odds or deducting stake.
        if(it->second.result == o.second.result)
        {
            m_purse += (odd-1)*stake;
            
            if(m_research)
            {
                addResultToResearch(decision, probability, m_correct);
            }
        }
        else if(decision != 0.0)
        {
            m_purse -= stake;
            
            if(m_research)
            {
                addResultToResearch(decision, probability, m_wrong);
            }
        }
        
        cout << m_date.year << "-" << m_date.month << "-" << m_date.day << " " << o.first << ": " << o.second.home <<
        "|" << o.second.draw << "|" << o.second.away << endl << "decision/outcome: " << Engine::decision(decision) <<
        "/" << Engine::decision(o.second.result) << " stake: " << stake << " purse: " << m_purse << endl;
    }
}

bool Simulation::isBefore(Date d)
{
    if(m_date.year == d.year)
    {
        if(m_date.month == d.month)
        {
            if(m_date.day < d.day)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(m_date.month < d.month)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(m_date.year < d.year)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Simulation::incrementDate()
{
    if(m_date.day == 31)
    {
        if(m_date.month == 12)
        {
            ++m_date.year;
            m_date.month = 1;
        }
        else
        {
            ++m_date.month;
        }
        
        m_date.day = 1;
    }
    else
    {
        ++m_date.day;
    }
}

void Simulation::setResearchParameters()
{
    string gap, result, probability;
    
    cout << "Choose probability gap: ";
    getline(std::cin, gap);
    
    m_probabilityGap = stod(gap);
    
    while(probability != "b" && probability != "c")
    {
        cout << "Study bookie's or calculated probabilities? (b/c) ";
        getline(std::cin, probability);
        
        if(probability == "b")
        {
            m_useBookiesProbability = true;
        }
        else
        {
            m_useBookiesProbability = false;
        }
    }

    while(true)
    {
        cout << "Choose result to be studied: ";
        getline(std::cin, result);
        
        if(result == "home")
        {
            m_result = 1;
            break;
        }
        else if(result == "draw")
        {
            m_result = 2;
            break;
        }
        else if(result == "away")
        {
            m_result = 3;
            break;
        }
        else
        {
            cout << "Please choose 'home', 'draw' or away'." << endl;
        }
    }
}

void Simulation::printResearch()
{
    for(double i = 1; i > -1; i -= m_probabilityGap)
    {
        auto correct = m_correct.find(i);
        auto wrong = m_wrong.find(i);
        int e, a;
        
        if(correct == m_correct.end())
        {
            e = 0;
        }
        else
        {
            e = correct->second;
        }
        
        if(wrong == m_wrong.end())
        {
            a = 0;
        }
        else
        {
            a = wrong->second;
        }
        
        if(e != 0 || a != 0)
        {
            double per = (double)e/(e+a);
            
            cout << i << ": " << e << "/" << a << "(" << per*100 << "%)" << endl;
        }
    }
}

void Simulation::addResultToResearch(int decision, double odds, map<double,int>& container)
{
    double probability = 1;
    
    if(decision == m_result)
    {
        while(probability >= 0)
        {
            if(odds >= probability)
            {
                auto it = container.find(probability);
                
                if(it != container.end())
                {
                    ++it->second;
                    break;
                }
                else
                {
                    container.insert(make_pair(probability, 1));
                    break;
                }
            }
            
            probability -= m_probabilityGap;
        }
    }
}






