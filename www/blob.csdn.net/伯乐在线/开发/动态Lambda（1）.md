# 动态Lambda（1） - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4923216.html)
前言：在DDD系列文章里面，我们在后台仓储里面封装了传递Lamada表达式的通用方法，类似这样：

C#
```
public virtual IQueryable Find(Expressionbool>> express)
  {
     Funcbool> lamada = express.Compile();
     return UnitOfWork.context.Set().Where(lamada).AsQueryable();
  }
```
通过前端传过来的Lamada表达式，直接放到Where条件里面查询。那么问题来了，我们前端如何传入Lamada呢？当然，有人说了，这个不用传啊，前端直接.Find(x=>x.Name==”abc”)这样写就好了啊。确实，如果前端条件只有一个条件，你确实可以这样简单处理，但是实际开发的过程中，我们很多时候是需要传递多个参数，并且.Find(x=>x.Name==”abc”)这种写法也不利于方法的封装。于是，我们神奇的动态Lamada诞生了。
## 一、再谈Lamada表达式
### 1、匿名委托
之前在介绍委托的时候我们介绍过一种特殊的匿名委托，它型如：

C#
```
class Program
    {
        private delegate void SayHello(string name);
        static void Main(string[] args)
        {
            Say("张三", delegate(string name)
            {
                Console.WriteLine("你好，" + name);
            });
            Say("Zhangsan", delegate(string name)
            {
                Console.WriteLine("Hello，" + name);
            });    
        }
        static void Say(string name,SayHello dTest)
        {
            dTest(name);
        }  
    }
```
也就是说，不用定义一种具体的委托方法去对应SayHello(string name);，而直接delegate(string name){}这样定义一种匿名的委托去执行，这样能减少部分定义具体方法的代码。
### 2、Lamada表达式进化史
了解了匿名委托的概念，我们来看看我们经常使用的Linq里面的扩展方法Where、Select等。先来看看一般用法：

C#
```
var lstTest = new Liststring>();//.......业务逻辑
var lstRes = lstTest.Where(x => x.Contains("_"));
```
我们来将Where里面的x => x.Contains(“_”)分解。
#### 初级进化（最原始的匿名委托形式）：

C#
```
Funcstring, bool> oFunc = delegate(string x) { return x.Contains("_"); };
lstRes = lstTest.Where(oFunc);
```
#### 高级进化（型如Lamada，但还有匿名委托的影子）：

C#
```
Funcstring, bool> oFunc = (string x) => { return x.Contains("_"); };
lstRes = lstTest.Where(oFunc);
```
#### 究极进化（完完全全的Lamada）

C#
```
Funcstring, bool> oFunc = x => x.Contains("_");
lstRes = lstTest.Where(oFunc);
```
有没有很强大，是不是一样一样的。其实这样一看lamada就是匿名委托的缩略形式。x => x.Contains(“_”)表达式左边的表示Func里面的string类型变量，x.Contains(“_”)表示bool类型的返回值。有了这份进化史，程序员再也不用担心面试官问我Lamada怎么回事了。
## 二、动态Lamada
与其叫动态Lamada，更加严谨一点应该叫动态Expression，因为拼接Lamada表达式用的基本都是Expression的类和方法。博主习惯，暂且就叫它动态Lamada吧。废话不多说，直接吃点栗子吧。

C#
```
public class DTO_ORDER
    {
        public string TO_ORDER_ID { get; set; }
        public string ORDER_NO { get; set; }
        public string ORDER_NAME { get; set; }
        public int ORDER_STATUS {get;set;}
    }
　　static void Main()
   {
            //1.定义lamada的参数，型如我们常写的“x=>”
            ParameterExpression m_Parameter = Expression.Parameter(typeof(DTO_ORDER), "x");
            //2.定义要使用lamada的属性成员（比如我们这里要对DTO_ORDER对象的ORDER_NO属性做筛选）
            MemberExpression member = Expression.PropertyOrField(m_Parameter, "ORDER_NO");
            //3.定义筛选的操作（是大于、等于、小于、like等）
            Expression expRes = Expression.Equal(member, Expression.Constant("aaaa", member.Type));
            //4.将表达式转换为Lamada的表达式
            Expressionbool>> exprelamada = Expression.Lambdabool>>(expRes, m_Parameter);
            var lstRes = new List();
            for (var i = 0; i 10; i++)
            {
                var oModel = new DTO_ORDER();
                oModel.ORDER_NO = i % 2 == 0 ? "aaaa" : "bbbb";
                lstRes.Add(oModel);
            }
            //5.将Expression表达式转换为Func委托，用于Where里面的参数
            var lamada = exprelamada.Compile();
            lstRes = lstRes.Where(lamada).ToList();
　　}
```
以上就构造了一个查询List对象里面ORDER_NO 属性等于aaaa的lamada表达式。我们看看运行效果截图：
![](http://jbcdn2.b0.upaiyun.com/2016/04/1e611cd64538b2e60852f4f09ee57235.png)
是不是已经得到了我们想要的表达式！有没有很简单。。。
## 三、动态Lamada的使用
看到这里有人就郁闷了，为了得到x=>x.ORDER_NO==”aaaa”这种表达式，你绕了这么大一圈，有什么屌用？直接lstRes=lstRes.Where(x=>x.ORDER_NO==”aaaa”);就能够搞定的事，你非要把简单问题复杂化。其实不然，有一定编程经验的朋友肯定知道，**一般我们前端传过来的查询参数肯定不会只有一个，当需要查询多个参数时需要我们构造一个统一的Lamada传递到后台；当然你也可以说，我将多个参数全部传递到后台，然后再后台使用IQueryable接口去过滤。当然，这确实可行，但是别忘了我们封装Find(Expression exp…)的意义，不就是为了简化方法么，从这点来说，构造动态Lamada非常必要。**
### 1、通用Lamada表达式类
博主封装了一个简单操作（大于、等于、小于、like等）的动态Lamada类。

C#
```
public class LamadaExtentionwhere Dto:new ()
    {
        private List m_lstExpression = null;
        private ParameterExpression m_Parameter = null;
        public LamadaExtention()
        {
            m_lstExpression = new List();
            m_Parameter = Expression.Parameter(typeof(Dto), "x");
        }
　　　　 //构造表达式，存放到m_lstExpression集合里面
        public void GetExpression(string strPropertyName, object strValue, ExpressionType expressType)
        {
            Expression expRes = null;
            MemberExpression member = Expression.PropertyOrField(m_Parameter, strPropertyName);
            if (expressType == ExpressionType.Contains)
            {
                expRes = Expression.Call(member, typeof(string).GetMethod("Contains"), Expression.Constant(strValue));
            }
            else if (expressType == ExpressionType.Equal)
            {
                expRes = Expression.Equal(member, Expression.Constant(strValue, member.Type));
            }
            else if (expressType == ExpressionType.LessThan)
            {
                expRes = Expression.LessThan(member, Expression.Constant(strValue, member.Type));
            }
            else if (expressType == ExpressionType.LessThanOrEqual)
            {
                expRes = Expression.LessThanOrEqual(member, Expression.Constant(strValue, member.Type));
            }
            else if (expressType == ExpressionType.GreaterThan)
            {
                expRes = Expression.GreaterThan(member, Expression.Constant(strValue, member.Type));
            }
            else if (expressType == ExpressionType.GreaterThanOrEqual)
            {
                expRes = Expression.GreaterThanOrEqual(member, Expression.Constant(strValue, member.Type));
            }
            //return expRes;
            m_lstExpression.Add(expRes);
        }
        //针对Or条件的表达式
        public void GetExpression(string strPropertyName, Listobject> lstValue)
        {
            Expression expRes = null;
            MemberExpression member = Expression.PropertyOrField(m_Parameter, strPropertyName);
            foreach (var oValue in lstValue)
            {
                if (expRes == null)
                {
                    expRes = Expression.Equal(member, Expression.Constant(oValue, member.Type));
                }
                else
                {
                    expRes = Expression.Or(expRes, Expression.Equal(member, Expression.Constant(oValue, member.Type)));
                }
            }
            m_lstExpression.Add(expRes);
        }
　　　　 //得到Lamada表达式的Expression对象
        public Expressionbool>> GetLambda()
        {
            Expression whereExpr = null;
            foreach (var expr in this.m_lstExpression)
            {
                if (whereExpr == null) whereExpr = expr;
                else whereExpr = Expression.And(whereExpr, expr);
            }
            if (whereExpr == null)
                return null;
            return Expression.Lambda>(whereExpr, m_Parameter);
        }
    }
　　 //用于区分操作的枚举
    public enum ExpressionType
    {
        Contains,//like
        Equal,//等于
        LessThan,//小于
        LessThanOrEqual,//小于等于
        GreaterThan,//大于
        GreaterThanOrEqual//大于等于
    }
```
### 2、使用场景
博主项目中有某一个页面，查询条件非常多，需要传递到后台很多参数。先来看看页面：
![](http://jbcdn2.b0.upaiyun.com/2016/04/fca5003fe83f9de87a584cae1046c0be.png)
来看后台web api代码

C#
```
public object Get(int limit, int offset, string strBodyno, string strVin, string strOrderno, string strEngincode,
                  string strOrderstatus, string strTranscode, string strVms, string strCarcode,
                  string strImportStartdate, string strImportEnddate, string strSendStartdate, string strSendEnddate)
        {
            //1.定义对象，传入泛型
            var oLamadaExtention = new LamadaExtention();
            //2.依次构造Lamada表达式
            if (!string.IsNullOrEmpty(strBodyno))
            {
                oLamadaExtention.GetExpression("BODY_NO", strBodyno, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strVin))
            {
                oLamadaExtention.GetExpression("VIN", strVin, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strOrderno))
            {
                oLamadaExtention.GetExpression("ORDER_NO", strOrderno, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strEngincode))
            {
                oLamadaExtention.GetExpression("ENGIN_CODE", strEngincode, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strOrderstatus))
            {
                if (strOrderstatus.Contains(","))
                {
                    var lstValue = strOrderstatus.Split(",".ToCharArray(), StringSplitOptions.RemoveEmptyEntries).ToList();
                    var lstObj = new Listobject>();
                    lstValue.ForEach(x => {
                        lstObj.Add(Convert.ToInt16(x));
                    });
                    oLamadaExtention.GetExpression("ORDER_STATUS", lstObj);
                }
                else
                {
                    oLamadaExtention.GetExpression("ORDER_STATUS", Convert.ToInt16(strOrderstatus), ExpressionType.Equal);
                }
            }
            if (!string.IsNullOrEmpty(strTranscode))
            {
                oLamadaExtention.GetExpression("TRANS_CODE", strTranscode, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strVms))
            {
                oLamadaExtention.GetExpression("VMS_NO", strVms, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strCarcode))
            {
                oLamadaExtention.GetExpression("TM_MODEL_MATERIAL_ID", strCarcode, ExpressionType.Contains);
            }
            if (!string.IsNullOrEmpty(strImportStartdate))
            {
                oLamadaExtention.GetExpression("CREATE_DATE", Convert.ToDateTime(strImportStartdate), ExpressionType.GreaterThanOrEqual);
            }
            if (!string.IsNullOrEmpty(strImportEnddate))
            {
                oLamadaExtention.GetExpression("CREATE_DATE", Convert.ToDateTime(strImportEnddate), ExpressionType.LessThanOrEqual);
            }
            if (!string.IsNullOrEmpty(strSendStartdate))
            {
                oLamadaExtention.GetExpression("OFFLINE_DATE_ACT", Convert.ToDateTime(strSendStartdate), ExpressionType.GreaterThanOrEqual);
            }
            if (!string.IsNullOrEmpty(strSendEnddate))
            {
                oLamadaExtention.GetExpression("OFFLINE_DATE_ACT", Convert.ToDateTime(strSendEnddate), ExpressionType.LessThanOrEqual);
            }
            //3.得到需要的Lamada表达式Expression
            var lamada = oLamadaExtention.GetLambda();
            var lstRes = orderManager.Find(lamada).ToList();
            //4.得到Bootstrap Table需要的对象
            var oRes = new PageRowData();
　　　　　　  return oRes; ;
        }
```
仓储基类里面的find方法：

C#
```
public virtual IQueryable Find(Expressionbool>> express)
 {
    Funcbool> lamada = express.Compile();
    return UnitOfWork.context.Set().Where(lamada).AsQueryable();
 }
```
## 四、小结
至此，所谓的动态Lambda就完了。如果你之前用过，请一笑而过；当然如果你没用过，学习点新东西也是好的。请不要嘲笑博主乱下定义，叫动态Lambda挺好的呢。当然你可以叫动态Expression，动态Linq都行，不管叫什么，正确使用才是王道。
