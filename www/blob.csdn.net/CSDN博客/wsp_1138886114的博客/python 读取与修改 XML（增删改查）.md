# python 读取与修改 XML（增删改查） - wsp_1138886114的博客 - CSDN博客





2019年01月21日 16:06:45[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：198











### XML操作
- [xml 访问与查找](#xml__1)
- [xml 创建与修改](#xml__61)
- [xml 增删改查 -- 综合案例](#xml____116)




#### xml 访问与查找

```python
import xml.etree.ElementTree as ET

tree = ET.parse('students.xml')
root = tree.getroot()  # 使用getroot()获取根节点，得到的是一个Element对象

#root = ET.fromstring(country_data_as_string) #从字符串变量中读取，返回的是Element对象


########################## 访问XML ###################################
"""
tag = element.text      #访问Element标签
attrib = element.attrib #访问Element属性
text = element.text     #访问Element文本
"""

for element in root.findall('student'):
    tag = element.tag                  # 访问Element标签
    attrib = element.attrib            # 访问Element属性
    text = element.find('name').text   # 访问Element文本
    print(tag, attrib, text)

print(root[0][0].text)                 # 子节点是嵌套的，我们可以通关索引访问特定的子节点

########################## 查找元素 ###################################
print("============ Element.iter() ===============")
for student in root.iter('student'):   # Element.iter()
    print(student[0].text)


print("============ Element.findall() ============")
for element in root.findall('student'):# Element.findall()
    name = element.find('name').text
    age = element.find('age').text
    score = element.find('score').text

    print (name,age,score)
```

students.xml文件内容如下：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<students>
	<student>
		<name>张三</name>
		<age>18</age>
		<score>89</score>
	</student>
	<student>
		<name>李四</name>
		<age>19</age>
		<score>81</score>
	</student>
	<student>
		<name>王五</name>
		<age>17</age>
		<score>92</score>
	</student>
</students>
```

#### xml 创建与修改

```python
import xml.etree.ElementTree as ET


########################## 创建xml文件 ################################

a = ET.Element("root")       #创建根节点

b = ET.SubElement(a,"sub1")  #创建子节点，并添加属性
b.attrib = {"name":"name attribute"}

c = ET.SubElement(a,"sub2")  #创建子节点，并添加数据
c.text = "test"

tree = ET.ElementTree(a)    #创建elementtree对象，写文件
tree.write("test.xml")

########################## 修改XML ###################################

"""
    ElementTree.write()       将构建的XML文档写入文件。
    Element.text = ''         直接改变字段内容
    Element.append(Element)   为当前的Elment对象添加子对象
    Element.remove(Element)   删除Element节点
    Element.set(key, value)   添加和修改属性
    ElementTree.write('filename.xml')   写出（更新）XMl文件
"""

updateTree = ET.parse("test.xml")   # 读取待修改文件
root = updateTree.getroot()

newEle = ET.Element("NewElement")   # 创建新节点并添加为root的子节点
newEle.attrib = {"name":"NewElement","age":"20"}
newEle.text = "This is a new element"
root.append(newEle)                 # 更新xml

sub1 = root.find("sub1")            # 修改sub1的name属性
sub1.set("name","New Name")

sub2 = root.find("sub2")            # 修改sub2的数据值
sub2.text = "New Value"


updateTree.write("test.xml")        # 写回原文件
```

```xml
test.xml文件内容:
<root><sub1 name="name attribute" /><sub2>test</sub2></root>

更新后：
<root><sub1 name="New Name" /><sub2>New Value</sub2><NewElement age="20" name="NewElement">This is a new element</NewElement></root>
```

#### xml 增删改查 – 综合案例

```python
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

def create_node(tag, property_map, content):
    '''新造一个节点
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
    ################ 1. 读取xml文件  ##########
    tree = read_xml("test_02.xml")

    ################ 2. 属性修改 ###############
    nodes = find_nodes(tree, "processers/processer")                   # 找到父节点
    result_nodes = get_node_by_keyvalue(nodes, {"name": "BProcesser"}) # 通过属性准确定位子节点
    change_node_properties(result_nodes, {"age": "1"})                 # 修改节点属性
    change_node_properties(result_nodes, {"value": ""}, True)          # 删除节点属性

    #################  3. 节点修改 ##############
    a = create_node("person", {"age": "15", "money": "200000"}, "this is the firest content") # 新建节点
    add_child_node(result_nodes, a) # 插入到父节点之下

    ################# 4. 删除节点 ################
    del_parent_nodes = find_nodes(tree, "processers/services/service")                            # 定位父节点
    target_del_node = del_node_by_tagkeyvalue(del_parent_nodes, "chain", {"sequency": "chain1"})  # 准确定位子节点并删除之

    ################# 5. 修改节点文本 ############
    text_nodes = get_node_by_keyvalue(find_nodes(tree, "processers/services/service/chain"), {"sequency": "chain3"}) # 定位节点
    change_node_text(text_nodes, "new text")

    ################ 6. 输出到结果文件  ##########
    write_xml(tree, "./xiugai.xml")
```

案例xml文件(修改前)

```xml
<?xml version="1.0" encoding="UTF-8"?>  
<framework>  
    <processers>  
        <processer name="AProcesser" file="lib64/A.so"  
            path="/tmp">  
        </processer>  
        <processer name="BProcesser" file="lib64/B.so" value="fordelete">  
        </processer>  
        <processer name="BProcesser" file="lib64/B.so2222222"/>  
          
        <services>  
            <service name="search" prefix="/bin/search?"  
                output_formatter="OutPutFormatter:service_inc">  
                  
                <chain sequency="chain1"/>  
                <chain sequency="chain2"></chain>  
            </service>  
            <service name="update" prefix="/bin/update?">  
                <chain sequency="chain3" value="fordelete"/>  
            </service>  
        </services>  
    </processers>  
</framework>
```

案例xml文件(修改后)

```xml
<?xml version='1.0' encoding='utf-8'?>
<framework>  
    <processers>  
        <processer file="lib64/A.so" name="AProcesser" path="/tmp">  
        </processer>  
        <processer age="1" file="lib64/B.so" name="BProcesser">  
        <person age="15" money="200000">this is the firest content</person></processer>  
        <processer age="1" file="lib64/B.so2222222" name="BProcesser"><person age="15" money="200000">this is the firest content</person></processer>  
          
        <services>  
            <service name="search" output_formatter="OutPutFormatter:service_inc" prefix="/bin/search?">  
                  
                <chain sequency="chain2" />  
            </service>  
            <service name="update" prefix="/bin/update?">  
                <chain sequency="chain3" value="fordelete">new text</chain>  
            </service>  
        </services>  
    </processers>  
</framework>
```

鸣谢
[http://www.runoob.com/python/python-xml.html](http://www.runoob.com/python/python-xml.html)
[https://www.cnblogs.com/AlwinXu/p/5483177.html](https://www.cnblogs.com/AlwinXu/p/5483177.html)

特别鸣谢
[https://blog.csdn.net/piaodexin/article/details/77371343](https://blog.csdn.net/piaodexin/article/details/77371343)



