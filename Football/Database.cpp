//
//  database.cpp
//  Football
//
//  Created by Victor Cardozo on 12/09/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//

#include "database.h"

#include <mysql.h>

using namespace std;

Database::Database(const string& league)
    : m_league(league)
{
	mysql_init(&m_mysql);
    
    m_connection = mysql_real_connect(&m_mysql,"localhost","root","root","Football",0,0,0);
    
    if (m_connection == NULL)
	{
		cout << mysql_error(&m_mysql) << endl;
		return;
	}
}

Database::~Database()
{
    mysql_close(m_connection);
}

void Database::insertResults(const string& table, const vector<vector<string>>& rows)
{
    int state;
	
    for(auto& row : rows)
    {
        string query = "insert into " + table + " values(";
        
        //Add values to query.
        for(auto& field : row)
        {
            query += "'" + field + "',";
        }
        
        //Replace last comma.
        query.replace(query.end()-1, query.end(), ")");
        
        state = mysql_query(m_connection, query.c_str());
	
        if (state !=0)
        {
            cout << mysql_error(m_connection) << endl;
            return;
        }
    }
}

void Database::insertBottomAverage(const string& table, const pair<double, double>& averages, int year)
{
    int state;
	
    string query = "insert into " + table + " values('" + to_string(year) + "', '"
                    + to_string(averages.first) + "', '" + to_string(averages.second) + "')";
        
    state = mysql_query(m_connection, query.c_str());
        
    if (state !=0)
    {
        cout << mysql_error(m_connection) << endl;
        return;
    }
}

double Database::goalsAverage(bool scored, bool home, const string& team,
                         const string& table, const string& start, const string& end)
{
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    //Get home/away and goals scored/conceded info.
    string location = locationColumn(home);
    string goals = goalsColumn(scored, home);
    
    string query = "select " + goals + " from " + table + " where " + location + " = '" + team +
            "' and date > '" + start + "' and date < '" + end + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
		return 0;
	}
	
	result = mysql_store_result(m_connection);

    int games = 0, netted = 0;
    
    //If team not present in dates asked, return -1.
    if ( ( row=mysql_fetch_row(result)) == NULL )
    {
        return -1.0;
    }
    
	while ( ( row=mysql_fetch_row(result)) != NULL )
	{
        goals += stoi(row[0]);
		++games;
	}
	
	mysql_free_result(result);
    
    return (double)netted/games;
}

double Database::lastMatchesAverage(bool scored, bool home, const string& team, int matches, Date date)
{
    double output;
    int state;
    
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    //Get home/away and goals scored/conceded info.
    string location = locationColumn(home);
    string goals = goalsColumn(scored, home);
    
    string query = "select " + goals + " from " + m_league + " where " + location + " = '" +
    team + "' and date < '" + to_string(date.year) + "-" + to_string(date.month) +
    "-" + to_string(date.day) + "' order by date desc limit " + to_string(matches);
    
    //Perform query
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
		return 0;
	}
    else
    {
        //Grab result
        result = mysql_store_result(m_connection);

        int games = 0, netted = 0;

        //Sum goals and add up games.
        while ( ( row=mysql_fetch_row(result)) != NULL )
        {
            netted += stoi(row[0]);
            ++games;
        }

        //If in the last year less games played then what was asked. (i.e. team recently promoted)
        if(games < matches)
        {
            //Grab average of goals by equivalent teams in the last season.
            double remaining = bottomMatchesAverage(date.year-1, matches-games);
            
            output = (double)(netted + remaining)/matches;
        }
        else
        {
            output = (double)netted/games;
        }

        mysql_free_result(result);

        return output;
    }
}

double Database::lastMatchesAverage(const string& team, Date date, int matches)
{
    string query;
    int state;
    
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    //query for last matches in the last year.
    query = "select home, away, fthg, ftag from " + m_league + " where (home = '" +
    team + "' or away = '" + team + "') and date < '" + to_string(date.year) + "-" +
    to_string(date.month) + "-" + to_string(date.day) + "' and date > '" +
    to_string(date.year-1) + "-7-31' order by date desc limit " + to_string(matches);
    
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
		return 0;
	}
    else
    {
        result = mysql_store_result(m_connection);
        
        int games = 0, goals = 0;
        
        //Sum goals and add number of matches up.
        while ( ( row=mysql_fetch_row(result)) != NULL )
        {
            if(row[0] == team)
            {
                goals += stoi(row[2]);
            }
            else
            {
                goals += stoi(row[3]);
            }
            
            ++games;
        }
        
        mysql_free_result(result);
        
        double output = 0.0;
        
        //If in the last year less games played then what was asked. (i.e. team recently promoted)
        if(games < matches)
        {
            //Grab average of goals by equivalent teams in the last season.
            double remaining = bottomMatchesAverage(date.year-1, matches-games);
            
            output = (double)(goals + remaining)/matches;
        }
        else
        {
            output = (double)goals/games;
        }
        
        return output;
    }
}


vector<string> Database::bottomTeams(int year)
{
    vector<string> output;
    
    string table = "bottom10" + m_league;
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    string query;
    
    query = "select team from " + table + " where year = '" + to_string(year) + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
	}
	else
    {
        result = mysql_store_result(m_connection);
    
        while ( ( row=mysql_fetch_row(result)) != NULL )
        {
            output.push_back(row[0]);
        }
        
        mysql_free_result(result);
	}
    
    return output;
}

pair<double, double> Database::bottomAverage(int year)
{
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    string query, start, end, team = "(";
    
    auto teams = teamsString(bottomTeams(year));
    
    start = to_string(year-1) + "-07-31";
    end = to_string(year) + "-07-31";
    
    string column[2] = {"fthg", "ftag"};
    double average[2] = {0.0, 0.0};
    int games[2] = {0, 0};
    
    for(int i = 0; i < 2; ++i)
    {
        query = "select " + column[i] + " from " + m_league + " where home in " + teams + " and date < '" + end + "' and date > '" + start + "'";
    
        state = mysql_query(m_connection, query.c_str());
    
        if (state !=0)
        {
            cout << mysql_error(m_connection) << endl;
        }
        else
        {
            result = mysql_store_result(m_connection);
        
            while ( ( row=mysql_fetch_row(result)) != NULL )
            {
                average[i] += stod(row[0]);
                ++games[i];
            }
        
            mysql_free_result(result);
        }
    }
    
    return make_pair(average[0]/games[0], average[1]/games[1]);
}

vector<pair<string, string>> Database::getMatches(const Date& d)
{
    vector<pair<string, string>> output;
    
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    string query;
    
    query = "select home, away from " + m_league + " where date = '" + to_string(d.year) + "-"
            + to_string(d.month) + "-" + to_string(d.day) + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
	}
	else
    {
        result = mysql_store_result(m_connection);
        
        while((row=mysql_fetch_row(result)) != NULL )
        {
            output.push_back(make_pair(row[0], row[1]));
        }
        
        mysql_free_result(result);
	}
    
    return output;
}

//Returns map with home team + (home odds, draw odds, away odds, result (1,2,3))
map<string, probabilities> Database::getOdds(const Date& d)
{
    map<string, probabilities> output;
    
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    string query;
    
    query = "select home, away, fthg, ftag, whh, whd, wha from " + m_league + " where date = '" + to_string(d.year) + "-"
    + to_string(d.month) + "-" + to_string(d.day) + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
	}
	else
    {
        result = mysql_store_result(m_connection);
        
        while((row=mysql_fetch_row(result)) != NULL )
        {
            probabilities temp;
            int fthg, ftag;
            
            //insert odds for home, draw and away results.
            temp.home = stod(row[4]);
            temp.draw = stod(row[5]);
            temp.away = stod(row[6]);
            
            //Home and Away goals.
            fthg = stoi(row[2]);
            ftag = stoi(row[3]);
            
            //indicate home, away or draw result.
            if(fthg > ftag)
            {
                temp.result = HOME;
            }
            else if(fthg < ftag)
            {
                temp.result = AWAY;
            }
            else
            {
                temp.result = DRAW;
            }
            
            //Insert home team paired with vector holding odds and result.
            output.insert(make_pair(row[0], temp));
        }
        
        mysql_free_result(result);
	}
    
    return output;
}

map<string, pair<int, int>> Database::getResults(const Date& d)
{
    map<string, pair<int, int>> output;
    
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    string query;
    
    query = "select home, fthg, ftag from " + m_league + " where date = '" + to_string(d.year) + "-"
    + to_string(d.month) + "-" + to_string(d.day) + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
	if (state !=0)
	{
		cout << mysql_error(m_connection) << endl;
    }
    else
    {
        result = mysql_store_result(m_connection);
            
        while((row=mysql_fetch_row(result)) != NULL )
        {
            output.insert(make_pair(row[0], make_pair(stoi(row[1]), stoi(row[2]))));
        }
            
        mysql_free_result(result);
    }
        
    return output;
}

//Stakes in database organised in the following way:
//If difference calculated betweeen bookies odds and calculated odds bigger than "difference" field, use number in "stake" field.
map<double, double> Database::getStakes(const string& res)
{
    map<double,double> output;
    
    int state;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    string query;
    
    query = "select difference, stake from stake where league = '" + m_league + "' and result = '" +
            res + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
    if (state !=0)
    {
        cout << mysql_error(m_connection) << endl;
    }
    else
    {
        result = mysql_store_result(m_connection);
        
        while((row=mysql_fetch_row(result)) != NULL )
        {
            output.insert(make_pair(stod(row[0]), stod(row[1])/100));
        }
        
        mysql_free_result(result);
    }
    
    return output;
}

vector<string> Database::parameters(const string& name)
{
    vector<string> output;
    
    int state;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    string query;
    
    query = "select * from parameters where name = '" + name + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
    if (state !=0)
    {
        cout << mysql_error(m_connection) << endl;
    }
    else
    {
        result = mysql_store_result(m_connection);
        
        while((row=mysql_fetch_row(result)) != NULL )
        {
            for(int i = 0; i < 7; ++i)
            {
                output.push_back(row[i]);
            }
        }
        
        mysql_free_result(result);
    }
    
    return output;
}

vector<double> Database::weights(const string &name)
{
    vector<double> output;
    
    int state;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    string query;
    
    query = "select * from AverageWeights where parameters = '" + name + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
    if (state !=0)
    {
        cout << mysql_error(m_connection) << endl;
    }
    else
    {
        result = mysql_store_result(m_connection);
        
        while((row=mysql_fetch_row(result)) != NULL )
        {
            for(int i = 1; i < 6; ++i)
            {
                output.push_back(stod(row[i]));
            }
        }
        
        mysql_free_result(result);
    }
    
    return output;
}

pair<int,int> Database::getResult(const string& home, const struct Date& d)
{
    int state, fthg, ftag;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    string query;
    
    query = "select fthg, ftag from " + m_league + " where home = '" + home + "' and date = '" + to_string(d.year) + "-"
    + to_string(d.month) + "-" + to_string(d.day) + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
    if (state !=0)
    {
        cout << mysql_error(m_connection) << endl;
    }
    else
    {
        result = mysql_store_result(m_connection);
        
        while((row=mysql_fetch_row(result)) != NULL )
        {
            fthg = stoi(row[0]);
            ftag = stoi(row[1]);
        }
        
        mysql_free_result(result);
    }
    
    return make_pair(fthg, ftag);
}

string Database::teamsString(const vector<string>& teams)
{
    string output = "(";
    
    for(auto& t : teams)
    {
        output += "'" + t + "'";
        
        if(t != teams.back())
        {
            output += ",";
        }
    }
    
    output += ")";
    
    return output;
}

double Database::bottomMatchesAverage(int year, int matches)
{
    int state;
    MYSQL_RES *result;
	MYSQL_ROW row;
    
    string league = "bottomAverage" + m_league;
    
    double output = 0.0;
    
    string query = "select home, away from " + league + " where year = '" + to_string(year) + "'";
    
    state = mysql_query(m_connection, query.c_str());
    
    if (state !=0)
    {
        cout << mysql_error(m_connection) << endl;
    }
    else
    {
        result = mysql_store_result(m_connection);
        
        while ( ( row=mysql_fetch_row(result)) != NULL )
        {
            output = (stod(row[0]) + stod(row[1])) / 2;
        }
        
        mysql_free_result(result);
    }
    
    return output*matches;
}

string Database::locationColumn(bool home)
{
    if(home)
    {
        return "home";
    }
    else
    {
        return "away";
    }
}

string Database::goalsColumn(bool scored, bool home)
{
    if ((scored && home) || (!scored && !home))
    {
        return "fthg";
    }
    else
    {
        return "ftag";
    }
}








