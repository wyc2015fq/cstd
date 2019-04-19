# 【linux指令】dialog实现终端下的GUI-1 - Koma Hub - CSDN博客
2019年01月08日 22:46:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：57
**目录**
[--title和--msgbox](#--title%E5%92%8C--msgbox)
[--inputbox](#--inputbox)
[--infobox](#--infobox)
[--menu](#--menu)
|类型|用于创建类型的选项|含义|
|----|----|----|
|复选框|--checklist|允许用户显示一个选项列表，每个选项都可以被单独选择|
|信息框|--infobox|在显示消息后，对话框将立刻返回，但并不清除屏幕|
|输入框|--inputbox|允许用户输入文本|
|菜单框|--menu|允许用户选择列表中的一项|
|消息框|--msgbox|向用户显示一条消息，同时显示一个OK按钮，用户可以通过选择该按钮继续操作|
|单选框|--radiolist|允许用户选择列表中的一个选项|
|文本框|--textbox|允许用户在带有滚动条的文本框中显示一个文件的内容|
|是/否框|--yesno|允许用户提问，用户可以选择yes或no|
# --title和--msgbox
```bash
$ dialog --title "Title" --msgbox "大风起兮云飞扬" 10 20 ; clear
```
![](https://img-blog.csdnimg.cn/20190108222616378.PNG)
# --inputbox
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
dialog --title "Title" --inputbox "大风起兮云飞扬?" 10 20 2> $tmp
clear
echo `cat $tmp`
```
![](https://img-blog.csdnimg.cn/20190108223748602.PNG)
# --infobox
```bash
#!/bin/bash
dialog --title "Title" --infobox "大风起兮云飞扬?" 10 20 
sleep 10
clear
```
![](https://img-blog.csdnimg.cn/20190108224020696.PNG)
# --menu
```bash
#!/bin/bash
tmp=$(mktemp tmpXXX.out)
dialog --title "Title" --menu "What your favorite food?" 15 30 4 \
    1 "meet" \
    2 "Meet" \
    3 "MEET" \
    2>$tmp
clear
echo `cat $tmp`
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190108224444230.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
今天就先写到这里吧，困了。
