# opencv（二）：FileStorage类 - Keith - CSDN博客





2017年05月15日 23:18:51[ke1th](https://me.csdn.net/u012436149)阅读数：2139








# opencv  FileStorage类

使用这个工具类，我们可以将 `opencv` 中的数据结构(或者`int`,`float`,`string`)保存到 `XML/YAML` 文件中去。或者从`XML/YAML`文件中加载这些数据。

**这个类的声明在 “opencv2\core.hpp” 头文件中**

## 保存

代码一：

```
FileStorage fs("test.xml", FileStorage::WRITE);

fs <<"name" << "keithyin";
fs<< "age"<<18;
//等价于
fs <<"name" << "keithyim"<<"age"<<18;

fs.release();
```

```xml
<?xml version="1.0"?>
<opencv_storage>
<name>keithyim</name>
<age>18</age>
</opencv_storage>
```

代码二：

```
FileStorage fs("test.xml", FileStorage::WRITE);
fs <<"name" << "[" <<"first name"<<"keith" <<"last name"<<"yin"<<"]";
fs.release();
```

```xml
<?xml version="1.0"?>
<opencv_storage>
<name>
  "first name" keith "last name" yin
</name>
</opencv_storage>
```

看出有”[]” 和 没 “[]”的不同之处了吗。”[]”表示里面的东西是个序列 `string`。

代码三：

```
FileStorage fs("test.xml", FileStorage::WRITE);
    fs <<"name" << "{" <<"first_name"<<"keith" <<"last_name"<<"yin"<<"}";
    fs.release();
```

```xml
<?xml version="1.0"?>
<opencv_storage>
<name>
  <first_name>keith</first_name>
  <last_name>yin</last_name></name>
</opencv_storage>
```

`{}`表示里面的东西还是键值对。

## 读取

```
FileStorage fs("test.xml", FileStorage::READ);
string name;
fs["name"]["first_name"] >> name;
cout << name << endl;
fs.release();
```
`keith`
这样就读出来了。

## 存储Mat类型数据

```
void test(){
    FileStorage fs("test.xml", FileStorage::WRITE);
    Mat mat(2, 2, CV_8UC3);
    fs << "mat_val" << mat;
    fs.release();
}
```

`test.xml`中数据的内容是：

```xml
<?xml version="1.0"?>
<opencv_storage>
<mat_val type_id="opencv-matrix">
  <rows>2</rows>
  <cols>2</cols>
  <dt>"3u"</dt>
  <data>
    205 205 205 205 205 205 205 205 205 205 205 205</data></mat_val>
</opencv_storage>
```

## 读取保存的Mat类型数据

```
void test(){
    FileStorage fs("test.xml", FileStorage::READ);
    Mat mat;
    fs["mat_val"] >> mat;
    cout << mat << endl;
    fs.release();
}
```

```
[205, 205, 205, 205, 205, 205;
  205, 205, 205, 205, 205, 205]
```

## 参考资料

[http://www.cnblogs.com/fourseas/p/5519681.html](http://www.cnblogs.com/fourseas/p/5519681.html)



