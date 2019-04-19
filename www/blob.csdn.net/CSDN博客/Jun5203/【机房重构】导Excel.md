# 【机房重构】导Excel - Jun5203 - CSDN博客
2019年03月11日 08:00:16[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：45
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
昨天晚上get到一项新技能——导出Excel，我们在查看账单及工作人员上班记录时都会用到这个功能，为了减少代码的冗余，我们可以把导出Excel的代码封装在一个类中，当需要导出Excel表的时候再去调用这个类中的方法即可，下面就来看一下我是怎么实现的吧！
**【U层添加引用】**
![](https://img-blog.csdnimg.cn/20190311075233788.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**【封装类】**
第一步：在U层添加一个ToExcel类，以便导出时调用
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190311075409350.jpg)
第二步：类中添加命名空间
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031107545934.jpg)
第三步：代码
```
public partial class ToExcel
    {
        public void DatatoExcel(DataGridView dgv)
        {
            if (dgv.Rows.Count == 0)   //判断控件中是否有数据。
            {
                MessageBox.Show("没有数据");
                return;
            }
            Excel.Application excel = new Excel.Application();   //实例化
            excel.Application.Workbooks.Add(true);
            excel.Visible = true;
            for (int i = 0; i < dgv.ColumnCount; i++)
            {
                excel.Cells[1, i + 1] = dgv.Columns[i].HeaderText;    //添加第一行的内容=表头
            }
            for (int i = 0; i < dgv.RowCount; i++)
            {
                for (int j = 0; j < dgv.ColumnCount; j++)
                {
                    excel.Cells[i + 2, j + 1] = dgv[j, i].Value;
                }
            }
        }
    }
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190311075906860.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
**【小结】**
推动你向前的，不是困难，而是梦想。过一种你想要的生活，做一个许多年后值得回忆的人。
