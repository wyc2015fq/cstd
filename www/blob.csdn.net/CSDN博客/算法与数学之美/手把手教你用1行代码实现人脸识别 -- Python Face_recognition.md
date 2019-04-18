# 手把手教你用1行代码实现人脸识别 -- Python Face_recognition - 算法与数学之美 - CSDN博客
2017年11月12日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：450
环境要求：
Ubuntu17.10
Python 2.7.14
环境搭建：
1. 安装 Ubuntu17.10 > 安装步骤在这里
2. 安装 Python2.7.14 (Ubuntu17.10 默认Python版本为2.7.14)
3. 安装 git 、cmake 、 python-pip
> 
# 安装 git
$ sudo apt-get install -y git
# 安装 cmake
$ sudo apt-get install -y cmake
# 安装 python-pip
$ sudo apt-get install -y python-pip
4. 安装编译dlib
安装face_recognition这个之前需要先安装编译dlib
> 
# 编译dlib前先安装 boost
$ sudo apt-get install libboost-all-dev
# 开始编译dlib
# 克隆dlib源代码
$ git clone https://github.com/davisking/dlib.git
$ cd dlib
$ mkdir build
$ cd build
$ cmake .. -DDLIB_USE_CUDA=0 -DUSE_AVX_INSTRUCTIONS=1
$ cmake --build .（注意中间有个空格）
$ cd ..
$ python setup.py install --yes USE_AVX_INSTRUCTIONS --no DLIB_USE_CUDA
5. 安装 face_recognition
> 
# 安装 face_recognition
$ pip install face_recognition
# 安装face_recognition过程中会自动安装 numpy、scipy 等
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3FOz3pQoIxaSia2z0J3bSiaHD7cnKrLabTI8HPKHQHrc1Vsrs4enBzWewl9g1e8UAWXYLXWgK3BvHA/640?wx_fmt=jpeg)
环境搭建完成后，在终端输入 face_recognition 命令查看是否成功
**实现人脸识别：**
**示例一(1行代码实现人脸识别)：**
1. 首先你需要提供一个文件夹，里面是所有你希望系统认识的人的图片。其中每个人一张图片，图片以人的名字命名：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3FOz3pQoIxaSia2z0J3bSiaHicguhGIsNCxD7dMlOF9bFB4LuGicY7QoiaXibFubAt2DibNFicP4LcyKPgiaw/640?wx_fmt=jpeg)
known_people文件夹下有babe、成龙、容祖儿的照片
2. 接下来，你需要准备另一个文件夹，里面是你要识别的图片：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3FOz3pQoIxaSia2z0J3bSiaH2RlWZaib92C8wpOxX6g0rSqUIxwl1cHX4Q8BibxficN8XeC8k8QsDpia8A/640?wx_fmt=jpeg)
unknown_pic文件夹下是要识别的图片，其中韩红是机器不认识的
3. 然后你就可以运行face_recognition命令了，把刚刚准备的两个文件夹作为参数传入，命令就会返回需要识别的图片中都出现了谁：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3MEzPh8JXaLKv8wsCK23hwGy8TKPQIDBDmu5ywgOibA75xe4RMwM4FibFOlsL3wFFzNmwKyXXKLmuQ/640?wx_fmt=jpeg)
识别成功！！！
**示例二(识别图片中的所有人脸并显示出来)：**
> 
# filename : find_faces_in_picture.py
# -*- coding: utf-8 -*-
# 导入pil模块 ，可用命令安装 apt-get install python-Imagingfrom PIL 
import Image
# 导入face_recogntion模块，可用命令安装 pip install face_recognition
import face_recognition
# 将jpg文件加载到numpy 数组中
image = face_recognition.load_image_file("/opt/face/unknown_pic/all_star.jpg")
# 使用默认的给予HOG模型查找图像中所有人脸
# 这个方法已经相当准确了，但还是不如CNN模型那么准确，因为没有使用GPU加速
# 另请参见: find_faces_in_picture_cnn.py
face_locations = face_recognition.face_locations(image)
# 使用CNN模型
# face_locations = face_recognition.
face_locations(image, number_of_times_to_upsample=0, model="cnn")
# 打印：我从图片中找到了 多少 张人脸
print("I found {} face(s) in this photograph.".format(len(face_locations)))
# 循环找到的所有人脸
for face_location in face_locations:        
# 打印每张脸的位置信息
        top, right, bottom, left = face_location
        print("A face is located at pixel location Top: 
            {}, Left: {}, Bottom: {}, Right: {}".format(top, left, bottom, right))
        
# 指定人脸的位置信息，然后显示人脸图片
        face_image = image[top:bottom, left:right]
        pil_image = Image.fromarray(face_image)
        pil_image.show()
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3MEzPh8JXaLKv8wsCK23hwHiaAwE3qLIjbfEm2PWo9BOd9EFqfVPw5BbuqYE2BJL5VMAsE0ibLWhRA/640?wx_fmt=jpeg)
用于识别的图片
> 
# 执行python文件
$ python find_faces_in_picture.py
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3MEzPh8JXaLKv8wsCK23hw2RkJqPcz5jbOTFhFiaUN64wu3oYHHWyEibZcuCaUpaNRnhrcLJag5bCQ/640?wx_fmt=jpeg)
从图片中识别出7张人脸，并显示出来
**示例三(自动识别人脸特征)：**
> 
# filename : find_facial_features_in_picture.py
# -*- coding: utf-8 -*-
# 导入pil模块 ，可用命令安装 apt-get install python-Imaging
from PIL import Image, ImageDraw
# 导入face_recogntion模块，可用命令安装 pip install face_recognition
import face_recognition
# 将jpg文件加载到numpy 数组中
image = face_recognition.load_image_file("biden.jpg")
＃查找图像中所有面部的所有面部特征
face_landmarks_list = face_recognition.face_landmarks(image)
print("I found {} face(s) in this photograph.".format(len(face_landmarks_list)))
for face_landmarks in face_landmarks_list:
＃打印此图像中每个面部特征的位置
   facial_features = [
        'chin',
        'left_eyebrow',
        'right_eyebrow',
        'nose_bridge',
        'nose_tip',
        'left_eye',
        'right_eye',
        'top_lip',
        'bottom_lip'
     ]
for facial_feature in facial_features:
        print("The {} in this face has the following points: {}".format(facial_feature,
face_landmarks[facial_feature]))
＃让我们在图像中描绘出每个人脸特征！
    pil_image = Image.fromarray(image)
    d = ImageDraw.Draw(pil_image)
    for facial_feature in facial_features:
        d.line(face_landmarks[facial_feature], width=5)
    pil_image.show()
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3MEzPh8JXaLKv8wsCK23hwyeCWWxT8REosGhzFee3AoW9qLfdRkHY5UXyMGgaoUBd6nnPTlfS0lQ/640?wx_fmt=jpeg)
自动识别出人脸特征
**示例四(识别人脸鉴定是哪个人)：**
> 
# filename : recognize_faces_in_pictures.py
# -*- conding: utf-8 -*-
# 导入face_recogntion模块，可用命令安装 pip install face_recognition
import face_recognition
＃将jpg文件加载到numpy数组中
babe_image = face_recognition.load_image_file("/opt/face/known_people/babe.jpeg")
Rong_zhu_er_image = face_recognition.load_image_file("/opt/face/known_people/Rong
 zhu er.jpg")
unknown_image = face_recognition.load_image_file("/opt/face/unknown_pic/babe2.jpg")
＃获取每个图像文件中每个面部的面部编码
＃由于每个图像中可能有多个面，所以返回一个编码列表。
＃但是由于我知道每个图像只有一个脸，我只关心每个图像中的第一个编码，所以我取索引0。
babe_face_encoding = face_recognition.face_encodings(babe_image)[0]
Rong_zhu_er_face_encoding = face_recognition.face_encodings(Rong_zhu_er_image)[0]
unknown_face_encoding = face_recognition.face_encodings(unknown_image)[0]
known_faces = [
    babe_face_encoding,
    Rong_zhu_er_face_encoding
]
＃结果是True/false的数组，未知面孔known_faces阵列中的任何人相匹配的结果
results = face_recognition.compare_faces(known_faces, unknown_face_encoding)
print("这个未知面孔是 Babe 吗? {}".format(results[0]))
print("这个未知面孔是 容祖儿 吗? {}".format(results[1]))
print("这个未知面孔是 我们从未见过的新面孔吗? {}".format(not True in results))
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3MEzPh8JXaLKv8wsCK23hwvgclXY9TW0wPemTGZ69542cSTEaQwnQXYjllGWEEOytAPqCT6LeSxw/640?wx_fmt=jpeg)
显示结果如图
**示例五(识别人脸特征并美颜)：**
> 
# filename : digital_makeup.py
# -*- coding: utf-8 -*-
# 导入pil模块 ，可用命令安装 apt-get install python-Imaging
from PIL import Image, ImageDraw
# 导入face_recogntion模块，可用命令安装 pip install face_recognition
import face_recognition
＃将jpg文件加载到numpy数组中
image = face_recognition.load_image_file("biden.jpg")
＃查找图像中所有面部的所有面部特征
face_landmarks_list = face_recognition.face_landmarks(image)
for face_landmarks in face_landmarks_list:
    pil_image = Image.fromarray(image)
    d = ImageDraw.Draw(pil_image, 'RGBA')
    ＃让眉毛变成了一场噩梦
    d.polygon(face_landmarks['left_eyebrow'], fill=(68, 54, 39, 128))
    d.polygon(face_landmarks['right_eyebrow'], fill=(68, 54, 39, 128))
    d.line(face_landmarks['left_eyebrow'], fill=(68, 54, 39, 150), width=5)
    d.line(face_landmarks['right_eyebrow'], fill=(68, 54, 39, 150), width=5)
    ＃光泽的嘴唇
    d.polygon(face_landmarks['top_lip'], fill=(150, 0, 0, 128))
    d.polygon(face_landmarks['bottom_lip'], fill=(150, 0, 0, 128))
    d.line(face_landmarks['top_lip'], fill=(150, 0, 0, 64), width=8)
    d.line(face_landmarks['bottom_lip'], fill=(150, 0, 0, 64), width=8)
    ＃闪耀眼睛
    d.polygon(face_landmarks['left_eye'], fill=(255, 255, 255, 30))
    d.polygon(face_landmarks['right_eye'], fill=(255, 255, 255, 30))
    ＃涂一些眼线
    d.line(face_landmarks['left_eye'] + [face_landmarks['left_eye'][0]], fill=(0,
 0, 0, 110), width=6)
    d.line(face_landmarks['right_eye'] + [face_landmarks['right_eye'][0]], fill=(0,
 0, 0, 110), width=6)
    pil_image.show()
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/L2QczVR5icb3MEzPh8JXaLKv8wsCK23hwibpvZrwnDecg46Cf37ETIWx745bpPpg9zbYdW4ucBYoaM4rspzkICBg/640?wx_fmt=jpeg)
美颜前后对比
来源：Kangvcar（简书）
**----****------**
**等的就是你，真的超有趣！高能金融抱团群发车啦~******加我拉你进群呦****
**![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyUggTiciccxLbccHxK1LFiaDv8qzaXm2NdCibibHicaWRFTibvcWtXhZr3rXUf5Sj8q3afOrTyibHicKpBAOA/640?wx_fmt=png)**
**算法数学之美微信公众号欢迎赐稿**
**稿件涉及数学、物理、算法、计算机、编程等相关领域。**
**稿件一经采用，我们将奉上稿酬。**
**投稿邮箱：math_alg@163.com商务合作：联系微信号hengzi5809**
