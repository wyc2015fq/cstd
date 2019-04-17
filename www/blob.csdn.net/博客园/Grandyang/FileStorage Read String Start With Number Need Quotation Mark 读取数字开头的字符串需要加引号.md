# FileStorage Read String Start With Number Need Quotation Mark 读取数字开头的字符串需要加引号 - Grandyang - 博客园







# [FileStorage Read String Start With Number Need Quotation Mark 读取数字开头的字符串需要加引号](https://www.cnblogs.com/grandyang/p/4480803.html)







```
// Write data
    FileStorage fs("test.yml", FileStorage::WRITE);
    fs << "MyString" << "123abc";
    fs.release();

    // Read data
    FileStorage fs2("test.yml", FileStorage::READ);
    string str = fs2["MyString"];
    cout << "MyString = " << str << endl;
    fs2.release();
```



In OpenCV, the FileStorage class can read and write data in the file. If you write the string "123abc" into the file, it will shows with quotation mark in the file. Now I want to read this string from the file into a string variable, you need to keep the quotation mark in the file which should be see as follows:

```
MyString: "123abc"
```



Now it can be read without problem, but if you change it to the followings:

```
MyString: 123abc
```



This will not gonna work this time, but if the string is not start with a number, it will work. I might treat it as a bug in the OpenCV. The three different situation is as follows:

```
MyString: "123abc"    // Work
MyString: 123abc      // NOT work
MyString: abc123      // Work
```



In sum, when you want to read a string into a variable, if it starts with a number, you need to add the quotation marker to the whole string in the file.














