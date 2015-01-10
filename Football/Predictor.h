//
//  Predictor.h
//  Football
//
//  Created by Victor Cardozo on 02/12/2014.
//  Copyright (c) 2014 Victor Cardozo. All rights reserved.
//

#ifndef __Football__Predictor__
#define __Football__Predictor__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

#include "Engine.h"

class Predictor : public Engine
{
public:
    //Constructor
    //'league' is league to predict.
    //'parameters' are the name of the parameters to be used in prediction.
    Predictor(const std::string& league, const std::string& parameters);
    
    //Set purse for predictions.
    void setPurse(double val);
    
    //Predict a result.
    //'home' is team playing at home.
    //'away' is team playing away.
    //'odds' are the [1] home, [2] draw and [3] away odds calculated.
    void predict(const std::string& home, const std::string& away,
                 const std::vector<double>& odds);
    
private:
    //returns current date in which prediction is being made.
    const Date currentDateTime();
    
    //Generate string indicating decision.
    //'d' int indicating decision.
    //Returns a string indicating decision.
    const std::string decision(int d);
};

#endif /* defined(__Football__Predictor__) */