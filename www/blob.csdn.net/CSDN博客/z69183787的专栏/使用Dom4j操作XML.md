# 使用Dom4j操作XML - z69183787的专栏 - CSDN博客
2014年01月20日 16:52:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：764
个人分类：[Dom4j](https://blog.csdn.net/z69183787/article/category/2229189)

要使用dom4j操作XML的话必须导入一个dom4j-1.6.1的jar包。
还有，如果要对XML进行CUD操作的话必须执行：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1 private static void save(Document document) throws FileNotFoundException,
2             UnsupportedEncodingException, IOException {
3         //创建保存的文档
4         FileOutputStream out = new FileOutputStream("src/操作XML/person.xml");
5         XMLWriter writer = new XMLWriter(out);
6         //添加内容对象
7         writer.write(document);
8         writer.close();
9     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
解析内容
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 //获得解析流
 2         SAXReader reader = new SAXReader();
 3         //获得解析的文档
 4         Document document = reader.read("src/操作XML/person.xml");
 5         //获得根元素
 6         Element rootElement = document.getRootElement();
 7         //获得所有chinese元素
 8         List allChinese = rootElement.elements("chinese");
 9         //遍历所有chinese元素
10         for (int i = 0; i < allChinese.size(); i++) {
11             //获得每一个chinese元素
12             Element chineseElement = (Element)allChinese.get(i);
13             //获得chinese元素的属性值
14             String id = chineseElement.attributeValue("id");
15             System.out.println(id);
16             //获得每个chinese下的子元素
17             List childList =  chineseElement.elements();
18             //遍历子元素集合
19             for (int j = 0; j < childList.size(); j++) {
20                 //获得每一个子元素
21                 Element childElement = (Element)childList.get(j);
22                 //获得子元素的内容
23                 String content = childElement.getTextTrim();
24                 System.out.println(content);
25             }
26         }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
创建元素、属性
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 private static void create() throws DocumentException {
 2         //获得解析器
 3         SAXReader reader = new SAXReader();
 4         //获得document
 5         Document document = reader.read("src/操作XML/person.xml");
 6         //获得根元素
 7         Element rootElement = document.getRootElement();
 8         //创建chinese元素
 9         Element newElement = DocumentHelper.createElement("chinese");
10         //创建chinese元素的属性id
11         Attribute arr = DocumentHelper.createAttribute(newElement, "id", "004");
12         //将属性添加到chinese元素中
13         newElement.add(arr);
14         //创建name元素
15         Element childElement = DocumentHelper.createElement("name");
16         //给元素设置内容
17         childElement.setText("张三");
18         //将元素添加到chinese中
19         newElement.add(childElement);
20         //将chinese元素添加到根元素下
21         rootElement.add(newElement);
22     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
修改内容
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1     private static void update() throws DocumentException {
 2         //获得解析器
 3         SAXReader reader = new SAXReader();
 4         //获得document
 5         Document document = reader.read("src/操作XML/person.xml");
 6         //获得需要修改的节点
 7         Node node = document.selectSingleNode("//chinese[@id='001']");
 8         //将节点强制转换成元素
 9         Element chineseElement = (Element)node;
10         //获得chinese下的name元素
11         Element nameElement = chineseElement.element("name");
12         //修改name元素的内容
13         nameElement.setText("张三");
14     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
删除元素
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 private static void delete() throws DocumentException {
 2         //获得解析器
 3         SAXReader reader = new SAXReader();
 4         //获得document
 5         Document document = reader.read("src/操作XML/person.xml");
 6         //获得指定的节点
 7         Node node = document.selectSingleNode("//name[@type='1']");
 8         if(null!=node)
 9         {
10             //获得父节点
11             Node parent = node.getParent();
12             //将父节点转换为父元素
13             Element parentElement= (Element)parent;
14             //删除节点
15             parentElement.remove(node);
16         }
17     }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 在这里提最后一点，上面的代码有些是这样的
  Node node = document.selectSingleNode("//chinese[@id='001']");
这些是Xpath，这里我就不介绍了。大家可以去下载个w3c API。看看
