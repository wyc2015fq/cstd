# thymeleaf引入静态文件cssjs的问题 - westbrook1998的博客 - CSDN博客





2019年01月01日 13:04:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：141








要这样写

```
<script src="./../static/layui/layui.all.js" th:src="@{/layui/layui.all.js}"></script>
```

注意`th:src`在springboot中默认就是static文件夹下，所以不用在./…/



