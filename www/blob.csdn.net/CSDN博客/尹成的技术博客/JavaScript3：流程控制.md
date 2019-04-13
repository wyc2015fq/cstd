
# JavaScript3：流程控制 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:04:51[尹成](https://me.csdn.net/yincheng01)阅读数：32


\#条件语句
通过条件来控制程序的走向，就需要用到条件语句。
\#\#运算符
1、算术运算符： +(加)、 -(减)、 *(乘)、 /(除)、 %(求余)
2、赋值运算符：=、 +=、 -=、 *=、 /=、 %=
3、条件运算符：、=、>、>=、<、<=、!=、&&(而且)、||(或者)、!(否)
\#\#if else
`var a = 6;
if(a==1)
{
    alert('语文');
}
else if(a==2)
{
    alert('数学');
}
else if(a==3)
{
    alert('英语');
}
else if(a==4)
{
    alert('美术');
}
else if(a==5)
{
    alert('舞蹈');
}
else
{
    alert('不补习');
}`\#\#switch
`var a = 6;
switch (a){
    case 1:
        alert('语文');
        break;
    case 2:
        alert('数学');
        break;
    case 3:
        alert('英语');
        break;
    case 4:
        alert('美术');
        break;
    case 5:
        alert('舞蹈');
        break;
    default:
        alert('不补习');
}`\#循环语句
程序中进行有规律的重复性操作，需要用到循环语句。
\#\#for循环
`for(var i=0;i<len;i++)
{
    ......
}`\#\#while循环
`var i=0;
while(i<8){
    ......
    i++;
}`\#\#数组去重
`var aList = [1,2,3,4,4,3,2,1,2,3,4,5,6,5,5,3,3,4,2,1];
var aList2 = [];
for(var i=0;i<aList.length;i++)
{
    if(aList.indexOf(aList[i])==i)
    {
        aList2.push(aList[i]);
    }
}
alert(aList2);`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

