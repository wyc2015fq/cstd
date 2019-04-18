# 在matlab中GUI环境下通过摄像头读入视频并处理 手记（未完） - 西西弗的石头 - CSDN博客





2014年01月25日 17:50:49[hzhaly](https://me.csdn.net/hzhaly)阅读数：10023








目标：在matlab中GUI环境下通过摄像头读入视频并实时处理（实时可能比较困难）




1、建立GUI界面，通过摄像头读取视频，并灰度化处理。  完整的m文件如下。（matlab2010a）



```
function varargout = faceDetection(varargin)
% FACEDETECTION M-file for faceDetection.fig
%      FACEDETECTION, by itself, creates a new FACEDETECTION or raises the existing
%      singleton*.
%
%      H = FACEDETECTION returns the handle to a new FACEDETECTION or the handle to
%      the existing singleton*.
%
%      FACEDETECTION('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FACEDETECTION.M with the given input arguments.
%
%      FACEDETECTION('Property','Value',...) creates a new FACEDETECTION or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before faceDetection_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to faceDetection_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help faceDetection

% Last Modified by GUIDE v2.5 25-Jan-2014 15:26:28

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @faceDetection_OpeningFcn, ...
                   'gui_OutputFcn',  @faceDetection_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before faceDetection is made visible.
function faceDetection_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to faceDetection (see VARARGIN)

% Choose default command line output for faceDetection
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes faceDetection wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = faceDetection_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%version1--------------------------------------------------------
% imaqhwinfo('winvideo');
% vid1=videoinput('winvideo',1);
% usbVidRes1=get(vid1,'videoResolution');
% nBands1=get(vid1,'NumberOfBands');
% axes(handles.axes1);
% hImage1=imshow(zeros(usbVidRes1(2),usbVidRes1(1),nBands1));
% preview(vid1,hImage1);

%version2---------------------------------------------------------这里是重点
imaqhwinfo('winvideo');
vid1=videoinput('winvideo',1);
usbVidRes1=get(vid1,'videoResolution');
nBands1=get(vid1,'NumberOfBands');
axes(handles.axes1);
hImage1=imshow(zeros(usbVidRes1(2),usbVidRes1(1),nBands1));
preview(vid1,hImage1);

while (1)
    frame=getsnapshot(vid1);
    frame=rgb2gray(frame);
    axes(handles.axes2);
    imshow(frame)
end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcf);
```
效果图：



![](https://img-blog.csdn.net/20140125175435921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHpoYWx5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





2. matlab- videoinput 参数说明

![](https://img-blog.csdn.net/20140125180637250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHpoYWx5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


输入格式：vid=videoinput（adaptorname，deviceID，format）


 
       TriggerRepeat 重复触发


 
       FramesPerTrigger : trigger触发时获取的帧数


 
       FrameGrabInterval帧的抓取间隔 （默认的是1，即每一帧都抓取；如果设置为3，则每3帧取一帧）

VideoResolution: 视频分辨率

NumberOfBands: 色彩（rgb是相当于是三通道）






