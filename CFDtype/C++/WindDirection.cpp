#include "WindDirection.h"
WindDirection::WindDirection(float longitude, float latitude, float altitude, float windDirection)
{
    this->longitude = longitude;
    this->latitude = latitude;
    this->altitude = latitude;
    this->windDirection = windDirection;
    datalen = 16;
    init();
}
WindDirection::WindDirection()  //for receive data
{
    datalen = 16;
    init();
}
void WindDirection::FromCFData(char _data[]) //generate from recieved data
{
    longitude = GetFloat(_data);
    latitude = GetFloat(_data);
    altitude = GetFloat(_data);
    windDirection = GetFloat(_data);

}

void WindDirection::ToCFData()   //generate data to send
{
    Add(longitude);
    Add(latitude);
    Add(altitude);
    Add(windDirection);
}

std::vector<WindDirection*> WindDirection::RecvArray()  //generate array of the type from recieved data
{
    //get array size
    int _size = RecvSize();
    //Declare data array
    std::vector<CFDtype*> _array;

    //generate array
    CFDtype::RecvArray(_array);

    std::vector<WindDirection*> _buff;
    for (int i = 0; i < _size; i++)
        _buff.push_back((WindDirection *) _array[i]);
    return _buff;
}