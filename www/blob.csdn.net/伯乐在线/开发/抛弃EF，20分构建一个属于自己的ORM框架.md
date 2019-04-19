# 抛弃EF，20分构建一个属于自己的ORM框架 - 文章 - 伯乐在线
原文出处： [Poiuyt_cyc](http://www.cnblogs.com/irenebbkiss/p/4157364.html)
相信EF大家都不陌生了，因为数据库表跟程序实体是一一对应的原因，我们能够通过lambda这种函数式的编程方式进行操作数据库，感觉非常清晰明了。与我们直接写SQL相比，lambda是强类型，拥有更好的扩展性，伸缩性，而且编程更加的方便，快捷。下面我们就基于Expression和lambda来与大家构建一个属于自己的ORM框架。
思路的话很简单，就是将lambda转换成我们对应的数据库所需的查询条件，然后执行查询，再将结果以反射的方式封装成List返回出去。
Expression
大家使用EF的时候多多少少会留意到有Expression这个东西。特别是查询时会看到要你传入Expression>这样类型的参数，它又和Func有什么比同呢？
Expression>是表达式树，我们可以通过它来分析我们的委托中的函数。当调用Compile方法后就会变成委托，才能执行。
Func只是一个普通的委托。
例如我们现在有个实体类Staff


C#
```
public class Staff
    {
        public string Name { get; set; }
        public int Age { get; set; }
        public string Code { get; set; }
        public DateTime? Birthday { get; set; }
        public bool Deletion { get; set; }
    }
```
我们还有一个这样的方法


C#
```
class Program
    {
        static void Main(string[] args)
        {
            FindAs(x => x.Code == "张三" & x.Name.Contains("张"));
        }
        public static List FindAs(Expressionbool>> func)
        {
            //将func转换成对应数据库的查询条件，然后执行查询
            return null;//将结果返回
        }
    }
```
我们希望通过 FindAs(x => x.Age
![](http://jbcdn2.b0.upaiyun.com/2016/06/355b65cb116260f286b4695113b8e579.jpg)
从上面的图我们可以看到当前的Expression是一个lambda表达式，我们点开它的body看看。
![](http://jbcdn2.b0.upaiyun.com/2016/06/943162edafa157dd39ecd7e76615c13d.jpg)
我们可以看到body里分为左边和右边，还有NodeType。和我们的lambda对比下看看’x => x.Code ==”张三” && x.Name.Contains(“张”)’是不是找到点灵感了？我们再继续把左边和右边拆开看看。
![](http://jbcdn2.b0.upaiyun.com/2016/06/f886e74235da514ae0246d02db058cd7.jpg)
![](http://jbcdn2.b0.upaiyun.com/2016/06/0082bceded4feab65800ce3314d3dd67.jpg)
可以看到我们需要的信息都有了，看来转换成SQL已经不是什么难事了，动手开搞了。


C#
```
class Program
    {
        static void Main(string[] args)
        {
            FindAs(x => x.Code == "张三" & x.Name.Contains("张"));
            FindAs(x => x.Age 12 & x.Name.Contains("张"));
            Console.ReadKey();
        }
        public static List FindAs(Expressionbool>> func)
        {
            BinaryExpression Binary = func.Body as BinaryExpression;
            string left = ResovleFunc(Binary.Left);
            string right = ResovleLinqToObject(Binary.Right);
            string oper = GetOperator(Binary.NodeType);
            string sql = string.Format("select * from {0} where {1}", typeof(T).Name, left + oper + right);
            Console.WriteLine(sql);
            return null;//将结果返回
        }
        //解析一般的条件，例如x=>x.name==xxxx   x.age==xxx
        public static string ResovleFunc(Expression express)
        {
            var inner = express as BinaryExpression;
            string Name = (inner.Left as MemberExpression).Member.Name;
            object Value = (inner.Right as ConstantExpression).Value;
            var Operator = GetOperator(inner.NodeType);
            string Result = string.Format("({0} {1} '{2}')", Name, Operator, Value);
            return Result;
        }
        //解析linq to object这类扩展方法
        public static string ResovleLinqToObject(Expression expression)
        {
            var MethodCall = expression as MethodCallExpression;
            var MethodName = MethodCall.Method.Name;
            if (MethodName == "Contains")
            {
                object Temp_Vale = (MethodCall.Arguments[0] as ConstantExpression).Value;
                string Value = string.Format("%{0}%", Temp_Vale);
                string Name = (MethodCall.Object as MemberExpression).Member.Name;
                string Result = string.Format("{0} like '{1}'", Name, Value);
                return Result;
            }
            return null;
        }
        public static string GetOperator(ExpressionType expressiontype)
        {
            switch (expressiontype)
            {
                case ExpressionType.And:
                    return "and";
                case ExpressionType.AndAlso:
                    return "and";
                case ExpressionType.Or:
                    return "or";
                case ExpressionType.OrElse:
                    return "or";
                case ExpressionType.Equal:
                    return "=";
                case ExpressionType.NotEqual:
                    return "";
                case ExpressionType.LessThan:
                    return "";
                case ExpressionType.LessThanOrEqual:
                    return "";
                case ExpressionType.GreaterThan:
                    return ">";
                case ExpressionType.GreaterThanOrEqual:
                    return ">=";
                default:
                    throw new Exception(string.Format("不支持{0}此种运算符查找！" + expressiontype));
            }
        }
    }
```
![](http://jbcdn2.b0.upaiyun.com/2016/06/0f7b8334de4c066822b30dca9573bcba.jpg)
已经初步的达到了我们的目的了，但是我们的查询条件不可能固定是2个，有可能是N个，这时左边和右边又要继续再分下去，直到无法再分(想到递归了吧？)。而且我们还需要将查询条件参数化。而且我们的条件删除时也会用到。所以我们应该把它独立出来。传入一个lambda，生成sql where部分的语句，生成sqlparameter[]。这才是关键。。于是我们来构建一个解析Expresstion的类。。下面我就直接给出我自己写的实现代码了。。


C#
```
public class ResolveExpress
    {
        public Dictionarystring, object> Argument;
        public string SqlWhere;
        public SqlParameter[] Paras;
        /// 
        /// 解析lamdba，生成Sql查询条件
        /// 
        /// 
        /// 
        public void ResolveExpression(Expression expression)
        {
            this.Argument = new Dictionarystring, object>();
            this.SqlWhere = Resolve(expression);
            this.Paras = Argument.Select(x => new SqlParameter(x.Key, x.Value)).ToArray();
        }
        private string Resolve(Expression expression)
        {
            if (expression is LambdaExpression)
            {
                LambdaExpression lambda = expression as LambdaExpression;
                expression = lambda.Body;
                return Resolve(expression);
            }
            if (expression is BinaryExpression)
            {
                BinaryExpression binary = expression as BinaryExpression;
                if (binary.Left is MemberExpression & binary.Right is ConstantExpression)//解析x=>x.Name=="123" x.Age==123这类
                    return ResolveFunc(binary.Left, binary.Right, binary.NodeType);
                if (binary.Left is MethodCallExpression & binary.Right is ConstantExpression)//解析x=>x.Name.Contains("xxx")==false这类的
                {
                    object value = (binary.Right as ConstantExpression).Value;
                    return ResolveLinqToObject(binary.Left, value, binary.NodeType);
                }
                if (binary.Left is MemberExpression & binary.Right is MemberExpression)//解析x=>x.Date==DateTime.Now这种
                {
                    LambdaExpression lambda = Expression.Lambda(binary.Right);
                    Delegate fn = lambda.Compile();
                    ConstantExpression value = Expression.Constant(fn.DynamicInvoke(null), binary.Right.Type);
                    return ResolveFunc(binary.Left, value, binary.NodeType);
                }
            }
            if (expression is UnaryExpression)
            {
                UnaryExpression unary = expression as UnaryExpression;
                if (unary.Operand is MethodCallExpression)//解析!x=>x.Name.Contains("xxx")或!array.Contains(x.Name)这类
                    return ResolveLinqToObject(unary.Operand, false);
                if (unary.Operand is MemberExpression & unary.NodeType == ExpressionType.Not)//解析x=>!x.isDeletion这样的 
                {
                    ConstantExpression constant = Expression.Constant(false);
                    return ResolveFunc(unary.Operand, constant, ExpressionType.Equal);
                }
            }
            if (expression is MemberExpression & expression.NodeType == ExpressionType.MemberAccess)//解析x=>x.isDeletion这样的 
            {
                MemberExpression member = expression as MemberExpression;
                ConstantExpression constant = Expression.Constant(true);
                return ResolveFunc(member, constant, ExpressionType.Equal);
            }
            if (expression is MethodCallExpression)//x=>x.Name.Contains("xxx")或array.Contains(x.Name)这类
            {
                MethodCallExpression methodcall = expression as MethodCallExpression;
                return ResolveLinqToObject(methodcall, true);
            }
            var body = expression as BinaryExpression;
            if (body == null)
                throw new Exception("无法解析" + expression);
            var Operator = GetOperator(body.NodeType);
            var Left = Resolve(body.Left);
            var Right = Resolve(body.Right);
            string Result = string.Format("({0} {1} {2})", Left, Operator, Right);
            return Result;
        }
        /// 
        /// 根据条件生成对应的sql查询操作符
        /// 
        /// 
        /// 
        private string GetOperator(ExpressionType expressiontype)
        {
            switch (expressiontype)
            {
                case ExpressionType.And:
                    return "and";
                case ExpressionType.AndAlso:
                    return "and";
                case ExpressionType.Or:
                    return "or";
                case ExpressionType.OrElse:
                    return "or";
                case ExpressionType.Equal:
                    return "=";
                case ExpressionType.NotEqual:
                    return "";
                case ExpressionType.LessThan:
                    return "";
                case ExpressionType.LessThanOrEqual:
                    return "";
                case ExpressionType.GreaterThan:
                    return ">";
                case ExpressionType.GreaterThanOrEqual:
                    return ">=";
                default:
                    throw new Exception(string.Format("不支持{0}此种运算符查找！" + expressiontype));
            }
        }
        private string ResolveFunc(Expression left, Expression right, ExpressionType expressiontype)
        {
            var Name = (left as MemberExpression).Member.Name;
            var Value = (right as ConstantExpression).Value;
            var Operator = GetOperator(expressiontype);
            string CompName = SetArgument(Name, Value.ToString());
            string Result = string.Format("({0} {1} {2})", Name, Operator, CompName);
            return Result;
        }
        private string ResolveLinqToObject(Expression expression, object value, ExpressionType? expressiontype = null)
        {
            var MethodCall = expression as MethodCallExpression;
            var MethodName = MethodCall.Method.Name;
            switch (MethodName)//这里其实还可以改成反射调用，不用写switch
            {
                case "Contains":
                    if (MethodCall.Object != null)
                        return Like(MethodCall);
                    return In(MethodCall, value);
                case "Count":
                    return Len(MethodCall, value, expressiontype.Value);
                case "LongCount":
                    return Len(MethodCall, value, expressiontype.Value);
                default:
                    throw new Exception(string.Format("不支持{0}方法的查找！", MethodName));
            }
        }
        private string SetArgument(string name, string value)
        {
            name = "@" + name;
            string temp = name;
            while (Argument.ContainsKey(temp))
            {
                int code = Guid.NewGuid().GetHashCode();
                if (code 0)
                    code *= -1;
                temp = name + code;
            }
            Argument[temp] = value;
            return temp;
        }
        private string In(MethodCallExpression expression, object isTrue)
        {
            var Argument1 = (expression.Arguments[0] as MemberExpression).Expression as ConstantExpression;
            var Argument2 = expression.Arguments[1] as MemberExpression;
            var Field_Array = Argument1.Value.GetType().GetFields().First();
            object[] Array = Field_Array.GetValue(Argument1.Value) as object[];
            Liststring> SetInPara = new Liststring>();
            for (int i = 0; i )
            {
                string Name_para = "InParameter" + i;
                string Value = Array[i].ToString();
                string Key = SetArgument(Name_para, Value);
                SetInPara.Add(Key);
            }
            string Name = Argument2.Member.Name;
            string Operator = Convert.ToBoolean(isTrue) ? "in" : " not in";
            string CompName = string.Join(",", SetInPara);
            string Result = string.Format("{0} {1} ({2})", Name, Operator, CompName);
            return Result;
        }
        private string Like(MethodCallExpression expression)
        {
            object Temp_Vale = (expression.Arguments[0] as ConstantExpression).Value;
            string Value = string.Format("%{0}%", Temp_Vale);
            string Name = (expression.Object as MemberExpression).Member.Name;
            string CompName = SetArgument(Name, Value);
            string Result = string.Format("{0} like {1}", Name, CompName);
            return Result;
        }
        private string Len(MethodCallExpression expression, object value, ExpressionType expressiontype)
        {
            object Name = (expression.Arguments[0] as MemberExpression).Member.Name;
            string Operator = GetOperator(expressiontype);
            string CompName = SetArgument(Name.ToString(), value.ToString());
            string Result = string.Format("len({0}){1}{2}", Name, Operator, CompName);
            return Result;
        }
    }
```


C#
```
static void Main(string[] args)
        {
            string[] Names = { "Andy", "Amy", "Mike" };
            Expressionbool>> func = x => (!Names.Contains(x.Name) & (x.Name == "A" || x.Name.Count() > 5));
            ResolveExpress resolve = new ResolveExpress();
            resolve.ResolveExpression(func);
            Console.WriteLine(resolve.SqlWhere);
            foreach (var item in resolve.Paras)
            {
                Console.WriteLine(item.ParameterName + ":" + item.Value);
            }
            Console.ReadKey();
        }
```
结果：
![](http://jbcdn2.b0.upaiyun.com/2016/06/0ead767fdeacc566f1f8c7cffc98e6e1.jpg)
这里有几个重要的东西要给大家讲下
string[] Names={“Andy”,”Amy”,”Mike”};
1.）x => Names.Contains(x.Name);
2.）x => Names.Contains(x.Name)==false;
3.）x => !Names.Contains(x.Name);
这3种在Expression中的表现都不一样
1的话会看成是一个静态方法（MethodCallExpression）
2的话会看成是一个2元运算（BinaryExpression）
3的话会看成是一个1元运算（UnaryExpression）
所以我们都要支持，处理都有所不同。
还有
x=>x.Birthday
string name=”123″;
x=>x.Name==name;
和
x=>x.Name==”123″
的处理也不一样。大家可以在例子中细细的看看。
这样的构造使得我们切换数据库变得非常简单。因为我们程序中的查询都是基于lambda。换了数据库只要添加一个对应的lamdba转数据库查询条件的实现就可以了。写得够多了。至于数据层怎么封装，到了这一步它已经变得没什么难度了。希望大家能从文章中有所启发和帮助
下篇文章将结合解析Expression和IQueryable来实现延迟加载
补充点东西
IEnumerable和IQueryable有什么不同？
为什么EF查询后返回的是IQueryable而不是IEnumerable。我们对着IQueryableF12去看看。
![](http://jbcdn2.b0.upaiyun.com/2016/06/2ff08f6169e28d0c01a4093551a102da.png)
啥都没，就继承了几个接口。鼠标移到IQueryable上。F12
![](http://jbcdn2.b0.upaiyun.com/2016/06/9ed51299e90659160e0cd2c23671fe47.jpg)
IQueryable中有3个属性。
Type是类型。
Expresstion是表达式。
那IQueryProvider是什么？
再看看IQueryProvider接口的定义。
CreateQuery是创建查询条件
Execute是执行查询（通常在GetEnumerator()中调用）
当我们IQueryable.Where(x=>x.xxx==”123″)时。其实Where方法内部应该是调用了IQueryable接口中的IQueryProvider属性的CreateQuery(Expresstion expresstion)方法，然后将方法的返回值又返回出来。
而参数（Expresstion ）呢？则是IQueryable.Where(x=>x.xxx==”123″)2部分的Expresstion相并。所以IQueryable只是创建条件。所以51楼的朋友说得非常对。
那什么时候执行呢？因为我们的IQueryable继承了IEnumabler，所以我们必须实现GetEnumerator()。我们ToList或foreach时，其实就会调用GetEnumerator()。这时我们就调用Execute进行解析Expresstion，从而得到我们想要的结果。
总结就是IQueryable只是创建条件,当我们调用a.Where(x=>xxx)时，其实是将a与后面的条件相并，生成一个新的IQueryable。当我们foreach时就会调用GetEnumerator()。这时我们一般会调用IQueryProvider里的Execute去解析Expresstion并查询出我们想要的结果
我也知道这篇文章介绍的和我们所说的“ORM”相差很远，但是所谓的ORM最复杂的莫非查询部分了，而依照我这思路走下去，我觉得是可以自己完成一个的。。
我刚开始写博客第二天，没想到这文章反响这么大。我承认有点重复造轮子，也非常不成熟，但我还是想通过自己的思考去构造属于自己的东西。
不知道大家有没看过头文字D，里头有个组织叫东堂垫，他们里面的人是拆掉ABS的。因为他们会长说，你要先学会不使用ABS进行刹车才知道ABS的真谛
