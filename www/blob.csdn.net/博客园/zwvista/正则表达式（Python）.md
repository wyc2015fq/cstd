# 正则表达式（Python） - zwvista - 博客园

## [正则表达式（Python）](https://www.cnblogs.com/zwvista/p/9505577.html)

### 课题
- 使用正则表达式匹配字符串

使用正则表达式 "\d{3}-(\d{4})-\d{2}" 匹配字符串 "123-4567-89"

返回匹配结果：’"123-4567-89" 以及 "4567"
- 使用正则表达式替换字符串（模式）

使用正则表达式 "(\d+)-(\d+)-(\d+)" 匹配字符串 "123-4567-89"

使用模式字符串 "$3-$1-$2" 替换匹配结果，返回结果 "89-123-4567"。
- 使用正则表达式替换字符串（回调）

使用正则表达式 "\d+" 匹配字符串 "123-4567-89"

将匹配结果即三个数字串全部翻转过来，返回结果 "321-7654-98"。
- 使用正则表达式分割字符串

使用正则表达式 "%(begin|next|end)%" 分割字符串"%begin%hello%next%world%end%"

返回正则表达式分隔符之间的两个字符串 "hello" 和 "world"。

### Python

```
import re

s = '123-4567-89'
m = re.search(r'\d{3}-(\d{4})-\d{2}', s)
if m:
    for i in range(0, len(m.groups()) + 1):
        print(f"group {i} : {m[i]}")
        
print(re.sub(r'(\d+)-(\d+)-(\d+)', r'\3-\1-\2', s))

# https://stackoverflow.com/questions/18737863/passing-a-function-to-re-sub-in-python
# https://stackoverflow.com/questions/931092/reverse-a-string-in-python
print(re.sub(r'\d+', lambda x: x.group()[::-1], s))

print(re.split('%(?:begin|next|end)%', '%begin%hello%next%world%end%'))

'''
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
['', 'hello', 'world', '']
'''
```


