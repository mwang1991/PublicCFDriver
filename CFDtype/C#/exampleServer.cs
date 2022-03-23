using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading;
using UnityEngine;

public class exServer : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        CFDtype.initServer();
        print("Connected");
        WindDirection[] w = new WindDirection[100000];
        for (int i = 0; i < w.Length; i++)
        {
            w[i] = new WindDirection(1.1f, 2.2f, 3.3f, 4.4f);
        }
        CFDtype.SendTable(w);

    }

    // Update is called once per frame
    void Update()
    {
        CFDtype.sendCommand('T');
    }

    void OnApplicationQuit()
    {
        CFDtype.disconnect();
    }
}
