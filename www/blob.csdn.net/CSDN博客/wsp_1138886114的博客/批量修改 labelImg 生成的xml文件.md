# 批量修改 labelImg 生成的xml文件 - wsp_1138886114的博客 - CSDN博客





2019年01月22日 19:45:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：379








使用tensorflow对目标检测图像进行数据训练时，需要标注大量的图像。这个工作量实在是不少。为了更加简便的标注图像，或者说增加训练集：
- 做如下操作：
先对每类图像各标注100-200张进行训练。

训练准确度提升完成后，将未标注的图像（测试集）进行预测。

人工分出预测对的图像，根据检测对的图像的检测框数据生成新的xml用来增加训练集。
由于之前有相应的xml文件模板，所以我们只需要修改之前的xml文件即可。

修改代码：

```python
import os
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
    tree.write(out_path, encoding="utf-8", xml_declaration=True)

def if_match(node, kv_map):
    '''''判断某个节点是否包含所有传入参数属性
       node: 节点
       kv_map: 属性及属性值组成的map'''
    for key in kv_map:
        if node.get(key) != kv_map.get(key):
            return False
    return True

# ----------------search -----------------
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

# ---------------change ----------------------
def change_node_properties(nodelist, kv_map, is_delete=False):
    '''修改/增加 /删除 节点的属性及属性值
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

def create_node(tag, property_map=None, content=None):
    '''新造一个节点
       tag:节点标签
       property_map:属性及属性值map
       content: 节点闭合标签里的文本内容
       return 新节点'''
    element = Element(tag,content, property_map)
    element.text = None
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

def get_xml(input_dir):
    xml_path_list = []
    for (root_path,dirname,filenames) in os.walk(input_dir):
        for filename in filenames:
            if filename.endswith('.xml'):
                xml_path = root_path+"/"+filename
                xml_path_list.append(xml_path)
    return  xml_path_list

if __name__ == "__main__":
    input_dir ="./"
    xml_path_list = get_xml(input_dir)


    for xml_file in xml_path_list:
        ################ 1. 读取xml文件  ##########
        root = read_xml(xml_file)
        data = [[1, 1, 1],
                ["abc",1, 1, 1,1],
                ["ABC",1, 1, 1,1]]
        # 修改图像属性
        size = root.find('size')
        size.find('height').text = "new text111"
        size.find('width').text = "new text111"
        size.find('depth').text = "new text111"

        # Find annotations.
        Object = root.findall('object')
        for i in range(len(Object)):
            if len(Object) ==2:
                # change_node_text(Object[i].find('name'),str(data[i+1][0]))
                Object[i].find('name').text = str(data[i+1][0])                 # 修改节点名
                Object[i].find('bndbox').find('ymin').text = str(data[i+1][1])  # 修改节点文本
                Object[i].find('bndbox').find('xmin').text = str(data[i+1][2])
                Object[i].find('bndbox').find('ymax').text = str(data[i+1][3])
                Object[i].find('bndbox').find('xmax').text = str(data[i+1][4])

            else:
                # change_node_text(Object[0].find('name'),"new text")
                Object[0].find('name').text = str(data[1][0])                 # 修改节点名
                Object[0].find('bndbox').find('ymin').text = str(data[1][1])  # 修改节点文本
                Object[0].find('bndbox').find('xmin').text = str(data[1][2])
                Object[0].find('bndbox').find('ymax').text = str(data[1][3])
                Object[0].find('bndbox').find('xmax').text = str(data[1][4])

                del_parent_nodes1 = find_nodes(Object[1], "object/name")
                del_parent_nodes2 = find_nodes(Object[1], "object/pose")
                del_parent_nodes3 = find_nodes(Object[1], "object/truncated")
                del_parent_nodes4 = find_nodes(Object[1], "object/difficult")
                del_parent_nodes5 = find_nodes(Object[1], "object/bndbox")
                del_parent_nodes6 = find_nodes(Object[1], "object/bndbox/ymin")
                del_parent_nodes7 = find_nodes(Object[1], "object/bndbox/xmin")
                del_parent_nodes8 = find_nodes(Object[1], "object/bndbox/ymax")
                del_parent_nodes9 = find_nodes(Object[1], "object/bndbox/xmax")

                Element.remove(del_parent_nodes1)
                Element.remove(del_parent_nodes2)
                Element.remove(del_parent_nodes3)
                Element.remove(del_parent_nodes4)
                Element.remove(del_parent_nodes5)
                Element.remove(del_parent_nodes6)
                Element.remove(del_parent_nodes7)
                Element.remove(del_parent_nodes8)
                Element.remove(del_parent_nodes9)


        ################ 输出到结果文件  ##########
        write_xml(root, "HKEN111113.xml")
```



