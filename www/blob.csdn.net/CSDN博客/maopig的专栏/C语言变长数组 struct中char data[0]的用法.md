# C语言变长数组 struct中char data[0]的用法 - maopig的专栏 - CSDN博客
2013年12月08日 09:47:08[maopig](https://me.csdn.net/maopig)阅读数：1449

摘要：在实际的编程中，我们经常需要使用变长数组，但是C语言并不支持变长的数组。此时，我们可以使用结构体的方法实现C语言变长数组。
![/](http://www.th7.cn/d/file/p/2013/12/03/284befdbbde1027d0fd8b020cb78e7ba.gif)struct MyData 
![/](http://www.th7.cn/d/file/p/2013/12/03/707fe0bc349e7639b34c9235b67a55dd.gif){
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) int nLen;
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) char data[0];
![/](http://www.th7.cn/d/file/p/2013/12/03/21e8d82b700c520a691d814bf64fada3.gif)}; 
在结构中，data是一个数组名；但该数组没有元素；该数组的真实地址紧随结构体MyData之后，而这个地址就是结构体后面数据的地址（如果给这个结构体分配的内容大于这个结构体实际大小，后面多余的部分就是这个data的内容）；这种声明方法可以巧妙的实现C语言里的数组扩展。
实际用时采取这样：
struct MyData *p = (struct MyData *)malloc(sizeof(struct MyData )+strlen(str))
这样就可以通过p->data 来操作这个str。
程序实例：
![/](http://www.th7.cn/d/file/p/2013/12/03/284befdbbde1027d0fd8b020cb78e7ba.gif)struct MyData 
![/](http://www.th7.cn/d/file/p/2013/12/03/707fe0bc349e7639b34c9235b67a55dd.gif){
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) int nLen;
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) char data[0];
![/](http://www.th7.cn/d/file/p/2013/12/03/21e8d82b700c520a691d814bf64fada3.gif)};
![/](http://www.th7.cn/d/file/p/2013/12/03/284befdbbde1027d0fd8b020cb78e7ba.gif)
![/](http://www.th7.cn/d/file/p/2013/12/03/284befdbbde1027d0fd8b020cb78e7ba.gif)int main()
![/](http://www.th7.cn/d/file/p/2013/12/03/707fe0bc349e7639b34c9235b67a55dd.gif){
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) int nLen = 10;
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) char str[10] = "123456789";
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif)
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) cout << "Size of MyData: " <<
sizeof(MyData) << endl;
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif)
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) MyData *myData = (MyData*)malloc(sizeof(MyData) +
 10);
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) memcpy(myData->data, str, 10);
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif)
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) cout << "myData's Data is: " << myData->data << endl;
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif)
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) free(myData);
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif)
![/](http://www.th7.cn/d/file/p/2013/12/03/0bb4c39f4de9e0edf3fcb730cdc505cf.gif) return 0;
![/](http://www.th7.cn/d/file/p/2013/12/03/21e8d82b700c520a691d814bf64fada3.gif)}
输出：
![/](http://www.th7.cn/d/file/p/2013/12/03/284befdbbde1027d0fd8b020cb78e7ba.gif)Size of MyData:
4
![/](http://www.th7.cn/d/file/p/2013/12/03/284befdbbde1027d0fd8b020cb78e7ba.gif)myData"s Data is: 123456789
