
#ifndef __CVCAM_H__
#define __CVCAM_H__

#ifdef __cplusplus
#define CVCAM_EXTERN_C extern "C" 
extern "C" {
#else
#define CVCAM_EXTERN_C
#endif /* __cplusplus */


#ifdef CVCAM_EXPORTS
#define CVCAM_API CVCAM_EXTERN_C __declspec(dllexport)
#else
#define CVCAM_API CVCAM_EXTERN_C
#endif /* CVCAM_DLLEXPORT */

/* Returns the actual number of currently available cameras */
CVCAM_API int cvcamGetCamerasCount();

/* get/set the property of the camera. returns 0 if the property is not supported */
CVCAM_API int cvcamGetProperty(int camera, const char* property, void* value);
CVCAM_API int cvcamSetProperty(int camera, const char* property, void* value);

/* gets all property names. the actual number of properties is returned. */
CVCAM_API int cvcamGetPropertiesList(int camera, const char** properties, int count);

CVCAM_API int cvcamBuildStereo(void); 

/* Prepares the currently enabled cameras for work */
CVCAM_API int cvcamInit(void);

/* Start the video */
CVCAM_API int cvcamStart(void);

/* Stop the video */
CVCAM_API int cvcamStop(void);

/* Pause the video; should be used for preventing data changes during frame reading 
    using "frame" and other properties */
CVCAM_API int cvcamPause(void);

/* Resume the video */
CVCAM_API int cvcamResume(void);

/* Frees all resources */
CVCAM_API int cvcamExit(void);

/*Pops up a camera(s) selection dialog
Return value - number of cameras selected (0,1 or 2);
Argument: an array of selected cameras numbers
NULL if none selected. Should be released with free() when not needed.
if NULL passed, not used.
*/
CVCAM_API int cvcamSelectCamera(int** out);

/*Plays a specified avi file into a specified window
if file is NULL, file browser is opened. if window is 0,
it is created. width and height mean output size's 0 means
those of avi file are used. __cdecl (*callback)(img_t*) would be
called on every frame. NULL means no callback*/
CVCAM_API int cvcamPlayAVI(const char* file, 
                           void* window, 
                           int width, 
                           int height,
                           void* callback); 


/*Advanced API for dealing with AVI files*/

typedef unsigned int cvcamAVIFILE;


/*Opens a given file or pops up a dialog if file is NULL
returns a handle to the file opened for success or -1 for failure*/
CVCAM_API cvcamAVIFILE cvcamAVIOpenFile(char* file);

/*The next functions just do what they say and return 0
for success, anything other for failure*/

CVCAM_API int cvcamAVICloseFile(cvcamAVIFILE file);

CVCAM_API int cvcamAVISetWindow(cvcamAVIFILE file, void* window);

CVCAM_API int cvcamAVISetCallback(cvcamAVIFILE file, void* callback);

CVCAM_API int cvcamAVISetSize(cvcamAVIFILE file, int width, int height);

CVCAM_API int cvcamAVIRun(cvcamAVIFILE file);

CVCAM_API int cvcamAVIStop(cvcamAVIFILE file);

CVCAM_API int cvcamAVIPause(cvcamAVIFILE file);

CVCAM_API int cvcamAVIResume(cvcamAVIFILE file);

CVCAM_API int cvcamAVIWaitCompletion(cvcamAVIFILE file);

CVCAM_API int cvcamAVIIsRunning(cvcamAVIFILE file);

static const char CVCAM_PROP_ENABLE[] = "enable";
static const char CVCAM_PROP_RENDER[] = "render";
static const char CVCAM_PROP_WINDOW[] = "window";
static const char CVCAM_PROP_CALLBACK[] = "callback";
static const char CVCAM_DESCRIPTION[] = "description";
static const char CVCAM_VIDEOFORMAT[] = "video_pp";
static const char CVCAM_CAMERAPROPS[] = "camera_pp";
static const char CVCAM_RNDWIDTH[] = "rendering_width";
static const char CVCAM_RNDHEIGHT[] = "rendering_height";
static const char CVCAM_SRCWIDTH[] = "source_width";
static const char CVCAM_SRCHEIGHT[] = "source_height";
static const char CVCAM_STEREO_CALLBACK[] = "stereo_callback";
static const char CVCAM_STEREO3_CALLBACK[] = "stereo3_callback";
static const char CVCAM_STEREO4_CALLBACK[] = "stereo4_callback";
static const char CVCAM_PROP_SETFORMAT[] = "set_video_format";
static const char CVCAM_PROP_RAW[] = "raw_image";
static const char CVCAM_PROP_TIME_FORMAT[] = "time_format";
static const char CVCAM_PROP_DURATION[] = "duration";
static const char CVCAM_PROP_POSITION[] = "current_position";

extern const int FRAMES_FORMAT ;
extern const int TIME_FORMAT ;

typedef struct
{
    char DeviceDescription[100];
    char device[100];
    int  channel;
    char ChannelDescription[100];
    int  maxwidth;
    int  maxheight;
    int  minwidth;
    int  minheight;
}CameraDescription;

typedef struct
{
    int width;
    int height;
    double framerate;
}VidFormat;


#define CVCAMTRUE  (void*)1
#define CVCAMFALSE (void*)0

typedef int cvcamWindow;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__CVCAM_H__

