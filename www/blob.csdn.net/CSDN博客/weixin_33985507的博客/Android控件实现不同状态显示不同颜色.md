# Android控件实现不同状态显示不同颜色 - weixin_33985507的博客 - CSDN博客
2016年12月21日 11:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：37
# 1. 对应的实体类 : ColorStateList
**!!! 不做解释 !!!**
# 2. XML 方式
### 2.1 xml 存放路径 : `res/color/filename.xml`
### 2.2 引用方式:
**(1) In Java : `R.color.filename`**
**(2) In XML : `@[package:]color/filename`**
### 2.3 XML 语法
```xml
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android" >
    <item
    
        android:color="hex_color"
        android:state_pressed=["true" | "false"]
        android:state_focused=["true" | "false"]
        android:state_selected=["true" | "false"]
        android:state_checkable=["true" | "false"]
        android:state_checked=["true" | "false"]
        android:state_enabled=["true" | "false"]
        android:state_window_focused=["true" | "false"] />
</selector>
```
**节点 :**
**<selector>**
- 根节点必须是 : `<selector>`
- 命名空间(`xmlns:android`)必须是 :`http://schemas.android.com/apk/res/android`
**<item>**
`android:color` : 颜色 , 必须指定.
### 2.4 XML 示例代码
**定义**
```xml
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android">
    <item android:state_pressed="true"
          android:color="#ffff0000"/> <!-- pressed -->
    <item android:state_focused="true"
          android:color="#ff0000ff"/> <!-- focused -->
    <item android:color="#ff000000"/> <!-- default -->
</selector>
```
**使用**
```xml
<Button
    android:layout_width="fill_parent"
    android:layout_height="wrap_content"
    android:text="@string/button_text"
    android:textColor="@color/button_text" />
```
