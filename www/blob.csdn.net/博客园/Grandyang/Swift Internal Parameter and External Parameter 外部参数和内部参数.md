# Swift Internal Parameter and External Parameter 外部参数和内部参数 - Grandyang - 博客园







# [Swift Internal Parameter and External Parameter 外部参数和内部参数](https://www.cnblogs.com/grandyang/p/4086310.html)





今天跟大神又学习了些关于IOS开发Swift语言的外部参数和内部参数



```
func doSomething(num1: Int, num2: Int) -> Int {
    return num1 + num2
}

doSomething(1, 2)

class A {
    func foo(#num1: Int, num2: Int) {
    
    }
    
    func objectAtIndex(num: Int forArray array: Int) {
    
    }
}

var a = A()

a.foo(num1: 1, num2: 12)

a.objectAtIndex(12, forArray: 12)
```














