# PADS下建立器件类型时怎么快速输入管脚名称----使用CSV/Excel/Txt格式文件 - xqhrs232的专栏 - CSDN博客
2015年03月22日 10:23:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3778
相关文章
1、[PADS2007小技巧收集----本人总结](http://blog.csdn.net/xqhrs232/article/details/8778834)----[http://blog.csdn.net/xqhrs232/article/details/8778834](http://blog.csdn.net/xqhrs232/article/details/8778834)-----选择搜索CSV这个关键词！！！
2、**[PADS] 使用 Excel 快速建立 PADS Logic 管脚信息 (GIF)----[http://www.amobbs.com/thread-5604178-1-1.html](http://www.amobbs.com/thread-5604178-1-1.html)**
3、PADS9.5-CAE封装如何导入CSV----[http://bbs.elecfans.com/jishu_456896_1_1.html](http://bbs.elecfans.com/jishu_456896_1_1.html)
建立PART TYPE时，怎么导入CSV文件----手机电话本也采用这种格式文件存储
    //1111----CSV是文本格式的表格，本质就是txt，写完数据后把txt的扩展名改成csv就可以。 常见的可以用excel之类的电子表格，选择文件另存为CSV格式。
   //2222----不知道要建立多少行，也不知道需要输入那些数据
   //3333----自己弄了一下老是报错，然后也导入不成功
   Warning: CSV file is not valid  format for pin data
  //4444----其实也可以不用CSV文件的方式----用EXCEL也可以很方便的
    a.用EXCEL的一列存储好PIN NAME然后COPY整列再PASTE到NAME(这个时候只需要选中一行就可以把整列PASTE过来的，不需要选中需要全部PASTE的行数)
 b.怎么在EXCEL里面COPY一行行，怎么COPY一整列
  //5555----在TXT里面按行输入好也可以整个一起COPY然后PASTE到PART TYPE的NAME的
 //6666----选中Pin Group全部的列，然后点右边的Edit，可以统一选择是设置为Gate-A/Signal Pin/Unused pin其中的一种，这样就不用一个个去选择了
