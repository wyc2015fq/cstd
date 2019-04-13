
# java获取键盘输入 - bitcarmanlee的博客 - CSDN博客


2017年05月25日 17:39:39[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1147



## 1.使用System.in.read
此种方法能从控制台接收一个字符，并且将该字符打印出来
```python
public
```
```python
static
```
```python
void
```
```python
t1
```
```python
() throws IOException {
        System.
```
```python
out
```
```python
.println(
```
```python
"Enter a Char: "
```
```python
);
```
```python
char
```
```python
c = (
```
```python
char
```
```python
) System.
```
```python
in
```
```python
.read();
        System.
```
```python
out
```
```python
.println(
```
```python
"your char is: "
```
```python
+ c);
    }
```
此方法的缺点显而易见：
1.每次只能获取一个字符。
2.read方法获取的是int类型，需要根据需求做各种类型转换。
## 2.使用BufferedReader
```python
public
```
```python
static
```
```python
void
```
```python
t2
```
```python
() {
        BufferedReader br =
```
```python
new
```
```python
BufferedReader(
```
```python
new
```
```python
InputStreamReader(System.
```
```python
in
```
```python
));
        String str;
```
```python
try
```
```python
{
            System.
```
```python
out
```
```python
.println(
```
```python
"please enter your name: "
```
```python
);
            String name = br.readLine();
            System.
```
```python
out
```
```python
.println(
```
```python
"Your name is: "
```
```python
+ name);
        }
```
```python
catch
```
```python
(IOException e) {
            e.printStackTrace();
        }
    }
```
这种方法可以从控制台接受一个字符串，。并且打印出来。但是这种方式对于多次输入也不是很方便。
## 3.使用Scanner
```python
public
```
```python
static
```
```python
void
```
```python
t3
```
```python
() {
        Scanner sc =
```
```python
new
```
```python
Scanner(System.
```
```python
in
```
```python
);
        System.
```
```python
out
```
```python
.println(
```
```python
"please input your name: "
```
```python
);
        String name = sc.nextLine();
        System.
```
```python
out
```
```python
.println(
```
```python
"please input your age: "
```
```python
);
```
```python
int
```
```python
age = sc.nextInt();
        System.
```
```python
out
```
```python
.println(
```
```python
"please input your salary: "
```
```python
);
```
```python
float
```
```python
salary = sc.nextFloat();
        System.
```
```python
out
```
```python
.println(
```
```python
"your msg is: "
```
```python
);
        System.
```
```python
out
```
```python
.println(
```
```python
"name: "
```
```python
+ name +
```
```python
", age: "
```
```python
+ age +
```
```python
", salary: "
```
```python
+ salary);
    }
```
从 JDK 5.0 开始，基本类库中增加了java.util.Scanner类，根据它的 API 文档说明，这个类是采用正则表达式进行基本类型和字符串分析的文本扫描器。使用它的Scanner(InputStream source)构造方法，可以传入系统的输入流System.in而从控制台中读取数据。
从这三种方式的对比很容易看出，用Scanner的方式获取数据是最容易与方便的！

