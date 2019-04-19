# Item组件 - LC900730的博客 - CSDN博客
2017年09月15日 18:10:18[lc900730](https://me.csdn.net/LC900730)阅读数：130
## Qt的Item组件
Qt Quick的所有可视化项目都继承自Item，单独的Item并没有可视化外观，但是他定义了可视化项目的所有通用特性。
### 1.作为容器
常用于对项目进行分组。 
如：包含一个图片Image对象和一个矩形Rectangle对象
```
Item{
    Image{
        x:80
        width:100
        height:100
        source:"title.png"
    }
    Rectangle{
        x:190;
        width:100
        height:100
    }
}
```
### 2.默认属性
Item有一个children属性和一个resource属性，前缀包含了可见孩子列表，后者包含不可见的资源。
```
Item{
    children:[
        Text{},
        Rectangle{}
    ]
    resource:[
        Timer{}
    ]
}
```
