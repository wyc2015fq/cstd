# js中的for和foreach - z69183787的专栏 - CSDN博客
2014年03月02日 10:16:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8776
js中没有foreach这个关键字，但是可以用var v in array来实现遍历。但是需要注意的是，
拿到的是key而不是value。看例子：
<script type="text/javascript">
**//****普通数组**
              var intArray = new Array();
              intArray[0] = "第一个";
              intArray[1] = "第二个";
**     for(var i = 0; i<intArray.length;i++)**
              {
                     alert(intArray[i]);    //第一个,第二个
              }
              //拿到的是下标(像dictionary的key)
**for(var key in intArray)**
              {
                     alert(key);       // 0,1
              }
**//****字典数组**
              var dicArray = new Array();
              dicArray["f"] = "第一个";
              dicArray["s"] = "第二个";
              //无法取到
**for(var i = 0; i<dicArray.length;i++)**
              {
                     alert(dicArray[i]);
              }
              //拿到的是下标
**for(var key in dicArray)**
              {
                     alert(key);       // f,s
              }
</script>
