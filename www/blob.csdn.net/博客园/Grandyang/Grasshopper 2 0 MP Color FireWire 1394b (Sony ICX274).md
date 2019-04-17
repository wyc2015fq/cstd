# Grasshopper 2.0 MP Color FireWire 1394b (Sony ICX274) - Grandyang - 博客园







# [Grasshopper 2.0 MP Color FireWire 1394b (Sony ICX274)](https://www.cnblogs.com/grandyang/p/4601378.html)








![Picture of Grasshopper 2.0 MP Color FireWire 1394b (Sony ICX274) ](http://www.ptgrey.com/content/images/thumbs/0002936_grasshopper-20-mp-color-firewire-1394b-sony-icx274_772.jpeg)






相机参数如下，参见[这里](http://www.ptgrey.com/grasshopper-2-0-mp-color-firewire-1394b-sony-icx274-camera)：
|Resolution|1624 x 1224|
|----|----|
|Frame Rate|30 FPS|
|Megapixels|2.0 MP|
|Chroma|Color|
|Sensor Name|Sony ICX274|
|Sensor Type|CCD|
|Readout Method|Global shutter|
|Sensor Format|1/1.8"|
|Pixel Size|4.4 µm|
|Lens Mount|C-mount|
|ADC|14-bit|
|Gain Range|0 dB to 24 dB|
|Exposure Range|0.02 ms to >10 seconds|
|Trigger Modes|Standard, bulb, skip frames, overlapped, multi-shot|
|Partial Image Modes|Pixel binning, ROI|
|Image Processing|Gamma, lookup table, white balance|
|Image Buffer|32 MB|
|User Sets|2 memory channels for custom camera settings|
|Flash Memory|512 KB non-volatile memory|
|Non-isolated I/O Ports|2 bi-directional|
|Serial Port|1 (over non-isolated I/O)|
|Auxiliary Output|3.3 V, 150 mA maximum|
|Interface|FireWire 1394b|
|Power Requirements|8 to 30 V|
|Power Consumption (Maximum)|3.5 W at 12 V|
|Dimensions|44 mm x 29 mm x 58 mm|
|Mass|104 g|
|Machine Vision Standard|IIDC v1.31|
|Compliance|CE, FCC, KCC, RoHS|
|Temperature (Operating)|0° to 40°C|
|Temperature (Storage)|-30° to 60°C|
|Humidity (Operating)|20 to 80% (no condensation)|
|Humidity (Storage)|20 to 95% (no condensation)|
|Warranty|3 years|



Sample Code for Capturing Images：



```
#include "FlyCapture2.h"
#include <string>
#include <vector>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>

using namespace FlyCapture2;
using namespace std;
using namespace cv;

enum AviType
{
    UNCOMPRESSED,
    MJPG,
    H264
};

void PrintError( Error error )
{
    error.PrintErrorTrace();
}

void PrintCameraInfo( CameraInfo* pCamInfo )
{
    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        pCamInfo->serialNumber,
        pCamInfo->modelName,
        pCamInfo->vendorName,
        pCamInfo->sensorInfo,
        pCamInfo->sensorResolution,
        pCamInfo->firmwareVersion,
        pCamInfo->firmwareBuildTime );
}

void SaveAviHelper(
    AviType aviType,
    std::vector<Image>& vecImages,
    std::string aviFileName,
    float frameRate)
{
    Error error;
    AVIRecorder aviRecorder;

    // Open the AVI file for appending images

    switch (aviType)
    {
    case UNCOMPRESSED:
        {
            AVIOption option;
            option.frameRate = frameRate;
            error = aviRecorder.AVIOpen(aviFileName.c_str(), &option);
        }
        break;
    case MJPG:
        {
            MJPGOption option;
            option.frameRate = frameRate;
            option.quality = 75;
            error = aviRecorder.AVIOpen(aviFileName.c_str(), &option);
        }
        break;
    case H264:
        {
            H264Option option;
            option.frameRate = frameRate;
            option.bitrate = 1000000;
            option.height = vecImages[0].GetRows();
            option.width = vecImages[0].GetCols();
            error = aviRecorder.AVIOpen(aviFileName.c_str(), &option);
        }
        break;
    }

    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return;
    }

    printf( "\nAppending %d images to AVI file: %s ... \n", vecImages.size(), aviFileName.c_str() );
    for (int imageCnt = 0; imageCnt < vecImages.size(); imageCnt++)
    {
        // Append the image to AVI file
        error = aviRecorder.AVIAppend(&vecImages[imageCnt]);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            continue;
        }

        printf("Appended image %d...\n", imageCnt);
    }

    // Close the AVI file
    error = aviRecorder.AVIClose( );
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return;
    }
}

int main(int /*argc*/, char** /*argv*/)
{
    Error error;
    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }
    cout << "numCameras = " << numCameras << endl;
    if ( numCameras < 1 )
    {
        printf( "No camera detected.\n" );
        return -1;
    }
    else
    {
        printf( "Number of cameras detected: %u\n", numCameras );
    }

    PGRGuid guid;
    error = busMgr.GetCameraFromIndex(0, &guid);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }
    printf( "Running the first camera.\n" );

    Camera cam;
    // Connect to a camera
    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // Get the camera information
    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }
    PrintCameraInfo(&camInfo);

    // Start capturing images
    printf( "Starting capture... \n" );
    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // The total number of images
    const int k_numImages = 100;
    std::vector<Image> vecImages;
    vecImages.resize(k_numImages);

    // Grab images
    Image rawImage;
    for ( int imageCnt=0; imageCnt < k_numImages; imageCnt++ )
    {
        error = cam.RetrieveBuffer(&rawImage);
        if (error != PGRERROR_OK)
        {
            printf("Error grabbing image %u\n", imageCnt);
            continue;
        }
        else
        {
            printf("Grabbed image %u\n", imageCnt);
        }

        vecImages[imageCnt].DeepCopy(&rawImage);
    }

    // Stop capturing images
    printf( "Stopping capture... \n" );
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    // Check if the camera supports the FRAME_RATE property
    printf( "Detecting frame rate from camera... \n" );
    PropertyInfo propInfo;
    propInfo.type = FRAME_RATE;
    error = cam.GetPropertyInfo( &propInfo );
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    float frameRateToUse = 15.0f;
    if ( propInfo.present == true )
    {
        // Get the frame rate
        Property prop;
        prop.type = FRAME_RATE;
        error = cam.GetProperty( &prop );
        if (error != PGRERROR_OK)
        {
            PrintError(error);
        }
        else
        {
            // Set the frame rate.
            // Note that the actual recording frame rate may be slower,
            // depending on the bus speed and disk writing speed.
            frameRateToUse = prop.absValue;
        }
    }

    printf("Using frame rate of %3.1f\n", frameRateToUse);

    char aviFileName[512] = {0};

    sprintf(aviFileName, "SaveImageToAviEx-Uncompressed-%u", camInfo.serialNumber);
    SaveAviHelper(UNCOMPRESSED, vecImages, aviFileName, frameRateToUse);

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    system("Pause");
    return 0;
}
```














