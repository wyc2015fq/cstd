# python将视频转为图片 - 别说话写代码的博客 - CSDN博客





2018年07月06日 17:50:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：2877








```python
本文核心代码来自：https://blog.csdn.net/u010555688/article/details/79182362
```

```python
import cv2
def getFrame(videoPath, svPath):
    cap = cv2.VideoCapture(videoPath)
    numFrame = 0
    while True:
        if cap.grab():
            flag, frame = cap.retrieve()
            if not flag:
                continue
            else:
                #cv2.imshow('video', frame)
                numFrame += 1
                newPath = svPath + str(numFrame) + ".jpg"
                cv2.imencode('.jpg', frame)[1].tofile(newPath)
        if cv2.waitKey(10) == 27:
            break

getFrame(videoPath,savePicturePath)
```




