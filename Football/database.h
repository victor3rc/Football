//
//  database.h
//  Football
//
//  Created by Victor Cardozo on 12/09/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//

#ifndef __Football__database__
#define __Football__database__

#include <iostream>
#include <vector>
#include <map>
#include <mysql.h>

#include "Date.h"

class Database
{
public:
    Database();
    ~Database();
    
    void insertResults(const std::vector<std::vector<std::string>>& rows);
    void insertBottomAverage(const std::pair<double, double>& av, int year);
    double average(bool scored, bool home, const std::string& team,
                   const std::string& start, const std::string& end);
    
    double lastMatchesAverage(bool scored, bool home, const std::string& team, int matches, struct Date d);
    double lastMatchesAverage(const std::string& team, struct Date d, int matches);
    
    std::vector<std::string> bottomTeams(int year);
    std::pair<double, double> bottomAverage(int year);
    
    std::vector<std::pair<std::string, std::string>> getMatches(const struct Date& d);
    std::map<std::string, std::vector<double>> getOdds(const struct Date& d);
    std::map<std::string, std::pair<int, int>> getResults(const struct Date& d);
    
    std::map<double, double> getStakes(const std::string& league, const std::string& res);
    std::vector<std::string> parameters(const std::string& name);
    std::vector<double> weights(const std::string& name);
    
    std::pair<int,int> getResult(const std::string& home, const struct Date& d);
    
private:
    MYSQL *m_connection, m_mysql;

    std::string teamsString(const std::vector<std::string>& teams);
    double bottomMatchesAverage(int year, int matches);
    
};

#endif /* defined(__Football__database__) */
