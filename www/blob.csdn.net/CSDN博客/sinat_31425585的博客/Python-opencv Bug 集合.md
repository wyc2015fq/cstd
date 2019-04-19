# Python-opencv Bug 集合 - sinat_31425585的博客 - CSDN博客
2018年05月11日 11:08:46[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：64
`1、VideoCapture读取本地视频，代码如下：`
```python
import numpy as np
import cv2 as cv
cap = cv.VideoCapture('project_video.mp4')
ret = True
while(cap.isOpened()):
    ret, frame = cap.read()
    # gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
#     if ret:
    cv.imshow('frame',frame)
    if cv.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv.destroyAllWindows()
```
当读取到最后一帧时，出现如下错误：`error: C:\builds\master_PackSlaveAddon-win64-vc12-static\opencv\modules\highgui\src\window.cpp:281: error: (-215) size.width>0 && size.height>0 in function cv::imshow`
`错误原因：有些摄像头需要一些"warmup"，这里对ret做一下判断就行了`
参考：
[http://answers.opencv.org/question/95889/how-to-solve-this-cvtcolor-error-cv2error-cbuildsmaster_packslaveaddon-win32-vc12-staticopencvmodulesimgprocsrccolorcpp7456-error-215-scn-3-scn-4-in/](http://answers.opencv.org/question/95889/how-to-solve-this-cvtcolor-error-cv2error-cbuildsmaster_packslaveaddon-win32-vc12-staticopencvmodulesimgprocsrccolorcpp7456-error-215-scn-3-scn-4-in/)
2、pip 升级后，没有main属性
```python
Traceback (most recent call last):
  File "/home/mirror/anaconda2/bin/pip", line 6, in <module>
    sys.exit(pip.main())
AttributeError: 'module' object has no attribute 'main'
```
处理方法：
```python
gedit /home/mirror/anaconda/bin/pip
```
修改为：
```python
import sys
from pip import __main__
if __name__ == '__main__':
    sys.exit(__main__._main())
```
参考资料为：
[https://blog.csdn.net/qq_20336433/article/details/80843282](https://blog.csdn.net/qq_20336433/article/details/80843282)
