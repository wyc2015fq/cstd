# 第四课：js对象 - PeterBishop - CSDN博客





2018年12月07日 20:29:56[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课来讲js对象


//这个用来写js对象相关的教学





*/** * js**对象的创建: * * var a = { *  属性名: 属性值, *  属性名: 属性值, *  属性名: { *      属性名: 属性值, *      ... *      属性名: 属性值 *  }, *  属性名: [属性值1，属性值2，...], *  属性名: 属性值 * }; */***var **stu = {

    id: 1001,

    name: 'xiaoye',

    age: 20

    //对象里当然也可以定义方法，但是通常我们后端人员不这么做，前端大佬才玩高端操作，我们会基本用法就行

};



**function **print(variable) {

    document.write(variable);

}



print(stu.id);

print(stu.name);

print(stu.age);




