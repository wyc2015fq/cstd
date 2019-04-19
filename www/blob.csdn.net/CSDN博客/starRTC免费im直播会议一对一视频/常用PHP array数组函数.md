# 常用PHP array数组函数 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月11日 16:19:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：342

# array_rand
 第二个参数用来确定要选出几个元素
如果选出的元素不止一个，则返回包含随机键名的数组，否则返回该元素的键名。
$a=array("red","green","blue","yellow","brown");
$random_keys=`array_rand($a,3)`;
echo $a[$random_keys[0]]."<br>";
echo $a[$random_keys[1]]."<br>";
echo $a[$random_keys[2]];

[array_combine()](http://www.w3school.com.cn/php/func_array_combine.asp)
通过合并两个数组来创建一个新数组，其中的一个数组是键名，另一个数组的值为键值。如果其中一个数组为空，或者两个数组的元素个数不同，则返回 false。
注：两个数组必须有相同数目的元素。
[array_count_values()](http://www.w3school.com.cn/php/func_array_count_values.asp)
统计数组中所有值出现的次数。返回一个数组，其元素的键名是原数组的值，键值是该值在原数组中出现的次数。
[array_key_exists()](http://www.w3school.com.cn/php/func_array_key_exists.asp)
判断某个数组中是否存在指定的 key，如果该 key 存在，则返回 true，否则返回 false。
[array_keys()](http://www.w3school.com.cn/php/func_array_keys.asp)
返回一个包含原数组中所有键名的新数组。
[array_merge()](http://www.w3school.com.cn/php/func_array_merge.asp)
把两个或多个数组合并为一个数组。如果键名有重复，该键的键值为最后一个键名对应的值（后面的覆盖前面的）。如果数组是数字索引的，则键名会以连续方式重新索引。
[array_merge_recursive()](http://www.w3school.com.cn/php/func_array_merge_recursive.asp)
与 array_merge() 函数 一样，将一个或多个数组的元素合并起来，一个数组中的值附加在前一个数组的后面。并返回作为结果的数组。
但是，与 array_merge() 不同的是，当有重复的键名时，值不会被覆盖，而是将多个相同键名的值递归组成一个子数组。
[array_pop()](http://www.w3school.com.cn/php/func_array_pop.asp)
删除数组中的最后一个元素。
[array_push()](http://www.w3school.com.cn/php/func_array_push.asp)
向第一个参数的数组尾部添加一个或多个元素（入栈），然后返回新数组的长度。 相当于多次调用 $array[] = $value。
[array_shift()](http://www.w3school.com.cn/php/func_array_shift.asp)
删除数组中的第一个元素，并返回被删元素的值。
[array_unshift()](http://www.w3school.com.cn/php/func_array_unshift.asp)
在数组开头插入一个或多个元素。
[in_array()](http://www.w3school.com.cn/php/func_array_in_array.asp)
在数组中搜索给定的值。
[array_search()](http://www.w3school.com.cn/php/func_array_search.asp)
与 in_array() 一样，在数组中查找一个键值。如果找到了该值，匹配元素的键名会被返回。如果没找到，则返回 false。
[array_unique()](http://www.w3school.com.cn/php/func_array_unique.asp)
移除数组中的重复的值，并返回结果数组。当几个数组元素的值相等时，只保留第一个元素，其他的元素被删除。返回的数组中键名不变。
[array_values()](http://www.w3school.com.cn/php/func_array_values.asp)
返回一个包含给定数组中所有键值的数组，但不保留键名。
[compact()](http://www.w3school.com.cn/php/func_array_compact.asp)
创建一个由参数所带变量组成的数组。如果参数中存在数组，该数组中变量的值也会被获取。本函数返回的数组是一个关联数组，键名为函数的参数，键值为参数中变量的值。
[count()](http://www.w3school.com.cn/php/func_array_count.asp)
数计算数组中的单元数目或对象中的属性个数。
[key()](http://www.w3school.com.cn/php/func_array_key.asp)
返回数组内部指针当前指向元素的键名。若失败，则返回 FALSE。该函数与 current() 类似，只是返回的结果不同。current()
 函数返回的是元素的值，而 key() 函数返回的是元素的键名。
[list()](http://www.w3school.com.cn/php/func_array_list.asp)
用数组中的元素为一组变量赋值。
|[array_splice()](http://www.w3school.com.cn/php/func_array_splice.asp)|删除并替换数组中指定的元素。|
is_array  Returns`TRUE`if`var`is
 an[array](http://php.net/manual/en/language.types.array.php),`FALSE`otherwise.
更多参见：
[http://www.w3school.com.cn/php/php_ref_array.asp](http://www.w3school.com.cn/php/php_ref_array.asp)
# array_rand
 第二个参数用来确定要选出几个元素
如果选出的元素不止一个，则返回包含随机键名的数组，否则返回该元素的键名。
$a=array("red","green","blue","yellow","brown");
$random_keys=`array_rand($a,3)`;
echo $a[$random_keys[0]]."<br>";
echo $a[$random_keys[1]]."<br>";
echo $a[$random_keys[2]];

[array_combine()](http://www.w3school.com.cn/php/func_array_combine.asp)
通过合并两个数组来创建一个新数组，其中的一个数组是键名，另一个数组的值为键值。如果其中一个数组为空，或者两个数组的元素个数不同，则返回 false。
注：两个数组必须有相同数目的元素。
[array_count_values()](http://www.w3school.com.cn/php/func_array_count_values.asp)
统计数组中所有值出现的次数。返回一个数组，其元素的键名是原数组的值，键值是该值在原数组中出现的次数。
[array_key_exists()](http://www.w3school.com.cn/php/func_array_key_exists.asp)
判断某个数组中是否存在指定的 key，如果该 key 存在，则返回 true，否则返回 false。
[array_keys()](http://www.w3school.com.cn/php/func_array_keys.asp)
返回一个包含原数组中所有键名的新数组。
[array_merge()](http://www.w3school.com.cn/php/func_array_merge.asp)
把两个或多个数组合并为一个数组。如果键名有重复，该键的键值为最后一个键名对应的值（后面的覆盖前面的）。如果数组是数字索引的，则键名会以连续方式重新索引。
[array_merge_recursive()](http://www.w3school.com.cn/php/func_array_merge_recursive.asp)
与 array_merge() 函数 一样，将一个或多个数组的元素合并起来，一个数组中的值附加在前一个数组的后面。并返回作为结果的数组。
但是，与 array_merge() 不同的是，当有重复的键名时，值不会被覆盖，而是将多个相同键名的值递归组成一个子数组。
[array_pop()](http://www.w3school.com.cn/php/func_array_pop.asp)
删除数组中的最后一个元素。
[array_push()](http://www.w3school.com.cn/php/func_array_push.asp)
向第一个参数的数组尾部添加一个或多个元素（入栈），然后返回新数组的长度。 相当于多次调用 $array[] = $value。
[array_shift()](http://www.w3school.com.cn/php/func_array_shift.asp)
删除数组中的第一个元素，并返回被删元素的值。
[array_unshift()](http://www.w3school.com.cn/php/func_array_unshift.asp)
在数组开头插入一个或多个元素。
[in_array()](http://www.w3school.com.cn/php/func_array_in_array.asp)
在数组中搜索给定的值。
[array_search()](http://www.w3school.com.cn/php/func_array_search.asp)
与 in_array() 一样，在数组中查找一个键值。如果找到了该值，匹配元素的键名会被返回。如果没找到，则返回 false。
[array_unique()](http://www.w3school.com.cn/php/func_array_unique.asp)
移除数组中的重复的值，并返回结果数组。当几个数组元素的值相等时，只保留第一个元素，其他的元素被删除。返回的数组中键名不变。
[array_values()](http://www.w3school.com.cn/php/func_array_values.asp)
返回一个包含给定数组中所有键值的数组，但不保留键名。
[compact()](http://www.w3school.com.cn/php/func_array_compact.asp)
创建一个由参数所带变量组成的数组。如果参数中存在数组，该数组中变量的值也会被获取。本函数返回的数组是一个关联数组，键名为函数的参数，键值为参数中变量的值。
[count()](http://www.w3school.com.cn/php/func_array_count.asp)
数计算数组中的单元数目或对象中的属性个数。
[key()](http://www.w3school.com.cn/php/func_array_key.asp)
返回数组内部指针当前指向元素的键名。若失败，则返回 FALSE。该函数与 current() 类似，只是返回的结果不同。current()
 函数返回的是元素的值，而 key() 函数返回的是元素的键名。
[list()](http://www.w3school.com.cn/php/func_array_list.asp)
用数组中的元素为一组变量赋值。
|[array_splice()](http://www.w3school.com.cn/php/func_array_splice.asp)|删除并替换数组中指定的元素。|
is_array  Returns**`TRUE`**if`var`is
 an[array](http://php.net/manual/en/language.types.array.php),**`FALSE`**otherwise.
更多参见：
[http://www.w3school.com.cn/php/php_ref_array.asp](http://www.w3school.com.cn/php/php_ref_array.asp)
