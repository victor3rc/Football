//
//  file_access.cpp
//  Football
//
//  Created by Victor Cardozo on 28/08/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//
#include <sstream>
#include <math.h>
#include <dirent.h>

#include "FileAccess.h"

void FileAccess::setFilename(const std::string& name)
{
    m_filename = name;
}

std::vector<std::vector<std::string>> FileAccess::getRows()
{
    std::vector<std::vector<std::string>> output;
    std::string line;
    
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
                //Swap day value for year value before inserting in database.
                std::swap(contents.at(1)[0], contents.at(1)[6]);
                std::swap(contents.at(1)[1], contents.at(1)[7]);
                
                std::vector<std::string> vec;
                
                vec.push_back(contents.at(1));//date
                vec.push_back(contents.at(2));//home team
                vec.push_back(contents.at(3));//away team
                vec.push_back(contents.at(4));//home goals scored
                vec.push_back(contents.at(5));//away goals scored
                vec.push_back(contents.at(11));//home shots
                vec.push_back(contents.at(12));//away shots
                vec.push_back(contents.at(13));//home shots target
                vec.push_back(contents.at(14));//away shots target
                vec.push_back(contents.at(38));//Williah Hill home odds
                vec.push_back(contents.at(39));//Williah Hill draw odds
                vec.push_back(contents.at(40));//Williah Hill away odds
                
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


std::vector<std::string> FileAccess::split(const std::string &line, const char &delimiter)
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
















