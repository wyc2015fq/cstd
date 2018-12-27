var x = 10;
 
(function foo() {
  var y = 20;
  (function bar() {
    var z = 30;
    // "x"和"y"是自由变量
    // 会在作用域链的下一个对象中找到（函数”bar”的互动对象之后）
    console.log(x + y + z);
  })();
})();
