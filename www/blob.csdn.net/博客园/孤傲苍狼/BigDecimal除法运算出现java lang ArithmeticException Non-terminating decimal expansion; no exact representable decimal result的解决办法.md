# BigDecimal除法运算出现java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result的解决办法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [BigDecimal除法运算出现java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result的解决办法](https://www.cnblogs.com/xdp-gacl/p/4000450.html)



　　今天在使用两个BigDecimal类型的数字做除法运算时，出现了一个如下的异常信息：

```
1 java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result
```

　　上网查了一下这个异常的，找到了原因所在：**通过BigDecimal的divide方法进行除法时当不整除，出现无限循环小数时，就会抛异常：java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result. **
　　解决的办法就是给divide方法设置精确的小数点，如：divide(xxxxx,2)。









