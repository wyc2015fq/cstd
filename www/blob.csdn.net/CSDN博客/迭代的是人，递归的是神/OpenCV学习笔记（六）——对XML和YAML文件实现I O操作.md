# OpenCV学习笔记（六）——对XML和YAML文件实现I/O操作 - 迭代的是人，递归的是神 - CSDN博客





2011年10月23日 10:22:51[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：38615
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









## 1. XML、YAML文件的打开和关闭

XML\YAML文件在OpenCV中的数据结构为FileStorage，打开操作例如：



```cpp
string filename = "I.xml";
FileStorage fs(filename, FileStorage::WRITE);
\\...
fs.open(filename, FileStorage::READ);
```

文件关闭操作会在FileStorage结构销毁时自动进行，但也可调用如下函数实现




```cpp
fs.release();
```




**2.文本和数字的输入和输出**



写入文件使用  <<  运算符，例如：



```cpp
fs << "iterationNr" << 100;
```

读取文件，使用 >> 运算符，例如






```cpp
int itNr;
fs["iterationNr"] >> itNr;
itNr = (int) fs["iterationNr"];
```


**3. OpenCV数据结构的输入和输出，和基本的C++形式相同**






```cpp
Mat R = Mat_<uchar >::eye (3, 3),
T = Mat_<double>::zeros(3, 1);
fs << "R" << R; // Write cv::Mat
fs << "T" << T;
fs["R"] >> R; // Read cv::Mat
fs["T"] >> T;
```


**4. vector（arrays） 和 maps的输入和输出**




vector要注意在第一个元素前加上“[”，在最后一个元素前加上"]"。例如：



```cpp
fs << "strings" << "["; // text - string sequence
fs << "image1.jpg" << "Awesomeness" << "baboon.jpg";
fs << "]"; // close sequence
```

对于map结构的操作使用的符号是"{"和"}"，例如：






```cpp
fs << "Mapping"; // text - mapping
fs << "{" << "One" << 1;
fs << "Two" << 2 << "}";
```

读取这些结构的时候，会用到FileNode和FileNodeIterator数据结构。对FileStorage类的[]操作符会返回FileNode数据类型，对于一连串的node，可以使用FileNodeIterator结构，例如：






```cpp
FileNode n = fs["strings"]; // Read string sequence - Get node
if (n.type() != FileNode::SEQ)
{
cerr << "strings is not a sequence! FAIL" << endl;
return 1;
}
FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
for (; it != it_end; ++it)
cout << (string)*it << endl;
```


**5. 读写自己的数据结构**




这部分比较复杂，参考最后的实例中的MyData结构自己领悟吧

最后，我这里上一个实例，供大家参考。

源文件里填入如下代码：





```cpp
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void help(char** av)
{
    cout << endl 
        << av[0] << " shows the usage of the OpenCV serialization functionality."         << endl
        << "usage: "                                                                      << endl
        <<  av[0] << " outputfile.yml.gz"                                                 << endl
        << "The output file may be either XML (xml) or YAML (yml/yaml). You can even compress it by "
        << "specifying this in its extension like xml.gz yaml.gz etc... "                  << endl
        << "With FileStorage you can serialize objects in OpenCV by using the << and >> operators" << endl
        << "For example: - create a class and have it serialized"                         << endl
        << "             - use it to read and write matrices."                            << endl;
}

class MyData
{
public:
    MyData() : A(0), X(0), id()
    {}
    explicit MyData(int) : A(97), X(CV_PI), id("mydata1234") // explicit to avoid implicit conversion
    {}
    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "A" << A << "X" << X << "id" << id << "}";
    }
    void read(const FileNode& node)                          //Read serialization for this class
    {
        A = (int)node["A"];
        X = (double)node["X"];
        id = (string)node["id"];
    }
public:   // Data Members
    int A;
    double X;
    string id;
};

//These write and read functions must be defined for the serialization in FileStorage to work
void write(FileStorage& fs, const std::string&, const MyData& x)
{
    x.write(fs);
}
void read(const FileNode& node, MyData& x, const MyData& default_value = MyData()){
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

// This function will print our custom class to the console
ostream& operator<<(ostream& out, const MyData& m) 
{ 
    out << "{ id = " << m.id << ", ";
    out << "X = " << m.X << ", ";
    out << "A = " << m.A << "}";
    return out;
}

int main(int ac, char** av)
{
    if (ac != 2)
    {
        help(av);
        return 1;
    }

    string filename = av[1];
    { //write
        Mat R = Mat_<uchar>::eye(3, 3),
            T = Mat_<double>::zeros(3, 1);
        MyData m(1);

        FileStorage fs(filename, FileStorage::WRITE);

        fs << "iterationNr" << 100;
        fs << "strings" << "[";                              // text - string sequence
        fs << "image1.jpg" << "Awesomeness" << "baboon.jpg";
        fs << "]";                                           // close sequence
        
        fs << "Mapping";                              // text - mapping
        fs << "{" << "One" << 1;
        fs <<        "Two" << 2 << "}";               

        fs << "R" << R;                                      // cv::Mat
        fs << "T" << T;

        fs << "MyData" << m;                                // your own data structures

        fs.release();                                       // explicit close
        cout << "Write Done." << endl;
    }

    {//read
        cout << endl << "Reading: " << endl;
        FileStorage fs; 
        fs.open(filename, FileStorage::READ);

        int itNr; 
        //fs["iterationNr"] >> itNr;
        itNr = (int) fs["iterationNr"];
        cout << itNr;
        if (!fs.isOpened())
        {
            cerr << "Failed to open " << filename << endl;
            help(av);
            return 1;
        }

        FileNode n = fs["strings"];                         // Read string sequence - Get node
        if (n.type() != FileNode::SEQ)
        {
            cerr << "strings is not a sequence! FAIL" << endl;
            return 1;
        }

        FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
        for (; it != it_end; ++it)
            cout << (string)*it << endl;
        
        
        n = fs["Mapping"];                                // Read mappings from a sequence
        cout << "Two  " << (int)(n["Two"]) << "; "; 
        cout << "One  " << (int)(n["One"]) << endl << endl; 
        

        MyData m;
        Mat R, T;

        fs["R"] >> R;                                      // Read cv::Mat
        fs["T"] >> T;
        fs["MyData"] >> m;                                 // Read your own structure_

        cout << endl 
            << "R = " << R << endl;
        cout << "T = " << T << endl << endl;
        cout << "MyData = " << endl << m << endl << endl;

        //Show default behavior for non existing nodes
        cout << "Attempt to read NonExisting (should initialize the data structure with its default).";  
        fs["NonExisting"] >> m;
        cout << endl << "NonExisting = " << endl << m << endl;
    }

    cout << endl 
        << "Tip: Open up " << filename << " with a text editor to see the serialized data." << endl;

    return 0;
}
```

编译后，在命令行进入到文件目录，执行test test.xml，运行结果如下，生成一个test . xml文件，内容如下：


```
<?xml version="1.0" ?> 
- <opencv_storage>
  <iterationNr>100</iterationNr> 
  <strings>image1.jpg Awesomeness baboon.jpg</strings> 
- <Mapping>
  <One>1</One> 
  <Two>2</Two> 
  </Mapping>
- <R type_id="opencv-matrix">
  <rows>3</rows> 
  <cols>3</cols> 
  <dt>u</dt> 
  <data>1 0 0 0 1 0 0 0 1</data> 
  </R>
- <T type_id="opencv-matrix">
  <rows>3</rows> 
  <cols>1</cols> 
  <dt>d</dt> 
  <data>0. 0. 0.</data> 
  </T>
- <MyData>
  <A>97</A> 
  <X>3.1415926535897931e+000</X> 
  <id>mydata1234</id> 
  </MyData>
  </opencv_storage>
```







