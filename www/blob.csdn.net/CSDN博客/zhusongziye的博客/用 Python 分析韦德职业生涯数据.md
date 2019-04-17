# 用 Python 分析韦德职业生涯数据 - zhusongziye的博客 - CSDN博客





2018年12月27日 21:39:56[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：412标签：[用 Python 分析韦德职业生涯数据](https://so.csdn.net/so/search/s.do?q=用 Python 分析韦德职业生涯数据&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









2018 注定是不平凡的一年，这一年有太多我们熟知的大师离去。他们的离去代表着一个时代的终结，也代表这我们 90 后青春的落幕。我们这代 90 后是个男生都会看场 NBA 的比赛。

![](https://img-blog.csdnimg.cn/2018122721343158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

2013 那年我高二也是 NBA 球队热火队最火爆的时期，詹伟连线无人可挡。

![](https://img-blog.csdnimg.cn/20181227213502993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



那年的总决赛，热火大比分 2-3 落后马刺，当时第六场的最后几秒热火的比分还是落后于马刺，眼看就要输掉了比赛。那时候我们还在上课，但为了看热火这场生死大战，我和几个死党一起逃课。当时我们都觉得热火已经没有希望了，都准备要回去，但就在比赛快要结束时雷阿伦投入高难度扳平三分，从而把比赛拖入加时，最终逆转夺冠！我们几个当场就大喊起来：热火牛逼！

![](https://img-blog.csdnimg.cn/20181227213518852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



绝杀总是能让人激动不已！在NBA历史上，留下了不少的经典绝杀！我们的闪电侠韦德也是个中佼佼者，其中 2009 年对阵公牛一役完成抢断三分绝杀就是一大例证，绝杀之后韦德跑到站台上更是霸气的喊到「This is my house」。



![](https://img-blog.csdnimg.cn/2018122721353871.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



当年的热火给我们的青春带来了太多的回忆，虽然曾经的队友，早已成为了彼此的对手。就像当年一起通宵的死党，如今都已变成陌生人，都在为现实奔波。



而就在昨天 12 月 11 日美国当地时间 10 日晚，NBA 常规赛湖人主场迎战热火队，这也是詹姆斯和韦德生涯最后一次常规赛对决。这场比赛就像多年以后老朋友再次相见，有太多不舍的回忆。赛后詹姆斯和韦德生互相交换了球衣，这个场景不知戳中多少球迷的心。



![](https://img-blog.csdnimg.cn/20181227213609910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



今天就带大家用 Python 分析韦德职业生涯的数据，多年以后或许还可以拿出这篇文章给自己的孩子诉说这自己的青春。



**数据获取**



数据的来源主要是爬 basketball-reference 网站上的数据。



![](https://img-blog.csdnimg.cn/20181227213627326.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



网站地址：



https://www.basketball-reference.com/players/w/wadedw01.html



网站把韦德所有的生涯数据都展示出来，结构也很清晰我们只要用 requests + pyquery 解析就好。



![](https://img-blog.csdnimg.cn/20181227213641155.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



最后我们把解析到的数据利用 pandas 保存到 excel 中即可。



具体的爬虫代码如下：



from pyquery import PyQuery as pq

from fake_useragent import UserAgent

import requests

import pandas as pd



def parse_html():

    url = "https://www.basketball-reference.com/players/w/wadedw01.html"

    ua = UserAgent()

    headers = {"User-Agent": ua.random}

    reponse = requests.get(url, headers=headers)

    if reponse.status_code == 200:

        html = reponse.text

        extract_htm(html)


def extract_htm(html):

    doc = pq(html)

    Dwyane_Wade_data = []

    data_items = doc.find("#div_per_game #per_game tbody tr").items()

    for itme in data_items:

        season = itme.find("th[data-stat='season'] a").text()

        age = itme.find("td[data-stat='age']").text()

        team = itme.find("td[data-stat='team_id'] a").text()

        league = itme.find("td[data-stat='lg_id'] a").text()

        position = itme.find("td[data-stat='pos']").text()

        games = itme.find("td[data-stat='g']").text()

        games_started = itme.find("td[data-stat='gs']").text()

        minutes_played_per_game = itme.find("td[data-stat='mp_per_g']").text()

        field_goals_per_game = itme.find("td[data-stat='mp_per_g']").text()

        field_goals_attempts_per_game = itme.find("td[data-stat='fga_per_g']").text()

        field_goal_percentage = itme.find("td[data-stat='fg_pct']").text()

        point_3_field_goal_per_game = itme.find("td[data-stat='fg3_per_g']").text()

        point_3_field_goal_percentage = itme.find("td[data-stat='fg3_pct']").text()

        point_2_filed_goals_per_game = itme.find("td[data-stat='fg2_per_g']").text()

        point_2_filed_goals_attempts_per_game = itme.find("td[data-stat='fg2a_per_g']").text()

        fg2_pct = itme.find("td[data-stat='fg2_pct']").text()

        efg_pct = itme.find("td[data-stat='efg_pct']").text()

        ft_per_g = itme.find("td[data-stat='ft_per_g']").text()

        fta_per_g = itme.find("td[data-stat='fta_per_g']").text()

        ft_pct = itme.find("td[data-stat='ft_pct']").text()

        orb_per_g = itme.find("td[data-stat='orb_per_g']").text()

        drb_per_g = itme.find("td[data-stat='drb_per_g']").text()

        trb_per_g = itme.find("td[data-stat='trb_per_g']").text()

        ast_per_g = itme.find("td[data-stat='ast_per_g']").text()

        stl_per_g = itme.find("td[data-stat='stl_per_g']").text()

        blk_per_g = itme.find("td[data-stat='blk_per_g']").text()

        tov_per_g = itme.find("td[data-stat='tov_per_g']").text()

        pf_per_g = itme.find("td[data-stat='pf_per_g']").text()

        pts_per_g = itme.find("td[data-stat='pts_per_g']").text()

        item_data = {'season': season, "age": age, "team": team, "league": league, "position": position, "games": games,

                "games_started": games_started, "minutes_played_per_game": minutes_played_per_game, "field_goals_per_game": field_goals_per_game,

                "field_goals_attempts_per_game": field_goals_attempts_per_game, "field_goal_percentage": field_goal_percentage,

                "point_3_field_goal_per_game": point_3_field_goal_per_game, "point_3_field_goal_percentage": point_3_field_goal_percentage,

                "point_2_filed_goals_per_game": point_2_filed_goals_per_game, "point_2_filed_goals_attempts_per_game": point_2_filed_goals_attempts_per_game,

                "fg2_pct": fg2_pct, "efg_pct": efg_pct, "ft_per_g": ft_per_g, "fta_per_g": fta_per_g, "ft_pct": ft_pct,

                "orb_per_g": orb_per_g, "drb_per_g": drb_per_g, "trb_per_g": trb_per_g, "ast_per_g": ast_per_g, "stl_per_g": stl_per_g,

                "blk_per_g": blk_per_g, "tov_per_g": tov_per_g, "pf_per_g": pf_per_g, "pts_per_g": pts_per_g}

        print(item_data)

        Dwyane_Wade_data.append(item_data)


    data = pd.DataFrame(Dwyane_Wade_data)

    data.to_csv("Dwyane_Wade_data.csv", encoding='utf_8_sig')



if __name__ == '__main__':

    parse_html()



最后保存的 excel 表如下：



![](https://img-blog.csdnimg.cn/20181227213718420.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**出场次数和首发数据分析**



首先我们对韦德生涯参加的场数和时间进行一个分析。



games:参与的比赛场数（都为82场)

games_start: 先发次数

age：年龄

minutes_played: 平均每场比赛进行的时间

![](https://img-blog.csdnimg.cn/20181227213743648.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



韦德 22 岁就进入 NBA 打比赛，出道就打了 61 场比赛，首发 56 场，往后的几年时间里都是打球队首发人员。一个赛季按 82 场来算，韦德在 2008 - 09 赛季恐怖的达到了 79 场比赛。



![](https://img-blog.csdnimg.cn/20181227213806526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



平均每场比赛时间都在 30 分钟以上。

![](https://img-blog.csdnimg.cn/20181227213825620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



从这些数据可以看出韦德是队伍中的主力军，绝对的领袖。



**投球命中率**

![](https://img-blog.csdnimg.cn/20181227213845838.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



韦德投球命中率平均在 0.47 。



![](https://img-blog.csdnimg.cn/20181227213904662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



2013-14 赛季达到职业生涯最大命中率：0.55。



由于篇幅的问题其他的数据就暂时不再继续分析，感兴趣的同学可获取相应的源代码进行分析，这里给大家一个单词对照表。

链接: https://pan.baidu.com/s/1PCQ-gEX-aaV5JvgDva_0lQ 提取码: dx73


|数据名|含义|
|----|----|
|Rk -- Rank|排名|
|G -- Games|参与的比赛场数（都为82场）|
|MP -- Minutes Played|平均每场比赛进行的时间|
|FG--Field Goals|投球命中次数|
|FGA--Field Goal Attempts|投射次数|
|FG%--Field Goal Percentage|投球命中次数|
|3P--3-Point Field Goals|三分球命中次数|
|3PA--3-Point Field Goal Attempts|三分球投射次数|
|3P%--3-Point Field Goal Percentage|三分球命中率|
|2P--2-Point Field Goals|二分球命中次数|
|2PA--2-point Field Goal Attempts|二分球投射次数|
|2P%--2-Point Field Goal Percentage|二分球命中率|
|FT--Free Throws|罚球命中次数|
|FTA--Free Throw Attempts|罚球投射次数|
|FT%--Free Throw Percentage|罚球命中率|
|ORB--Offensive Rebounds|进攻篮板球|
|DRB--Defensive Rebounds|防守篮板球|
|TRB--Total Rebounds|篮板球总数|
|AST--Assists|辅助|
|STL--Steals|偷球|
|BLK -- Blocks|封阻|
|TOV -- Turnovers|失误|
|PF -- Personal Fouls|个犯|
|PTS -- Points|得分|
- 



在很多年后，当老詹和骚韦挺着日渐臃肿的肚子来到球场，看着场上的小詹和小腮帮在球场上尽情挥洒着天赋，一次次的空接暴扣，一次次的抢断反击，听着场下一声又一声的呐喊，会不会忽然就湿润了眼眶。



![](https://img-blog.csdnimg.cn/20181227213931137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)





此文纪念我们逝去的青春。





