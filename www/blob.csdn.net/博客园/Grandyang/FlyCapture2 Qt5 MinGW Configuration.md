# FlyCapture2 Qt5 MinGW Configuration - Grandyang - 博客园







# [FlyCapture2 Qt5 MinGW Configuration](https://www.cnblogs.com/grandyang/p/4612164.html)







Install FlyCatprue2 to the folder "C:\PointGreyResearch\"



Add the following to the .pro file:

```
# Add FlyCapture2
INCLUDEPATH += C:\PointGreyResearch\FlyCapture2\include
INCLUDEPATH += C:\PointGreyResearch\FlyCapture2\include\C

LIBS += "C:\PointGreyResearch\FlyCapture2\lib\C\FlyCapture2_C.lib"
LIBS += "C:\PointGreyResearch\FlyCapture2\lib\C\FlyCapture2GUI_C.lib"
```



Note:

The C++ library only works with visual studio on Windows, not MinGW. So if we want to use MinGW, only the C library would work!



Sample FlyCapture2 API C Code:



```
#include "C/FlyCapture2_C.h"
#include <stdio.h>

typedef enum _AviType
{
    UNCOMPRESSED,
    MJPG,
    H264
} AviType;

void PrintCameraInfo( fc2Context context )
{
    fc2Error error;
    fc2CameraInfo camInfo;
    error = fc2GetCameraInfo( context, &camInfo );
    if ( error != FC2_ERROR_OK )
    {
        // Error
    }

    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        camInfo.serialNumber,
        camInfo.modelName,
        camInfo.vendorName,
        camInfo.sensorInfo,
        camInfo.sensorResolution,
        camInfo.firmwareVersion,
        camInfo.firmwareBuildTime );
}

int SaveAVIHelper(fc2Context context, AviType aviType, float frameRate)
{
    fc2Error error;
    const int k_numImagesToGrab = 100;    
    fc2Image rawImage;
    fc2AVIContext aviContext;
    fc2AVIOption aviOption;
    fc2H264Option h264Option;
    fc2MJPGOption mjpgOption;
    int i;

    error = fc2CreateAVI(&aviContext);
    if (error != FC2_ERROR_OK)
    {
        printf("Error in fc2CreateAVI: %d\n", error);
        return -1;
    }

    error = fc2CreateImage( &rawImage );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateImage: %d\n", error );           
        fc2DestroyAVI(aviContext);
        return -1;
    }

    for (i=0; i < k_numImagesToGrab; i++)
    {
        // Retrieve the image
        error = fc2RetrieveBuffer(context, &rawImage);
        if (error != FC2_ERROR_OK)
        {
            printf("Error in retrieveBuffer: %d\n", error);
            continue;
        }

        // Perform some initialization for the first time
        if (i == 0)
        {
            switch (aviType)
            {
            case UNCOMPRESSED:
                aviOption.frameRate = frameRate;
                error = fc2AVIOpen(aviContext, "SaveImageToAviEx_C-Uncompressed", &aviOption);
                if (error != FC2_ERROR_OK)
                {
                    printf("Error opening AVI: %d\n", error);
                }
                break;

            case MJPG:
                mjpgOption.frameRate = frameRate;
                mjpgOption.quality = 75;
                error = fc2MJPGOpen(aviContext, "SaveImageToAviEx_C-MJPG", &mjpgOption);
                if (error != FC2_ERROR_OK)
                {
                    printf("Error opening AVI: %d\n", error);
                }
                break;

            case H264:     
                h264Option.frameRate = frameRate;
                h264Option.bitrate = 1000000;
                h264Option.width = rawImage.cols;
                h264Option.height = rawImage.rows;
                error = fc2H264Open(aviContext, "SaveImageToAviEx_C-H264", &h264Option);
                if (error != FC2_ERROR_OK)
                {
                    printf("Error opening AVI: %d\n", error);
                }
                break;            
            }
        }

        error = fc2AVIAppend(aviContext, &rawImage);
        if (error != FC2_ERROR_OK)
        {
            printf("Error appending to AVI: %d\n", error);
        }

        printf("Appended image %d\n", i);        
    }  

    error = fc2DestroyImage(&rawImage);
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateImaged: %d\n", error );           
        fc2DestroyAVI(aviContext);
        return -1;
    }

    error = fc2DestroyAVI(aviContext);
    if (error != FC2_ERROR_OK)
    {
        printf("Error in fc2DestroyAVI: %d\n", error);
        return -1;
    }

    return 0;
}



float GetFrameRate(fc2Context context)
{
    fc2Error error;
    fc2PropertyInfo propInfo;
    fc2Property prop;

    // Check if the camera supports the FRAME_RATE property
    printf( "Detecting frame rate from camera... \n" );
    propInfo.type = FC2_FRAME_RATE;
    error = fc2GetPropertyInfo(context, &propInfo);
    if (error != FC2_ERROR_OK)
    {
        return 0.0f;
    }

    if (propInfo.present)
    {
        // Get the frame rate
        prop.type = FC2_FRAME_RATE;
        error = fc2GetProperty(context, &prop);
        if (error != FC2_ERROR_OK)
        {
            return 0.0f;
        }

        // Set the frame rate.
        // Note that the actual recording frame rate may be slower,
        // depending on the bus speed and disk writing speed.
        return prop.absValue;        
    }

    return 0.0f;
}

int RunCamera(fc2Context context, fc2PGRGuid guid)
{
    fc2Error error;
    float frameRate = 0.0f;

    error = fc2Connect( context, &guid );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2Connect: %d\n", error );
        return -1;
    }

    PrintCameraInfo( context );    

    error = fc2StartCapture( context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2StartCapture: %d\n", error );
        return -1;
    }

    frameRate = GetFrameRate(context);
    if (frameRate == 0.0f)
    {
        printf("Invalid frame rate returned\n");
        return -1;
    }

    SaveAVIHelper(context, UNCOMPRESSED, frameRate);
    SaveAVIHelper(context, H264, frameRate);
    SaveAVIHelper(context, MJPG, frameRate);

    error = fc2StopCapture( context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2StopCapture: %d\n", error );
        return -1;
    }

    return 0;
}

int main(int argc, char** argv)
{        
    fc2Error error;
    fc2Context context;
    fc2PGRGuid guid;
    unsigned int numCameras = 0;      

    //PrintBuildInfo();

    error = fc2CreateContext( &context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateContext: %d\n", error );
        return 0;
    }        

    error = fc2GetNumOfCameras( context, &numCameras );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2GetNumOfCameras: %d\n", error );
        return 0;
    }

    if ( numCameras == 0 )
    {
        // No cameras detected
        printf( "No cameras detected.\n");
        return -1;
    }

    // Get the 0th camera
    error = fc2GetCameraFromIndex( context, 0, &guid );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2GetCameraFromIndex: %d\n", error );
        return -1;
    }    

    if (RunCamera(context, guid) != 0)
    {
        printf("Error running camera\n");
        return -1;
    }

    error = fc2DestroyContext( context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2DestroyContext: %d\n", error );
        return -1;
    }

    printf( "Done! Press Enter to exit...\n" );
    getchar();

    return 0;
}
```














