
#ifndef _DISPLAY_V4l2_H_
#define _DISPLAY_V4l2_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <linux/fb.h>

#define VID0_DEVICE	"/dev/video2"
#define VID1_DEVICE	"/dev/video3"
#define OSD0_DEVICE	"/dev/fb/0"
#define OSD1_DEVICE	"/dev/fb/2"
#define DISPLAY_V4L2_NUM_OUT_BUF  3

typedef struct
{
    int id;
    Uint8 *virtAddr;
    Uint8 *physAddr;
} DISPLAY_BufInfo;

typedef struct
{
    int width;
    int height;
    int lineOffsetV;
    int lineOffsetH;
} DISPLAY_FrameInfo;

typedef struct
{
    Uint8 *outBufVirtAddr[DISPLAY_V4L2_NUM_OUT_BUF];
    int outBufNum;
    int outBufSize;
    int v4l2FileHndl;
    struct v4l2_buffer v4l2DisplayBufCur;
} DISPLAY_V4l2Ctrl;

int DISPLAY_v4l2Create(int mode, DISPLAY_FrameInfo *info);
int DISPLAY_v4l2Delete(void);
int DISPLAY_v4l2GetBuf(DISPLAY_BufInfo *buf);
int DISPLAY_v4l2PutBuf(DISPLAY_BufInfo *buf);
int DISPLAY_v4l2Start(void);
int DISPLAY_v4l2Stop(void);

#endif 
