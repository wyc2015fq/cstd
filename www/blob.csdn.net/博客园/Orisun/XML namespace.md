# XML namespace - Orisun - 博客园







# [XML namespace](https://www.cnblogs.com/zhangchaoyang/articles/1793752.html)





## 命名冲突

由于 XML 中的元素名是预定义的，当两个不同的文档使用相同的元素名时，就会发生命名冲突。

这个 XML 文档携带着某个表格中的信息：


```
1 <table>
2    <tr>
3        <td>Apples</td>
4        <td>Bananas</td>
5    </tr>
6  </table>
```


这个 XML 文档携带有关桌子的信息（一件家具）：


```
1 <table>
2    <name>African Coffee Table</name>
3    <width>80 </width>
4    <length>120</length>
5  </table>
```




假如这两个 XML 文档被一起使用，由于两个文档都包含带有不同内容和定义的 <table> 元素，就会发生命名冲突。


## 使用前缀来避免命名冲突

此文档带有某个表格中的信息：


```
1 <h:table>
2    <h:tr>
3    <h:td>Apples</h:td>
4    <h:td>Bananas</h:td>
5    </h:tr>
6  </h:table>
```


此 XML 文档携带着有关一件家具的信息：


```
<f:table>
   <f:name>African Coffee Table</f:name>
   <f:width>80 </f:width>
   <f:length>120</f:length>
</f:table>
```




现在，命名冲突不存在了，这是由于两个文档都使用了不同的名称来命名它们的 <table> 元素 (<h:table> 和 <f:table>)。

通过使用前缀，我们创建了两种不同类型的 <table> 元素。



## 使用命名空间（Namespaces）

这个 XML 文档携带着某个表格中的信息：


```
1 <h:table xmlns:h="http://www.w3.org/TR/html4/">
2    <h:tr>
3    <h:td>Apples</h:td>
4    <h:td>Bananas</h:td>
5    </h:tr>
6  </h:table>
```


此 XML 文档携带着有关一件家具的信息：


```
<f:table xmlns:f="http://www.w3school.com.cn/furniture">

    <f:name>African Coffee Table</f:name>
    <f:width>80</f:width>
    <f:length>120</f:length>
</f:table>
```




与仅仅使用前缀不同，我们为 <table> 标签添加了一个 xmlns 属性，这样就为前缀赋予了一个与某个命名空间相关联的限定名称。



## XML Namespace (xmlns) 属性

XML 命名空间属性被放置于某个元素的开始标签之中，并使用以下的语法：


```
xmlns:namespace-prefix="namespaceURI"
```


当一个命名空间被定义在某个元素的开始标签中时，所有带有相同前缀的子元素都会与同一个命名空间相关联。

注释：用于标示命名空间的地址不会被解析器用于查找信息。其惟一的作用是赋予命名空间一个惟一的名称。不过，很多公司常常会作为指针来使用命名空间指向某个实存的网页，这个网页包含着有关命名空间的信息。













