//
//  main.cpp
//  Football
//
//  Created by Victor Cardozo on 28/08/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "FileAccess.h"
#include "Database.h"
#include "Predictor.h"
#include "Simulation.h"

std::vector<std::string> addFiles()
{
    std::vector<std::string> output;
    
    output.push_back("England_1_1415.csv");
//    files.push_back("England_1_1314.csv")
//    files.push_back("England_1_1213.csv");
//    files.push_back("England_1_1112.csv");
//    files.push_back("England_1_1011.csv");
    
    return output;
}

int main(int argc, const char * argv[])
{
    std::string menu = "v";
    
    while(menu != "q")
    {
        std::cout << "Update, Simulate or Predict? (u/s/p - 'q' to quit)";
        std::getline(std::cin, menu);
        
        if(menu == "u")
        {
            std::string table;
            std::string folder = "/Users/victorcardozo/Documents/Football/League_sheets/";
            
            std::cout << "Update chosen. Which table to update? " << std::endl;
            std::getline(std::cin, table);
            
            auto files = addFiles();
            
            FileAccess file;
            Database db;
            
            for(auto f : files)
            {
                file.setFilename(folder + f);
                
                auto rows = file.getRows();
                
                db.insertResults(table, rows);
            }
        }
        else if(menu == "p")
        {
            std::string league, parameters;
            
            std::cout << "Predictor chosen. Which league?" << std::endl;
            std::getline(std::cin, league);
            
            std::cout << "Choose parameters: ";
            std::getline(std::cin, parameters);
            
            Predictor pre(league, parameters);
            
            std::string home, away, pur, c = "y";
            std::vector<double> odds = {0.0, 0.0, 0.0};
            double p = 0.0;
            
            std::cout << "Purse: ";
            std::getline(std::cin, pur);
            p = std::stod(pur);
            
            pre.setPurse(p);
            
            std::cout << std::endl;
            
            while(c != "n")
            {
                std::string h, d, a;
                
                std::cout << "Home: ";
                std::getline(std::cin, home);
                
                std::cout << "Away: ";
                std::getline(std::cin, away);
                
                std::cout << "Home odds: ";
                std::getline(std::cin, h);
                odds[0] = std::stod(h);
                
                std::cout << "Draw odds: ";
                std::getline(std::cin, d);
                odds[1] = std::stod(d);
                
                std::cout << "Away odds: ";
                std::getline(std::cin, a);
                odds[2] = std::stod(a);
                
                pre.predict(home, away, odds);
                
                std::cout << "Continue (y/n)? ";
                std::getline(std::cin, c);
                
                std::cout << std::endl;
            }
        }
        else if(menu == "s")
        {
            std::string league, parameters, start = "2014", end = "2015", r;
            bool research = false;
            
            std::cout << "Simulator chosen. Which league?" << std::endl;
            std::getline(std::cin, league);
            
            std::cout << "Choose parameters: ";
            std::getline(std::cin, parameters);
            
            std::cout << "Start year: ";
            std::getline(std::cin, start);
            
            std::cout << "End year: ";
            std::getline(std::cin, end);
            
            std::cout << "Carry out research? (y/n)";
            std::getline(std::cin, r);
            
            if(r == "y")
            {
                research = true;
            }
            
            int s = std::stoi(start);
            int e = std::stoi(end);
            
            Simulation sim(1, 8, s, league, parameters, research);
            
            Date d;
            
            d.day = 1;
            d.month = 8;
            d.year = e;
            
            sim.simulate(d);
        }
        else
        {
            if(menu != "q")
            {
                std::cout << "Please re-enter instruction." << std::endl;
            }
            else
            {
                std::cout << "Bye!" << std::endl;
            }
        }
    }
    
    return 0;
}

