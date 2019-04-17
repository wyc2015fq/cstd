# Python2.7开发经典RPG游戏《魔塔》 - zhusongziye的博客 - CSDN博客





2018年06月09日 17:53:12[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1744








《魔塔》是一种策略类的固定数值RPG游戏。游戏需要动很多脑筋，任何一个轻率的选择都可能导致游戏的失败。魔塔游戏虽不大，但是制作精美，道具很多，而且难度不低，对智商是一次艰巨的考验。

虽然魔塔的界面很像是一般的地牢游戏，貌似随便的打打杀杀就可以过关，但事实上玩这个游戏需要动很多脑筋，任何一个轻率的选择都可能导致游戏的失败，该游戏有属性攻击、防御、生命、金币、经验。对怪物的伤害次数计算公式，是敌人的生命/（自己的攻击-敌人的防御）；而伤害的数目计算是怪物伤害次数（敌人的攻击-自己的防御）。




这个是Python2.7版本的

这个游戏内含的元素

故事背景

怪物（名称、生命、攻击、防御、击杀获取经验值）

房间

物品名称

人物属性：经验值、生命值、攻击、防御

![](https://img-blog.csdn.net/20180609175146406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







main.py




```
# -*- coding:utf-8 -*-
import display,fight
import re
u"""
魔王抓走了公主，玩家要进入魔塔打败魔王，救出公主。
魔塔由很多房间组成，房间里有物品、怪物，还要公主和魔王。
玩家要打败怪物升级，搜索房间获得物品，让自己属性变得更强，才能打败魔王。
要用一个引擎让这个充满房间的魔塔运行。
"""
#游戏引擎类
class Engine(object):
    def __init__(self,tower):
        self.tower = tower
    def play(self):
#游戏介绍
        print u"""
        欧洲中世纪时期，一位叫玛丽的公主被恶魔掳走了。
        国王悲痛欲绝，许下诺言说谁要是能救回公主，就把国家的国土分他一半。
        可是并没有人敢答应，因为大家都知道恶魔十分恐怖，它所住的地方--魔塔，
        更是一个阴森恐怖，极度危险的地方，到过那的人没有一个能活着回来。
        这个时候，一个叫卡尔的年轻人主动上前，表示愿意试一试。
        国王说，年轻人那就拜托你了，魔塔离这里路途遥远，不过我可以用时光之杖直接送你过去。
        只见一道白光，你被传送到了魔塔。
        """
        raw_input('>')
        print u"""
        你来到了魔塔，这里阴森恐怖，有很多怪物在这出没，也有一些地方藏着很厉害的武器，
        你要试着去找到那些武器让自己变得更加强大，挑战不同的怪物让自己升级，才能让自己
        有足够能力打败魔王！
        在游戏中，Q键可以查看你和怪物的属性，战斗预测可以告诉你打败怪物会消耗的血量。
        E键可以查看小地图，了解你在地图中的位置。
        游戏开始了，加油！
        """
        raw_input('>')
        now_room = None
        next_room = self.tower.first_room
        while True:
            room = self.tower.enter_room(next_room,now_room)
            now_room = next_room
            next_room = room
#魔塔类
class Tower(object):
    def __init__(self,first_room):
        self.room = {"room1":Room1(),"room2":Room2(),"room3":Room3(),
                "room4":Room4(),"room5":Room5(),"room6":Room6(),
                "room7":Room7(),"room8":Room8(),"room9":Room9(),
                "basement":basement(),"palace":palace()}
        self.first_room = first_room
    def enter_room(self,next_room,now_room):
        next_room = self.room.get(next_room)
        return next_room.enter(now_room)
#房间类
class Room(object):
    room_dict = {'room1':1,'room2':2,'room3':3,'room4':4,
                'room5':5,'room6':6,'room7':7,'room8':8,
                'room9':9,'palace':-1,'basement':11}
    toward_dict = {"w":"up","s":"down",
                   "a":"left","d":"right","f":"middle"}
    open_flag = 0
    def enter(self,last_room):
#显示房间剧情
        print "---------------------------------------------\n"
        num = re.findall(r"\d",self.room_name)
        if num == []:
            if self.room_name == "palace":
                print u"宫殿"
            else:
                print u"地下室"
        else:
            print u"进入房间%s" % num[0]
        print self.words
#获取玩家在房间的位置
        self.player_pos = self.get_playerpos(last_room)
#显示房间样子
        display.display_room(self.player_pos,self.monster,self.monster_pos,
        self.door_pos,self.goods)
#如果去过地下室，则可以进入宫殿
        if self.room_name == "basement":
            Room.open_flag = 1
#进入玩家选择循环
        while True:
            print u"请选择：1、搜索物品  2、消灭怪物  3、离开房间"
            print u"Q键查看玩家属性  E键显示小地图"
            choose0 = raw_input('>')
            if choose0 == "q":
                display.display_property(self.monster)
            elif choose0 == "e":
                display.display_pos(self.room_name)
            elif choose0 == "1":
                fight.find_goods(self.goods)
                self.goods = []
            elif choose0 == "2":
                print u"请选择你要挑战的怪物：W、上 S、下 A、左 D、右  F、中  R、返回"
                choose1 = raw_input('>')
                if choose1 == "r":
                    continue
                monster_toward = Room.toward_dict.get(choose1)
                if monster_toward not in self.monster_pos:
                    print u"那里没有怪物。"
                    continue
                fight.fight(self.monster)
                self.monster_pos.remove(monster_toward)
            elif choose0 == "3":
                print u"请选择你要进入的房间：W、上 S、下 A、左 D、右    R、返回"
                choose2 = raw_input('>')
                if choose2 == "r":
                    continue
                door_toward = Room.toward_dict.get(choose2)
                if door_toward not in self.door_pos:
                    print u"那是一堵墙。"
                    continue
                if door_toward in self.monster_pos:
                    print u"你必须打败门前的怪物，才能进去。"
                    continue
                #根据方向得到下个房间的名字
                num_dict = {"up":-3,"down":3,
                               "left":-1,"right":1}
                next_roomnum = Room.room_dict.get(self.room_name)
                next_roomnum += num_dict.get(door_toward)
                for key,value in Room.room_dict.items():
                    if value == next_roomnum:
                        next_room =  key
                        break
                if next_room == "palace" and Room.open_flag == 0:
                    print u"这个地方感觉很危险，我还是等会再进去吧~"
                    continue
                return next_room
            else:
                print u"指令错误。"
    def get_playerpos(self,last_room):
        if last_room == None:
            return "middle"
        last_roomnum = Room.room_dict.get(last_room)
        now_roomnum = Room.room_dict.get(self.room_name)
        num = now_roomnum - last_roomnum
        if num == 3:
            return "up"
        elif num == 1:
            return "left"
        elif num == -1:
            return "right"
        elif num == -3:
            return "down"
        else:
            print u"无法确定玩家位置！"
#房间1
class Room1(Room):
    def __init__(self):
        self.room_name = "room1"
        self.goods = [u"圣剑",u"中血瓶"]
        self.monster = ""
        self.monster_pos = []
        self.door_pos = ["right"]
        self.words = u"这是个存放物品的房间，说不定藏着什么好东西！"
#房间2
class Room2(Room):
    def __init__(self):
        self.room_name = "room2"
        self.goods = []
        self.monster = u"巫师"
        self.monster_pos = ["left","middle","up","right"]
        self.door_pos = ["left","up","down"]
        self.words = u"房间里似乎有一些可怕的生物，你要打败门口的怪物，才能打开相应的门。"
#房间3
class Room3(Room):
    def __init__(self):
        self.room_name = "room3"
        self.goods = [u"圣盾",u"超大血瓶"]
        self.monster = ""
        self.monster_pos = []
        self.door_pos = ["down"]
        self.words = u"这是个存放物品的房间，说不定藏着什么好东西！"
#房间4
class Room4(Room):
    def __init__(self):
        self.room_name = "room4"
        self.goods = []
        self.monster = u"史莱姆"
        self.monster_pos = ["left","middle","up","down"]
        self.door_pos = ["right","down"]
        self.words = u"房间里似乎有一些可怕的生物，你要打败门口的怪物，才能打开相应的门。"
#房间5
class Room5(Room):
    def __init__(self):
        self.room_name = "room5"
        self.goods = []
        self.monster = ""
        self.monster_pos = []
        self.door_pos = ["left","right","up","down"]
        self.words = u"这是一个空荡荡的房间，可以通向四面八方。"
#房间6
class Room6(Room):
    def __init__(self):
        self.room_name = "room6"
        self.goods = []
        self.monster = u"小魔王"
        self.monster_pos = ["up"]
        self.door_pos = ["left","up"]
        self.words = u"你走进了小恶魔的房间，这是魔塔里十分强大的怪物，最好不要惹他。"
#房间7
class Room7(Room):
    def __init__(self):
        self.room_name = "room7"
        self.goods = [u"小血瓶"]
        self.monster = ""
        self.monster_pos = []
        self.door_pos = ["up"]
        self.words = u"这是个存放物品的房间，说不定藏着什么好东西！"
#房间8
class Room8(Room):
    def __init__(self):
        self.room_name = "room8"
        self.goods = []
        self.monster = u"骑士"
        self.monster_pos = ["left","down","right","middle"]
        self.door_pos = ["down","up","right"]
        self.words = u"房间里似乎有一些可怕的生物，你要打败门口的怪物，才能打开相应的门。"
#房间9
class Room9(Room):
    def __init__(self):
        self.room_name = "room9"
        self.goods = [u"大血瓶",u"圣水"]
        self.monster = ""
        self.monster_pos = []
        self.door_pos = ["left"]
        self.words = u"这是个存放物品的房间，说不定藏着什么好东西！"
#地下室
class basement(Room):
    def __init__(self):
        self.room_name = "basement"
        self.goods = []
        self.monster = u"公主"
        self.monster_pos = ["middle"]
        self.door_pos = ["up"]
        self.words = u"""
        这是一个的地下室，有个人被囚禁在其中。原来是公主！
        你叫醒了公主，说到"公主，我是你父王派来救你的，快跟我走吧！"
        公主揉了揉眼睛，说："非常感谢你英雄，可是我被魔王用法术禁锢在这里了，
        只有打败大魔王，才能离开。大魔王的位置在地图最上边的神秘房间里，
        那个地方十分危险，在进去之前一定要做好充分的准备！"
        "恩，等我的好消息吧！"，你说到。
        """
""
#魔王宫殿
class palace(Room):
    def __init__(self):
        self.room_name = "palace"
        self.goods = []
        self.monster = u"大魔王"
        self.monster_pos = ["middle"]
        self.door_pos = []
        self.words = u"""你走进了大魔王的宫殿。
        "年轻人，你终于来了，我等你很久了。我要告诉你一个不幸的消息，这个房子不打败我
        是无法离开的。哈哈哈，乖乖受死吧！"
        """
#角色类
class Role(object):
    pass
tower = Tower("room5")
game = Engine(tower)
game.play()
```




figth.py




```
# -*- coding:utf-8 -*-
import copy
u"""
本模块用于配置玩家和怪物的属性，提供战斗函数。另外提供一个物品使用函数。
"""
#配置玩家属性
player_property = {u"名字":u"卡尔",u"生命值":100,u"攻击力":10,u"护甲":10,u"经验":0,u"符号":"(^_^)"}
##配置怪物属性
Slime_property = {u"名字":u"史莱姆",u"生命值":25,u"攻击力":15,u"护甲":15,u"符号":"~0Y0~"}
wizard_property = {u"名字":u"巫师",u"生命值":50,u"攻击力":40,u"护甲":10,u"符号":">@Y@<"}
knight_property = {u"名字":u"骑士",u"生命值":350,u"攻击力":30,u"护甲":50,u"符号":"\OYO/"}
s_devil_property = {u"名字":u"小魔王",u"生命值":400,u"攻击力":100,u"护甲":100,u"符号":"~/-Y-\~"}
g_devil_property = {u"名字":u"大魔王",u"生命值":3000,u"攻击力":300,u"护甲":300,u"符号":"~~//-Y-\\\~~"}
princess_property = {u"名字":u"公主",u"符号":"^o^"}
def fight(monster):
    u"""
    函数功能：让玩家和怪物进行战斗，若玩家胜利，则设置玩家剩余生命值，若失败，则退出游戏。
    输入参数：monster--怪物，有效的值：u"史莱姆"，u"巫师"，u"骑士"，u"小魔王"，u"大魔王"
    """
    global player_property,Slime_property,wizard_property
    global knight_property,s_devil_property,g_devil_property
    #判断是什么怪物
    if monster == Slime_property[u"名字"]:
        monster_property = copy.deepcopy(Slime_property)
    elif monster == wizard_property[u"名字"]:
        monster_property = copy.deepcopy(wizard_property)
    elif monster == knight_property[u"名字"]:
        monster_property = copy.deepcopy(knight_property)
    elif monster == s_devil_property[u"名字"]:
        monster_property = copy.deepcopy(s_devil_property)
    elif monster == g_devil_property[u"名字"]:
        monster_property = copy.deepcopy(g_devil_property)
    elif monster == princess_property[u"名字"]:
        print u"是不是傻！公主你也杀。"
        exit()
    else:
        print u"怪物名字输入错误！"
        exit()
#开始战斗
    who_round = "player"
    while player_property[u"生命值"] > 0 and monster_property[u"生命值"] > 0:
        if who_round == "player":
            monster_property[u"生命值"] -= (player_property[u"护甲"] + player_property[u"攻击力"])
            who_round = "monster"
        else:
            player_property[u"生命值"] -= (monster_property[u"护甲"] + monster_property[u"攻击力"])
            who_round = "player"
#战斗结果处理
    if player_property[u"生命值"] <= 0:
        print u'"不自量力的家伙，哼!"  怪物将你大卸八块，吃掉了！'
        print u"你被打败了，游戏结束！"
        exit()
    else:
        print u"你打败了怪物，赢得了胜利，获得25点经验！"
        player_property[u"经验"] += 25
#打败小魔王升3级
    if monster == s_devil_property[u"名字"]:
            print u"打败小魔王，连升3级!生命值+300，攻击力加30，护甲加30！"
            player_property[u"生命值"] += 300
            player_property[u"攻击力"] += 30
            player_property[u"护甲"] += 30
#打败大魔王游戏结束
    if monster == g_devil_property[u"名字"]:
            print u"""
            "啊~不，不可能！我竟然被打败了！"，大魔王怒吼道,
            "既然你打败了我，那么我就要你陪葬，你们谁都跑不了，哈哈哈哈哈~"
            这时地动山摇，墙壁开裂，屋顶的石头不断落下，魔塔开始坍塌。
            你趁机从墙壁的裂缝里溜了出去，救出了公主。
            从此，英雄卡尔的名字成了一个传说~
            恭喜你，通关！
            """
            exit()
#经验达到100升级属性
    if player_property[u"经验"] == 100:
        print u"恭喜你，升了1级，生命值+100，攻击力加10，护甲加10！"
        player_property[u"生命值"] += 100
        player_property[u"攻击力"] += 10
        player_property[u"护甲"] += 10
        player_property[u"经验"] = 0
def forecast_fight(monster):
    u"""
    函数功能：预测玩家和怪物战斗需要的生命值
    输入参数：monster--怪物名字，有效的值：u"史莱姆"，u"巫师"，u"骑士"，u"小魔王"，u"大魔王"
    返回参数：health--消耗玩家的生命值
    """
    global player_property,Slime_property,wizard_property
    global knight_property,s_devil_property,g_devil_property
    #判断是什么怪物
    if monster == Slime_property[u"名字"]:
        monster_property = copy.deepcopy(Slime_property)
    elif monster == wizard_property[u"名字"]:
        monster_property = copy.deepcopy(wizard_property)
    elif monster == knight_property[u"名字"]:
        monster_property = copy.deepcopy(knight_property)
    elif monster == s_devil_property[u"名字"]:
        monster_property = copy.deepcopy(s_devil_property)
    elif monster == g_devil_property[u"名字"]:
        monster_property = copy.deepcopy(g_devil_property)
    else:
        print u"怪物名字输入错误！"
        exit()
    player_health =  player_property[u"生命值"]
#模拟战斗
    who_round = "player"
    while monster_property[u"生命值"] > 0:
        if who_round == "player":
            monster_property[u"生命值"] -= (player_property[u"护甲"] + player_property[u"攻击力"])
            who_round = "monster"
        else:
            player_health -= (monster_property[u"护甲"] + monster_property[u"攻击力"])
            who_round = "player"
#计算消耗的血量
    health = player_property[u"生命值"] - player_health
    return health
def find_goods(goods):
    u"""
    函数功能：搜索物品
    输入参数：goods--物品，类型--列表
    """
    if goods == []:
        print u"什么都没找到！"
    else:
        for good in goods:
            if good == u"小血瓶":
                print u"你获得一个小血瓶，生命值增加100点！"
                player_property[u"生命值"] += 100
            elif good == u"中血瓶":
                print u"你获得一个中血瓶，生命值增加200点！"
                player_property[u"生命值"] += 200
            elif good == u"大血瓶":
                print u"你获得一个大血瓶，生命值增加400点！"
                player_property[u"生命值"] += 400
            elif good == u"超大血瓶":
                print u"你获得一个超大血瓶，生命值增加600点！"
                player_property[u"生命值"] += 600
            elif good == u"圣水":
                print u"你获得一瓶圣水，生命值增加一倍！攻击力增加一倍！护甲增加一倍！"
                player_property[u"生命值"] *= 2
                player_property[u"攻击力"] *= 2
                player_property[u"护甲"] *= 2
            elif good == u"圣剑":
                print u"你获得一把圣剑，攻击力增加100！"
                player_property[u"攻击力"] += 100
            elif good == u"圣盾":
                print u"你获得一个圣盾，护甲增加100！"
                player_property[u"护甲"] += 100
            elif good == u"钥匙":
                print u"你获得一把钥匙！"
            else:
                print u"物品名称错误！"
```




display.py




```
# -*- coding:utf-8 -*-
import copy
import fight
u"""
这个模块提供一些实现显示功能的函数。
"""
def display_pos(room):
    u"""
    函数功能：显示当前所在位置
    输入参数：room--房间号 ，类型--字符串
    """
    room_dict = {'room1':1,'room2':2,'room3':3,'room4':4,'room5':5,
                 'room6':6,'room7':7,'room8':8,'room9':9,
                 'palace':0,'basement':10}
    pos = [' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ']
    pos[room_dict.get(room)] = 2
    print u"""
        ================
        |   神秘房间   |
        |      %c       |
    ==========---===========
    | 房间1 | 房间2 | 房间3 |
    |  %c    |  %c    |  %c    |
    |       |       |       |
    |=======|=======|=======|
    | 房间4 | 房间5 | 房间6 |
    |  %c    |  %c    |  %c    |
    |       |       |       |
    |=======|=======|=======|
    | 房间7 | 房间8 | 房间9 |
    |  %c    |  %c    |  %c    |
    |       |       |       |
    ===========---===========
        |    地下室    |
        |      %c       |
        ================
    """ % (pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7],pos[8],pos[9],pos[10])
def display_room(player_pos,monster,monster_pos,door_pos,goods):
    u"""
    函数功能：显示房间内的景象。
    输入参数：
    玩家的位置--player_pos，类型--字符串,有效的值--"up","left","right","down","middle",0（不存在）
    怪物的种类--monster，类型--字符串
        有效的值：  "史莱姆"   ~0Y0~
                   "巫师"   >@Y@<
                   "骑士"   \OYO/
                   "小魔王"  ~/-Y-\~
                   "大魔王"  ~~//-Y-\\\~~
                   "公主"   ^o^
                   ""     不存在怪物
    怪物的位置--monster_pos,类型--列表  空列表表示不存在怪物
    列表中每个元素，代表一个怪物的位置，类型--字符串,有效的值--"up","left","right","down","middle"
    门的位置--door_pos,类型--列表     空列表表示不存在门
    列表中每个元素--代表一个门的位置，类型--字符串,有效的值--"up","left","right","down"
    物品的种类--goods,类型--列表     空列表表示不存在物品
    列表中每个元素--代表一种物品，类型--字符串
        有效的值："Excalibur" 圣剑   +---
                 "圣盾"       [+]
                 "钥匙"    C=--
                 "小血瓶"    (o)
                 "中血瓶"    (oo)
                 "大血瓶"    (ooo)
                 "超大血瓶"    (oooo)
                 "圣水"   [o]
                 "地图"    [|]
    备注：玩家图形：  (^_^)
    """
    drawlist = []
    good_pos = [6,7,9]
    for i in range(14):
        drawlist.append('  ')
    if player_pos == "up":          #根据输入参数，让玩家图形在对应的位置显示
        drawlist[2] = convert_sign(fight.player_property[u"符号"])
    elif player_pos == "down":
         drawlist[12] = convert_sign(fight.player_property[u"符号"])
    elif player_pos == "left":
         drawlist[5] = convert_sign(fight.player_property[u"符号"])
    elif player_pos == "right":
         drawlist[10] = convert_sign(fight.player_property[u"符号"])
    elif player_pos == "middle":
         drawlist[7] = convert_sign(fight.player_property[u"符号"])
    else:
        print u"玩家位置错误！"
    if monster == u"史莱姆":      #根据输入参数，设置怪物图形
        monster_sign = convert_sign(fight.Slime_property[u"符号"])
    elif monster == u"巫师":
        monster_sign = convert_sign(fight.wizard_property[u"符号"])
    elif monster == u"骑士":
        monster_sign = convert_sign(fight.knight_property[u"符号"])
    elif monster == u"公主":
        monster_sign = convert_sign(fight.princess_property[u"符号"])
    elif monster == u"小魔王":
        monster_sign = convert_sign(fight.s_devil_property[u"符号"])
    elif monster == u"大魔王":
        monster_sign = convert_sign(fight.g_devil_property[u"符号"])
    elif monster == "":
        pass
    else:
        print u"怪物名称错误！"
        monster_sign = convert_sign("  ")
    for mon in monster_pos:           #根据输入参数，让怪物图形在对应的位置显示
        if mon == "up":
            drawlist[2] = monster_sign
        elif mon == "down":
             drawlist[12] = monster_sign
        elif mon == "left":
             drawlist[5] = monster_sign
        elif mon == "right":
             drawlist[10] = monster_sign
        elif mon == "middle":
             drawlist[7] = monster_sign
        else:
            print u"怪物位置错误!"
    for door in door_pos:           #根据输入参数，让门图形在对应的位置显示
        if door == "up":
            drawlist[0] = '_ '
            drawlist[1] = '\b| |'
        elif door == "down":
             drawlist[13] = '\b|_|'
        elif door == "left":
             drawlist[3] = '|\\'
             drawlist[4] = ' \\'
        elif door == "right":
             drawlist[8] = '/|'
             drawlist[11] = '/ '
        else:
            print u"门位置错误!"
    for good in goods:             #根据输入参数判断是什么物品，并显示在指定的位置
        if good == u"圣剑":
            gooddraw = convert_sign("+---")
        elif good == u"圣盾":
            gooddraw = convert_sign("[+]")
        elif good == u"钥匙":
            gooddraw = convert_sign("C=--")
        elif good == u"小血瓶":
            gooddraw = convert_sign("(o)")
        elif good == u"中血瓶":
            gooddraw = convert_sign("(oo)")
        elif good == u"大血瓶":
            gooddraw = convert_sign("(ooo)")
        elif good == u"超大血瓶":
            gooddraw = convert_sign("(oooo)")
        elif good == u"圣水":
            gooddraw = convert_sign("[o]")
        elif good == u"地图":
            gooddraw = convert_sign("[|]")
        else:
            print u"物品名称错误！"
            continue
        drawlist[good_pos.pop(0)] = gooddraw
    draw = (drawlist[0],drawlist[1],drawlist[2],drawlist[3],drawlist[4],
    drawlist[5],drawlist[6],drawlist[7],drawlist[8],drawlist[9],drawlist[10],
    drawlist[11],drawlist[12],drawlist[13])
    print """
    ==================================
    |-                              -|
    | -                   %s       - |
    |  -                  %s      -  |
    |   --------------------------   |
    |   -                  %s    -   |
    |%s -                        -   |
    |%s -    %s       %s         -   |
    |   -             %s         - %s|
    |   -             %s      %s - %s|
    |   -      %s                -   |
    |   --------------------------   |
    | -       %s                   - |
    ==================================
    """ % draw
##输入图形格式转换函数,保证%s所在位置就是字符串的最右端,用于对齐格式
def convert_sign(sign):
    return '\b'*(len(sign)-2) + sign
def display_property(monster):
    u"""
    函数功能：显示玩家和怪物属性
    输入参数：monster--怪物名字，类型：字符串
        有效的值： u"史莱姆"，u"巫师"，u"骑士"，u"小魔王"，u"大魔王",""(不存在怪物)
    """
    monster_exist = True
    #判断是什么怪物
    if monster == fight.Slime_property[u"名字"]:
        monster_property = copy.deepcopy(fight.Slime_property)
    elif monster == fight.wizard_property[u"名字"]:
        monster_property = copy.deepcopy(fight.wizard_property)
    elif monster == fight.knight_property[u"名字"]:
        monster_property = copy.deepcopy(fight.knight_property)
    elif monster == fight.s_devil_property[u"名字"]:
        monster_property = copy.deepcopy(fight.s_devil_property)
    elif monster == fight.g_devil_property[u"名字"]:
        monster_property = copy.deepcopy(fight.g_devil_property)
    elif monster == "":
        monster_exist = False
    else:
        print u"怪物名字输入错误！"
        exit()
    if monster_exist == True:
        draw = (convert_sign(fight.player_property[u"符号"]),convert_sign(fight.player_property[u"名字"]),
        fight.player_property[u"生命值"],fight.player_property[u"攻击力"],fight.player_property[u"护甲"],
        convert_sign(monster_property[u"符号"]),convert_sign(monster_property[u"名字"]),
        monster_property[u"生命值"],monster_property[u"攻击力"],monster_property[u"护甲"],
        fight.forecast_fight(monster_property[u"名字"]))
    else:
        draw = (convert_sign(fight.player_property[u"符号"]),convert_sign(fight.player_property[u"名字"]) ,
        fight.player_property[u"生命值"],fight.player_property[u"攻击力"],
        fight.player_property[u"护甲"],"","","","","","")
    print u"""
    _________________________________________________
              生命值    攻击力    护甲    战斗预测
        %s
      %s      %s       %s       %s
        %s
      %s      %s       %s       %s      %s
    _________________________________________________
    """ % draw
# #测试
# display_property(0)
# display_property(u"史莱姆")
# display_property(u"骑士")
# display_property(u"大魔王")
# # #房间1
# # print u"房间1"
# display_room("right",0,[],["right"],[u"圣剑",u"钥匙",u"中血瓶"])
# # #房间2
# # print u"房间2"
# display_room("down",u"巫师",["left","right","up","middle"],["left","up","down"],[])
# # #房间3
# print u"房间3"
# display_room("down",0,[],["down"],[u"大血瓶",u"圣盾"])
# # #房间4
# print u"房间4"
# display_room("right",u"史莱姆",["left","down","up","middle"],["right","down"],[])
# # #房间5
# print u"房间5"
# display_room("middle",0,[],["up","left","right","down"],[])
# # #房间4
# print u"房间6"
# display_room("left",u"小魔王",["middle"],["left","up"],[])
# # #房间1
# print u"房间7"
# display_room("up",0,[],["up"],[u"地图",u"小血瓶"])
# # #房间8
# print u"房间8"
# display_room("up",u"骑士",["left","down","right","middle"],["up","right","down"],[])
# print u"房间9"
# display_room("left",0,[],["left"],[u"圣水",u"大血瓶"])
# # #恐怖之地
# print u"恐怖之地"
# display_room("down",u"大魔王",["middle"],["down"],[])
# # #地下室
# print u"地下室"
# display_room("up",u"公主",["middle"],["up"],[])
```




game.py

```
# -*- coding:utf-8 -*-
import display
import fight
class Room(object):
    def choose_do(self,lastroom):
        #根据上个房间的号码和当前房间号码的差得到当前玩家在房间里的位置
        compare = lastroom - self.roomnum
        if compare == -3:
            self.player_pos = "up"
        elif compare == -1:
            self.player_pos = "left"
        elif compare == 1:
            self.player_pos = "right"
        elif compare == 3:
            self.player_pos = "down"
        else:
            pass
        #显示房间场景
        display.display_room(self.player_pos,self.monster,
                        self.monster_pos,self.door_pos,self.goods)
        while True:
            print u"你可以选择：1、搜索房间  2、离开房间  3、查看小地图  4、查看自己的属性 5、消灭怪物"
            choice = raw_input(">")
            if choice == '1':
                if self.goods != []:
                    print self.word
                    fight.use_goods(self.goods)
                    self.goods = []
                else:
                    print u"\t什么也没有！\n"
            elif choice == '2':
                while True:
                    print u"请选择你要进入的房间：W:上 A:左 S：下 D：右 Q:返回"
                    choose_room = raw_input('>')
                    if choose_room == "w":
                        choose_room = "up"
                    elif choose_room == "a":
                        choose_room = "left"
                    elif choose_room == "s":
                        choose_room = "down"
                    elif choose_room == "d":
                        choose_room = "right"
                    elif choose_room == 'q':
                        break
                    else:
                        pass
                    if choose_room not in self.door_pos:
                        print u"选择错误！"
                    else:
                        if choose_room in self.monster_pos:
                            print u"你必须打败门口的守卫，才能进入这个房间！"
                        else:
                            if choose_room == "up":
                                return self.roomnum - 3
                            elif choose_room == "left":
                                return self.roomnum - 1
                            elif choose_room == "down":
                                return self.roomnum + 3
                            elif choose_room == "right":
                                return self.roomnum + 1
                            else:
                                pass
            elif choice == '3':
                display.display_pos(self.roomnum)
            elif choice == '4':
                display.display_property(self.monster)
            elif choice == '5':
                if self.monster_pos == []:
                    print u"没有怪物！"
                else:
                    print u"请选择你要攻击的怪物：W:上 A:左 S：下 D：右  E:中 Q:返回"
                    choose_monster = raw_input('>')
                    if choose_monster == "w":
                        choose_monster = "up"
                    elif choose_monster == "a":
                        choose_monster = "left"
                    elif choose_monster == "s":
                        choose_monster = "down"
                    elif choose_monster == "d":
                        choose_monster = "right"
                    elif choose_monster == "e":
                        choose_monster = "middle"
                    elif choose_monster == 'q':
                        continue
                    else:
                        pass
                    if choose_monster not in self.monster_pos:
                        print u"这个地方没有怪物!"
                        continue
                    fight.fight(self.monster)
                    self.monster_pos.remove(choose_monster)
            else:
                print u"输入错误！"
class Room1(Room):
    def __init__(self):
        self.roomnum = 1
        self.player_pos = "right"
        self.goods = [u"圣剑",u"钥匙",u"中血瓶"]
        self.monster = 0
        self.monster_pos = []
        self.door_pos = ["right"]
        self.word = u"""
得到一把圣剑 +---，攻击力上升100！
得到一个中血瓶 (oo)，生命值上升200！
得到一把钥匙 C=--！
        """
    def story_room(self,lastroom):
        u"""
        函数功能：交代room1剧情，显示房间场景，获取用户选择，做相应处理。
        输入参数：无
        返回参数：self.nextroom--下个房间的号码，类型--数字类型
        0--神秘房间   10--地下室
        """
        print u"房间1"
        if self.goods != []:
            print u"你进入了一个昏暗的房间，房间里似乎有一些东西在发光！"
        else:
            print u"你进入了一个昏暗的房间，房间里空无一物。"
        #进入游戏循环
        return Room.choose_do(self,lastroom)
class Room2(Room):
    def __init__(self):
        self.roomnum = 2
        self.player_pos = " "
        self.goods = []
        self.monster = u"巫师"
        self.monster_pos = ["up","middle","left","right"]
        self.door_pos = ["left","up","down"]
        self.word = u"""
        """
    def story_room(self,lastroom):
        u"""
        函数功能：交代room2剧情，显示房间场景，获取用户选择，做相应处理。
        输入参数：无
        返回参数：self.nextroom--下个房间的号码，类型--数字类型
        -1--神秘房间   11--地下室
        """
        #剧情交代
        print u"房间2"
        if self.monster_pos != []:
            print u"你进入了一个昏暗的房间，房间里似乎有一些可怕的生物！"
        else:
            print u"你进入了一个昏暗的房间，房间里空无一物。"
        #进入游戏循环
        return Room.choose_do(self,lastroom)
# room1 = Room1(1)
# room1.story_room1()
#启动游戏的类
class Game(object):
    def start(self):
        last_room = 1
        room1 = Room1()
        room2 = Room2()
        nextroom = room1.story_room(last_room)
        while True:
            if nextroom == 1:
                nextroom = room1.story_room(last_room)
                last_room = 1
            elif nextroom == 2:
                nextroom = room2.story_room(last_room)
                last_room = 2
            else:
                pass
playgame = Game()
playgame.start()
```




运行结果

![](https://img-blog.csdn.net/20180609175231913?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



