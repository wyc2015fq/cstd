# python图片剪裁（图片按四个点坐标剪裁） - 别说话写代码的博客 - CSDN博客





2018年08月06日 14:29:00[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：4011








用了两种方法保存图片，opencv和Image，实践证明opencv非常快

```python
from PIL import Image
import os
import cv2
import time
import matplotlib.pyplot as plt
def label2picture(cropImg,framenum,tracker):
    pathnew ="E:\\DeeCamp\\img2\\"
    # cv2.imshow("image", cropImg)
    # cv2.waitKey(1)
    if (os.path.exists(pathnew + tracker)):
        cv2.imwrite(pathnew + tracker+'\\'+framenum + '.jpg', cropImg,[int(cv2.IMWRITE_JPEG_QUALITY), 100])

    else:
        os.makedirs(pathnew + tracker)
        cv2.imwrite(pathnew + tracker+'\\'+framenum + '.jpg', cropImg,[int(cv2.IMWRITE_JPEG_QUALITY), 100])

f = open("E:\\DeeCamp\\deecamp项目\\deep_sort-master\\hypotheses.txt","r")
lines = f.readlines()
for line in lines:
    li  = line.split(',')
    print(li[0],li[1],li[2],li[3],li[4],li[5])
    filename = li[0]+'.jpg'
    img = cv2.imread("E:\\DeeCamp\\img1\\" + filename)
    crop_img = img[int(li[3][:-3]):(int(li[3][:-3]) + int(li[5][:-3])),
               int(li[2][:-3]):(int(li[2][:-3]) + int(li[4][:-3]))]
    # print(int(li[2][:-3]),int(li[3][:-3]),int(li[4][:-3]),int(li[5][:-3]))
    label2picture(crop_img, li[0], li[1])
# #
# x,y,w,h = 87,158,109,222
# img = cv2.imread("E:\\DeeCamp\\img1\\1606.jpg")
# # print(img.shape)
# crop = img[y:(h+y),x:(w+x)]
# cv2.imshow("image", crop)
# cv2.waitKey(0)
# img = Image.open("E:\\DeeCamp\\img1\\3217.jpg")
#
# cropImg = img.crop((x,y,x+w,y+h))
# cropImg.show()
    # img = Image.open("E:\\DeeCamp\\deecamp项目\\deep_sort-master\\MOT16\\train\\try1\\img1\\"+filename)
    # print(int(li[2][:-3]),(int(li[2][:-3])+int(li[4][:-3])), int(li[3][:-3]),(int(li[3][:-3])+int(li[5][:-3])))

    # #裁切图片
    # # cropImg = img.crop(region)
    # # cropImg.show()
    # framenum ,tracker= li[0],li[1]
    # pathnew = 'E:\\DeeCamp\\deecamp项目\\deep_sort-master\\crop_picture\\'
    # if (os.path.exists(pathnew + tracker)):
    #     # 保存裁切后的图片
    #     plt.imshow(cropImg)
    #     plt.savefig(pathnew + tracker+'\\'+framenum + '.jpg')
    # else:
    #     os.makedirs(pathnew + tracker)
    #     plt.imshow(cropImg)
    #     plt.savefig(pathnew + tracker+'\\'+framenum + '.jpg')
```





