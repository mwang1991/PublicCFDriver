#pragma once
#include "CFDtype.h"
class WindDirection :
	public CFDtype
{
public:
    float longitude;
    float latitude;
    float altitude;
    float windDirection;

    void ToCFData();	//generate length and add bytes from data
    void FromCFData(char _data[]);		//read data from bytes
    WindDirection(float longitude, float latitude, float altitude, float windDirection);
    WindDirection();
    static std::vector<WindDirection*> RecvArray();
};

