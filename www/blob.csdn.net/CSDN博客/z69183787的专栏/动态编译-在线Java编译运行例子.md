# 动态编译-在线Java编译运行例子 - z69183787的专栏 - CSDN博客
2018年09月17日 12:08:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：278
[https://www.cnblogs.com/hujunzheng/p/5203067.html](https://www.cnblogs.com/hujunzheng/p/5203067.html)
首先简单介绍一下思路：
　　1.得到java程序的源代码，不需要导入包。得到源码之后在前面加入”import java.util.*;”
　　2.通过JavaCompiler对象可以帮助我们将java源代码编译成class文件。
　　3.通过DiagnosticCollector对象可以获得编译过程中产生的编译信息。
　　4.通过StandardJavaFileManager对象管理生成的class文件，例如文件的存放位置。
　　5.StringSourceJavaObject对象可以对java源码进行包装并处理。
数据是控制台输入的，所以要重定向System.in(注意保存标准的输入流)；另外程序的输出是到标准的输出流的，为了获得输出结果，我的方法是重定向输出流到ByteArrayOutputStream，然后利用ByteArrayOutputStream构造BufferedReader。
　　6.运行程序，通过java的反射机制，获得main函数的Method对象。
　　7.运行时间的计算: 通过System.currentTimeMillis()方法。
　　8.程序所需内存: 通过Runtime的freeMemory()方法。
　　9.异常信息的获取：StringWriter sw = new StringWriter();  e.printStackTrace(new PrintWriter(sw, true)); sw.toString();
遇到的问题：
1. JavaCompiler compiler = ToolProvider.getSystemJavaCompiler(); 出现NullPointerException。
　 查看部分源码如下：
```java
private static final String[] defaultToolsLocation = { "lib", "tools.jar" };
private Class<?> findSystemToolClass(String toolClassName)
        throws MalformedURLException, ClassNotFoundException {
    // try loading class directly, in case tool is on the bootclasspath
    try {
        return Class.forName(toolClassName, false, null);
    } catch (ClassNotFoundException e) {
        trace(FINE, e);
        // if tool not on bootclasspath, look in default tools location (tools.jar)
        ClassLoader cl = (refToolClassLoader == null ? null : refToolClassLoader.get());
        if (cl == null) {
            File file = new File(System.getProperty("java.home"));
            if (file.getName().equalsIgnoreCase("jre"))
                file = file.getParentFile();
            for (String name : defaultToolsLocation)
                file = new File(file, name);
            // if tools not found, no point in trying a URLClassLoader
            // so rethrow the original exception.
            if (!file.exists())
                throw e;
            URL[] urls = { file.toURI().toURL() };
            trace(FINE, urls[0].toString());
            cl = URLClassLoader.newInstance(urls);
            refToolClassLoader = new WeakReference<ClassLoader>(cl);
        }
        return Class.forName(toolClassName, false, cl);
    }
}
```
打印 System.out.println(System.getProperty("java.home")); 如下：
 C:\Program Files (x86)\Java\jre6
defaultToolsLocation = { "lib", "tools.jar" }; 也就是最终到
C:\Program Files (x86)\Java\jre6\lib\tools.jar中寻找tools.jar
然而jre6\lib中没有tools.jar, 而是在C:\Program Files (x86)\Java\jdk\lib中。最直接的办法就是将它复制进去就行了。
2.异常信息的获取。
3.输入流和输出流的重定向。
详细内容请看代码!
```java
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.Method;
import java.net.URI;
import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.tools.Diagnostic;
import javax.tools.DiagnosticCollector;
import javax.tools.JavaCompiler;
import javax.tools.JavaCompiler.CompilationTask;
import javax.tools.JavaFileObject;
import javax.tools.SimpleJavaFileObject;
import javax.tools.StandardJavaFileManager;
import javax.tools.StandardLocation;
import javax.tools.ToolProvider;
public class CompileAndRunJavaFile {
    public static void main(String[] args) {
        StringBuilder code = new StringBuilder();
        try {
            BufferedReader br = new BufferedReader(new FileReader(new File("测试程序地址")));
            String content;
            while((content = br.readLine()) != null){
                code.append(content).append("\n");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        CompileAndRunJavaFile cr = new CompileAndRunJavaFile();
        cr.compileAndRunJavaFile(code.toString());
        if(cr.isCompileAndRunOK()) {
            System.out.println("运行时间: " + cr.getUseTime() + "ms");
            System.out.println("内存使用: " + cr.getUseMemory() + "kb9");
            System.out.println("运行结果: \n" + cr.getOutMsg());
        } else if(cr.isCompilerError()) {
            System.out.println("编译错误: " + cr.getCE());
        } else if(cr.isRunningError()) {
            System.out.println("运行错误: " + cr.getError());
        }
    }
    //编译错误
    private StringBuilder ce = new StringBuilder();
    public String getCE(){
        return ce.toString();
    }
    
    //内存使用
    private double useMemory = 0.0;
    public double getUseMemory(){
        return useMemory;
    }
    
    //运行时间
    private long useTime = 0;
    public long getUseTime(){
        return useTime;
    }
    //输出信息
    private StringBuilder outMsg = new StringBuilder();
    public String getOutMsg(){
        return outMsg.toString();
    }
    //异常信息
    private String error = null;
    public String getError(){
        return error;
    }
    //是否正常编译并运行
    private boolean isCompileAndRunOK = false; 
    
    public boolean isCompileAndRunOK(){
        return isCompileAndRunOK;
    }
    
    //程序的运行时间, 单位：ms
    private int limitTime = 2000;
    //程序所占内存, 单位 ：KB
    private double limitMemory = 256000.0;
    
    public void setLimitTime(int limitTime){
        this.limitTime = limitTime;
    }
    
    public void setLimitMemory(double limitMemory){
        this.limitMemory = limitMemory;
    }
    
    //是否为编译错误
    private boolean isCompilerError = false;
    public boolean isCompilerError(){
        return isCompilerError;
    }
    
    //是否为运行错误
    private boolean isRunningError = false;
    public boolean isRunningError(){
        return isRunningError;
    }
    
    private static final String className = "Main";
    private static final String methodName = "main";
    private String getClassOutput(){
        //设置class文件的存放位置
        if(System.getProperty("java.class.path").contains("bin")) return "bin/";
        else return "./";
    }
    
    private void compileAndRunJavaFile(String code){
        PrintStream ps = null;
        FileInputStream fis = null;
        BufferedReader br = null;
        //保存标准输出流
        InputStream stdIn = System.in;
        //保存标准输入流
        PrintStream stdOut = System.out;
        
        //为源代码导入默认的包
        code = "import java.util.*;\n" + code;
        try {
            JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
            // define the diagnostic object, which will be used to save the
            // diagnostic information
            DiagnosticCollector<JavaFileObject> oDiagnosticCollector = new DiagnosticCollector<JavaFileObject>();
            StandardJavaFileManager fileManager = compiler.getStandardFileManager(oDiagnosticCollector, null, null);
            // set class output location
            fileManager.setLocation(StandardLocation.CLASS_OUTPUT, Arrays.asList(new File[] { new File(getClassOutput()) }));
            
            StringSourceJavaObject sourceObject = new CompileAndRunJavaFile.StringSourceJavaObject(className, code);
            Iterable<? extends JavaFileObject> fileObjects = Arrays.asList(sourceObject);
            CompilationTask task = compiler.getTask(null, fileManager, oDiagnosticCollector, null, null, fileObjects);
            boolean result = task.call();
            
            if (result) {
                Runtime runtime = Runtime.getRuntime();
                Class<?> clazz = Class.forName(className);
                Method method = clazz.getMethod(methodName, new Class<?>[]{String[].class});
                
                //重置输入流， 需要存放数据文件的文件名
                fis = new FileInputStream(new File("数据文件地址"));
                System.setIn(fis);
                //重置输出流，需要获得控制台的输出
                ByteArrayOutputStream bao = new ByteArrayOutputStream();
                ps = new PrintStream(bao);
                System.setOut(ps);
                
                long startFreeMemory = runtime.freeMemory();//Java 虚拟机中的空闲内存量
                //执行时间也是无法知道，因为dos执行java命令，程序无法知道它到底执行到那里了，两个进程，互不了解
                long startCurrentTime = System.currentTimeMillis();//获取系统当前时间
                method.invoke(null, new Object[]{null});
                long endCurrentTime = System.currentTimeMillis();
                long endFreeMemory = runtime.freeMemory();
                //内存的使用情况，不是很精确
                useMemory = (startFreeMemory-endFreeMemory)/1024.0;
                if(useMemory > limitMemory) throw new Exception("Out Limit Memory!");
                useTime = endCurrentTime-startCurrentTime;
                if(useTime > limitTime) throw new Exception("Time Limited!");
                
                //获得控制台的输出
                br = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(bao.toByteArray())));
                String outc = null;
                while((outc = br.readLine()) != null)
                    outMsg.append(outc).append("\n");
                //正常编译并运行
                isCompileAndRunOK = true;
            } else {
                isCompilerError = true;
                //打印编译的错误信息
                Pattern p = Pattern.compile("Main.java\\D*(\\d+):", Pattern.DOTALL);
                for (Diagnostic<? extends JavaFileObject> oDiagnostic : oDiagnosticCollector.getDiagnostics()){
                    /*信息示例：
                      Compiler Error: Main.java:8: 找不到符号
                         符号： 类 Scanner
                                位置： 类 Main
                    */
                    //将行号减1
                    Matcher m = p.matcher("Compiler Error: " + oDiagnostic.getMessage(null));
                    if(m.find()) {
                        ce.append(m.replaceAll("Main.java " + String.valueOf(Integer.valueOf(m.group(1))-1)) + ":").append("\n");
                    } else {
                        ce.append("Compiler Error: " + oDiagnostic.getMessage(null)).append("\n");
                    }
                }
            }
            
        } catch (Exception e) {
            isRunningError = true;
            StringWriter sw = new StringWriter();
            e.printStackTrace(new PrintWriter(sw, true));
            Pattern p = Pattern.compile("Main.java\\D*(\\d+)", Pattern.DOTALL);
            Matcher m = p.matcher(sw.toString());
            if(m.find()){
                error = m.replaceAll("Main.java " + String.valueOf(Integer.valueOf(m.group(1))-1) + ":");
            } else {
                error = sw.toString();
            }
        } finally {
            //关闭流
            try {
                if(fis != null)
                    fis.close();
                if(ps != null)
                    ps.close();    
                if(br != null)
                    br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            //恢复输入输出流
            System.setIn(stdIn);
            System.setOut(stdOut);
        }
    }
    
    private class StringSourceJavaObject extends SimpleJavaFileObject {
        private String content = null;
        public StringSourceJavaObject(String name, String content) {
            super(URI.create(name.replace('.', '/') + Kind.SOURCE.extension), Kind.SOURCE);
            this.content = content;
        }
        public CharSequence getCharContent(boolean ignoreEncodingErrors) {
            return content;
        }
    }
}
```
运行结果：
**1.正常运行**
运行时间: 16ms
内存使用: 225.5546875kb
运行结果: 
5 4 3 2 1 
**2.编译错误**
编译错误: Compiler Error: Main.java 8 找不到符号
符号： 类 Scanner
位置： 类 Main:
Compiler Error: Main.java 8 找不到符号
符号： 类 Scanner
位置： 类 Main:
**3.运行错误**
(1)运行错误: java.lang.reflect.InvocationTargetException
at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
at sun.reflect.NativeMethodAccessorImpl.invoke(Unknown Source)
at sun.reflect.DelegatingMethodAccessorImpl.invoke(Unknown Source)
at java.lang.reflect.Method.invoke(Unknown Source)
at com.test.CompileAndRunJavaFile.compileAndRunJavaFile(CompileAndRunJavaFile.java:163)
at com.test.CompileAndRunJavaFile.main(CompileAndRunJavaFile.java:44)
Caused by: java.lang.StackOverflowError
at Main.fun(Main.java 4:)
at Main.fun(Main.java 4:)
**(2)运行错误**: java.lang.reflect.InvocationTargetException
at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
at sun.reflect.NativeMethodAccessorImpl.invoke(Unknown Source)
at sun.reflect.DelegatingMethodAccessorImpl.invoke(Unknown Source)
at java.lang.reflect.Method.invoke(Unknown Source)
at com.test.CompileAndRunJavaFile.compileAndRunJavaFile(CompileAndRunJavaFile.java:163)
at com.test.CompileAndRunJavaFile.main(CompileAndRunJavaFile.java:44)
Caused by: java.lang.ArrayIndexOutOfBoundsException: 6
at Main.main(Main.java 18:)
... 6 more
demo：
```java
public class Main {
    public static void fun(){
        fun();
    }
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        int n = scan.nextInt();
        int[] array = new int[n];
        for(int i=0; i<n; ++i)
            array[i] = scan.nextInt();
        
        for(int i=0; i<n; ++i)
            System.out.print(array[i] + " ");
        System.out.println();
        
        //array[n+1] = 0;
        //fun();
    }
 
}
```
