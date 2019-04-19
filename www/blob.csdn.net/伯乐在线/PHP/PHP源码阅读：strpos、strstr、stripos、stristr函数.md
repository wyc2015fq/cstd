# PHP源码阅读：strpos、strstr、stripos、stristr函数 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [hoohack](http://www.jobbole.com/members/hector) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
我在github有对PHP源码更详细的注解。感兴趣的可以围观一下，给个star。[PHP5.4源码注解](https://github.com/hoohack/read-php-src)。可以通过[commit记录](https://github.com/hoohack/read-php-src/commits/master)查看已添加的注解。
# strpos

PHP
```
mixed strpos ( string $haystack, mixed $needle [, int $offset = 0 ] )
```
如果offset指定了，查找会从offset的位置开始。offset不能为负数。
返回needle第一次出现在haystack的位置。如果在haystack中找不到needle，则返回FALSE。
needle，如果needle不是字符串，它会被转换成整型数值并赋值为该数值的ASCII字符。请看下面例子。
## 例子

PHP
```
$str = "hello";
$pos = strpos($str, 111);
// 111的ASCII值是o，因此$pos = 4
```
## strpos核心源码

PHP
```
if (Z_TYPE_P(needle) == IS_STRING) {
     if (!Z_STRLEN_P(needle)) {
          php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty needle");
          RETURN_FALSE;
     }
     // 调用php_memnstr函数查找needle
     found = php_memnstr(haystack + offset,
                            Z_STRVAL_P(needle),
                            Z_STRLEN_P(needle),
                            haystack + haystack_len);
     } else {
          // 如果不是字符串，转换成数字并赋值为该数字的ASCII字符。
          if (php_needle_char(needle, needle_char TSRMLS_CC) != SUCCESS) {
               RETURN_FALSE;
          }
          //设置结束字符
          needle_char[1] = 0;
          found = php_memnstr(haystack + offset,
                            needle_char,
                            1,
                            haystack + haystack_len);
　　  }
}
```
有一点要注意的是，如果needle不是字符串的话，会调用php_needle_char函数将needle转成整型数字并转换为其ASCII值。
## 查找函数
函数最后返回的是found，php_memnstr函数实现了查找的方法。那么再继续看看php_memnstr函数做了什么：

PHP
```
#define php_memnstr zend_memnstr
```
php_memnstr是函数zend_memnstr的宏定义，查看zend_memnstr函数如下：


```
static inline char *
zend_memnstr(char *haystack, char *needle, int needle_len, char *end)
{
    char *p = haystack;
    char ne = needle[needle_len-1];
    if (needle_len == 1) {
        return (char *)memchr(p, *needle, (end-p));
    }
    if (needle_len > end-haystack) {
        return NULL;
    }
    // 第一个优化，只查找end - needle_len次
    end -= needle_len;
    while (p <= end) {
        // 第二个优化，先判断字符串的开头和结尾是否一样再判断整个字符串
        if ((p = (char *)memchr(p, *needle, (end-p+1))) && ne == p[needle_len-1]) {
            if (!memcmp(needle, p, needle_len-1)) {
                return p;
            }
        }
        if (p == NULL) {
            return NULL;
        }
        p++;
    }
    return NULL;
}
```
第一个优化，因为(char *)memchr(p, *needle, (end-p+1)是在end – needle_len + 1（即haystack_len+1）中查找，如果p为空，说明needle的第一个字符在p中从未出现过。
# strstr

PHP
```
string strstr ( string $haystack, mixed $needle [, bool $before_needle = false ] ) 返回needle在haystack中第一次出现的位置到结束的字符串。
```
这个函数的区分大小写的。
如果needle在haystack中不存在，返回FALSE。
如果before_needle为true，则返回haystack中needle在haystack第一次出现的位置之前的字符串。
## strstr核心源码

PHP
```
if (found) {
        // 计算出found的位置
        found_offset = found - haystack;
        if (part) {
            RETURN_STRINGL(haystack, found_offset, 1);
        } else {
            RETURN_STRINGL(found, haystack_len - found_offset, 1);
        }
}
```
strstr函数的前半部分跟strpos类似，区别在于strstr函数在找到位置后，需要返回haystack部分的字符串。part变量就是调用strstr函数时传递的before_needle变量。
# stripos

PHP
```
mixed stripos ( string $haystack, string $needle [, int $offset = 0 ] ) 不区分大小写的strpos。实现方式跟下面的类似，主要是使用一份拷贝然后将需要比较的字符串转换成小写字符后进行再进行查找。
```
# stristr

PHP
```
string stristr ( string $haystack, mixed $needle [, bool $before_needle = false ] ) 不区分大小写的strstr。
```
## 核心源码

PHP
```
// 拷贝一份haystack
haystack_dup = estrndup(haystack, haystack_len);
if (Z_TYPE_P(needle) == IS_STRING) {
    char *orig_needle;
    if (!Z_STRLEN_P(needle)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty needle");
        efree(haystack_dup);
        RETURN_FALSE;
    }
    orig_needle = estrndup(Z_STRVAL_P(needle), Z_STRLEN_P(needle));
    // 调用php_stristr函数找出orig_needle的值。
    found = php_stristr(haystack_dup, orig_needle,    haystack_len, Z_STRLEN_P(needle));
    efree(orig_needle);
} else {
    if (php_needle_char(needle, needle_char TSRMLS_CC) != SUCCESS) {
        efree(haystack_dup);
        RETURN_FALSE;
    }
    needle_char[1] = 0;
    found = php_stristr(haystack_dup, needle_char,    haystack_len, 1);
}
if (found) {
    found_offset = found - haystack_dup;
    if (part) {
        RETVAL_STRINGL(haystack, found_offset, 1);
    } else {
        RETVAL_STRINGL(haystack + found_offset, haystack_len - found_offset, 1);
    }
} else {
    RETVAL_FALSE;
}
// 释放变量
efree(haystack_dup);
```
可以知道，found是从php_stristr中得到的，继续查看php_stristr函数：

PHP
```
PHPAPI char *php_stristr(char *s, char *t, size_t s_len, size_t t_len)
{
    php_strtolower(s, s_len);
    php_strtolower(t, t_len);
    return php_memnstr(s, t, t_len, s + s_len);
}
```
这个函数的功能就是将字符串都转成小写之后调用php_mennstr函数来查找needle在haystack第一次出现的位置。
# 总结
因为strpos/stripos返回的是位置，位置从0开始计算，所以判断查找失败都用=== FALSE更适合。
阅读PHP的源码收获挺多，一方面可以知道某个函数的具体实现原理是怎样的，另一方面可以学习到一些编程优化方案。
到此本文结束，如果还有什么疑问或者建议，可以多多交流，原创文章，文笔有限，才疏学浅，文中若有不正之处，万望告知。
最后再安利一下，我在github有对PHP源码更详细的注解。感兴趣的可以围观一下，给个star。[PHP5.4源码注解](https://github.com/hoohack/read-php-src)。可以通过[commit记录](https://github.com/hoohack/read-php-src/commits/master)查看已添加的注解。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7aa70eaa311d5a5dec82ca0d49624741.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/1a4b335ec981d013f4002205a23fedd0.jpg)
