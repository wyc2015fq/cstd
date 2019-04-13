
# Capture images using V4L2 on Linux - 嵌入式Linux - CSDN博客

2016年01月21日 19:28:00[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：553


这文章相当好，没有理由不转载
I have always been using OpenCV’s VideoCapture API to capture images from webcam or USB cameras. OpenCV supportsV4L2 and I wanted to use something other than OpenCV’s VideoCapture API so I started digging up about v4l2 and got few links using and few examples using which I successfully wrote a small code to grab an image usingV4L2 and convert it to OpenCV’s Mat structure and display the image.
What is V4L2?
V4L2 is the second version of Video For Linux which is a video capturing API for Linux.Here you can find amazing documentation about the API. So it gives you a very easy inteface to use it with C, C++ and Python. I haven’t tried Python bindings yet.
How To Use V4L2 API?
I started reading documentation but didn’t really understand much until I foundthis example. The code had some issues and wasn’t working properly. But I just copied it and tried understanding it. So this is my understanding of the code.
Step 1: Open the Capture Device.
In Linux, default capture devide is generally /dev/video0, but if you’re using USB webcams, the index will vary accordingly.
```python
int
```
```python
fd;
fd = open(
```
```python
"/dev/video0"
```
```python
, O_RDWR);
```
```python
if
```
```python
(fd == -
```
```python
1
```
```python
)
{
```
```python
// couldn't find capture device
```
```python
perror(
```
```python
"Opening Video device"
```
```python
);
```
```python
return
```
```python
1
```
```python
;
}
```
Step 2: Query the Capture
So, basically you check if the capture is available or not. V4L2 doesn’t support some cameras so it would throw an error here. We need to usev4l2_capability structure and VIDIOC_QUERYCAP to query the capture. Read Morehere.
```python
struct
```
```python
v4l2_capability caps = {
```
```python
0
```
```python
};
```
```python
if
```
```python
(-
```
```python
1
```
```python
== xioctl(fd, VIDIOC_QUERYCAP, &caps))
{
    perror(
```
```python
"Querying Capabilites"
```
```python
);
```
```python
return
```
```python
1
```
```python
;
}
```
Here xioctl is a wrapper function over ioctl. ioctl() is a function to manipulate device parameters of special files. Read morehere.
```python
#include <sys/ioctl.h>
```
```python
static
```
```python
int
```
```python
xioctl(
```
```python
int
```
```python
fd,
```
```python
int
```
```python
request,
```
```python
void
```
```python
*arg)
{
```
```python
int
```
```python
r;
```
```python
do
```
```python
r = ioctl (fd, request, arg);
```
```python
while
```
```python
(-
```
```python
1
```
```python
== r &&
```
```python
EINTR
```
```python
== errno);
```
```python
return
```
```python
r;
}
```
Step 3: Image Format
V4L2 provides an easy interface to check the image formats and colorspace that your webcam supports and provide.v4l2_format sturcture is to be used to change image format.
```python
struct v4l2_format fmt = {
```
```python
0
```
```python
}
```
```python
;
```
```python
fmt
```
```python
.type
```
```python
= V4L2_BUF_TYPE_VIDEO_CAPTURE
```
```python
;
```
```python
fmt
```
```python
.fmt
```
```python
.pix
```
```python
.width
```
```python
=
```
```python
320
```
```python
;
```
```python
fmt
```
```python
.fmt
```
```python
.pix
```
```python
.height
```
```python
=
```
```python
240
```
```python
;
```
```python
fmt
```
```python
.fmt
```
```python
.pix
```
```python
.pixelformat
```
```python
= V4L2_PIX_FMT_MJPEG
```
```python
;
```
```python
fmt
```
```python
.fmt
```
```python
.pix
```
```python
.field
```
```python
= V4L2_FIELD_NONE
```
```python
;
```
```python
if (-
```
```python
1
```
```python
== xioctl(fd, VIDIOC_S_FMT, &fmt))
{
    perror(
```
```python
"Setting Pixel Format"
```
```python
)
```
```python
;
```
```python
return
```
```python
1
```
```python
;
```
```python
}
```
I have set image width and height to be 320 and 240 respectively. You should check out the format that your camera supports. My Camera supports MJPEG and YUV and hence I have set image format to MJPEG.
Step 4: Request Buffers
A buffer contains data exchanged by application and driver using Streaming I/O methods.v4l2_requestbuffers is used to allocate device buffers. Read more here.
struct v4l2_requestbuffers req = {0};
req.count = 1;
req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
req.memory = V4L2_MEMORY_MMAP;
if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
{
perror(“Requesting Buffer”);
return 1;
}
The ioctl is used to initialize memory mapped(mmap), user pointer based I/O.
Step 5: Query Buffer
After requesting buffer from the device, we need to query the buffer in order to get raw data. Read morehere
```python
struct
```
```python
v4l2_buffer buf = {
```
```python
0
```
```python
};
buf.
```
```python
type
```
```python
= V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory = V4L2_MEMORY_MMAP;
buf.index = bufferindex;
```
```python
if
```
```python
(-
```
```python
1
```
```python
== xioctl(fd, VIDIOC_QUERYBUF, &buf))
{
    perror(
```
```python
"Querying Buffer"
```
```python
);
```
```python
return
```
```python
1
```
```python
;
}
```
buffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
The mmap() function asks to map length bytes starting at offset in the memory of the device specified by fd into the application address space, preferably at address start. Read morehere
Step 6: Capture Image
After querying the buffer, the only thing left is capturing the frame and saving it in the buffer.
```python
if
```
```python
(-
```
```python
1
```
```python
== xioctl(fd, VIDIOC_STREAMON, &buf
```
```python
.type
```
```python
))
{
    perror(
```
```python
"Start Capture"
```
```python
);
```
```python
return
```
```python
1
```
```python
;
}
fd_set fds;
FD_ZERO(&fds);
FD_SET(fd, &fds);
```
```python
struct
```
```python
timeval tv = {
```
```python
0
```
```python
};
tv
```
```python
.tv_sec
```
```python
=
```
```python
2
```
```python
;
```
```python
int
```
```python
r = select(fd+
```
```python
1
```
```python
, &fds,
```
```python
NULL
```
```python
,
```
```python
NULL
```
```python
, &tv);
```
```python
if
```
```python
(-
```
```python
1
```
```python
== r)
{
    perror(
```
```python
"Waiting for Frame"
```
```python
);
```
```python
return
```
```python
1
```
```python
;
}
```
```python
if
```
```python
(-
```
```python
1
```
```python
== xioctl(fd, VIDIOC_DQBUF, &buf))
{
    perror(
```
```python
"Retrieving Frame"
```
```python
);
```
```python
return
```
```python
1
```
```python
;
}
```
Step 7: Store data in OpenCV datatype
I wanted to stored the retrieved data in OpenCV image structure. It took me few hours to figure out the perfect way. So here’s how I did it.
CvMat cvmat = cvMat(480, 640, CV_8UC3, (void*)buffer);
IplImage * img;
img = cvDecodeImage(&cvmat, 1);
So this how I captured frames from my webcam and stored in OpenCV Image data structure.
You can find the complete code here on my GitHub
P.S. Coding period for gsoc has started and I have to start working.
If you have some feedback or questions regarding this post, please add comments. I’d be happy to get some feedback.

