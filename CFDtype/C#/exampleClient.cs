using UnityEngine;

public class exClient : MonoBehaviour
{

    // Start is called before the first frame update
    void Start()
    {

        if(CFDtype.initClient("127.0.0.1") == 1)    //connect to server
            print("Connection established");
        else
            print("Connection failed");


        WindDirection w = new WindDirection();
        WindDirection[] t;

        t = WindDirection.RecvArray();
            //print(w.longitude);
    }

    // Update is called once per frame
    void Update()
    {
       
        print(CFDtype.recvCommand());

    }

    void OnApplicationQuit()
    {
        CFDtype.disconnect();
    }
}
