@echo off

adb devices
set DEV=GB8ZAEXLX4
set DEV=10.1.106.195:5555
set DEV=10.1.106.119:5555
set DEV=10.1.106.134:5555
copy Z:\yiwei\git\rongyi_term_driver\android\facedetect\rywfacedetect .\
::copy Z:\yiwei\git\rongyi_term_driver\android\facedetect\andorid\libs\arm64-v8a\libgnustl_shared.so .\
::copy Z:\yiwei\git\rongyi_term_driver\android\facedetect\andorid\libs\arm64-v8a\rywfacedetect .\
::copy Z:\0.common\android\usr\lib\arch-arm64\libprotobuf.so .\
adb -s %DEV% push  libgnustl_shared.so /system/lib64/
cp libprotobuf.so libprotobuf.so.9
adb -s %DEV% push  libprotobuf.so /system/lib64/
adb -s %DEV% push  libprotobuf.so.9 /system/lib64/
adb -s %DEV% push  rywfacedetect /system/bin/

::adb -s GB8ZAEXLX4 push  Z:\yiwei\git\rongyi_term_driver\android\facedetect\protoc /system/bin/
::adb -s GB8ZAEXLX4 push  Z:\yiwei\git\rongyi_term_driver\android\facedetect\human_face_report.proto /system/bin/
::adb -s GB8ZAEXLX4 push  Z:\yiwei\git\rongyi_term_driver\android\facedetect\universal_report.proto /system/bin/
::adb -s GB8ZAEXLX4 push  Z:\yiwei\git\rongyi_term_driver\android\facedetect\libprotobuf.so /system/lib64/
::adb -s %DEV% push  Z:\0.common\android\usr\lib\arch-arm64\libprotobuf.so /system/lib64/
::ln -s libprotobuf.so libprotobuf.so.9
::goto end
adb connect %DEV%
adb -s %DEV% root
adb -s %DEV% remount
adb -s %DEV% shell
:end
