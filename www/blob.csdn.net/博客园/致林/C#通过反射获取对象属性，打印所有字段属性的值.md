
# C\#通过反射获取对象属性，打印所有字段属性的值 - 致林 - 博客园






# [C\#通过反射获取对象属性，打印所有字段属性的值](https://www.cnblogs.com/bincoding/p/8144392.html)
获取所有字段的值：
publicvoidPrintProperties(Object obj)
{
    Type type=obj.GetType();foreach( PropertyInfo pintype.GetProperties())
    {
        Console.Write(p.GetValue());
    }
}





