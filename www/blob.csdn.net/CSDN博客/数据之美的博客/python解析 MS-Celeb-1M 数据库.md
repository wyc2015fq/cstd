# python解析 MS-Celeb-1M 数据库 - 数据之美的博客 - CSDN博客
2017年03月21日 16:22:58[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1829
我在微软下载人脸识别[数据库](http://lib.csdn.net/base/mysql)[MS-Celeb-1M](https://www.microsoft.com/en-us/research/project/ms-celeb-1m-challenge-recognizing-one-million-celebrities-real-world/),
 下载完的格式是 .tsv格式。看了数据库官网的文件格式说明：
File format: text files, each line is an image record containing 7 columns, delimited by TAB. 
Column1: Freebase MID 
Column2: ImageSearchRank 
Column3: ImageURL 
Column4: PageURL 
Column5: FaceID 
Column6: FaceRectangle_Base64Encoded (four floats, relative coordinates of UpperLeft and BottomRight corner) 
Column7: FaceData_Base64Encoded
我决定用[Python](http://lib.csdn.net/base/python)件来解析这个.tsv文件，获取图片及相应的人脸信息。以下是解析代码：
```python
import base64
import struct
import os
def readline(line):
    MID,ImageSearchRank,ImageURL,PageURL,FaceID,FaceRectangle,FaceData=line.split("\t")
    rect=struct.unpack("ffff",base64.b64decode(FaceRectangle))
    return MID,ImageSearchRank,ImageURL,PageURL,FaceID,rect,base64.b64decode(FaceData)
def writeImage(filename,data):
    with open(filename,"wb") as f:
        f.write(data)
def unpack(filename,target="img"):
    i=0
    with open(filename,"r",encoding="utf-8") as f:
        for line in f:
            MID,ImageSearchRank,ImageURL,PageURL,FaceID,FaceRectangle,FaceData=readline(line)
            img_dir=os.path.join(target,MID)
            if not os.path.exists(img_dir):
                os.mkdir(img_dir)
            img_name="%s-%s"%(ImageSearchRank,FaceID)+".jpg"
            writeImage(os.path.join(img_dir,img_name),FaceData)
            i+=1
            if i%1000==0:
                print(i,"imgs finished")
    print("all finished")    
filename="MsCelebV1-Faces-Aligned.tsv" 
unpack(filename)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
tsv文件和csv文件是类似的，tsv的数据用Tab键分隔，csv文件用逗号分隔。根据说明文件，把每一行的信息读取出来
```python
MID,ImageSearchRank,ImageURL,PageURL,FaceID,FaceRectangle,FaceData=line.split("\t")
```
- 1
- 1
人脸矩形框的位置信息是（左上点坐标，右下点坐标），用Base64编码的，格式为4个浮点数。
```python
rect=struct.unpack("ffff",base64.b64decode(FaceRectangle))
```
- 1
- 1
人脸信息是用Base64编码的，需要解码，然后保存为图像：
```
data=base64.b64decode(FaceData)
with open(filename,"wb") as f:
    f.write(data)
```
