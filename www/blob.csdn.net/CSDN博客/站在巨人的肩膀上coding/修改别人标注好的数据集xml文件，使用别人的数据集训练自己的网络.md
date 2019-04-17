# 修改别人标注好的数据集xml文件，使用别人的数据集训练自己的网络 - 站在巨人的肩膀上coding - CSDN博客





2018年01月15日 15:16:59[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1744








        最近研究faster rcnn，一开始使用的是公开的数据集训练测试自己的网络性能，后来老板项目需求，需要用自己的数据集，但是数据集又是另一个人在另一台电脑上标注生成的，要直接在服务器下训练还不是很方便，主要是xml文件中路径属性的不同，那么今天就来和大家一起利用python脚本语言批量修改xml文件的属性内容：

       让我们来看看要修改的内容是什么样子



```
<annotation verified="no">
  <folder>清晰度高</folder>
  <filename>0000001</filename>
  <path>/root/mx-net/py-faster-rcnn/data/VOCdevkit/VOC2007/images/cxx_data/清晰度高/0000001.jpg</path>
  <source>
    <database>Unknown</database>
  </source>
  <size>
    <width>1200</width>
    <height>800</height>
    <depth>3</depth>
  </size>
  <segmented>0</segmented>
  <object>
    <name>shanxing</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>700</xmin>
      <ymin>417</ymin>
      <xmax>1157</xmax>
      <ymax>643</ymax>
    </bndbox>
  </object>
</annotation>
```





废话不多说，直接贴代码



```
#!/usr/bin/python  
# -*- coding=utf-8 -*-  
# author : wklken@yeah.net  
# date: 2012-05-25  
# version: 0.1  
       
from xml.etree.ElementTree import ElementTree,Element  
       
def read_xml(in_path):  
  '''''读取并解析xml文件 
    in_path: xml路径 
    return: ElementTree'''  
  tree = ElementTree()  
  tree.parse(in_path)  
  return tree  
       
def write_xml(tree, out_path):  
  '''''将xml文件写出 
    tree: xml树 
    out_path: 写出路径'''  
  tree.write(out_path, encoding="utf-8",xml_declaration=True)  
       
def if_match(node, kv_map):  
  '''''判断某个节点是否包含所有传入参数属性 
    node: 节点 
    kv_map: 属性及属性值组成的map'''  
  for key in kv_map:  
    if node.get(key) != kv_map.get(key):  
      return False  
  return True  
       
#---------------search -----  
def find_nodes(tree, path):  
  '''''查找某个路径匹配的所有节点 
    tree: xml树 
    path: 节点路径'''  
  return tree.findall(path)  
       
def get_node_by_keyvalue(nodelist, kv_map):  
  '''''根据属性及属性值定位符合的节点，返回节点 
    nodelist: 节点列表 
    kv_map: 匹配属性及属性值map'''  
  result_nodes = []  
  for node in nodelist:  
    if if_match(node, kv_map):  
      result_nodes.append(node)  
  return result_nodes  
       
#---------------change -----  
def change_node_properties(nodelist, kv_map, is_delete=False):  
  '''''修改/增加 /删除 节点的属性及属性值 
    nodelist: 节点列表 
    kv_map:属性及属性值map'''  
  for node in nodelist:  
    for key in kv_map:  
      if is_delete:  
        if key in node.attrib:  
          del node.attrib[key]  
      else:  
        node.set(key, kv_map.get(key))  
       
def change_node_text(nodelist, text, is_add=False, is_delete=False):  
  '''''改变/增加/删除一个节点的文本 
    nodelist:节点列表 
    text : 更新后的文本'''  
  for node in nodelist:  
    if is_add:  
      node.text += text  
    elif is_delete:  
      node.text = ""  
    else:  
      node.text = text  
       
def create_node(tag, property_map, content):  
  '''''新造一个节点 
    tag:节点标签 
    property_map:属性及属性值map 
    content: 节点闭合标签里的文本内容 
    return 新节点'''  
  element = Element(tag, property_map)  
  element.text = content  
  return element  
       
def add_child_node(nodelist, element):  
  '''''给一个节点添加子节点 
    nodelist: 节点列表 
   element: 子节点'''  
  for node in nodelist:  
    node.append(element)  
       
def del_node_by_tagkeyvalue(nodelist, tag, kv_map):  
  '''''同过属性及属性值定位一个节点，并删除之 
    nodelist: 父节点列表 
    tag:子节点标签 
    kv_map: 属性及属性值列表'''  
  for parent_node in nodelist:  
    children = parent_node.getchildren()  
    for child in children:  
      if child.tag == tag and if_match(child, kv_map):  
        parent_node.remove(child)  
       
if __name__ == "__main__":  
  #1. 读取xml文件  
  tree = read_xml("./0000001.xml")  
  
  path = tree.find("path")

  path.text = "New Value"  
  #2. 属性修改  
       #A. 找到父节点  
 # nodes = find_nodes(tree, "path")  
       #B. 通过属性准确定位子节点  
  #result_nodes = get_node_by_keyvalue(nodes, {"path":""})  
       #C. 修改节点属性  
 # change_node_properties(nodes, {"path": "1dhihs"})  
       #D. 删除节点属性  
     # change_node_properties(result_nodes, {"value":""}, True)  
       
      #3. 节点修改  
       #A.新建节点  
#      a = create_node("person", {"age":"15","money":"200000"}, "this is the firest content")  
       #B.插入到父节点之下  
#      add_child_node(result_nodes, a)  
       
      #4. 删除节点  
        #定位父节点  
#      del_parent_nodes = find_nodes(tree, "processers/services/service")  
        #准确定位子节点并删除之  
#      target_del_node = del_node_by_tagkeyvalue(del_parent_nodes, "chain", {"sequency" : "chain1"})  
       
      #5. 修改节点文本  
        #定位节点  
#      text_nodes = get_node_by_keyvalue(find_nodes(tree, "processers/services/service/chain"), {"sequency":"chain3"})  
#      change_node_text(text_nodes, "new text")  
       
      #6. 输出到结果文件  
  write_xml(tree, "./out.xml")
```








批量修改后的效果：



```
<?xml version='1.0' encoding='utf-8'?>
<annotation verified="no">
  <folder>清晰度高</folder>
  <filename>0000001</filename>
  <path>New Value</path>
  <source>
    <database>Unknown</database>
  </source>
  <size>
    <width>1200</width>
    <height>800</height>
    <depth>3</depth>
  </size>
  <segmented>0</segmented>
  <object>
    <name>shanxing</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>700</xmin>
      <ymin>417</ymin>
      <xmax>1157</xmax>
      <ymax>643</ymax>
    </bndbox>
  </object>
</annotation>
```





几点说明：


1.被我注释掉的部分是扩展内容，其实相比与一般的python脚本来说，这里的元素没有属性，只是元素修改元素path的文本内容，其实简单两句话就搞定了。简单修改某个元素的内容代码如下。如果还需要修改其他元素的文本内容，那么就加进去就好了




```
from xml.etree.ElementTree import ElementTree,Element  
       
def read_xml(in_path):  
  '''''读取并解析xml文件 
    in_path: xml路径 
    return: ElementTree'''  
  tree = ElementTree()  
  tree.parse(in_path)  
  return tree  
       
def write_xml(tree, out_path):  
  '''''将xml文件写出 
    tree: xml树 
    out_path: 写出路径'''  
  tree.write(out_path, encoding="utf-8",xml_declaration=True)  
 
if __name__ == "__main__":    
  tree = read_xml("./0000001.xml")  
  path = tree.find("path")
  path.text = "New Value" 
 write_xml(tree, "./out.xml")
```


2.上面的修改代码还不是批量修改的，只是修改某个文件做了一个demo,接下来，我们来看，如何批量对文件夹下面的所有xml文件进行修改？



自己写个循环遍历文件名


未完待续...








参考博客：

http://blog.csdn.net/wklken/article/details/7603071




