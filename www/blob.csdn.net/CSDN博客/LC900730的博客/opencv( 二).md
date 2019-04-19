# opencv( 二) - LC900730的博客 - CSDN博客
2017年09月28日 13:34:58[lc900730](https://me.csdn.net/LC900730)阅读数：107标签：[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[opencv](https://blog.csdn.net/LC900730/article/category/7199508)
## imread()
```
//@param1:图片路径名称
//@param2:int类型的flags，载入标识，指定一个加载图像的颜色类型，自带默认值1.省略则表示三通道的彩色图像。
Mat imread(const string &filename,int flag=1);
//higui.h
enum{
    //将图像转换成灰度在返回，新版本废弃
    CV_LOAD_IMAGE_GRAYSCALE=0;
    //总是转换图像到彩色在返回
    CV_LOAD_IMAGE_COLOR=1;
    //如果取2，且载入的图像是16位或者32位，就返回对应深度的图像。否则转换成8位图像在返回
    CV_LOAD_IMAGE_ANYDEPTH=2;
    CV_LOAD_IMAGE_ANYCOLOR=4;
}
解码后的图像会以BGR的通道顺序存储，不是通常的RGB顺序。
<h2>图像的显示：imshow()</h2>
```CPP
//@param1:const string &类型的winname，需要显示的窗口的标识名称
//@param2:InputArray类型的mat，需要显示的图像
void isshow(const string &winname,InputArray mat);
如果图像是CV_WINDOW_AUTOSIZE(默认值)标识创建的，那么显示图像原始大小。否则缩放图像得以适合窗口。而isshow函数缩放图像，取决于图像的深度。
<div class="se-preview-section-delimiter"></div>
```
- 1.如果载入的图像是8位无符号类型，显示图像本来样子。
- 2.如果载入的图像是16位无符号类型或者32位整型，用像素值除以256。值的范围是[0,255*255]映射到[0,255]。
- 3.如果载入的图像是32位浮点型，用像素值乘以255。值的范围是[0,1]映射到[0,255]。
## 输出图像到文件imwrite()
“` 
imwrite(const string &filename,Inputarray img,const vector¶ms=vector())
```xml
<li>1.如果载入的图像是8位无符号类型，显示图像本来样子。</li>
<li>2.如果载入的图像是16位无符号类型或者32位整型，用像素值除以256。值的范围是[0,255*255]映射到[0,255]。</li>
<li>3.如果载入的图像是32位浮点型，用像素值乘以255。值的范围是[0,1]映射到[0,255]。</li>
<h2>输出图像到文件imwrite()</h2>
```CPP
//@param1:filename,需要加扩展名字
//@param2:Inputarray类型的img，一般需要填写Mat类型的图像数据
//@param3:const vector<int> &类型的params，表示为特定格式保存的参数编码。
    默认值是vector<int>()
imwrite(const string &filename,Inputarray img,const vector<int>¶ms=vector<int>())
<div class="se-preview-section-delimiter"></div>
```
## 鼠标操作
opencv中的鼠标操作和滑动条的消息映射方法很类似，都是通过一个中介函数配合一个回调函数来实现的。 
创建滑动条的回调函数是createTrackBar; 
而指定鼠标操作消息回调函数为SetMouseCallback。 
SetMouseCallback是为指定的窗口设置鼠标回调函数。 
“`
```
<h2>鼠标操作</h2>
opencv中的鼠标操作和滑动条的消息映射方法很类似，都是通过一个中介函数配合一个回调函数来实现的。
创建滑动条的回调函数是createTrackBar;
而指定鼠标操作消息回调函数为SetMouseCallback。
SetMouseCallback是为指定的窗口设置鼠标回调函数。
```CPP
//@param1:const string &类型的窗口名字
//@param2:MouseCallBack类型的onMouse，指定窗口里每次鼠标事件发生的时候，被调用的函数指针
//@param3:void *类型的userdata，用户定义的传递到回调函数的参数，有默认值是0
void SetMouseCallback(const string&winname,MouseCallback onMouse, 
                        void *userdata=0)
函数原型类似：
@param1:event是EVENT_+变量之一
@param2:x,y是鼠标指针在图像坐标系(不是窗口坐标系)的坐标值；
@param3: EVENT_FLAG的组合
@param4:是用户定义的传递到2SetMouseCallback函数调用的参数，
        如EVENT_MOUSEMOVE为鼠标移动，
        如EVENT_LBUTTONDOWN为鼠标左键按下消息
void Foo(int event,int x,int y,int flags,void * param);
```
