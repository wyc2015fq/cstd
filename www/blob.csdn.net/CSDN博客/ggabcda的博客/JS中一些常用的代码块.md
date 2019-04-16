# JS中一些常用的代码块 - ggabcda的博客 - CSDN博客





2017年09月14日 20:42:12[ggabcda](https://me.csdn.net/ggabcda)阅读数：173








本文记录了一些工作中常用到的js代码。

1. 生成指定范围内的随机数

例如随机获取颜色rgba的参数值时

function setRandomNum(m,n){

　　return Math.floor(Math.random()*(n-m+1)) + m;

}

2. json转url参数

当进行http请求，可能需要把json转化为url参数
function json2url(json) {
    var arr = [];
    for(var name in json) {
        arr.push(name+'='+json[name])
    }
    return arr.join('&')
}
3. 验证是否为数组
function isArray(obj) {
    return Array.isArray(obj) || Object.prototype.toString().call(obj) === '[object Array]';
}
4. 打乱数组的顺序
方式1:arr.sort(function () {
    return Math.random() - 0.5
})
方式2:function shuffle(arr) {
    var i,j,templ
    for(var i = arr.length - 1;i > 0;i--){
        j = Math.floor(Math.random() * (i + 1));
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}
5. 去除字符串的空格
function trim(str) {
    return str.replace(/(^\s*)|(\s*$)/g,"");//去除首尾空格
}function trim(str) {
    return str.replace(/(\s+)/g,"");//去除所有空格
}


6.获取数组中的最大值或最小值 
function maxAndMin(arr) {
    return {
　　　　max: Math.max.apply(null,arr.join(',').split(',')),
　　　　min: Math.min.apply(null,arr.join(',').split(','))　　}
}

















