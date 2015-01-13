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
    Database(const std::string& league);
    ~Database();
    
    //Class used to insert results into database.
    //'table' is table to insert values into.
    //'rows' contains rows to be inserted.
    void insertResults(const std::string& table, const std::vector<std::vector<std::string>>& rows);
    
    //Insert values into BottomAverage table in database.
    //It's the table which holds average of goals scored at home and away by bottom 10 teams.
    //'table' is table to insert values into.
    //'averages' is pair of averages (home and away).
    //'year' is year of averages to be inserted.
    void insertBottomAverage(const std::string& table, const std::pair<double, double>& averages, int year);
    
    //Calculates average of goals scored or conceded by a team, at home or away, during a period of time.
    //'scored' indicates whether to find goals scored (true) or conceded (false).
    //'home' indicates whether to look at games played at home (true) or away (false).
    //'team' is team to calculate average for.
    //'table' is table where results are stored.
    //'start' is date to calculate average from.
    //'end' is date to calculate average until.
    //returns average of goals scored/conceded in period specified.
    double goalsAverage(bool scored, bool home, const std::string& team, const std::string& table,
                        const std::string& start, const std::string& end);
    
    //Calculates average of goals scored/conceded at home/away in number of matches specified, from date given.
    //'scored' indicates if requesting goals scored (true) or conceded (false).
    //'home' indicates if requested goals at home (true) or away (false).
    //'team' is team average calculated regards.
    //'matches' is number of matches to calculate average for.
    //'date' is date to calculate average from.
    //returns average found.
    double lastMatchesAverage(bool scored, bool home, const std::string& team, int matches, struct Date date);
    
    //Calculates average of goals scored both at home/away in number of matches specified, from date given, in the last year only.
    //'team' is team average calculated regards.
    //'date' is date to calculate average from.
    //'matches' is number of matches to calculate average for.
    //returns average found.
    double lastMatchesAverage(const std::string& team, struct Date date, int matches);
    
    //Query for bottom 10 teams in year specified.
    //'year' is year to query for.
    //returns a vector containing bottom teams in strings.
    std::vector<std::string> bottomTeams(int year);
    
    
    std::pair<double, double> bottomAverage(int year);
    
    std::vector<std::pair<std::string, std::string>> getMatches(const struct Date& d);
    std::map<std::string, std::vector<double>> getOdds(const struct Date& d);
    std::map<std::string, std::pair<int, int>> getResults(const struct Date& d);
    
    std::map<double, double> getStakes(const std::string& res);
    std::vector<std::string> parameters(const std::string& name);
    std::vector<double> weights(const std::string& name);
    
    std::pair<int,int> getResult(const std::string& home, const struct Date& d);
    
private:
    MYSQL *m_connection, m_mysql;
    std::string m_league;

    std::string teamsString(const std::vector<std::string>& teams);
    double bottomMatchesAverage(int year, int matches);
    std::string locationColumn(bool home);
    std::string goalsColumn(bool scored, bool home);
    
};

#endif /* defined(__Football__database__) */
