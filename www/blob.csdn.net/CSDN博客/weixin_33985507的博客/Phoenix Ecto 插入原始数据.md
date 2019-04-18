# Phoenix Ecto 插入原始数据 - weixin_33985507的博客 - CSDN博客
2016年07月08日 15:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
假设有如下数据 Records
```
rank  name  desc
1  apple  fruit
2  cat  animal
3  bike  vehicle
```
如果你想把你的集合拆分成小的分组，`chunk`就是你要找的函数：
```
iex> Enum.chunk([1, 2, 3, 4, 5, 6], 2)
[[1, 2], [3, 4], [5, 6]]
```
使用`~w()`这个魔术符可以将空格分割的字符串转换为列表：
```
iex> ~w(1  apple  fruit)
["1", "apple", "fruit"]
```
### 1. 数据预处理：输入所有数据到data列表变量
```
iex> data = ~w(1  apple  fruit
...> 2  cat  animal
...> 3  bike  vehicle)
["1", "apple", "fruit", "2", "cat", "animal", "3", "bike", "vehicle"]
```
### 2. 数据分组：使用`chunk`函数进行分组
```
iex> Enum.chunk(data, 3)
[["1", "apple", "fruit"], ["2", "cat", "animal"], ["3", "bike", "vehicle"]]
```
当然，也可以写成
```
data |> Enum.chunk(3)
```
结果是一样的
### 3. 构成结构体
可以使用Enum.zip组合两个列表
```
iex> Enum.zip([:rank, :name, :desc], ["1", "apple", "fruit"])
[rank: "1", name: "apple", desc: "fruit"]
```
如果在后面使用Enum.into就可以构造map类型啦！
```
iex> Enum.zip([:rank, :name, :desc], ["1", "apple", "fruit"]) |> Enum.into(%{})
%{desc: "fruit", name: "apple", rank: "1"}
```
### 4. Map VS Struct
这里我们回顾一下Elixir Map和Struct的区别，map属于基本结构，形式为`%{usernmae: "szy", passwd: "elixir"}`，注意到这里的username拼写错误，在使用时：
```
iex> user.username
** (KeyError) key :username not found in: %{passwd: "elixir", usernmae: "szy"}
```
发现提示没有找到这个key。这说明map只在runtime进行key的验证，在插入的时候没有进行验证。如果希望在插入数据时就对key是否存在进行验证，则需要使用struct。
Struct本质上也是一个Map，只不过多了一个Key，也就是`__struct__`。
```
iex> user = %User{username: "szy"}
%Elecity.User{__meta__: #Ecto.Schema.Metadata<:built>, description: nil,
 email: nil, id: nil, inserted_at: nil, password: nil, password_hash: nil,
 updated_at: nil,
 user_role_id: nil, username: "szy"}
```
如果插入时没有key会立即提示错误
```
iex> user2 = %User{usernmae: "szy"}
** (CompileError) iex:3: unknown key :usernmae for struct Elecity.User
```
此外，插入时没有声明的部分会自动填充默认内容。
### 5. 综合
现在我们就来构造结构体吧！同样使用`~w()a`，这里后面的a代表转换的是atom类型。
```
iex> key = ~w(rank  name  desc)a
[:rank, :name, :desc]
iex> data = ~w(1  apple  fruit
...> 2  cat  animal
...> 3  bike  vehicle)
["1", "apple", "fruit", "2", "cat", "animal", "3", "bike", "vehicle"]
iex> data = Enum.chunk(data, 3)
[["1", "apple", "fruit"], ["2", "cat", "animal"], ["3", "bike", "vehicle"]]
iex(19)> for record <- data do
...(19)>   Enum.zip(key, record)
...(19)>   |> Enum.into(%Record{})
...(19)>   |> Repo.insert!()
...(19)> end
```
当然，使用`insert_all`可以大大简化
```
MyApp.Repo.insert_all(Post, [[title: "hello", body: "world"], [title: "another", body: "post"]])
```
第一个data
```
iex(21)> for rec <- data do
...(21)> Enum.zip(key, rec)
...(21)> end
[[rank: "1", name: "apple", desc: "fruit"],
 [rank: "2", name: "cat", desc: "animal"],
 [rank: "3", name: "bike", desc: "vehicle"]]
```
总结一下，就是：
- ~w()a转换key
- ~w()转换数据
- Enum.chunk分组数据
- Enum.zip添加key
- Enum.into转换Struct后逐个插入Ecto 1.x
- 批量 insert_all (Step4) Ecto 2.x
[http://blog.plataformatec.com.br/2016/05/ectos-insert_all-and-schemaless-queries/](https://link.jianshu.com?t=http://blog.plataformatec.com.br/2016/05/ectos-insert_all-and-schemaless-queries/)
[http://www.elixir-cn.com/posts/136](https://link.jianshu.com?t=http://www.elixir-cn.com/posts/136)
