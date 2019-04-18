# [翻译]NUnit---Condition Asserts && Comparisons Asserts && Type Asserts (五) - weixin_33985507的博客 - CSDN博客
2017年08月14日 10:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
网址：http://www.cnblogs.com/kim01/archive/2013/03/31/2991597.html
## Condition Asserts
测试指定条件的方法称作条件测试，测试结果作为第一个参数，第二个是可选的消息参数。提供了如下方法：
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.IsTrue( bool condition );
Assert.IsTrue( bool condition, string message );
Assert.IsTrue( bool condition, string message, object[] parms );
Assert.True( bool condition );
Assert.True( bool condition, string message );
Assert.True( bool condition, string message, object[] parms );
Assert.IsFalse( bool condition);
Assert.IsFalse( bool condition, string message );
Assert.IsFalse( bool condition, string message, object[] parms );
Assert.False( bool condition);
Assert.False( bool condition, string message );
Assert.False( bool condition, string message, object[] parms );
Assert.IsNull( object anObject );
Assert.IsNull( object anObject, string message );
Assert.IsNull( object anObject, string message, object[] parms );
Assert.Null( object anObject );
Assert.Null( object anObject, string message );
Assert.Null( object anObject, string message, object[] parms );
Assert.IsNotNull( object anObject );
Assert.IsNotNull( object anObject, string message );
Assert.IsNotNull( object anObject, string message, object[] parms );
Assert.NotNull( object anObject );
Assert.NotNull( object anObject, string message );
Assert.NotNull( object anObject, string message, object[] parms );
Assert.IsNaN( double aDouble );
Assert.IsNaN( double aDouble, string message );
Assert.IsNaN( double aDouble, string message, object[] parms );
Assert.IsEmpty( string aString );
Assert.IsEmpty( string aString, string message );
Assert.IsEmpty( string aString, string message,
          params object[] args );
Assert.IsNotEmpty( string aString );
Assert.IsNotEmpty( string aString, string message );
Assert.IsNotEmpty( string aString, string message,
          params object[] args );
Assert.IsEmpty( ICollection collection );
Assert.IsEmpty( ICollection collection, string message );
Assert.IsEmpty( ICollection collection, string message,
          params object[] args );
Assert.IsNotEmpty( ICollection collection );
Assert.IsNotEmpty( ICollection collection, string message );
Assert.IsNotEmpty( ICollection collection, string message,
          params object[] args );
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
为True、False、Null、NotNull提供了两种方式。“Is”形式与早期NUnit兼容，没有“Is”的提供了与NUnitLite兼容性。
Assert.IsEmpty and Assert.IsNotEmpty可以用于一个字符串或者集合。
## Comparisons (NUnit 2.2.4)
以下方法测试了一个对象是否大于另一个对象。与Asserts顺序序相反，这些方法是英文字母顺序或者数学顺序。因此**Assert.Greater( x, y )**意思是X>y。
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.Greater( int arg1, int arg2 );
Assert.Greater( int arg1, int arg2, string message );
Assert.Greater( int arg1, int arg2, string message, 
                object[] parms );
Assert.Greater( uint arg1, uint arg2 );
Assert.Greater( uint arg1, uint arg2, string message );
Assert.Greater( uint arg1, uint arg2, string message, 
                object[] parms );
Assert.Greater( long arg1, long arg2 );
Assert.Greater( long arg1, long arg2, string message );
Assert.Greater( long arg1, long arg2, string message, 
                object[] parms );
Assert.Greater( ulong arg1, ulong arg2 );
Assert.Greater( ulong arg1, ulong arg2, string message );
Assert.Greater( ulong arg1, ulong arg2, string message, 
                object[] parms );
Assert.Greater( decimal arg1, decimal arg2 );
Assert.Greater( decimal arg1, decimal arg2, string message );
Assert.Greater( decimal arg1, decimal arg2, string message, 
                object[] parms );
Assert.Greater( double arg1, double arg2 );
Assert.Greater( double arg1, double arg2, string message );
Assert.Greater( double arg1, double arg2, string message, 
                object[] parms );
Assert.Greater( double arg1, double arg2 );
Assert.Greater( double arg1, double arg2, string message );
Assert.Greater( double arg1, double arg2, string message, 
                object[] parms );
Assert.Greater( float arg1, float arg2 );
Assert.Greater( float arg1, float arg2, string message );
Assert.Greater( float arg1, float arg2, string message, 
                object[] parms );
Assert.Greater( IComparable arg1, IComparable arg2 );
Assert.Greater( IComparable arg1, IComparable arg2, string message );
Assert.Greater( IComparable arg1, IComparable arg2, string message, 
                object[] parms );
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
接下来的方法测试了一个对象是否大于或者等于另一个对象。与Asserts正序相反，这些方法是英文字母顺序或者数学顺序。**Assert.GreaterOrEqual( x, y )** 意思是x>=y
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.GreaterOrEqual( int arg1, int arg2 );
Assert.GreaterOrEqual( int arg1, int arg2, string message );
Assert.GreaterOrEqual( int arg1, int arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( uint arg1, uint arg2 );
Assert.GreaterOrEqual( uint arg1, uint arg2, string message );
Assert.GreaterOrEqual( uint arg1, uint arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( long arg1, long arg2 );
Assert.GreaterOrEqual( long arg1, long arg2, string message );
Assert.GreaterOrEqual( long arg1, long arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( ulong arg1, ulong arg2 );
Assert.GreaterOrEqual( ulong arg1, ulong arg2, string message );
Assert.GreaterOrEqual( ulong arg1, ulong arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( decimal arg1, decimal arg2 );
Assert.GreaterOrEqual( decimal arg1, decimal arg2, string message );
Assert.GreaterOrEqual( decimal arg1, decimal arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( double arg1, double arg2 );
Assert.GreaterOrEqual( double arg1, double arg2, string message );
Assert.GreaterOrEqual( double arg1, double arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( double arg1, double arg2 );
Assert.GreaterOrEqual( double arg1, double arg2, string message );
Assert.GreaterOrEqual( double arg1, double arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( float arg1, float arg2 );
Assert.GreaterOrEqual( float arg1, float arg2, string message );
Assert.GreaterOrEqual( float arg1, float arg2, string message, 
                object[] parms );
Assert.GreaterOrEqual( IComparable arg1, IComparable arg2 );
Assert.GreaterOrEqual( IComparable arg1, IComparable arg2, string message );
Assert.GreaterOrEqual( IComparable arg1, IComparable arg2, string message, 
                object[] parms );
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
接下来的方法测试了一个对象是否小于另一个对象。与Asserts正序相反，这些方法是英文字母顺序或者数学顺序。**Assert.Less( x, y )** 意思是x<y
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.Less( int arg1, int arg2 );
Assert.Less( int arg1, int arg2, string message );
Assert.Less( int arg1, int arg2, string message, 
                object[] parms );
                
Assert.Less( uint arg1, uint arg2 );
Assert.Less( uint arg1, uint arg2, string message );
Assert.Less( uint arg1, uint arg2, string message, 
                object[] parms );
                
Assert.Less( long arg1, long arg2 );
Assert.Less( long arg1, long arg2, string message );
Assert.Less( long arg1, long arg2, string message, 
                object[] parms );
Assert.Less( ulong arg1, ulong arg2 );
Assert.Less( ulong arg1, ulong arg2, string message );
Assert.Less( ulong arg1, ulong arg2, string message, 
                object[] parms );
Assert.Less( decimal arg1, decimal arg2 );
Assert.Less( decimal arg1, decimal arg2, string message );
Assert.Less( decimal arg1, decimal arg2, string message, 
                object[] parms );
                
Assert.Less( double arg1, double arg2 );
Assert.Less( double arg1, double arg2, string message );
Assert.Less( double arg1, double arg2, string message, 
                object[] parms );
                
Assert.Less( float arg1, float arg2 );
Assert.Less( float arg1, float arg2, string message );
Assert.Less( float arg1, float arg2, string message, 
                object[] parms );
                
Assert.Less( IComparable arg1, IComparable arg2 );
Assert.Less( IComparable arg1, IComparable arg2, string message );
Assert.Less( IComparable arg1, IComparable arg2, string message, 
                object[] parms );
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
接下来的方法测试了一个对象是否小于或者等于另一个对象。与Asserts正序相反，这些方法是英文字母顺序或者数学顺序。**Assert.LessOrEqual** 意思是x<=y
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.LessOrEqual( int arg1, int arg2 ); Assert.LessOrEqual( int arg1, int arg2, string message ); Assert.LessOrEqual( int arg1, int arg2, stringmessage, object[] parms ); Assert.LessOrEqual( uint arg1, uint arg2 ); Assert.LessOrEqual( uint arg1, uint arg2, string message ); Assert.LessOrEqual(uint arg1, uint arg2, string message, object[] parms ); Assert.LessOrEqual( long arg1, long arg2 ); Assert.LessOrEqual( long arg1, long arg2, stringmessage ); Assert.LessOrEqual( long arg1, long arg2, string message, object[] parms ); Assert.LessOrEqual( ulong arg1, ulong arg2 ); Assert.LessOrEqual(ulong arg1, ulong arg2, string message ); Assert.LessOrEqual( ulong arg1, ulong arg2, string message, object[] parms ); Assert.LessOrEqual( decimal arg1,decimal arg2 ); Assert.LessOrEqual( decimal arg1, decimal arg2, string message ); Assert.LessOrEqual( decimal arg1, decimal arg2, string message,object[] parms ); Assert.LessOrEqual( double arg1, double arg2 ); Assert.LessOrEqual( double arg1, double arg2, string message ); Assert.LessOrEqual(double arg1, double arg2, string message, object[] parms ); Assert.LessOrEqual( float arg1, float arg2 ); Assert.LessOrEqual( float arg1, float arg2,string message ); Assert.LessOrEqual( float arg1, float arg2, string message, object[] parms ); Assert.LessOrEqual( IComparable arg1, IComparable arg2 ); Assert.LessOrEqual( IComparable arg1, IComparable arg2, string message ); Assert.LessOrEqual( IComparable arg1, IComparable arg2, string message,object[] parms );
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
## Type Asserts (NUnit 2.2.3 / 2.5)
以下方法可以用来对一个对象的类型进行比较。
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.IsInstanceOfType( Type expected, object actual );
Assert.IsInstanceOfType( Type expected, object actual, 
                string message );
Assert.IsInstanceOfType( Type expected, object actual, 
                string message, params object[] parms );
                
Assert.IsNotInstanceOfType( Type expected, object actual );
Assert.IsNotInstanceOfType( Type expected, object actual, 
                string message );
Assert.IsNotInstanceOfType( Type expected, object actual, 
                string message, params object[] parms );
            
Assert.IsAssignableFrom( Type expected, object actual );
Assert.IsAssignableFrom( Type expected, object actual, 
                string message );
Assert.IsAssignableFrom( Type expected, object actual, 
                string message, params object[] parms );
                
Assert.IsNotAssignableFrom( Type expected, object actual );
Assert.IsNotAssignableFrom( Type expected, object actual, 
                string message );
Assert.IsNotAssignableFrom( Type expected, object actual, 
                string message, params object[] parms );
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
从NUnit2.5开始，可以使用.NET2.0的泛型比较。
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Assert.IsInstanceOf<T>( object actual );
Assert.IsInstanceOf<T>( object actual, string message );
Assert.IsInstanceOf<T>( object actual, 
                string message, params object[] parms );
                
Assert.IsNotInstanceOf<T>( object actual );
Assert.IsNotInstanceOf<T>( object actual, string message ); 
Assert.IsNotInstanceOf<T>( object actual, 
                string message, params object[] parms );
            
Assert.IsAssignableFrom<T>( object actual );
Assert.IsAssignableFrom<T>( object actual, string message );
Assert.IsAssignableFrom<T>( object actual, 
                string message, params object[] parms );
                
Assert.IsNotAssignableFrom<T>( object actual );
Assert.IsNotAssignableFrom<T>( object actual, string message );
Assert.IsNotAssignableFrom<T>( object actual, 
                string message, params object[] parms );
```
