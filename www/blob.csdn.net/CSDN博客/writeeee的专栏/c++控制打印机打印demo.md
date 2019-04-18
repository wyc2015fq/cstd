# c++控制打印机打印demo - writeeee的专栏 - CSDN博客
2015年07月20日 13:52:09[writeeee](https://me.csdn.net/writeeee)阅读数：2277
```cpp
#include <stdafx.h>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;
#define PRINTER_NAME "HP LaserJet 5200 Series PCL 5-hzz3"
int _tmain() {
    string hello = "!!! Hello Printer !!!\f";
    HANDLE hPrinter = NULL;
    DOC_INFO_1 DocInfo;
    DWORD      dwJob;
    DWORD      dwBytesWritten = 0L;
    if(OpenPrinter( (LPTSTR)_T(PRINTER_NAME), &hPrinter, NULL )) {
        cout << "printer opened" << endl;
        DocInfo.pDocName = (LPTSTR)_T("My Document");
        DocInfo.pOutputFile = NULL;
        DocInfo.pDatatype = (LPTSTR)_T("RAW");
        dwJob = StartDocPrinter( hPrinter, 1, (LPBYTE)&DocInfo );
        if (dwJob != 0) {
            cout << "Print job open" << endl;
            if (StartPagePrinter( hPrinter )) {
                cout << "Page started" << endl;
                // Send the data to the printer.
                if (WritePrinter( hPrinter, (void*)hello.c_str(), hello.length(), &dwBytesWritten)) {
                    if (dwBytesWritten == hello.length()) { cout << "Message sent to printer" << endl; }
                }
                cout << "Page Closed" << endl;
            }
            // Inform the spooler that the document is ending.
            EndDocPrinter( hPrinter );
            cout << "Print job open" << endl;
        } else {
            cout << "Could not create print job" << endl;
        }
        // Close the printer handle.
        ClosePrinter( hPrinter );
        cout << "printer closed" << endl;
    } else {
        cout << "Could not open Printer" << endl;
    }
    cout << "done";
    return 0;
}
```
