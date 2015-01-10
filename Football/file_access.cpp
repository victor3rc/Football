//
//  file_access.cpp
//  Football
//
//  Created by Victor Cardozo on 28/08/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//


#include <sstream>
#include <math.h>
#include "file_access.h"
#include <dirent.h>

file_access::file_access()
{}

void file_access::setFilename(const std::string& name)
{
    m_filename = name;
}

std::pair<double,double> file_access::standardDeviation(double averageHome, double averageAway,
                                                        const std::string& team)
{
    std::pair<double,double> output;
    double deviationHome = 0.0, deviationAway = 0.0;
    int gamesAway = 0, gamesHome = 0;
    std::string line;
    
    m_file.open(m_filename);
    
    if (m_file.is_open())
    {
        while(getline(m_file,line))
        {
            std::vector<std::string> contents;
            
            contents = split(line, ',');
            
            if(contents.at(2) == team)
            {
                deviationHome += fabs(std::stoi(contents.at(4)) - averageHome);
                ++gamesHome;
            }
            else if(contents.at(3) == team)
            {
                deviationAway += fabs(std::stoi(contents.at(5)) - averageAway);
                ++gamesAway;
            }
        }
        
        m_file.close();
    }
    
    output.first = (double)deviationHome/gamesHome;
    output.second = (double)deviationAway/gamesAway;
    
    return output;
}

std::pair<double,double> file_access::average(const std::string& team)
{
    std::pair<double,double> output;
    std::string line;
    int goalsHome = 0, gamesHome = 0, goalsAway = 0, gamesAway = 0;
    
    m_file.open(m_filename);
    
    if (m_file.is_open())
    {
        while(getline(m_file,line))
        {
            std::vector<std::string> contents;
        
            contents = split(line, ',');
        
            if(contents.at(2) == team)
            {
                goalsHome += std::stoi(contents.at(4));
                ++gamesHome;
            }
            else if(contents.at(3) == team)
            {
                goalsAway += std::stoi(contents.at(5));
                ++gamesAway;
            }
        }
        
        m_file.close();
    }
    
    output.first = (double)goalsHome/gamesHome;
    output.second = (double)goalsAway/gamesAway;
    
    return output;
}

std::map<std::string, std::vector<std::pair<std::string, std::string>>> file_access::getCategories()
{
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> output;
    std::string line;
    
    m_file.open(m_filename);
    
    if (m_file.is_open())
    {
        while(getline(m_file,line))
        {
            std::vector<std::string> contents;
            
            contents = split(line, ',');
            
            auto it = output.find(contents.at(0));
            
            //Insert into container if not in there already.
            if(it == output.end())
            {
                std::vector<std::pair<std::string, std::string>> categories;
                
                //Grab each category and year.
                for(int i = (int)contents.size(); i > 1; i--)
                {
                    std::string cat = contents.at(i-1);
                    categories.push_back(std::make_pair(cat.substr(0,2), cat.substr(3, 1)));
                }
                
                output.insert(std::pair<std::string, std::vector<std::pair<std::string, std::string>>>(contents.at(0), categories));
            }
        }
    }
    
    return output;
}

std::vector<std::vector<std::string>> file_access::getRows()
{
    std::vector<std::vector<std::string>> output;
    
    std::string line;
    int index = 0;
    
    m_file.open(m_filename);
    
    if (m_file.is_open())
    {
        while(getline(m_file,line))
        {
            std::vector<std::string> contents;
            
            contents = split(line, ',');
            
            if(contents.at(1) == "Date")
            {
                continue;
            }
            
            try
            {
                std::swap(contents.at(1)[0], contents.at(1)[6]);
                std::swap(contents.at(1)[1], contents.at(1)[7]);
                
                std::vector<std::string> vec;
                
                vec.push_back(contents.at(1));
                vec.push_back(contents.at(2));
                vec.push_back(contents.at(3));
                vec.push_back(contents.at(4));
                vec.push_back(contents.at(5));
                vec.push_back(contents.at(38));
                vec.push_back(contents.at(39));
                vec.push_back(contents.at(40));
                
                output.push_back(vec);
            }
            catch (const std::exception& ex)
            {
                std::cout << ex.what() << std::endl;
            }
        }
        
        m_file.close();
    }
    
    return output;
}


std::vector<std::string> file_access::split(const std::string &line, const char &delimiter)
{
    std::stringstream ss(line);
    std::string s;
    
    std::vector<std::string> out;
    
    while(getline(ss, s, delimiter))
    {
        out.push_back(s);
    }
    
    return out;
}
















