# Linq操作带属性的XML - ggabcda的博客 - CSDN博客





2017年09月02日 16:45:41[ggabcda](https://me.csdn.net/ggabcda)阅读数：125








      关于XML，之前解析过电文收发方面的，就是所谓的带表头、前缀（命名空间）SOAP格式。这次需求是解析一个xml的模板（xls内容），然后填充数据，最后保存。需要时可转换xls、pdf等文件。关于这种带属性xml文件， 只要了解linq操作的方法， 在參照微软官网上有样例，XAttribute类就是定义属性。基本上多数都可以解决。详细方法，随笔XML分类中，介绍的比较详细，至于今天要介绍的xls内容的xml文件的解析，以及创建节点也不在分篇，大致介绍下整体的思路。

比如其中某个定义表节点：

```
<cto:Worksheet cto:Name="工作表1">
    <cto:Table cto:ExpandedColumnCount="12" cto:ExpandedRowCount="3" x:FullColumns="1" 
    x:FullRows="1" cto:DefaultColumnWidth="54" cto:DefaultRowHeight="16.5">
      <cto:Column cto:Width="69.75" />
      <cto:Column cto:AutoFitWidth="0" cto:Width="84" />
      <cto:Column cto:AutoFitWidth="0" cto:Width="66" />
      <cto:Column cto:Width="69.75" />
      <cto:Column cto:Index="6" cto:Width="69.75" cto:Span="1" />
      <cto:Column cto:Index="8" cto:Width="57" />
      <cto:Column cto:Width="69.75" />
      <cto:Column cto:Width="71.25" />
      <cto:Row cto:Height="17.25">
        <cto:Cell cto:StyleID="s19"><cto:Data cto:Type="String">欄位1</cto:Data></cto:Cell>
        <cto:Cell cto:StyleID="s18"><cto:Data cto:Type="String">欄位2</cto:Data></cto:Cell>
        <cto:Cell cto:StyleID="s18"><cto:Data cto:Type="String">欄位3</cto:Data></cto:Cell>
        <cto:Cell cto:StyleID="s18"><cto:Data cto:Type="String">欄位4</cto:Data></cto:Cell>
        <cto:Cell cto:StyleID="s18"><cto:Data cto:Type="String">欄位5</cto:Data></cto:Cell>
      </cto:Row>
      <cto:Row>
        <cto:Cell><cto:Data cto:Type="String">1001</cto:Data></cto:Cell>
        <cto:Cell><cto:Data cto:Type="String">1002</cto:Data></cto:Cell>
        <cto:Cell><cto:Data cto:Type="String">1003</cto:Data></cto:Cell>
        <cto:Cell><cto:Data cto:Type="String">1004</cto:Data></cto:Cell>
        <cto:Cell><cto:Data cto:Type="String">1005</cto:Data></cto:Cell>
      </cto:Row>
    </cto:Table>
```

这种xls类型的xml，用Linq来解析，和之前的Soap格式的并无二致，均是XElement 加载，命名空间+节点名 来获取集合，在通过栏位值、属性值等来获取到某个指定的节点，最后进行设置。

如想让栏位1 的值改成 列名1，操作如下

```
XNamespace ss = @"xxxx...";
          //獲取待賦值集合
          var setcode = from d in xml.Descendants(cto + "Data")
                select d;
          //給固定表頭賦值                
          foreach (var item in setcode.ToList())
          {
             if (item.Value == "欄位1")
             {
                    item.SetValue("列名1");
             }
          }
```

可见并没有任何质的改变，也就是所谓 命名空间+节点 格式，除此之外，该xml更多的定义了表的规范，长度，宽度，栏位行数、列数、节点属性（不可小看，后面会用到）之类的，看的人眼花缭乱。这种格式的xml，解析之后当然是往其中填充数据，比如从某表中导出某条件下的多笔数据，将其填充至此xml中。

       仔细分析，该xml的表节点定义了一行列名，一行预览值，一般分两种情况：如果是单笔数据，那就是将预览行的值替换为表中捞取的数据；如果是多笔，从第三行开始，循环创建节点并赋值，最后将其节点插入预览行同级节点的下一个节点。其实没必要这样，直接循环组一个节点并赋值，然后插入预览行下放，最后删除预览行即可。

(ps:row 为读取DataTable行数)

```
//创建节点              
                XElement addElement = null;
                //填充节点
                for (int i = 0; i < row; i++)
                {
                    addElement = new XElement(cto+ "Row", 
                        new XElement(cto + "Cell",
                             new XAttribute(cto + "Index", "2"),
                            new XElement(cto + "Data", new XAttribute(cto+ "Type", "String"), dt.Rows[i][0])),
                        new XElement(cto + "Cell",
                            new XElement(cto+ "Data", new XAttribute(cto+ "Type", "String"), dt.Rows[i][1])),
                        new XElement(cto+ "Cell",
                            new XElement(cto+ "Data", new XAttribute(cto+ "Type", "String"), dt.Rows[i][2])),
                        new XElement(cto+ "Cell",
                            new XElement(ss + "Data", new XAttribute(cto+ "Type", "String"), dt.Rows[i][3])),
                        new XElement(cto+ "Cell",
                            new XElement(cto+ "Data", new XAttribute(cto+ "Type", "String"), dt.Rows[i][4]))
                    );
                    //插入數據
                    set.FirstOrDefault().AddAfterSelf(addElement);
                }
                //刪除預覽行
                set.Remove();
                //保存文件                                                         
                xml.Save(newxmlPath);
```

写到这里，本以为XML已填充完成，实则不然，这里我也是踩到坑，后来将其xml转xls的时候，一直包失败，无法打开调用时的excle。网上有的说是WPS的锅，后来卸载，然后安装Office 又遇到了同样的错误。后来小组长中终于找到了其中的原因，是XML属性的锅！！！ 前面提到xml定义表格的时候，指明了行数，cto:ExpandedRowCount=“3” ，这样当插入节点中的行数超过了这个设置值时，打开excle就会出错。问题原因就在这里。所以在插入节点之后保存xml之前，一定要重新设置其这个值，前面的方法已经介绍过如何设置值，这里就不在多说。

       当将xml转换为xls格式的时候，方法有很多中，nopi 实际上是创建一个xls，然后按照一行一列的赋值，设置样式最后保存文件。对于本就是xls内容的xml使用nopi方法是十分愚蠢也是不可行的。总不能挨个解析其中xml的每个节点值和属性值，然后挨个在xls中赋值。其次还有 spire方式，这种方法是先去官网注册然后下载，安装，最后将安装文件中的dll添加至项目引用中，功能和强大，需要转xls就引用xls的dll，需要转pdf就引用pdf的dll
 等等，代码也只需要三行，简洁又方便，CSDN中不少博主立推这种方法，我想说的是，这是一个坑，没错，就是一个坑，因为spire插件转xls 只支持200行，如果想要更多的话需要买会员！！！而且转PDF的时候，左下角会有水印之类的spire的标签以及官网地址...

对于此类Xml格式，以上两种均不可行，然后就剩下第三种，引用 Microsoft.Office.Interop.Excel
插件，写几行代码，原理大致就是，用本地的excle打开需要转换的xml，然后保存成xls格式。

不少园主对这种方法都有说明介绍，代码量也很少，这里就不贴了。毕竟这里我也是复制别人的代码。。。

      对于转pdf格式，转换之后是分页的，比较纳闷，好好的一个表格，从中间切开，这种问题某度上面也是说法不一，当然是excle表长度的问题，不得不说还是小组长厉害，在茫茫xml中定义电子表格的时候，添加一个节点，设置纸张方向为横向 <Layout
 x:Orientation="Landscape"/>，在调节下编剧大小即可满足pdf的标准产出。

谨以此文记录操作xls内容的xml文件时的一些方法以及坑



---市人皆大笑，举手揶揄之



