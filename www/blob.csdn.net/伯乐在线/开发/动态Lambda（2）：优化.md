# 动态Lambda（2）：优化 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4950336.html)
## 一、原理分析
上篇里面我们说了动态Lambda的使用必要性以及使用场景，但是感觉用在项目里面还不太方便，最难用的就是需要传递属性名称的字符串，感觉这有点太lower了。然后就是那个枚举的使用着实感觉没啥必要，我们只需要将Contains、Equal、LessThan、GreaterThan等方法分别封装一个独立的方法即可。好了，多说容易让人头晕，直接上代码吧。
## 二、代码示例

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
　　　　 //只读属性，返回生成的Lamada
        public Expressionbool>> Lamada 
        {　　　　　　　　
            get 
            {
                return GetLambda();
            }
        }
        /// 
        /// 字符串Contains筛选
        /// 
        /// 
        /// 
        public void Contains(Expressionstring>> expProperty, object strValue)
        {
            Expression expRes = Expression.Call(expProperty.Body, typeof(string).GetMethod("Contains"),
                                Expression.Constant(strValue));
            m_lstExpression.Add(expRes);
        }
        /// 
        /// 等于
        /// 
        /// 
        /// 
        public void Equal(Expressionobject>> expProperty, object strValue)
        {
            var member = GetMemberExpression(expProperty);
            Expression expRes = Expression.Equal(member, Expression.Constant(strValue, member.Type));
            m_lstExpression.Add(expRes);
        }
        /// 
        /// 小于
        /// 
        /// 
        /// 
        public void LessThan(Expressionobject>> expProperty, object strValue)
        {
            var member = GetMemberExpression(expProperty);
            Expression expRes = Expression.LessThan(member, Expression.Constant( strValue, member.Type));
            m_lstExpression.Add(expRes);
        }
        /// 
        /// 小于等于
        /// 
        /// 
        /// 
        public void LessThanOrEqual(Expressionobject>> expProperty, object strValue)
        {
            var member = GetMemberExpression(expProperty);
            Expression expRes = Expression.LessThanOrEqual(member, Expression.Constant(strValue, member.Type));
            m_lstExpression.Add(expRes);
        }
        /// 
        /// 大于
        /// 
        /// 
        /// 
        public void GreaterThan(Expressionobject>> expProperty, object strValue)
        {
            var member = GetMemberExpression(expProperty);
            Expression expRes = Expression.GreaterThan(member, Expression.Constant(strValue, member.Type));
            m_lstExpression.Add(expRes);
        }
        /// 
        /// 大于等于
        /// 
        /// 
        /// 
        public void GreaterThanOrEqual(Expressionobject>> expProperty, object strValue)
        {
            var member = GetMemberExpression(expProperty);
            Expression expRes = Expression.GreaterThanOrEqual(member, Expression.Constant(strValue, member.Type));
            m_lstExpression.Add(expRes);
        }private Expressionbool>> GetLambda()
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
        //得到MemberExpression
        private MemberExpression GetMemberExpression(Expressionobject>> exp)
        {
            var arrSplit = exp.Body.ToString().Split("(.)".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            var strProperty = arrSplit[arrSplit.Length - 1];
            MemberExpression member = Expression.PropertyOrField(m_Parameter, strProperty);
            return member;
        }
    }
```
可以看出，对于常用的操作我们封装了Contains、Equal、LessThan、LessThanOrEqual、GreaterThan、GreaterThanOrEqual六个方法，除了Contains方法的参数直接使用了Expression>类型以为，其他都用的Expression>。因为Contains方法只可能是string类型的变量操作，而其他操作可能涉及其他类型，就是为了传这个object类型，有个问题博主调试了很久，由于传过来的是object，这个要得到属性的真是类型貌似不那么容易了，找了很久都没找到。最后只能通过GetMemberExpression这个方法来得到MemberExpression。
还是来看看如何使用：

C#
```
public object GetUsers(int limit, int offset, string username, string fullname)
        {
            var oLamadaExtention = new LamadaExtention();
            oLamadaExtention.Equal(x => x.USER_NAME, username);
            oLamadaExtention.LessThan(x => x.MODIFYTIME, DateTime.Now);
　　　　　　  var lstRes = UserManager.Find(oLamadaExtention.lamada).ToList();
　　　　 }
```
最大的方便就是我们想要筛选的字段可以通过lamada点出来了，再看看之前的那种用法

C#
```
oLamadaExtention.GetExpression("USER_NAME", username, ExpressionType.Contains);
```
有没有瞬间高大上。USER_NAME直接点出来，比敲字符串要爽吧。感谢神奇的Lamada，感谢全能的C#。
- [《C#进阶系列：动态Lamada》](http://blog.jobbole.com/99430/)
