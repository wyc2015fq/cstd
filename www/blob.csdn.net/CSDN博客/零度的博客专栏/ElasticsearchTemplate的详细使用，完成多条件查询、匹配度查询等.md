# ElasticsearchTemplate的详细使用，完成多条件查询、匹配度查询等 - 零度的博客专栏 - CSDN博客
2018年05月23日 11:24:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：2802

原文出处：https://blog.csdn.net/tianyaleixiaowu/article/details/77965257
ElasticsearchTemplate是Spring对ES的java api进行的封装，提供了大量的相关的类来完成各种各样的查询。在日常的使用中，应该说最常用的查询就是queryList方法。
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- public <T> List<T> queryForList(SearchQuery query, Class<T> clazz) {  
- returnthis.queryForPage(query, clazz).getContent();  
-     }  
```java
public <T> List<T> queryForList(SearchQuery query, Class<T> clazz) {
        return this.queryForPage(query, clazz).getContent();
    }
```
而我们的主要工作就是构建SearchQuery查询条件，包括排序、分页等条件都包含在SearchQuery中。在之前的一篇文章中已经简单的讲过这几个类的继承关系了，这里我们主要看看使用方法（http://blog.csdn.net/tianyaleixiaowu/article/details/76149547  先阅读该篇了解基本的继承关系）。
直接上例子，Post对象
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- package com.example.demo.pojo;  
- 
- import org.springframework.data.annotation.Id;  
- import org.springframework.data.elasticsearch.annotations.Document;  
- 
- @Document(indexName="projectname",type="post",indexStoreType="fs",shards=5,replicas=1,refreshInterval="-1")  
- publicclass Post {  
- @Id
- private String id;  
- 
- private String title;  
- 
- private String content;  
- 
- privateint userId;  
- 
- privateint weight;  
- 
- @Override
- public String toString() {  
- return"Post{" +  
- "id='" + id + '\'' +  
- ", title='" + title + '\'' +  
- ", content='" + content + '\'' +  
- ", userId=" + userId +  
- ", weight=" + weight +  
- '}';  
-     }  
- 
- public String getId() {  
- return id;  
-     }  
- 
- publicvoid setId(String id) {  
- this.id = id;  
-     }  
- 
- public String getTitle() {  
- return title;  
-     }  
- 
- publicvoid setTitle(String title) {  
- this.title = title;  
-     }  
- 
- public String getContent() {  
- return content;  
-     }  
- 
- publicvoid setContent(String content) {  
- this.content = content;  
-     }  
- 
- publicint getUserId() {  
- return userId;  
-     }  
- 
- publicvoid setUserId(int userId) {  
- this.userId = userId;  
-     }  
- 
- publicint getWeight() {  
- return weight;  
-     }  
- 
- publicvoid setWeight(int weight) {  
- this.weight = weight;  
-     }  
- }  
```java
package com.example.demo.pojo;
import org.springframework.data.annotation.Id;
import org.springframework.data.elasticsearch.annotations.Document;
@Document(indexName="projectname",type="post",indexStoreType="fs",shards=5,replicas=1,refreshInterval="-1")
public class Post {
    @Id
    private String id;
    private String title;
    private String content;
    private int userId;
    private int weight;
    @Override
    public String toString() {
        return "Post{" +
                "id='" + id + '\'' +
                ", title='" + title + '\'' +
                ", content='" + content + '\'' +
                ", userId=" + userId +
                ", weight=" + weight +
                '}';
    }
    public String getId() {
        return id;
    }
    public void setId(String id) {
        this.id = id;
    }
    public String getTitle() {
        return title;
    }
    public void setTitle(String title) {
        this.title = title;
    }
    public String getContent() {
        return content;
    }
    public void setContent(String content) {
        this.content = content;
    }
    public int getUserId() {
        return userId;
    }
    public void setUserId(int userId) {
        this.userId = userId;
    }
    public int getWeight() {
        return weight;
    }
    public void setWeight(int weight) {
        this.weight = weight;
    }
}
```
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- import com.example.demo.pojo.Post;  
- import org.springframework.data.elasticsearch.repository.ElasticsearchRepository;  
- 
- publicinterface PostRepository extends ElasticsearchRepository<Post, String> {  
- }  
```java
import com.example.demo.pojo.Post;
import org.springframework.data.elasticsearch.repository.ElasticsearchRepository;
public interface PostRepository extends ElasticsearchRepository<Post, String> {
}
```
初始化数据
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- package com.example.demo.config;  
- 
- import com.example.demo.pojo.Post;  
- import com.example.demo.repository.PostRepository;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.stereotype.Component;  
- 
- import javax.annotation.PostConstruct;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- @Component
- publicclass Init {  
- @Autowired
-     PostRepository postRepository;  
- 
- @PostConstruct
- publicvoid init() {  
- //只初始化一次
-         Iterable<Post> posts = postRepository.findAll();  
- if (posts.iterator().hasNext()) {  
- return;  
-         }  
- for (int i = 0; i < 40; i++) {  
-             Post post = new Post();  
-             post.setTitle(getTitle().get(i));  
-             post.setContent(getContent().get(i));  
-             post.setWeight(i);  
-             post.setUserId(i % 10);  
-             postRepository.save(post);  
-         }  
-     }  
- 
- private List<String> getTitle() {  
-         List<String> list = new ArrayList<>();  
-         list.add("《如梦令·常记溪亭日暮》");  
-         list.add("《醉花阴·薄雾浓云愁永昼》");  
-         list.add("《声声慢·寻寻觅觅》");  
-         list.add("《永遇乐·落日熔金》");  
-         list.add("《如梦令·昨夜雨疏风骤》");  
-         list.add("《渔家傲·雪里已知春信至》");  
-         list.add("《点绛唇·蹴[1]罢秋千》");  
-         list.add("《点绛唇·寂寞深闺》");  
-         list.add("《蝶恋花·泪湿罗衣脂粉满》");  
-         list.add("《蝶恋花 离情》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《减字木兰花·卖花担上》");  
-         list.add("《临江仙·欧阳公作《蝶恋花》");  
-         list.add("《临江仙·庭院深深深几许》");  
-         list.add("《念奴娇·萧条庭院》");  
-         list.add("《菩萨蛮·风柔日薄春犹早》");  
-         list.add("《菩萨蛮·归鸿声断残云碧》");  
-         list.add("《武陵春·风住尘香花已尽》");  
-         list.add("《一剪梅·红藕香残玉蕈秋》");  
-         list.add("《渔家傲·天接云涛连晓雾》");  
-         list.add("《鹧鸪天·暗淡轻黄体性柔》");  
-         list.add("《鹧鸪天·寒日萧萧上锁窗》");  
-         list.add("《一剪梅·红藕香残玉簟秋》");  
-         list.add("《如梦令·常记溪亭日暮》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《蝶恋花·泪湿罗衣脂粉满》");  
-         list.add("《蝶恋花·暖日晴风初破冻》");  
-         list.add("《鹧鸪天·寒日萧萧上锁窗》");  
-         list.add("《醉花阴·薄雾浓云愁永昼》");  
-         list.add("《鹧鸪天·暗淡轻黄体性柔》");  
-         list.add("《蝶恋花·永夜恹恹欢意少》");  
-         list.add("《浣溪沙》");  
-         list.add("《浣溪沙》");  
-         list.add("《如梦令·谁伴明窗独坐》");  
- return list;  
-     }  
- 
- private List<String> getContent() {  
-         List<String> list = new ArrayList<>();  
-         list.add("初中 宋·李清照 常记溪亭日暮，沉醉不知归路，兴尽晚回舟，误入藕花深处。争渡，争渡");  
-         list.add("重阳节 宋·李清照 薄雾浓云愁永昼，瑞脑消金兽。佳节又重阳，玉枕纱厨，半夜凉初透。东");  
-         list.add("闺怨诗 宋·李清照 寻寻觅觅，冷冷清清，凄凄惨惨戚戚。乍暖还寒时候，最难将息。三杯两");  
-         list.add("元宵节 宋·李清照 落日熔金，暮云合璧，人在何处。染柳烟浓，吹梅笛怨，春意知几许。元");  
-         list.add("婉约诗 宋·李清照 昨夜雨疏风骤，浓睡不消残酒，试问卷帘人，却道海棠依旧。知否，知否");  
-         list.add("描写梅花 宋·李清照 雪里已知春信至，寒梅点缀琼枝腻，香脸半开娇旖旎，当庭际，玉人浴出");  
-         list.add(" 宋·李清照 蹴罢秋千，起来慵整纤纤手。露浓花瘦，薄汗轻衣透。见客入来，袜刬金");  
-         list.add("闺怨诗 宋·李清照 寂寞深闺，柔肠一寸愁千缕。惜春春去。几点催花雨。倚遍阑干，只是无");  
-         list.add("婉约诗 宋·李清照 泪湿罗衣脂粉满。四叠阳关，唱到千千遍。人道山长水又断。萧萧微雨闻");  
-         list.add("描写春天 宋·李清照 暖雨晴风初破冻，柳眼梅腮，已觉春心动。酒意诗情谁与共？泪融残粉花");  
-         list.add("寒食节 宋·李清照 淡荡春光寒食天，玉炉沈水袅残烟，梦回山枕隐花钿。海燕未来人斗草，");  
-         list.add(" 宋·李清照 髻子伤春慵更梳，晚风庭院落梅初，淡云来往月疏疏，玉鸭薰炉闲瑞脑，");  
-         list.add(" 宋·李清照 莫许杯深琥珀浓，未成沉醉意先融。疏钟已应晚来风。瑞脑香消魂梦断，");  
-         list.add("闺怨诗 宋·李清照 小院闲窗春已深，重帘未卷影沉沉。倚楼无语理瑶琴，远岫出山催薄暮。");  
-         list.add("爱情诗 宋·李清照 绣幕芙蓉一笑开，斜偎宝鸭亲香腮，眼波才动被人猜。一面风情深有韵，");  
-         list.add("描写春天 宋·李清照 卖花担上，买得一枝春欲放。泪染轻匀，犹带彤霞晓露痕。怕郎猜道，奴");  
-         list.add("》 宋·李清照 欧阳公作《蝶恋花》，有“深深深几许”之句，予酷爱之。用其语作“庭");  
-         list.add("描写梅花 宋·李清照 庭院深深深几许，云窗雾阁春迟，为谁憔悴损芳姿。夜来清梦好，应是发");  
-         list.add("寒食节 宋·李清照 萧条庭院，又斜风细雨，重门须闭。宠柳娇花寒食近，种种恼人天气。险");  
-         list.add("思乡诗 宋·李清照 风柔日薄春犹早，夹衫乍著心情好。睡起觉微寒，梅花鬓上残。故乡何处");  
-         list.add("描写春天 宋·李清照 归鸿声断残云碧，背窗雪落炉烟直。烛底凤钗明，钗头人胜轻。角声催晓");  
-         list.add("闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪");  
-         list.add(" 宋·李清照 红藕香残玉蕈秋，轻解罗裳，独上兰舟。云中谁寄锦书来？雁字回时，月");  
-         list.add("豪放诗 宋·李清照 天接云涛连晓雾。星河欲转千帆舞。仿佛梦魂归帝所。闻天语。殷勤问我");  
-         list.add("描写花 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。");  
-         list.add("描写秋天 宋·李清照 寒日萧萧上琐窗，梧桐应恨夜来霜。酒阑更喜团茶苦，梦断偏宜瑞脑香。");  
-         list.add("闺怨诗 宋·李清照 红藕香残玉簟秋。轻解罗裳，独上兰舟。云中谁寄锦书来？雁字回时，月");  
-         list.add(" 宋·李清照 常记溪亭日暮。沈醉不知归路。兴尽晚回舟，误入藕花深处。争渡。争渡");  
-         list.add(" 宋·李清照 莫许杯深琥珀浓。未成沈醉意先融。已应晚来风。瑞脑香消魂梦断，");  
-         list.add(" 宋·李清照 小院闲窗春色深。重帘未卷影沈沈。倚楼无语理瑶琴。远岫出山催薄暮，");  
-         list.add(" 宋·李清照 淡荡春光寒食天。玉炉沈水袅残烟。梦回山枕隐花钿。海燕未来人斗草，");  
-         list.add(" 宋·李清照 泪湿罗衣脂粉满。四叠阳关，唱到千千遍。人道山长山又断。萧萧微雨闻");  
-         list.add(" 宋·李清照 暖日晴风初破冻。柳眼眉腮，已觉春心动。酒意诗情谁与共。泪融残粉花");  
-         list.add(" 宋·李清照 寒日萧萧上锁窗。梧桐应恨夜来霜。酒阑更喜团茶苦，梦断偏宜瑞脑香。");  
-         list.add(" 宋·李清照 薄雾浓云愁永昼。瑞脑消金兽。佳节又重阳，玉枕纱厨，半夜凉初透。东");  
-         list.add(" 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。");  
-         list.add(" 宋·李清照 永夜恹恹欢意少。空梦长安，认取长安道。为报今年春色好。花光月影宜");  
-         list.add(" 宋·李清照 髻子伤春慵更梳。晚风庭院落梅初。淡云来往月疏疏。玉鸭熏炉闲瑞脑，");  
-         list.add(" 宋·李清照 绣面芙蓉一笑开。斜飞宝鸭衬香腮。眼波才动被人猜。一面风情深有韵，");  
-         list.add(" 宋·李清照 谁伴明窗独坐，我共影儿俩个。灯尽欲眠时，影也把人抛躲。无那，无那");  
- return list;  
-     }  
- 
- }  
```java
package com.example.demo.config;
import com.example.demo.pojo.Post;
import com.example.demo.repository.PostRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import javax.annotation.PostConstruct;
import java.util.ArrayList;
import java.util.List;
@Component
public class Init {
    @Autowired
    PostRepository postRepository;
    @PostConstruct
    public void init() {
        //只初始化一次
        Iterable<Post> posts = postRepository.findAll();
        if (posts.iterator().hasNext()) {
            return;
        }
        for (int i = 0; i < 40; i++) {
            Post post = new Post();
            post.setTitle(getTitle().get(i));
            post.setContent(getContent().get(i));
            post.setWeight(i);
            post.setUserId(i % 10);
            postRepository.save(post);
        }
    }
    private List<String> getTitle() {
        List<String> list = new ArrayList<>();
        list.add("《如梦令·常记溪亭日暮》");
        list.add("《醉花阴·薄雾浓云愁永昼》");
        list.add("《声声慢·寻寻觅觅》");
        list.add("《永遇乐·落日熔金》");
        list.add("《如梦令·昨夜雨疏风骤》");
        list.add("《渔家傲·雪里已知春信至》");
        list.add("《点绛唇·蹴[1]罢秋千》");
        list.add("《点绛唇·寂寞深闺》");
        list.add("《蝶恋花·泪湿罗衣脂粉满》");
        list.add("《蝶恋花 离情》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《减字木兰花·卖花担上》");
        list.add("《临江仙·欧阳公作《蝶恋花》");
        list.add("《临江仙·庭院深深深几许》");
        list.add("《念奴娇·萧条庭院》");
        list.add("《菩萨蛮·风柔日薄春犹早》");
        list.add("《菩萨蛮·归鸿声断残云碧》");
        list.add("《武陵春·风住尘香花已尽》");
        list.add("《一剪梅·红藕香残玉蕈秋》");
        list.add("《渔家傲·天接云涛连晓雾》");
        list.add("《鹧鸪天·暗淡轻黄体性柔》");
        list.add("《鹧鸪天·寒日萧萧上锁窗》");
        list.add("《一剪梅·红藕香残玉簟秋》");
        list.add("《如梦令·常记溪亭日暮》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《蝶恋花·泪湿罗衣脂粉满》");
        list.add("《蝶恋花·暖日晴风初破冻》");
        list.add("《鹧鸪天·寒日萧萧上锁窗》");
        list.add("《醉花阴·薄雾浓云愁永昼》");
        list.add("《鹧鸪天·暗淡轻黄体性柔》");
        list.add("《蝶恋花·永夜恹恹欢意少》");
        list.add("《浣溪沙》");
        list.add("《浣溪沙》");
        list.add("《如梦令·谁伴明窗独坐》");
        return list;
    }
    private List<String> getContent() {
        List<String> list = new ArrayList<>();
        list.add("初中 宋·李清照 常记溪亭日暮，沉醉不知归路，兴尽晚回舟，误入藕花深处。争渡，争渡");
        list.add("重阳节 宋·李清照 薄雾浓云愁永昼，瑞脑消金兽。佳节又重阳，玉枕纱厨，半夜凉初透。东");
        list.add("闺怨诗 宋·李清照 寻寻觅觅，冷冷清清，凄凄惨惨戚戚。乍暖还寒时候，最难将息。三杯两");
        list.add("元宵节 宋·李清照 落日熔金，暮云合璧，人在何处。染柳烟浓，吹梅笛怨，春意知几许。元");
        list.add("婉约诗 宋·李清照 昨夜雨疏风骤，浓睡不消残酒，试问卷帘人，却道海棠依旧。知否，知否");
        list.add("描写梅花 宋·李清照 雪里已知春信至，寒梅点缀琼枝腻，香脸半开娇旖旎，当庭际，玉人浴出");
        list.add(" 宋·李清照 蹴罢秋千，起来慵整纤纤手。露浓花瘦，薄汗轻衣透。见客入来，袜刬金");
        list.add("闺怨诗 宋·李清照 寂寞深闺，柔肠一寸愁千缕。惜春春去。几点催花雨。倚遍阑干，只是无");
        list.add("婉约诗 宋·李清照 泪湿罗衣脂粉满。四叠阳关，唱到千千遍。人道山长水又断。萧萧微雨闻");
        list.add("描写春天 宋·李清照 暖雨晴风初破冻，柳眼梅腮，已觉春心动。酒意诗情谁与共？泪融残粉花");
        list.add("寒食节 宋·李清照 淡荡春光寒食天，玉炉沈水袅残烟，梦回山枕隐花钿。海燕未来人斗草，");
        list.add(" 宋·李清照 髻子伤春慵更梳，晚风庭院落梅初，淡云来往月疏疏，玉鸭薰炉闲瑞脑，");
        list.add(" 宋·李清照 莫许杯深琥珀浓，未成沉醉意先融。疏钟已应晚来风。瑞脑香消魂梦断，");
        list.add("闺怨诗 宋·李清照 小院闲窗春已深，重帘未卷影沉沉。倚楼无语理瑶琴，远岫出山催薄暮。");
        list.add("爱情诗 宋·李清照 绣幕芙蓉一笑开，斜偎宝鸭亲香腮，眼波才动被人猜。一面风情深有韵，");
        list.add("描写春天 宋·李清照 卖花担上，买得一枝春欲放。泪染轻匀，犹带彤霞晓露痕。怕郎猜道，奴");
        list.add("》 宋·李清照 欧阳公作《蝶恋花》，有“深深深几许”之句，予酷爱之。用其语作“庭");
        list.add("描写梅花 宋·李清照 庭院深深深几许，云窗雾阁春迟，为谁憔悴损芳姿。夜来清梦好，应是发");
        list.add("寒食节 宋·李清照 萧条庭院，又斜风细雨，重门须闭。宠柳娇花寒食近，种种恼人天气。险");
        list.add("思乡诗 宋·李清照 风柔日薄春犹早，夹衫乍著心情好。睡起觉微寒，梅花鬓上残。故乡何处");
        list.add("描写春天 宋·李清照 归鸿声断残云碧，背窗雪落炉烟直。烛底凤钗明，钗头人胜轻。角声催晓");
        list.add("闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪");
        list.add(" 宋·李清照 红藕香残玉蕈秋，轻解罗裳，独上兰舟。云中谁寄锦书来？雁字回时，月");
        list.add("豪放诗 宋·李清照 天接云涛连晓雾。星河欲转千帆舞。仿佛梦魂归帝所。闻天语。殷勤问我");
        list.add("描写花 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。");
        list.add("描写秋天 宋·李清照 寒日萧萧上琐窗，梧桐应恨夜来霜。酒阑更喜团茶苦，梦断偏宜瑞脑香。");
        list.add("闺怨诗 宋·李清照 红藕香残玉簟秋。轻解罗裳，独上兰舟。云中谁寄锦书来？雁字回时，月");
        list.add(" 宋·李清照 常记溪亭日暮。沈醉不知归路。兴尽晚回舟，误入藕花深处。争渡。争渡");
        list.add(" 宋·李清照 莫许杯深琥珀浓。未成沈醉意先融。已应晚来风。瑞脑香消魂梦断，");
        list.add(" 宋·李清照 小院闲窗春色深。重帘未卷影沈沈。倚楼无语理瑶琴。远岫出山催薄暮，");
        list.add(" 宋·李清照 淡荡春光寒食天。玉炉沈水袅残烟。梦回山枕隐花钿。海燕未来人斗草，");
        list.add(" 宋·李清照 泪湿罗衣脂粉满。四叠阳关，唱到千千遍。人道山长山又断。萧萧微雨闻");
        list.add(" 宋·李清照 暖日晴风初破冻。柳眼眉腮，已觉春心动。酒意诗情谁与共。泪融残粉花");
        list.add(" 宋·李清照 寒日萧萧上锁窗。梧桐应恨夜来霜。酒阑更喜团茶苦，梦断偏宜瑞脑香。");
        list.add(" 宋·李清照 薄雾浓云愁永昼。瑞脑消金兽。佳节又重阳，玉枕纱厨，半夜凉初透。东");
        list.add(" 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。");
        list.add(" 宋·李清照 永夜恹恹欢意少。空梦长安，认取长安道。为报今年春色好。花光月影宜");
        list.add(" 宋·李清照 髻子伤春慵更梳。晚风庭院落梅初。淡云来往月疏疏。玉鸭熏炉闲瑞脑，");
        list.add(" 宋·李清照 绣面芙蓉一笑开。斜飞宝鸭衬香腮。眼波才动被人猜。一面风情深有韵，");
        list.add(" 宋·李清照 谁伴明窗独坐，我共影儿俩个。灯尽欲眠时，影也把人抛躲。无那，无那");
        return list;
    }
}
```
## 测试开始
定义出来
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- @Autowired
- private ElasticsearchTemplate elasticsearchTemplate;  
```java
@Autowired
    private ElasticsearchTemplate elasticsearchTemplate;
```
### 1 单字符串全文查询
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 单字符串模糊查询，默认排序。将从所有字段中查找包含传来的word分词后字符串的数据集
-      */
- @RequestMapping("/singleWord")  
- public Object singleTitle(String word, @PageableDefault Pageable pageable) {  
- //使用queryStringQuery完成单字符串查询
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(queryStringQuery(word)).withPageable(pageable).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 单字符串模糊查询，默认排序。将从所有字段中查找包含传来的word分词后字符串的数据集
     */
    @RequestMapping("/singleWord")
    public Object singleTitle(String word, @PageableDefault Pageable pageable) {
        //使用queryStringQuery完成单字符串查询
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(queryStringQuery(word)).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
请求 http://localhost:8080/singleWord?word=浣溪沙&size=20
结果：
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- [  
-   {  
- "id": "AV56c5adcu4Vu652ZQrK",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 莫许杯深琥珀浓，未成沉醉意先融。疏钟已应晚来风。瑞脑香消魂梦断，",  
- "userId": 2,  
- "weight": 12
-   },  
-   {  
- "id": "AV56c5bfcu4Vu652ZQrM",  
- "title": "《浣溪沙》",  
- "content": "爱情诗 宋·李清照 绣幕芙蓉一笑开，斜偎宝鸭亲香腮，眼波才动被人猜。一面风情深有韵，",  
- "userId": 4,  
- "weight": 14
-   },  
-   {  
- "id": "AV56c5jdcu4Vu652ZQrb",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 小院闲窗春色深。重帘未卷影沈沈。倚楼无语理瑶琴。远岫出山催薄暮，",  
- "userId": 9,  
- "weight": 29
-   },  
-   {  
- "id": "AV56c5nfcu4Vu652ZQrj",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 髻子伤春慵更梳。晚风庭院落梅初。淡云来往月疏疏。玉鸭熏炉闲瑞脑，",  
- "userId": 7,  
- "weight": 37
-   },  
-   {  
- "id": "AV56c5aBcu4Vu652ZQrJ",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 髻子伤春慵更梳，晚风庭院落梅初，淡云来往月疏疏，玉鸭薰炉闲瑞脑，",  
- "userId": 1,  
- "weight": 11
-   },  
-   {  
- "id": "AV56c5Zucu4Vu652ZQrI",  
- "title": "《浣溪沙》",  
- "content": "寒食节 宋·李清照 淡荡春光寒食天，玉炉沈水袅残烟，梦回山枕隐花钿。海燕未来人斗草，",  
- "userId": 0,  
- "weight": 10
-   },  
-   {  
- "id": "AV56c5i-cu4Vu652ZQra",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 莫许杯深琥珀浓。未成沈醉意先融。已应晚来风。瑞脑香消魂梦断，",  
- "userId": 8,  
- "weight": 28
-   },  
-   {  
- "id": "AV56c5j-cu4Vu652ZQrc",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 淡荡春光寒食天。玉炉沈水袅残烟。梦回山枕隐花钿。海燕未来人斗草，",  
- "userId": 0,  
- "weight": 30
-   },  
-   {  
- "id": "AV56c5a_cu4Vu652ZQrL",  
- "title": "《浣溪沙》",  
- "content": "闺怨诗 宋·李清照 小院闲窗春已深，重帘未卷影沉沉。倚楼无语理瑶琴，远岫出山催薄暮。",  
- "userId": 3,  
- "weight": 13
-   },  
-   {  
- "id": "AV56c5oAcu4Vu652ZQrk",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 绣面芙蓉一笑开。斜飞宝鸭衬香腮。眼波才动被人猜。一面风情深有韵，",  
- "userId": 8,  
- "weight": 38
-   },  
-   {  
- "id": "AV56c5Uwcu4Vu652ZQq-",  
- "title": "《如梦令·常记溪亭日暮》",  
- "content": "初中 宋·李清照 常记溪亭日暮，沉醉不知归路，兴尽晚回舟，误入藕花深处。争渡，争渡",  
- "userId": 0,  
- "weight": 0
-   },  
-   {  
- "id": "AV56c5ifcu4Vu652ZQrZ",  
- "title": "《如梦令·常记溪亭日暮》",  
- "content": " 宋·李清照 常记溪亭日暮。沈醉不知归路。兴尽晚回舟，误入藕花深处。争渡。争渡",  
- "userId": 7,  
- "weight": 27
-   },  
-   {  
- "id": "AV56c5e-cu4Vu652ZQrT",  
- "title": "《武陵春·风住尘香花已尽》",  
- "content": "闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪",  
- "userId": 1,  
- "weight": 21
-   }  
- ]  
```java
[
  {
    "id": "AV56c5adcu4Vu652ZQrK",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 莫许杯深琥珀浓，未成沉醉意先融。疏钟已应晚来风。瑞脑香消魂梦断，",
    "userId": 2,
    "weight": 12
  },
  {
    "id": "AV56c5bfcu4Vu652ZQrM",
    "title": "《浣溪沙》",
    "content": "爱情诗 宋·李清照 绣幕芙蓉一笑开，斜偎宝鸭亲香腮，眼波才动被人猜。一面风情深有韵，",
    "userId": 4,
    "weight": 14
  },
  {
    "id": "AV56c5jdcu4Vu652ZQrb",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 小院闲窗春色深。重帘未卷影沈沈。倚楼无语理瑶琴。远岫出山催薄暮，",
    "userId": 9,
    "weight": 29
  },
  {
    "id": "AV56c5nfcu4Vu652ZQrj",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 髻子伤春慵更梳。晚风庭院落梅初。淡云来往月疏疏。玉鸭熏炉闲瑞脑，",
    "userId": 7,
    "weight": 37
  },
  {
    "id": "AV56c5aBcu4Vu652ZQrJ",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 髻子伤春慵更梳，晚风庭院落梅初，淡云来往月疏疏，玉鸭薰炉闲瑞脑，",
    "userId": 1,
    "weight": 11
  },
  {
    "id": "AV56c5Zucu4Vu652ZQrI",
    "title": "《浣溪沙》",
    "content": "寒食节 宋·李清照 淡荡春光寒食天，玉炉沈水袅残烟，梦回山枕隐花钿。海燕未来人斗草，",
    "userId": 0,
    "weight": 10
  },
  {
    "id": "AV56c5i-cu4Vu652ZQra",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 莫许杯深琥珀浓。未成沈醉意先融。已应晚来风。瑞脑香消魂梦断，",
    "userId": 8,
    "weight": 28
  },
  {
    "id": "AV56c5j-cu4Vu652ZQrc",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 淡荡春光寒食天。玉炉沈水袅残烟。梦回山枕隐花钿。海燕未来人斗草，",
    "userId": 0,
    "weight": 30
  },
  {
    "id": "AV56c5a_cu4Vu652ZQrL",
    "title": "《浣溪沙》",
    "content": "闺怨诗 宋·李清照 小院闲窗春已深，重帘未卷影沉沉。倚楼无语理瑶琴，远岫出山催薄暮。",
    "userId": 3,
    "weight": 13
  },
  {
    "id": "AV56c5oAcu4Vu652ZQrk",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 绣面芙蓉一笑开。斜飞宝鸭衬香腮。眼波才动被人猜。一面风情深有韵，",
    "userId": 8,
    "weight": 38
  },
  {
    "id": "AV56c5Uwcu4Vu652ZQq-",
    "title": "《如梦令·常记溪亭日暮》",
    "content": "初中 宋·李清照 常记溪亭日暮，沉醉不知归路，兴尽晚回舟，误入藕花深处。争渡，争渡",
    "userId": 0,
    "weight": 0
  },
  {
    "id": "AV56c5ifcu4Vu652ZQrZ",
    "title": "《如梦令·常记溪亭日暮》",
    "content": " 宋·李清照 常记溪亭日暮。沈醉不知归路。兴尽晚回舟，误入藕花深处。争渡。争渡",
    "userId": 7,
    "weight": 27
  },
  {
    "id": "AV56c5e-cu4Vu652ZQrT",
    "title": "《武陵春·风住尘香花已尽》",
    "content": "闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪",
    "userId": 1,
    "weight": 21
  }
]
```
共13个结果，按照默认的排序方式，即匹配相关度排序，前10个匹配度最高，都是完全带“浣溪沙”三个字的。第10个、11个都是题目和正文都包含“溪”字而且出现了2次“溪”，最后一个是正文带一个“溪”。
我们修改一下排序方式，按照weight从大到小排序
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 单字符串模糊查询，单字段排序。
-      */
- @RequestMapping("/singleWord1")  
- public Object singlePost(String word, @PageableDefault(sort = "weight", direction = Sort.Direction.DESC) Pageable pageable) {  
- //使用queryStringQuery完成单字符串查询
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(queryStringQuery(word)).withPageable(pageable).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 单字符串模糊查询，单字段排序。
     */
    @RequestMapping("/singleWord1")
    public Object singlePost(String word, @PageableDefault(sort = "weight", direction = Sort.Direction.DESC) Pageable pageable) {
        //使用queryStringQuery完成单字符串查询
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(queryStringQuery(word)).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
请求：
http://localhost:8080/singleWord1?word=浣溪沙&size=20
结果：
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- [  
-   {  
- "id": "AV56c5oAcu4Vu652ZQrk",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 绣面芙蓉一笑开。斜飞宝鸭衬香腮。眼波才动被人猜。一面风情深有韵，",  
- "userId": 8,  
- "weight": 38
-   },  
-   {  
- "id": "AV56c5nfcu4Vu652ZQrj",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 髻子伤春慵更梳。晚风庭院落梅初。淡云来往月疏疏。玉鸭熏炉闲瑞脑，",  
- "userId": 7,  
- "weight": 37
-   },  
-   {  
- "id": "AV56c5j-cu4Vu652ZQrc",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 淡荡春光寒食天。玉炉沈水袅残烟。梦回山枕隐花钿。海燕未来人斗草，",  
- "userId": 0,  
- "weight": 30
-   },  
-   {  
- "id": "AV56c5jdcu4Vu652ZQrb",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 小院闲窗春色深。重帘未卷影沈沈。倚楼无语理瑶琴。远岫出山催薄暮，",  
- "userId": 9,  
- "weight": 29
-   },  
-   {  
- "id": "AV56c5i-cu4Vu652ZQra",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 莫许杯深琥珀浓。未成沈醉意先融。已应晚来风。瑞脑香消魂梦断，",  
- "userId": 8,  
- "weight": 28
-   },  
-   {  
- "id": "AV56c5ifcu4Vu652ZQrZ",  
- "title": "《如梦令·常记溪亭日暮》",  
- "content": " 宋·李清照 常记溪亭日暮。沈醉不知归路。兴尽晚回舟，误入藕花深处。争渡。争渡",  
- "userId": 7,  
- "weight": 27
-   },  
-   {  
- "id": "AV56c5e-cu4Vu652ZQrT",  
- "title": "《武陵春·风住尘香花已尽》",  
- "content": "闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪",  
- "userId": 1,  
- "weight": 21
-   },  
-   {  
- "id": "AV56c5bfcu4Vu652ZQrM",  
- "title": "《浣溪沙》",  
- "content": "爱情诗 宋·李清照 绣幕芙蓉一笑开，斜偎宝鸭亲香腮，眼波才动被人猜。一面风情深有韵，",  
- "userId": 4,  
- "weight": 14
-   },  
-   {  
- "id": "AV56c5a_cu4Vu652ZQrL",  
- "title": "《浣溪沙》",  
- "content": "闺怨诗 宋·李清照 小院闲窗春已深，重帘未卷影沉沉。倚楼无语理瑶琴，远岫出山催薄暮。",  
- "userId": 3,  
- "weight": 13
-   },  
-   {  
- "id": "AV56c5adcu4Vu652ZQrK",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 莫许杯深琥珀浓，未成沉醉意先融。疏钟已应晚来风。瑞脑香消魂梦断，",  
- "userId": 2,  
- "weight": 12
-   },  
-   {  
- "id": "AV56c5aBcu4Vu652ZQrJ",  
- "title": "《浣溪沙》",  
- "content": " 宋·李清照 髻子伤春慵更梳，晚风庭院落梅初，淡云来往月疏疏，玉鸭薰炉闲瑞脑，",  
- "userId": 1,  
- "weight": 11
-   },  
-   {  
- "id": "AV56c5Zucu4Vu652ZQrI",  
- "title": "《浣溪沙》",  
- "content": "寒食节 宋·李清照 淡荡春光寒食天，玉炉沈水袅残烟，梦回山枕隐花钿。海燕未来人斗草，",  
- "userId": 0,  
- "weight": 10
-   },  
-   {  
- "id": "AV56c5Uwcu4Vu652ZQq-",  
- "title": "《如梦令·常记溪亭日暮》",  
- "content": "初中 宋·李清照 常记溪亭日暮，沉醉不知归路，兴尽晚回舟，误入藕花深处。争渡，争渡",  
- "userId": 0,  
- "weight": 0
-   }  
- ]  
```java
[
  {
    "id": "AV56c5oAcu4Vu652ZQrk",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 绣面芙蓉一笑开。斜飞宝鸭衬香腮。眼波才动被人猜。一面风情深有韵，",
    "userId": 8,
    "weight": 38
  },
  {
    "id": "AV56c5nfcu4Vu652ZQrj",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 髻子伤春慵更梳。晚风庭院落梅初。淡云来往月疏疏。玉鸭熏炉闲瑞脑，",
    "userId": 7,
    "weight": 37
  },
  {
    "id": "AV56c5j-cu4Vu652ZQrc",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 淡荡春光寒食天。玉炉沈水袅残烟。梦回山枕隐花钿。海燕未来人斗草，",
    "userId": 0,
    "weight": 30
  },
  {
    "id": "AV56c5jdcu4Vu652ZQrb",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 小院闲窗春色深。重帘未卷影沈沈。倚楼无语理瑶琴。远岫出山催薄暮，",
    "userId": 9,
    "weight": 29
  },
  {
    "id": "AV56c5i-cu4Vu652ZQra",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 莫许杯深琥珀浓。未成沈醉意先融。已应晚来风。瑞脑香消魂梦断，",
    "userId": 8,
    "weight": 28
  },
  {
    "id": "AV56c5ifcu4Vu652ZQrZ",
    "title": "《如梦令·常记溪亭日暮》",
    "content": " 宋·李清照 常记溪亭日暮。沈醉不知归路。兴尽晚回舟，误入藕花深处。争渡。争渡",
    "userId": 7,
    "weight": 27
  },
  {
    "id": "AV56c5e-cu4Vu652ZQrT",
    "title": "《武陵春·风住尘香花已尽》",
    "content": "闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪",
    "userId": 1,
    "weight": 21
  },
  {
    "id": "AV56c5bfcu4Vu652ZQrM",
    "title": "《浣溪沙》",
    "content": "爱情诗 宋·李清照 绣幕芙蓉一笑开，斜偎宝鸭亲香腮，眼波才动被人猜。一面风情深有韵，",
    "userId": 4,
    "weight": 14
  },
  {
    "id": "AV56c5a_cu4Vu652ZQrL",
    "title": "《浣溪沙》",
    "content": "闺怨诗 宋·李清照 小院闲窗春已深，重帘未卷影沉沉。倚楼无语理瑶琴，远岫出山催薄暮。",
    "userId": 3,
    "weight": 13
  },
  {
    "id": "AV56c5adcu4Vu652ZQrK",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 莫许杯深琥珀浓，未成沉醉意先融。疏钟已应晚来风。瑞脑香消魂梦断，",
    "userId": 2,
    "weight": 12
  },
  {
    "id": "AV56c5aBcu4Vu652ZQrJ",
    "title": "《浣溪沙》",
    "content": " 宋·李清照 髻子伤春慵更梳，晚风庭院落梅初，淡云来往月疏疏，玉鸭薰炉闲瑞脑，",
    "userId": 1,
    "weight": 11
  },
  {
    "id": "AV56c5Zucu4Vu652ZQrI",
    "title": "《浣溪沙》",
    "content": "寒食节 宋·李清照 淡荡春光寒食天，玉炉沈水袅残烟，梦回山枕隐花钿。海燕未来人斗草，",
    "userId": 0,
    "weight": 10
  },
  {
    "id": "AV56c5Uwcu4Vu652ZQq-",
    "title": "《如梦令·常记溪亭日暮》",
    "content": "初中 宋·李清照 常记溪亭日暮，沉醉不知归路，兴尽晚回舟，误入藕花深处。争渡，争渡",
    "userId": 0,
    "weight": 0
  }
]
```
可以看到排序已经按照weight从大到小来排了，比字符串匹配度的权重更大。
### 2 某字段按字符串模糊查询
查询某个字段中模糊包含目标字符串，使用matchQuery
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 单字段对某字符串模糊查询
-      */
- @RequestMapping("/singleMatch")  
- public Object singleMatch(String content, Integer userId, @PageableDefault Pageable pageable) {  
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("content", content)).withPageable(pageable).build();  
- //        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("userId", userId)).withPageable(pageable).build();
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 单字段对某字符串模糊查询
     */
    @RequestMapping("/singleMatch")
    public Object singleMatch(String content, Integer userId, @PageableDefault Pageable pageable) {
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("content", content)).withPageable(pageable).build();
//        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("userId", userId)).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
请求：
http://localhost:8080/singleMatch?content=落日熔金&size=20
部分结果：
![](https://img-blog.csdn.net/20170914104035813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结果集是content字段包含“落日熔金”相关字样。
把注释放开，我们可以尝试用userId查询，http://localhost:8080/singleMatch?userId=1&size=20
![](https://img-blog.csdn.net/20170914104323606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
matchQuery应该是比较常用的功能。
### 3 PhraseMatch查询，短语匹配
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 单字段对某短语进行匹配查询，短语分词的顺序会影响结果
-      */
- @RequestMapping("/singlePhraseMatch")  
- public Object singlePhraseMatch(String content, @PageableDefault Pageable pageable) {  
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchPhraseQuery("content", content)).withPageable(pageable).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 单字段对某短语进行匹配查询，短语分词的顺序会影响结果
     */
    @RequestMapping("/singlePhraseMatch")
    public Object singlePhraseMatch(String content, @PageableDefault Pageable pageable) {
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchPhraseQuery("content", content)).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
结果只有一条：
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- [  
-   {  
- "id": "AV56c5WDcu4Vu652ZQrB",  
- "title": "《永遇乐·落日熔金》",  
- "content": "元宵节 宋·李清照 落日熔金，暮云合璧，人在何处。染柳烟浓，吹梅笛怨，春意知几许。元",  
- "userId": 3,  
- "weight": 3
-   }  
- ]  
```java
[
  {
    "id": "AV56c5WDcu4Vu652ZQrB",
    "title": "《永遇乐·落日熔金》",
    "content": "元宵节 宋·李清照 落日熔金，暮云合璧，人在何处。染柳烟浓，吹梅笛怨，春意知几许。元",
    "userId": 3,
    "weight": 3
  }
]
```
和match查询类似，match_phrase查询首先解析查询字符串来产生一个词条列表。然后会搜索所有的词条，但只保留包含了所有搜索词条的文档，并且词条的位置要邻接。一个针对短语“中华共和国”的查询不会匹配“中华人民共和国”，因为没有含有邻接在一起的“中华”和“共和国”词条。
这种完全匹配比较严格，类似于数据库里的“%落日熔金%”这种，使用场景比较狭窄。如果我们希望能不那么严格，譬如搜索“中华共和国”，希望带“我爱中华人民共和国”的也能出来，就是分词后，中间能间隔几个位置的也能查出来，可以使用slop参数。
我们先来添加一个类似的数据
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- @RequestMapping("/add")  
- public Object add() {  
-         Post post = new Post();  
-         post.setTitle("我是");  
-         post.setContent("我爱中华人民共和国");  
-         post.setWeight(1);  
-         post.setUserId(1);  
-         postRepository.save(post);  
-         post = new Post();  
-         post.setTitle("我是");  
-         post.setContent("中华共和国");  
-         post.setWeight(2);  
-         post.setUserId(2);  
- return postRepository.save(post);  
-     }  
```java
@RequestMapping("/add")
    public Object add() {
        Post post = new Post();
        post.setTitle("我是");
        post.setContent("我爱中华人民共和国");
        post.setWeight(1);
        post.setUserId(1);
        postRepository.save(post);
        post = new Post();
        post.setTitle("我是");
        post.setContent("中华共和国");
        post.setWeight(2);
        post.setUserId(2);
        return postRepository.save(post);
    }
```
使用如下方法：
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- @RequestMapping("/singlePhraseMatch")  
- public Object singlePhraseMatch(String content, @PageableDefault Pageable pageable) {  
- //        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchPhraseQuery("content", content)).withPageable(pageable).build();
- //少匹配一个分词也OK、
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchPhraseQuery("content", content).slop(2)).withPageable(pageable).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
@RequestMapping("/singlePhraseMatch")
    public Object singlePhraseMatch(String content, @PageableDefault Pageable pageable) {
//        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchPhraseQuery("content", content)).withPageable(pageable).build();
        //少匹配一个分词也OK、
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchPhraseQuery("content", content).slop(2)).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
当请求“中华共和国时”结果如下
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- [  
-   {  
- "id": "AV5-ekRHcu4Vu652ZQrw",  
- "title": "我是",  
- "content": "中华共和国",  
- "userId": 2,  
- "weight": 2
-   },  
-   {  
- "id": "AV5-ekBvcu4Vu652ZQrv",  
- "title": "我是",  
- "content": "我爱中华人民共和国",  
- "userId": 1,  
- "weight": 1
-   }  
- ]  
```java
[
  {
    "id": "AV5-ekRHcu4Vu652ZQrw",
    "title": "我是",
    "content": "中华共和国",
    "userId": 2,
    "weight": 2
  },
  {
    "id": "AV5-ekBvcu4Vu652ZQrv",
    "title": "我是",
    "content": "我爱中华人民共和国",
    "userId": 1,
    "weight": 1
  }
]
```
关于slop的解释，可以看看这一篇http://blog.csdn.net/xifeijian/article/details/51090707
精确短语(Exact-phrase)匹配也许太过于严格了。也许我们希望含有"quick brown fox"的文档也能够匹配"quick fox"查询，即使位置并不是完全相等的。
我们可以在短语匹配使用slop参数来引入一些灵活性：
GET /my_index/my_type/_search
{
    "query": {
        "match_phrase": {
            "title": {
                "query": "quick fox",
                "slop":  1
            }
        }
    }
}
slop参数告诉match_phrase查询词条能够相隔多远时仍然将文档视为匹配。相隔多远的意思是，你需要移动一个词条多少次来让查询和文档匹配？
我们以一个简单的例子来阐述这个概念。为了让查询quick fox能够匹配含有quick brown fox的文档，我们需要slop的值为1：
            Pos 1         Pos 2         Pos 3
-----------------------------------------------
Doc:        quick         brown         fox
-----------------------------------------------
Query:      quick         fox
Slop 1:     quick                 ↳     fox
尽管在使用了slop的短语匹配中，所有的单词都需要出现，但是单词的出现顺序可以不同。如果slop的值足够大，那么单词的顺序可以是任意的。
为了让fox quick查询能够匹配我们的文档，需要slop的值为3：
            Pos 1         Pos 2         Pos 3
-----------------------------------------------
Doc:        quick         brown         fox
-----------------------------------------------
Query:      fox           quick
Slop 1:     fox|quick  ↵  
Slop 2:     quick      ↳  fox
Slop 3:     quick                 ↳     fox
### 4 Term查询
这个是最严格的匹配，属于低级查询，不进行分词的，参考这篇文章http://www.cnblogs.com/muniaofeiyu/p/5616316.html
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * term匹配，即不分词匹配，你传来什么值就会拿你传的值去做完全匹配
-      */
- @RequestMapping("/singleTerm")  
- public Object singleTerm(Integer userId, @PageableDefault Pageable pageable) {  
- //不对传来的值分词，去找完全匹配的
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(termQuery("userId", userId)).withPageable(pageable).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * term匹配，即不分词匹配，你传来什么值就会拿你传的值去做完全匹配
     */
    @RequestMapping("/singleTerm")
    public Object singleTerm(Integer userId, @PageableDefault Pageable pageable) {
        //不对传来的值分词，去找完全匹配的
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(termQuery("userId", userId)).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
我们可以用这个来做那种需要==查询的操作，当传userId=1时，会查询出来所有userId为1的集合。
通常情况下，我们不会使用term查询，绝大部分情况我们使用ES的目的就是为了使用它的分词模糊查询功能。而term一般适用于做过滤器filter的情况，譬如我们去查询title中包含“浣溪沙”且userId=1时，那么就可以用termQuery("userId", 1)作为查询的filter。
### 5 multi_match多个字段匹配某字符串
如果我们希望title，content两个字段去匹配某个字符串，只要任何一个字段包括该字符串即可，就可以使用multimatch。
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 多字段匹配
-      */
- @RequestMapping("/multiMatch")  
- public Object singleUserId(String title, @PageableDefault(sort = "weight", direction = Sort.Direction.DESC) Pageable pageable) {  
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(multiMatchQuery(title, "title", "content")).withPageable(pageable).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 多字段匹配
     */
    @RequestMapping("/multiMatch")
    public Object singleUserId(String title, @PageableDefault(sort = "weight", direction = Sort.Direction.DESC) Pageable pageable) {
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(multiMatchQuery(title, "title", "content")).withPageable(pageable).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
访问http://localhost:8080/multiMatch?title=我是
结果如下
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- [  
-   {  
- "id": "AV56c5oicu4Vu652ZQrl",  
- "title": "《如梦令·谁伴明窗独坐》",  
- "content": " 宋·李清照 谁伴明窗独坐，我共影儿俩个。灯尽欲眠时，影也把人抛躲。无那，无那",  
- "userId": 9,  
- "weight": 39
-   },  
-   {  
- "id": "AV56c5mecu4Vu652ZQrh",  
- "title": "《鹧鸪天·暗淡轻黄体性柔》",  
- "content": " 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。",  
- "userId": 5,  
- "weight": 35
-   },  
-   {  
- "id": "AV56c5g0cu4Vu652ZQrW",  
- "title": "《鹧鸪天·暗淡轻黄体性柔》",  
- "content": "描写花 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。",  
- "userId": 4,  
- "weight": 24
-   },  
-   {  
- "id": "AV56c5gIcu4Vu652ZQrV",  
- "title": "《渔家傲·天接云涛连晓雾》",  
- "content": "豪放诗 宋·李清照 天接云涛连晓雾。星河欲转千帆舞。仿佛梦魂归帝所。闻天语。殷勤问我",  
- "userId": 3,  
- "weight": 23
-   },  
-   {  
- "id": "AV56c5e-cu4Vu652ZQrT",  
- "title": "《武陵春·风住尘香花已尽》",  
- "content": "闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪",  
- "userId": 1,  
- "weight": 21
-   },  
-   {  
- "id": "AV56c5c7cu4Vu652ZQrP",  
- "title": "《临江仙·庭院深深深几许》",  
- "content": "描写梅花 宋·李清照 庭院深深深几许，云窗雾阁春迟，为谁憔悴损芳姿。夜来清梦好，应是发",  
- "userId": 7,  
- "weight": 17
-   },  
-   {  
- "id": "AV56c5YGcu4Vu652ZQrF",  
- "title": "《点绛唇·寂寞深闺》",  
- "content": "闺怨诗 宋·李清照 寂寞深闺，柔肠一寸愁千缕。惜春春去。几点催花雨。倚遍阑干，只是无",  
- "userId": 7,  
- "weight": 7
-   },  
-   {  
- "id": "AV5-ekRHcu4Vu652ZQrw",  
- "title": "我是",  
- "content": "中华共和国",  
- "userId": 2,  
- "weight": 2
-   },  
-   {  
- "id": "AV5-ekBvcu4Vu652ZQrv",  
- "title": "我是",  
- "content": "我爱中华人民共和国",  
- "userId": 1,  
- "weight": 1
-   }  
- ]  
```java
[
  {
    "id": "AV56c5oicu4Vu652ZQrl",
    "title": "《如梦令·谁伴明窗独坐》",
    "content": " 宋·李清照 谁伴明窗独坐，我共影儿俩个。灯尽欲眠时，影也把人抛躲。无那，无那",
    "userId": 9,
    "weight": 39
  },
  {
    "id": "AV56c5mecu4Vu652ZQrh",
    "title": "《鹧鸪天·暗淡轻黄体性柔》",
    "content": " 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。",
    "userId": 5,
    "weight": 35
  },
  {
    "id": "AV56c5g0cu4Vu652ZQrW",
    "title": "《鹧鸪天·暗淡轻黄体性柔》",
    "content": "描写花 宋·李清照 暗淡轻黄体性柔。情疏迹远只香留。何须浅碧深红色，自是花中第一流。",
    "userId": 4,
    "weight": 24
  },
  {
    "id": "AV56c5gIcu4Vu652ZQrV",
    "title": "《渔家傲·天接云涛连晓雾》",
    "content": "豪放诗 宋·李清照 天接云涛连晓雾。星河欲转千帆舞。仿佛梦魂归帝所。闻天语。殷勤问我",
    "userId": 3,
    "weight": 23
  },
  {
    "id": "AV56c5e-cu4Vu652ZQrT",
    "title": "《武陵春·风住尘香花已尽》",
    "content": "闺怨诗 宋·李清照 风住尘香花已尽，日晚倦梳头。物是人非事事休，欲语泪先流。闻说双溪",
    "userId": 1,
    "weight": 21
  },
  {
    "id": "AV56c5c7cu4Vu652ZQrP",
    "title": "《临江仙·庭院深深深几许》",
    "content": "描写梅花 宋·李清照 庭院深深深几许，云窗雾阁春迟，为谁憔悴损芳姿。夜来清梦好，应是发",
    "userId": 7,
    "weight": 17
  },
  {
    "id": "AV56c5YGcu4Vu652ZQrF",
    "title": "《点绛唇·寂寞深闺》",
    "content": "闺怨诗 宋·李清照 寂寞深闺，柔肠一寸愁千缕。惜春春去。几点催花雨。倚遍阑干，只是无",
    "userId": 7,
    "weight": 7
  },
  {
    "id": "AV5-ekRHcu4Vu652ZQrw",
    "title": "我是",
    "content": "中华共和国",
    "userId": 2,
    "weight": 2
  },
  {
    "id": "AV5-ekBvcu4Vu652ZQrv",
    "title": "我是",
    "content": "我爱中华人民共和国",
    "userId": 1,
    "weight": 1
  }
]
```
可以看到，无论是title还是content中，包含“我”“是”字样的都被查询了出来。
MultiMatch还有一些其他的属性，
![](https://img-blog.csdn.net/20170920133241818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170920133326088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
包括Type、tieBreaker等的解释看下图
![](https://img-blog.csdn.net/20170920133516860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170920133555984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170920133635062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以上图片来源于http://www.cnblogs.com/yjf512/p/4897294.html
### 完全包含查询
之前的查询中，当我们输入“我天”时，ES会把分词后所有包含“我”和“天”的都查询出来，如果我们希望必须是包含了两个字的才能被查询出来，那么我们就需要设置一下Operator。
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 单字段包含所有输入
-      */
- @RequestMapping("/contain")  
- public Object contain(String title) {  
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("title", title).operator(MatchQueryBuilder.Operator.AND)).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 单字段包含所有输入
     */
    @RequestMapping("/contain")
    public Object contain(String title) {
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("title", title).operator(MatchQueryBuilder.Operator.AND)).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
无论是matchQuery，multiMatchQuery，queryStringQuery等，都可以设置operator。默认为Or，设置为And后，就会把符合包含所有输入的才查出来。
如果是and的话，譬如用户输入了5个词，但包含了4个，也是显示不出来的。我们可以通过设置精度来控制。
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 单字段包含所有输入(按比例包含)
-      */
- @RequestMapping("/contain")  
- public Object contain(String title) {  
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("title", title).operator(MatchQueryBuilder.Operator.AND).minimumShouldMatch("75%")).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 单字段包含所有输入(按比例包含)
     */
    @RequestMapping("/contain")
    public Object contain(String title) {
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(matchQuery("title", title).operator(MatchQueryBuilder.Operator.AND).minimumShouldMatch("75%")).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
minimumShouldMatch可以用在match查询中，设置最少匹配了多少百分比的能查询出来。
### 合并查询
即boolQuery，可以设置多个条件的查询方式。它的作用是用来组合多个Query，有四种方式来组合，must，mustnot，filter，should。
must代表返回的文档必须满足must子句的条件，会参与计算分值；
filter代表返回的文档必须满足filter子句的条件，但不会参与计算分值；
should代表返回的文档可能满足should子句的条件，也可能不满足，有多个should时满足任何一个就可以，通过minimum_should_match设置至少满足几个。
mustnot代表必须不满足子句的条件。
譬如我想查询title包含“XXX”，且userId=“1”，且weight最好小于5的结果。那么就可以使用boolQuery来组合。
**[java]**[view plain](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[copy](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
[print](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)[?](https://blog.csdn.net/tianyaleixiaowu/article/details/77965257#)
- /**
-      * 多字段合并查询
-      */
- @RequestMapping("/bool")  
- public Object bool(String title, Integer userId, Integer weight) {  
-         SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(boolQuery().must(termQuery("userId", userId))  
-                 .should(rangeQuery("weight").lt(weight)).must(matchQuery("title", title))).build();  
- return elasticsearchTemplate.queryForList(searchQuery, Post.class);  
-     }  
```java
/**
     * 多字段合并查询
     */
    @RequestMapping("/bool")
    public Object bool(String title, Integer userId, Integer weight) {
        SearchQuery searchQuery = new NativeSearchQueryBuilder().withQuery(boolQuery().must(termQuery("userId", userId))
                .should(rangeQuery("weight").lt(weight)).must(matchQuery("title", title))).build();
        return elasticsearchTemplate.queryForList(searchQuery, Post.class);
    }
```
如果某个字段需要匹配多个值，譬如userId为1，2，3任何一个的，类似于mysql中的in，那么可以使用termsQuery("userId", ids).
如果某字段是字符串，我建议空的就设置为null，不要为""空串，貌似某些版本的ES，使用matchQuery空串会不生效。
详细点的看这篇http://blog.csdn.net/dm_vincent/article/details/41743955
boolQuery使用场景非常广泛，应该是主要学习的知识之一。
### Query和Filter的区别
![](https://img-blog.csdn.net/20170920151721228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从代码上就能看出来，query和Filter都是QueryBuilder，也就是说在使用时，你把Filter的条件放到withQuery里也行，反过来也行。那么它们两个区别在哪？
查询在Query查询上下文和Filter过滤器上下文中，执行的操作是不一样的：
1、查询：是在使用query进行查询时的执行环境，比如使用search的时候。
在查询上下文中，查询会回答这个问题——“这个文档是否匹配这个查询，它的相关度高么？”
ES中索引的数据都会存储一个_score分值，分值越高就代表越匹配。即使lucene使用倒排索引，对于某个搜索的分值计算还是需要一定的时间消耗。
2、过滤器：在使用filter参数时候的执行环境，比如在bool查询中使用Must_not或者filter
在过滤器上下文中，查询会回答这个问题——“这个文档是否匹配？”
它不会去计算任何分值，也不会关心返回的排序问题，因此效率会高一点。
另外，经常使用过滤器，ES会自动的缓存过滤器的内容，这对于查询来说，会提高很多性能。
总而言之：
1 查询上下文：查询操作不仅仅会进行查询，还会计算分值，用于确定相关度；
2 过滤器上下文：查询操作仅判断是否满足查询条件，不会计算得分，查询的结果可以被缓存。
所以，根据实际的需求是否需要获取得分，考虑性能因素，选择不同的查询子句。
这篇大概就讲这么多，已经能满足大部分场景了。
