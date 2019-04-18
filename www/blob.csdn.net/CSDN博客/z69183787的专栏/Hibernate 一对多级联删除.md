# Hibernate 一对多级联删除 - z69183787的专栏 - CSDN博客
2014年02月27日 11:08:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7740
## 实例一
背景：
系统中有post表和replay表。分表表示帖子和帖子回复的表。一篇帖子可以有多个回复。
要求：
    现在要求删除一篇帖子的同时删除对应的replay表中该帖子的所有回复。
实现：
     在post.hbm.xml中这样配置 (即在“一方”的hbm.xml文件中)
```
<set name="replaies" inverse="true" cascade="all" > 或者：cascade="delete"
```
      操作删除的代码
```
Post post=postDAO.findById(postId);
postDAO.delete(post);
```
      这样就会将某篇帖子及其回复全部删除了。
## 实例二
背景：
系统中有post表和replay表。分表表示帖子和帖子回复的表。一篇帖子可以有多个回复。
要求：
打开某篇帖子，删除了其中的某一条“回复”。这时候要同时删除该主题贴以及所有回复。（不太符合常理）。
实现：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 在post.hbm.xml中这样配置 
         <set name="replaies" inverse="true" cascade="all" >
          或者：cascade="delete"
   在replay.hbm.xml中这样配置
           <many-to-one name="post" 
                          class="com.lhp.hibernate.beans.Post" 
                          fetch="select" 
                          lazy="false" 
                          cascade="delete" >
            <column name="postId" not-null="true" />
        </many-to-one>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
操作删除的代码
```
Replay replay=replayDAO.findById(replayId);
  replayDAO.delete(replay);
```
            解读：在这个“一对多”关系中，“一方”和“多方”都设置了级联删除。那么删除了“多方”中的一条记录，就会级联删除“一方”中的记录，然后“一方”又会级联删除“多方”的所有记录。
