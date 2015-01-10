//
//  file_access.h
//  Football
//
//  Created by Victor Cardozo on 28/08/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//

#ifndef __Football__file_access__
#define __Football__file_access__

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class file_access
{
public:
    file_access();
    
    void setFilename(const std::string& name);
    std::pair<double,double> average(const std::string& team);
    std::pair<double,double> standardDeviation(double averageHome, double averageAway, const std::string& team);
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> getCategories();
    std::vector<std::vector<std::string>> getRows();
    
private:
    std::vector<std::string> split(const std::string &line, const char &delimiter);
    
    std::ifstream m_file;
    std::string m_filename;

};
#endif /* defined(__Football__file_access__) */
