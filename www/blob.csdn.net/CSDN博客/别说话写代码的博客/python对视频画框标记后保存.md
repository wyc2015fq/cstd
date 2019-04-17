# python对视频画框标记后保存 - 别说话写代码的博客 - CSDN博客





2018年08月27日 17:53:31[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：605








需要画框取消注释rectangle

```python
import cv2
import os,sys,shutil
import numpy as np

# Open the input movie file, input the filepath as
input_filepath = sys.argv[1]
input_movie = cv2.VideoCapture(input_filepath)
length = int(input_movie.get(cv2.CAP_PROP_FRAME_COUNT))

#设置output
output_movie = cv2.VideoWriter(input_filepath.replace("mp4","avi").replace("input","output"), cv2.VideoWriter_fourcc('D', 'I', 'V', 'X'), 25, (1280, 720))

# Initialize some variables
frame_number = 0

while True:
    # Grab a single frame of video
    ret, frame = input_movie.read()

    frame_number += 1

    # Quit when the input video file ends
    if not ret:
        break

    # Draw a box around the body: input the top left point(x,y) and bottom right point(x,y)
    #cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

    # Write the resulting image to the output video file
    print("Writing frame {} / {}".format(frame_number, length))
    output_movie.write(frame)

# All done!
input_movie.release()
cv2.destroyAllWindows()
```





