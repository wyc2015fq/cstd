# js中的json对象和字符串之间的转化 - z69183787的专栏 - CSDN博客
2014年09月05日 09:42:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1205
字符串转对象(strJSON代表json字符串) 
  var obj = eval(strJSON); 
  var obj = strJSON.parseJSON(); 
  var obj = JSON.parse(strJSON)； 
json对象转字符串(obj代表json对象) 
  var str = obj.toJSONString(); 
  var str = JSON.stringify(obj) 
运用时候需要除了eval()以外需要json.js包（切记哦） 
