# PHP相关系列 - PHP细节拾遗 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月26日 12:58:05[initphp](https://me.csdn.net/initphp)阅读数：891







- isset函数，当变量不存在或者数组键值不存在的时候，isset值为false，其它为true

```php
<?php
/* 测试 isset 函数 */
//变量存在情况
$val = 'test';
echo (isset($val)) ? 'true' : 'false'; //true
echo '<br/>';
//变量为空
$val = '';
echo (isset($val)) ? 'true' : 'false';  //true
echo '<br/>';
//变量为true
$val = true;
echo (isset($val)) ? 'true' : 'false';  //true
echo '<br/>';
//变量为false
$val = false;
echo (isset($val)) ? 'true' : 'false';  //true
echo '<br/>';
//变量不存在
echo (isset($val2)) ? 'true' : 'false';  //false
echo '<br/>';
//变量为空数组
$val2 = array();
echo (isset($val2)) ? 'true' : 'false';  //true
echo '<br/>';
//检测变量键
$val2 = array();
echo (isset($val2['username'])) ? 'true' : 'false';  //false
echo '<br/>';
```
- 直接拿变量做判断，在变量值为false，未定义，0，NULL，空，空数组，数组键值不存在等这些情况下都会是false

```php
<?php
//为布尔true的情况
$val = true;
echo ($val) ? 'true' : 'false'; //true   
echo '<br/>';  
//为布尔false的情况
$val = false;
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//变量未定义
echo ($val2) ? 'true' : 'false'; //false  
echo '<br/>';
//变量为空
$val = '';
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//变量为NULL
$val = NULL;
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//变量为0
$val = 0;
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//变量为1
$val = 1;
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//空数组
$val = array();
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//非空数组
$val = array(1);
echo ($val) ? 'true' : 'false'; //false  
echo '<br/>';
//键值不存在
$val = array();
echo ($val['username']) ? 'true' : 'false'; //false  
echo '<br/>';
```
- 直接用!非判断，结果正好与上面的拿变量判断结果相反，在变量为true，变量存在，数组不为空的情况下为false

```php
<?php
//为布尔true的情况
$val = true;
echo (!$val) ? 'true' : 'false'; //false   
echo '<br/>';  
//为布尔false的情况
$val = false;
echo (!$val) ? 'true' : 'false'; //true  
echo '<br/>';
//变量未定义
echo (!$val2) ? 'true' : 'false'; //true  
echo '<br/>';
//变量为空
$val = '';
echo (!$val) ? 'true' : 'false'; //true  
echo '<br/>';
//变量为NULL
$val = NULL;
echo (!$val) ? 'true' : 'false'; //true  
echo '<br/>';
//变量为0
$val = 0;
echo (!$val) ? 'true' : 'false'; //true  
echo '<br/>';
//变量为1
$val = 1;
echo (!$val) ? 'true' : 'false'; //false  
echo '<br/>';
//空数组
$val = array();
echo (!$val) ? 'true' : 'false'; //true  
echo '<br/>';
//非空数组
$val = array(1);
echo (!$val) ? 'true' : 'false'; //false  
echo '<br/>';
//键值不存在
$val = array();
echo (!$val['username']) ? 'true' : 'false'; //true  
echo '<br/>';
```
- empty函数，在为false，0，NULL，''，array()的情况下，empty都认为是空的

```php
//检测empty情况
//为布尔true的情况
$val = true;
echo (empty($val)) ? 'true' : 'false'; //false   
echo '<br/>';  
//为布尔false的情况
$val = false;
echo (empty($val)) ? 'true' : 'false'; //true  
echo '<br/>';  
//为数字1的情况
$val = 1;
echo (empty($val)) ? 'true' : 'false'; //false
echo '<br/>';  
//为数字0的情况
$val = 0;
echo (empty($val)) ? 'true' : 'false'; //true  
echo '<br/>';   
//为NULL的情况
$val = NULL;
echo (empty($val)) ? 'true' : 'false'; //true  
echo '<br/>'; 
//为字符串空的情况
$val = '';
echo (empty($val)) ? 'true' : 'false'; //true  
echo '<br/>'; 
//为空数组的情况
$val = array();
echo (empty($val)) ? 'true' : 'false'; //true  
echo '<br/>'; 
//为正常字符串
$val = 'asdasd';
echo (empty($val)) ? 'true' : 'false'; //false
echo '<br/>'; 
//为正常数组
$val = array(1);
echo (empty($val)) ? 'true' : 'false'; //false
echo '<br/>';
```


**总结：**
- 在做变量判断的时候，如果能知道返回的数据格式或者类型，那么最好不采用上面四种方法判断。
- 尽量少用empty判断，这个函数陷阱比较多。
- 判断值是否存在或者数组键值是否存在，用isset方法比较靠谱。
- 做是非判断if (!$result) 这种情况要必须认识到遇到0 或者 空数组这样的情况的处理。








