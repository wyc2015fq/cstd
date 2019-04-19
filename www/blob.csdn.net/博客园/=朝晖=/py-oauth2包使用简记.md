# py-oauth2包使用简记 - =朝晖= - 博客园
# [py-oauth2包使用简记](https://www.cnblogs.com/dhcn/p/7614086.html)
       接前两天线上项目py2升级py3的书，老廖的一个旧库snspy，他已经不维护了，用的api又比较久，不好升级，最后速度找了个OAuth库取代了它，由于时间紧张，直接在pypi上搜索了一下，找到这个支持QQ的py-oauth2包，这个包的特点是够简洁，且支持很多国内OAuth，特别是很多OAuth都不支持的QQ OAuth，缺陷就是太简单了，本身的支持的主要接口基本就一个access_token,然后其他接口传参啥的主要靠自己，除了access_token以外，主要提供的是http访问接口的封装，不说啥了，虽说简陋了，凑活也算挺好用的，就它先顶着吧。

