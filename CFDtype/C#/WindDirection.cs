using System.Linq;

public class WindDirection : CFDtype
{
    public float longitude;
    public float latitude;
    public float altitude;
    public float windDirection;

    public WindDirection(float longitude, float latitude, float altitude, float windDirection)
    {
        this.longitude = longitude;
        this.latitude = latitude;
        this.altitude = latitude;
        this.windDirection = windDirection;
        datalen = 16;
        init();
    }
    public WindDirection()  //for receive data
    {
        datalen = 16;
        init();
    }
    protected override void FromCFData(byte[] _data) //generate from recieved data
    {
        longitude = GetFloat(ref _data);
        latitude = GetFloat(ref _data);
        altitude = GetFloat(ref _data);
        windDirection = GetFloat(ref _data);

    }

    protected override void ToCFData()   //generate data to send
    {
        Add(longitude);
        Add(latitude);
        Add(altitude);
        Add(windDirection);
    }

    public static WindDirection[] RecvArray()  //generate array of the type from recieved data
    {
        //get array size
        int _size = RecvSize();  
        //Declare data array
        WindDirection[] _array = new WindDirection[_size];

        //generate array
        RecvArray(_array);
        return _array;
    }
}
