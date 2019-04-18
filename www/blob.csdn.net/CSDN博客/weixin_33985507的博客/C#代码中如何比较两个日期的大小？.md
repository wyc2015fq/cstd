# C#代码中如何比较两个日期的大小？ - weixin_33985507的博客 - CSDN博客
2018年07月28日 18:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
C#中比较两个日期大小的方法
```
/// <summary>
        /// 比较两个日期大小
        /// </summary>
        /// <param name="dateStr1">日期1</param>
        /// <param name="dateStr2">日期2</param>
        /// <param name="msg">返回信息</param>
        public void CompanyDate(string dateStr1, string dateStr2,ref string msg) {
            //将日期字符串转换为日期对象
            DateTime t1 = Convert.ToDateTime(dateStr1);
            DateTime t2 = Convert.ToDateTime(dateStr2);
            //通过DateTIme.Compare()进行比较（）
            int compNum = DateTime.Compare(t1, t2);
            //t1> t2
            if (compNum > 0) {
                msg = "t1:(" + dateStr1 + ")大于" + "t2(" + dateStr2 + ")";              
            }
            //t1= t2
            if (compNum == 0) {
                msg = "t1:(" + dateStr1 + ")等于" + "t2(" + dateStr2 + ")";
            }
            //t1< t2
            if (compNum < 0) {
                msg = "t1:(" + dateStr1 + ")小于" + "t2(" + dateStr2 + ")";
            }
        }
```
调用的代码
```
public ActionResult Index()
        {
            //t1< t2
            string msg1 = string.Empty;
            string date1 = "2018-07-25";
            string date2 = "2018-07-27";
            CompanyDate(date1,date2,ref msg1);
            
            //t1> t2
            string msg2 = string.Empty;
            string date3 = "2018-07-27";
            string date4 = "2018-07-25";
            CompanyDate(date3, date4, ref msg2);
            return View();
        }
```
整体demo源码
```
public class TestController : Controller
    {
        // GET: Test
        public ActionResult Index()
        {
            //t1< t2
            string msg1 = string.Empty;
            string date1 = "2018-07-25";
            string date2 = "2018-07-27";
            CompanyDate(date1,date2,ref msg1);
            
            //t1> t2
            string msg2 = string.Empty;
            string date3 = "2018-07-27";
            string date4 = "2018-07-25";
            CompanyDate(date3, date4, ref msg2);
            return View();
        }
        /// <summary>
        /// 比较两个日期大小
        /// </summary>
        /// <param name="dateStr1">日期1</param>
        /// <param name="dateStr2">日期2</param>
        /// <param name="msg">返回信息</param>
        public void CompanyDate(string dateStr1, string dateStr2,ref string msg) {
            //将日期字符串转换为日期对象
            DateTime t1 = Convert.ToDateTime(dateStr1);
            DateTime t2 = Convert.ToDateTime(dateStr2);
            //通过DateTIme.Compare()进行比较（）
            int compNum = DateTime.Compare(t1, t2);
            //t1> t2
            if (compNum > 0) {
                msg = "t1:(" + dateStr1 + ")大于" + "t2(" + dateStr2 + ")";              
            }
            //t1= t2
            if (compNum == 0) {
                msg = "t1:(" + dateStr1 + ")等于" + "t2(" + dateStr2 + ")";
            }
            //t1< t2
            if (compNum < 0) {
                msg = "t1:(" + dateStr1 + ")小于" + "t2(" + dateStr2 + ")";
            }
        }
    }
```
具体效果图：
1）图1
![](https://images2018.cnblogs.com/blog/775247/201807/775247-20180728185441620-1101087961.png)
2）图2
![](https://images2018.cnblogs.com/blog/775247/201807/775247-20180728185524165-1398795598.png)
