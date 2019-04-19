# Angular教程测试运行过程中产生的一些问题 - =朝晖= - 博客园
# [Angular教程测试运行过程中产生的一些问题](https://www.cnblogs.com/dhcn/p/7709785.html)
      主要问题就是最后一个HTTP Demo需要angular-in-memory-web-api模块：
```
TS2307:Cannot find module 'angular-in-memory-web-api'.
```
      这个时候如果用
```
npm install angular-in-memory-web-api --save
```
      有可能出不来结果，出现状况是，component从service的相关接口取不到数据，最后看了官方代码，猜测有可能是这个模块的版本问题，于是乎改了package.json:
```
"angular-in-memory-web-api": "^0.4.0",
```
      运行恢复正常

