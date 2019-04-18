# 关于Promise中resolve解析出现的异常 - weixin_33985507的博客 - CSDN博客
2018年02月27日 17:29:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
```bash
var p3 = new Promise( function(resolve,reject){
	resolve( "B" );
} );
var p1 = new Promise( function(resolve,reject){
	resolve( p3 );
} );
var p2 = new Promise( function(resolve,reject){
	resolve( "A" );
} );
p1.then( function(v){
	console.log( v );
} );
p2.then( function(v){
	console.log( v );
} );
//在chrome中打印A,B
//在node 9.0中打印B,A
复制代码
```
复习Promise，看到以上内容，瞬间爆炸，试图理解，查询诸多资料后得出结论，Promise中解析其他Promise的操作，不属于规范内容，是由宿主环境自行实现。【手动滑稽】
分割线
发现了更详细的解释，豁然开朗，具体参考[知乎-为什么下面这段 Promise 代码在不同浏览器表现不一致？](https://link.juejin.im?target=https%3A%2F%2Fwww.zhihu.com%2Fquestion%2F56549795%2Fanswer%2F150097440)
