# PIL_Image —> to —> OpenCV_image - wsp_1138886114的博客 - CSDN博客





2018年10月18日 14:13:26[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：31








PIL.Image转换成OpenCV格式：

```python
import cv2  
from PIL import Image  
import numpy  

image = Image.open("plane.jpg")  
image.show()  
img = cv2.cvtColor(numpy.asarray(image),cv2.COLOR_RGB2BGR)  
cv2.imshow("OpenCV",img)  
cv2.waitKey()
cv2.destroyAllWindows()
```

OpenCV转换成PIL.Image格式：

```python
import cv2  
from PIL import Image  
import numpy  

img = cv2.imread("plane.jpg")  
cv2.imshow("OpenCV",img)  
image = Image.fromarray(cv2.cvtColor(img,cv2.COLOR_BGR2RGB))  
image.show()  
cv2.waitKey()
cv2.destroyAllWindows()
```

特别鸣谢
[https://blog.csdn.net/JohinieLi/article/details/81012572](https://blog.csdn.net/JohinieLi/article/details/81012572)



