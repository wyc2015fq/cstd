
# 535. Encode and Decode TinyURL - OraYang的博客 - CSDN博客

2017年12月04日 10:45:12[OraYang](https://me.csdn.net/u010665216)阅读数：342标签：[hash																](https://so.csdn.net/so/search/s.do?q=hash&t=blog)[hashtab																](https://so.csdn.net/so/search/s.do?q=hashtab&t=blog)[encode																](https://so.csdn.net/so/search/s.do?q=encode&t=blog)[decode																](https://so.csdn.net/so/search/s.do?q=decode&t=blog)[url																](https://so.csdn.net/so/search/s.do?q=url&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=decode&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=encode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=encode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=encode&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=hashtab&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=hash&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=hash&t=blog)


## 题目
Note: This is a companion problem to the System Design problem: Design TinyURL.
TinyURL is a URL shortening service where you enter a URL such as[https://leetcode.com/problems/design-tinyurl](https://leetcode.com/problems/design-tinyurl)and it returns a short URL such as[http://tinyurl.com/4e9iAk](http://tinyurl.com/4e9iAk).
Design the encode and decode methods for the TinyURL service. There is no restriction on how your encode/decode algorithm should work. You just need to ensure that a URL can be encoded to a tiny URL and the tiny URL can be decoded to the original URL.
## 思路
这道题目比较搞笑，很多人评价了不喜欢，倒不是因为题目出的不好，而是因为题目出的比较不严谨。为什么这样说呢？题目要求编写一个url的编码算法与解码算法。于是乎，很多人直接return了。如下所示：
```python
class
```
```python
Solution
```
```python
{
```
```python
public
```
```python
:
```
```python
// Encodes a URL to a shortened URL.
```
```python
string
```
```python
encode(
```
```python
string
```
```python
longUrl) {
```
```python
return
```
```python
longUrl;
    }
```
```python
// Decodes a shortened URL to its original URL.
```
```python
string
```
```python
decode(
```
```python
string
```
```python
shortUrl) {
```
```python
return
```
```python
shortUrl;
    }
};
```
```python
// Your Solution object will be instantiated and called as such:
```
```python
// Solution solution;
```
```python
// solution.decode(solution.encode(url));
```
上述两行代码号称最佳解决方案。但是咱们肯定不能这样做，那么这道题目的初衷又是什么呢？其实本质上，这道题目是一道关于哈希映射的题目，本质上是为了做url的函数表达。我们来分析下编码与解码的具体思路。
这道题目希望你把形如：[https://leetcode.com/problems/design-tinyurl](https://leetcode.com/problems/design-tinyurl)的url链接，编码成前缀为：[http://tinyurl.com/](http://tinyurl.com/)+字符串的格式。
那么我们首先想到的是C++ STL中有个hash函数，可以将任意字符串hash成一个整数。那么我们可以建立两个unorder_map函数，其中一个函数存储longUrl与shortUrl的映射关系，另一个函数存储shortUrl与longUrl之间的映射关系。
但是这样是不够的，我们还要考虑在每次做hash时，原存储容器中是否已经存在某相同url了，如果存在我们就不做重复的hash操作。
## 代码
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
unordered_map
```
```python
<
```
```python
string
```
```python
,
```
```python
string
```
```python
>
```
```python
longmap;
```
```python
unordered_map
```
```python
<
```
```python
string
```
```python
,
```
```python
string
```
```python
>
```
```python
tinymap;
```
```python
string
```
```python
prefix =
```
```python
"http://tinyurl.com/"
```
```python
;
```
```python
// Encodes a URL to a shortened URL.
```
```python
string
```
```python
encode(
```
```python
string
```
```python
longUrl) {
```
```python
if
```
```python
(longmap.find(longUrl) != longmap.end())
            ;
```
```python
else
```
```python
{  
            hash<
```
```python
string
```
```python
> hash_fn;
            longmap[longUrl] = prefix + to_string(hash_fn(longUrl));
            tinymap[longmap[longUrl]] = longUrl;
        }
```
```python
return
```
```python
longmap[longUrl];
    }
```
```python
// Decodes a shortened URL to its original URL.
```
```python
string
```
```python
decode(
```
```python
string
```
```python
shortUrl) {
```
```python
return
```
```python
tinymap[shortUrl];
    }
};
```
```python
// Your Solution object will be instantiated and called as such:
```
```python
// Solution solution;
```
```python
// solution.decode(solution.encode(url));
```

