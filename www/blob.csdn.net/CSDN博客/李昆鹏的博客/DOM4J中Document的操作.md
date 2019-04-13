
# DOM4J中Document的操作 - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:55:32[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1244


---------------------------------------DOM4J中Document的操作--------------------------------
### Document操作 (*****)

#### 1　遍历students.xml
涉及的相关方法：
l  Element getRootElement()：Document的方法，用来获取根元素；
l  List elements()：Element的方法，用来获取所有孩子元素；
l  String attributeValue(String name)：Element的方法，用来获取指定名字的属性值；
l  Element element(String name)：Element的方法，用来获取第一个指定名字的子元素；
l  String elementText(String name)：Element的方法，用来获取第一个指定名字的子元素的文本内容。
分析步骤：
l  获取Document对象；
l  获取root元素；
l  获取root所有子元素
l  遍历每个student元素；
Ø  打印student元素number属性；
Ø  打印student元素的name子元素内容；
Ø  打印student元素的age子元素内容；
Ø  打印student元素的sex子元素内容。

#### 2　给学生元素添加<score>子元素
涉及的相关方法：
l  Element addElement(String name)：Element的方法，为当前元素添加指定名字子元素。返回值为新建元素对象；
l  setText(String text)：Element的方法，为当前元素设置文本内容。
分析步骤：
l  获取Document对象；
l  获取root对象；
l  获取root所有子元素；
l  遍历所有学生子元素；
Ø  创建<score>元素，为<score>添加文本内容；
Ø  把<score>元素添加到学生元素中。
l  保存Document对象。

#### 3
#### 为张三添加
#### friend属性，指定为李四学号
涉及方法：
l  addAttribute(String name, String value)：Element的方法，为当前元素添加属性。
分析步骤：
l  获取Document对象；
l  获取root对象；
l  获取root所有子元素；
l  创建两个Element引用：zhangSanEle、liSiEle，赋值为null；
l  遍历所有学生子元素；
Ø  如果zhangSanEle和liSiEle都不是null，break；
Ø  判断当前学生元素的name子元素文本内容是zhangSan，那么把当前学生元素赋给zhangSanEle；
Ø  判断当前学生元素的name子元素文本内容是liSi，那么把当前学生元素赋给liSiEle。
l  判断zhangSanEle和liSiEle都不为null时：
Ø  获取liSiEle的number属性。
Ø  为zhangSanEle添加friend属性，属性值为liSi的number属性值。
Ø  保存Document对象。

#### 4　删除number为ID_1003的学生元素
涉及方法：
l  boolean remove(Element e)：Element和Document的方法，移除指定子元素；
l  Element getParent()：获取父元素，根元素的父元素为null。
分析步骤：
l  获取Document对象；
l  获取root对象；
l  获取root所有子元素；
l  遍历所有学生子元素；
Ø  判断当前学生元素的number属性是否为ID_1003；
²  获取当前元素的父元素；
²  父元素中删除当前元素；
l  保存Document对象.

#### 5　通过List<Student>生成Document并保存
涉及方法：
l  DocumentHelper.createDocument()：创建Document对象；
l  DocumentHelper.createElement(String name)：创建指定名称的Element元素。

分析步骤：
l  创建Document对象；
l  为Document添加根元素<students>；
l  循环遍历学生集合List<Student>；
Ø  把当前学生对象转换成Element元素；
Ø  把Element元素添加到根元素中；
l  保存Document对象。
把学生转换成Element步骤分析：
l  创建Element对象；
l  为Element添加number属性，值为学生的number；
l  为Element添加name子元素，文本内容为学生的name；
l  为Element添加age子元素，文本内容为学生的age；
l  为Element添加sex子元素，文本内容为学生的sex。

#### 6　新建赵六学生元素，插入到李四之前
涉及方法：
l  int indexOf(Node node)：Branch的方法，查找指定节点，在当前Branch的子节点集合中的下标位置。
分析步骤：
l  创建赵六学生对象；
l  通过学生对象创建赵六学生元素；
l  通过名称查找李四元素；
l  查看李四元素在其父元素中的位置；
l  获取学生子元素List；
l  将赵六元素插入到List中。
通过名字查找元素：
l  获取Document；
l  获取根元素；
l  获取所有学生元素；
l  遍历学生元素；
Ø  获取学生元素name子元素的文本内容，与指定名称比较；
²  返回当前学生元素。

#### 7　其它方法介绍
Node方法：
l  String asXML()：把当前节点转换成字符串，如果当前Node是Document，那么就会把整个XML文档返回；
l  String getName()：获取当前节点名字；Document的名字就是绑定的XML文档的路径；Element的名字就是元素名称；Attribute的名字就是属性名；
l  Document getDocument()：返回当前节点所在的Document对象；
l  short getNodeType()：获取当前节点的类型；
l  String getNodeTypeName()：获取当前节点的类型名称，例如当前节点是Document的话，那么该方法返回Document；
l  String getStringValue()：获取当前节点的子孙节点中所有文本内容连接成的字符串；
l  String getText()：获取当前节点的文本内容。如果当前节点是Text等文本节点，那么本方法返回文本内容；例如当前节点是Element，那么当前节点的内容不是子元素，而是纯文本内容，那么返回文本内容，否则返回空字符串；
l  void setDocument(Document doc)：给当前节点设置文档元素；
l  void setParent(Element parent)：给当前节点设置父元素；
l  void setText(String text)：给当前节点设置文本内容；

Branch方法：
l  void add(Element e)：添加子元素；
l  void add(Node node)：添加子节点；
l  void add(Comment comment)：添加注释；
l  Element addElement(String eleName)：通过名字添加子元素，返回值为子元素对象；
l  void clearContent()：清空所有子内容；
l  List content()：获取所有子内容，与获取所有子元素的区别是，<name>liSi</name>元素没有子元素，但有子内容；
l  Element elementById(String id)：如果元素有名为“ID”的属性，那么可以使用这个方法来查找；
l  int indexOf(Node node)：查找子节点在子节点列表中的下标位置；
l  Node node(int index)：通过下标获取子节点；
l  int nodeCount()：获取子节点的个数；
l  Iterator nodeIterator()：获取子节点列表的迭代器对象；
l  boolean remove(Node node)：移除指定子节点；
l  boolean remove(Commont commont)：移除指定注释；
l  boolean remove(Element e)：移除指定子元素；
l  void setContent(List content) ：设置子节点内容；

Document方法：
l  Element getRootElement()：获取根元素；
l  void setRootElement()：设置根元素；
l  String getXmlEncoding()：获取XML文档的编码；
l  void setXmlEncoding()：设置XML文档的编码；

Element方法：
l  void add(Attribute attr)：添加属性节点；
l  void add(CDATA cdata)：添加CDATA段节点；
l  void add(Text Text)：添加Text节点；
l  Element addAttribute(String name, String value)：添加属性，返回值为当前元素本身；
l  Element addCDATA(String cdata)：添加CDATA段节点；
l  Element addComment(String comment)：添加属性节点；
l  Element addText(String text)：添加Text节点；
l  void appendAttributes(Element e)：把参数元素e的所有属性添加到当前元素中；
l  Attribute attribute(int index)：获取指定下标位置上的属性对象；
l  Attribute attribute(String name)：通过指定属性名称获取属性对象；
l  int attributeCount()：获取属性个数；
l  Iterator attributeIterator()：获取当前元素属性集合的迭代器；
l  List attributes()：获取当前元素的属性集合；
l  String attributeValue(String name)：获取当前元素指定名称的属性值；
l  Element createCopy()：clone当前元素对象，但不会copy父元素。也就是说新元素没有父元素，但有子元素；
l  Element element(String name)：获取当前元素第一个名称为name的子元素；
l  Iterator elementIterator()：获取当前元素的子元素集合的迭代器；
l  Iterator elementIterator(String name)：获取当前元素中指定名称的子元素集合的迭代器；
l  List elements()：获取当前元素子元素集合；
l  List elements(String name)：获取当前元素指定名称的子元素集合；
l  String elementText(String name)：获取当前元素指定名称的第一个元素文件内容；
l  String elementTextTrime(String name)：同上，只是去除了无用空白；
l  boolean isTextOnly()：当前元素是否为纯文本内容元素；
l  boolean remove(Attribute attr)：移除属性；
l  boolean remove(CDATA cdata)：移除CDATA；
l  boolean remove(Text text)：移除Text。

DocumentHelper静态方法介绍：
l  static Document createDocument()：创建Dcoument对象；
l  static Element createElement(String name)：创建指定名称的元素对象；
l  static Attribute createAttrbute(Element owner, String name, Stringvalue)：创建属性对象；
l  static Text createText(String text)：创建属性对象；
l  static Document parseText(String text)：通过给定的字符串生成Document对象；


