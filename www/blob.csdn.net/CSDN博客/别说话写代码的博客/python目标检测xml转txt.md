# python目标检测xml转txt - 别说话写代码的博客 - CSDN博客





2018年12月18日 16:15:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：183










![](https://img-blog.csdnimg.cn/20181218161716855.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

转换之后的txt格式如下：可以根据 自己需要的格式调整，分别为图片名，坐标还有，类别

![](https://img-blog.csdnimg.cn/2018121816141071.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```python
import os
import sys
import xml.etree.ElementTree as ET
import glob

def xml_to_txt(indir,outdir):

    os.chdir(indir)
    annotations = os.listdir('.')
    annotations = glob.glob(str(annotations)+'*.xml')
    print(annotations)
    for i, file in enumerate(annotations):

        file_save = file.split('.')[0]+'.txt'
        # file_txt=os.path.join(outdir,file_save)
        file_txt = outdir+"\\"+file_save
        # print(file_save)
        f_w = open(file_txt, 'w')

        # actual parsing
        in_file = open(file)
        tree=ET.parse(in_file)
        root = tree.getroot()
        filename = root.find('filename').text  #这里是xml的根，获取filename那一栏
        for obj in root.iter('object'):
                current = list()
                name = obj.find('name').text   #这里获取多个框的名字，底下是获取每个框的位置

                xmlbox = obj.find('bndbox')
                xn = xmlbox.find('xmin').text   
                xx = xmlbox.find('xmax').text
                yn = xmlbox.find('ymin').text
                yx = xmlbox.find('ymax').text
                #print xn
                f_w.write(filename +' '+xn+' '+yn+' '+xx+' '+yx+' ')
                f_w.write(name+'\n')

indir='.\\BCCD\\Annotations'   #xml目录
outdir='./BCCD/Annotations_txt'  #txt目录

xml_to_txt(indir,outdir)
```





