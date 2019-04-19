# 转：django模板标签{% for %}的使用（含forloop用法） - _天枢 - 博客园
## [转：django模板标签{% for %}的使用（含forloop用法）](https://www.cnblogs.com/yhleng/p/8582314.html)
2018-03-16 15:30 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8582314)
# django模板标签{% for %}的使用（含forloop用法）
** {%  %}虽然这个是写在html中，但是这里边写的是服务端代码**
**在`django`模板标签中，`{% for %}` 标签用于迭代序列中的各个元素。 **
**与 Python 的 for 语句类似,语法是 `for X in Y` ,其中 Y 是要迭代的序列, X 是单次循环中使用的变量。每次迭代时,模板系统会渲染 `{% for %}` 和 `{% endfor %}` 之间的内容。**
### 1. 可以使用下述模板显示 athlete_list 变量中的运动员:
```
<ul>
    {% for athlete in athlete_list %}
        <li>{{ athlete.name }}</li>
    {% endfor %}
</ul>
```
` `
### 2. 加入 reversed 标签，反向迭代列表
```
{% for athlete in athlete_list reversed %}
…
{% endfor %}
```
### 3. 对二元组解包
```
{% for x, y in points %}
    <p>There is a point at {{ x }},{{ y }}</p>
{% endfor %}
```
### 4. 字典解包
```
{% for key, value in data.items %}
    {{ key }}: {{ value }}
{% endfor %}
```
### 5. 判断列表是否为空，为空就显示特定内容
**第一种使用`{% if %}`标签**
```
{% if athlete_list %}
    {% for athlete in athlete_list %}
        <p>{{ athlete.name }}</p>
    {% endfor %}
{% else %}
    <p>There are no athletes. Only computer programmers.</p>
{% endif %}
```
**第二种使用`{% empty %}`标签**
```
{% for athlete in athlete_list %}
    <p>{{ athlete.name }}</p>
{% empty %}
    <p>There are no athletes. Only computer programmers.</p>
{% endfor %}
```
### 5. `{% for %}`标签内部模板变量`forloop`
**在`{% for %}`循环内部,可以访问一个名为`forloop`的模板变量。这个变量有若干属性,通过它们可以获知循环进程的一些信息。**
#### **5.1 `forloop.counter`**
**`forloop.counter` 的值是一个整数,表示循环的次数。这个属性的值从 1 开始,因此第一次循环时,forloop.counter 等于 1 。**
```
{% for item in todo_list %}
<p>{{ forloop.counter }}: {{ item }}</p>
{% endfor %}
```
**` `**
#### **5.2 `forloop.counter0`**
**`forloop.counter0` 与 `forloop.counter` 类似,不过是从零开始的。第一次循环时,其值为 0 。**
#### **5.3 `forloop.revcounter`**
**`forloop.revcounter`的值是一个整数,表示循环中剩余的元素数量。第一次循环时, `forloop.revcounter` 的值是序列中要遍历的元素总数。最后一次循环时, `forloop.revcounter`的值为 1 。**
#### **5.4 `forloop.revcounter0`**
**`forloop.revcounter0` 与 `forloop.revcounter`类似,不过索引是基于零的。第一次循环时, `forloop.revcounter0`的值是序列中元素数量减去一。最后一次循环时, `forloop.revcounter0` 的值为 0 。**
#### **5.5 `forloop.first`**
**`forloop.first` 是个布尔值,第一次循环时为 True 。需要特殊处理第一个元素时很方便:**
```
{% for object in objects %}
    {% if forloop.first %}
        <li class="first">
    {% else %}
        <li>
    {% endif %}
    {{ object }}
    </li>
{% endfor %}
```
` `
#### **5.6 `forloop.last`**
**`forloop.last`是个布尔值,最后一次循环时为`True` 。经常用它在一组链接之间放置管道符号:**
```
{% for link in links %}
    {{ link }}{% if not forloop.last %} | {% endif %}
{% endfor %}
```
**上述模板代码的输出可能是:**
**`Link1 | Link2 | Link3 | Link4`**
**此外,还经常用它在一组单词之间放置逗号:**
```
<p>Favorite places:</p>
{% for p in places %}
    {{ p }}{% if not forloop.last %}, {% endif %}
{% endfor %}
```
#### **5.7 `forloop.parentloop`**
**在嵌套的循环中, `forloop.parentloop`引用父级循环的 `forloop` 对象。下面举个例子:**
```
{% for country in countries %}
    <table>
    {% for city in country.city_list %}
        <tr>
        <td>Country #{{ forloop.parentloop.counter }}</td>
        <td>City #{{ forloop.counter }}</td>
        <td>{{ city }}</td>
        </tr>
    {% endfor %}
    </table>
{% endfor %}
```
` `
### **小贴士**
> 
**`forloop` 变量只在循环内部可用。模板解析器遇到 `{% endfor %}` 时, `forloop` 随之消失。 **
**  上下文和 `forloop` 变量 **
**  在 `{% for %}` 块中,现有变量会让位,防止覆盖 `forloop` 变量。`Django` 把移动的上下文放到 **
**`forloop.parentloop` 中。通常,你无须担心,但是如果有名为 `forloop` 的模板变量(不建议这 **
**  么做),在 `{% for %}` 块中会重命名为 `forloop.parentloop` 。**
