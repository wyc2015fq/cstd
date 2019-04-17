# python修改xml文件内容 - 别说话写代码的博客 - CSDN博客





2019年01月19日 23:12:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：32








```python
import xml.etree.ElementTree as ET
def change_xml(xml_path):
    filelist = os.listdir(xml_path)
    print(filelist)
    # 打开xml文档
    for xmlfile in filelist:
        doc = ET.parse(xml_path+xmlfile)
        root = doc.getroot()
        sub1 = root.find('filename')  #找到filename标签，
        sub1.text = xmlfile     #修改标签内容
        
        doc.write(xml_path+xmlfile)   #保存修改

change_xml('C:/Users/49691/Desktop/gangjin/gangjin_test/Annotations1/') //xml文件总路径
```





