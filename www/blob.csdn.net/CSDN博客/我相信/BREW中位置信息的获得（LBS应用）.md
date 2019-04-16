# BREW中位置信息的获得（LBS应用） - 我相信...... - CSDN博客





2010年11月04日 16:21:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1121标签：[brew																[callback																[velocity																[buffer																[function																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)




IPosDet接口可以直接获得经纬度等相关信息，应用将这些信息传递给服务器与GIS系统配合使用就可以开发出各种丰富多彩的应用了，IPosDet的使用示例如下：

// create the instance for PosDet

ISHELL_CreateInstance(pi->a.m_pIShell, AEECLSID_POSDET, (void **) &(pi->m_pPosDet));                                           

if(pi->m_pPosDet)        {

                // Initialize the Callback function

                CALLBACK_Init(&pi->SimpleGPSCB, simplegps_PosDetCB, pi);

                // You also can get seperate information through

                IPOSDET_GetGPSInfo(pi->m_pPosDet, AEEGPS_GETINFO_LOCATION|AEEGPS_GETINFO_VELOCITY|AEEGPS_GETINFO_ALTITUDE,AEEGPS_ACCURACY_LEVEL1, &pi->m_gpsInfo, &pi->SimpleGPSCB);

}

                return(TRUE);

Simplegps_PosDetCB函数将得到的位置信息显示的屏幕上，以显示纬度为例的代码如下：

// wasFactor is the factor between AEEGPSInfo and real world data.

   STR_TO_WSTR("186413.5111",szNum,sizeof(szNum));

   wgsFactor= WSTR_TO_FLOAT(szNum);

   //Display Latitude on the screen

   val = IntegerToFloat(pMe->m_gpsInfo.dwLat);

   val = FDIV(val, wgsFactor);

   FLOATTOWSTR(val, szNum, TEXT_BUFFER_SIZE);

   STR_TO_WSTR("Lat:", szText, TEXT_BUFFER_SIZE);

   WSTRCAT(szText,szNum);

   // IDISPLAY_ClearScreen(pMe->a.m_pIDisplay);

   IDISPLAY_DrawText(pMe->a.m_pIDisplay,    // Display instance

                  AEE_FONT_BOLD,       // Use BOLD font

                  szText,              // Text - Normally comes from resource

                  -1,                  // -1 = Use full string length

                  0,                   // Ignored - IDF_ALIGN_CENTER

                  32,                   // Ignored - IDF_ALIGN_MIDDLE

                  NULL,                // No clipping

                  IDF_ALIGN_CENTER);



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/29/5914128.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/29/5914128.aspx)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=velocity&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




