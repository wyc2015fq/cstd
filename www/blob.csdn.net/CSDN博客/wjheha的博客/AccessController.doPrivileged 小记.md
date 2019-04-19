# AccessController.doPrivileged 小记 - wjheha的博客 - CSDN博客
2017年04月28日 15:26:01[wjheha](https://me.csdn.net/wjheha)阅读数：123
转载地址：http://blog.csdn.net/teamlet/article/details/1809165
AccessController.doPrivileged是一个在AccessController类中的静态方法，允许在一个类实例中的代码通知这个AccessController：它的代码主体是享受"privileged(特权的)"，它单独负责对它的可得的资源的访问请求，而不管这个请求是由什么代码所引发的。
这就是说，一个调用者在调用doPrivileged方法时，可被标识为 "特权"。在做访问控制决策时，如果checkPermission方法遇到一个通过doPrivileged调用而被表示为 "特权"的调用者，并且没有上下文自变量，checkPermission方法则将终止检查。如果那个调用者的域具有特定的许可，则不做进一步检查，checkPermission安静地返回，表示那个访问请求是被允许的；如果那个域没有特定的许可，则象通常一样，一个异常被抛出。
一、"特权"特性的正常使用如下所示：
1、如果你不需要从"特权"块内返回一个值，按下列代码去做：
somemethod() {
      ...normal code here...
      AccessController.doPrivileged(new PrivilegedAction() {
public Object run() {
                    // privileged code goes here, for example:
                    System.loadLibrary("awt");
                    returnnull; // nothing to return
            }
      });
      ...normal code here...
} 
PrivilegedAction是一个接口，它带有一个被称为run的方法，这个方法返回一个Object。上述例子显示了一个用来实现那个接口的匿名内类的创建，并提供了一个run方法的具体实现。
当做一个doPrivileged调用时，一个PrivilegedAction实现的实例被传递给它。doPrivileged方法在使特权生效后，从PrivilegedAction实现中调用run方法，并返回run方法的返回值以作为doPrivileged的返回值，这一点在本例中被忽略。
2、如果你需要返回一个值，你可按如下方法去做：
somemethod() {
        ...normal code here...
        String user = (String) AccessController.doPrivileged(new PrivilegedAction() {
                  public Object run() {
                         return System.getProperty("user.name");
                 }
        });
        ...normal code here...
}
3、如果用你的run方法执行的动作可能扔出一个"检查"的异常（包括在一个方法的throws子句列表中），则你需要使用PrivilegedExceptionAction接口，而不是使用PrivilegedAction接口：
somemethod() throws FileNotFoundException {
        ...normal code here...
        try {
               FileInputStream fis = (FileInputStream)
               AccessController.doPrivileged(new PrivilegedExceptionAction() {
                     public Object run() throws FileNotFoundException {
                            returnnew FileInputStream("someFile");
                     }
               });
        } catch (PrivilegedActionException e) {
                 // e.getException() should be an instance of
                 // FileNotFoundException,
                 // as only "checked" exceptions will be "wrapped" in a
                 // PrivilegedActionException.
                throw (FileNotFoundException) e.getException();
        }
       ...normal code here...
}
有关被授予特权的一些重要事项：
首先，这个概念仅存在于一个单独线程内。一旦特权代码完成了任务，特权将被保证清除或作废。
第二，在这个例子中，在run方法中的代码体被授予了特权。然而，如果它调用无特权的不可信代码，则那个代码将不会获得任何特权；只有在特权代码具有许可并且在直到checkPermission调用的调用链中的所有随后的调用者也具有许可时, 一个许可才能被准予。
二、使用事例：
final String name = MyClass.class.getName();
String className = AccessController.doPrivileged(new PrivilegedAction<String>() {
public String run() {
return System.getProperty(name);
                }
});
