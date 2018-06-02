@echo off
set out=Z:\yiwei\git\rongyi_term_driver\android\facedetect
set code=Z:\yiwei\git\rongyi_term_driver\android\facedetect\code
set ext=Z:\yiwei\git\rongyi_term_driver\android\facedetect\ext
copy /y E:\code\cstd\lib\curl\curl_config.h %ext%\curl\
copy /y E:\code\cstd\lib\curl\curl.inl %ext%\curl\
copy /y curl\curl.inl %ext%\curl\
copy /y ..\cap\capv4l.inl %code%\cap\capv4l.inl
copy /y ..\sys\sys_linux.inl %code%\sys\sys_linux.inl
copy /y ..\face\test_facetrack.inl %code%\face\test_facetrack.inl
copy /y ..\face\asmfit.inl %code%\face\asmfit.inl
copy /y ..\face\facetrack.inl %code%\face\facetrack.inl
for %%a in (test_zip.inl facecfg.inl rongyitcp.inl rongyihttp.inl libcurl.c test_http.inl main.cpp libcap.c test_cam.inl test_rongyinet.inl rongyinet.inl) do (
 copy /y %%a %out%\
 echo.>>%out%\%%a
)
