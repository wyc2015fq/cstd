
import cv2
import os
import sys
import numpy as np
import random
from PIL import Image, ImageDraw, ImageFont

ch = u'1advafsdf'
bch = ch.encode(encoding='utf-8')
print(type(bch))


sys.path.append('/test')  # 要用绝对路径
print(sys.path)  # 查看模块路径


def rand():
    return random.random()


def randi(min_int, max_int):
    lis = []
    for i in range(len(max_int)):
        lis.append(randint(min_int[i], max_int[i]))
    return tuple(lis)


def randint(min_int, max_int):
    return int(min_int + (max_int-min_int)*random.random())


def randint1(a, b):
    return random.randint(a, b)


def randint1(b):
    return random.randint(0, b)


def mkdir(path):
    isExists = os.path.exists(path)
    if not isExists:
        os.makedirs(path)


def gauss_blur(image, ksize, sigma):
    return cv2.GaussianBlur(image, (ksize, ksize), sigma, sigma)


def motion_blur(image, degree=12, angle=45):
    image = np.array(image)
    # 这里生成任意角度的运动模糊kernel的矩阵， degree越大，模糊程度越高
    M = cv2.getRotationMatrix2D((degree / 2, degree / 2), angle, 1)
    motion_blur_kernel = np.diag(np.ones(degree))
    motion_blur_kernel = cv2.warpAffine(
        motion_blur_kernel, M, (degree, degree))

    motion_blur_kernel = motion_blur_kernel / degree
    blurred = cv2.filter2D(image, -1, motion_blur_kernel)

    # convert to uint8
    cv2.normalize(blurred, blurred, 0, 255, cv2.NORM_MINMAX)
    blurred = np.array(blurred, dtype=np.uint8)
    return blurred


def add_noise(img, n):
    for i in range(n):  # 添加点噪声
        temp_x = np.random.randint(0, img.shape[0])
        temp_y = np.random.randint(0, img.shape[1])
        img[temp_x][temp_y] = 255
    return img


def odd(k):
    if (k % 2) == 0:
        return k+1
    return k


def add_erode(img, k):
    k = odd(k)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))
    img = cv2.erode(img, kernel)
    return img


def add_dilate(img, k):
    k = odd(k)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))
    img = cv2.dilate(img, kernel)
    return img


def bytes2hexstr(a_bytes):
    return ''.join(['%02x' % b for b in a_bytes])


def YUVequalizeHist(img):
    imgYUV = cv2.cvtColor(img, cv2.COLOR_BGR2YCrCb)
    channelsYUV = cv2.split(imgYUV)
    channelsYUV[0] = cv2.equalizeHist(channelsYUV[0])
    channels = cv2.merge(channelsYUV)
    result = cv2.cvtColor(channels, cv2.COLOR_YCrCb2BGR)
    return result


def HSVequalizeHist(img):
    imgHSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    channelsHSV = cv2.split(imgHSV)
    channelsHSV[2] = cv2.equalizeHist(channelsHSV[2])
    channels = cv2.merge(channelsHSV)
    result = cv2.cvtColor(channels, cv2.COLOR_HSV2BGR)
    return result


def equalizeHist(img):
    (b, g, r) = cv2.split(img)
    bH = cv2.equalizeHist(b)
    gH = cv2.equalizeHist(g)
    rH = cv2.equalizeHist(r)
    # 合并每一个通道
    result = cv2.merge((bH, gH, rH))
    return result


def circle(img, a, c, r, color):
    img2 = img.copy()
    cv.circle(img2, c, r, color, -1)
    img3 = img*(1-a) + img2*a
    return img3


cv = cv2

# 定义旋转rotate函数


def rotate(image1, angle, center=None, scale=1.0):
    k = 50
    image = cv2.copyMakeBorder(image1, k, k, k, k, cv.BORDER_REFLECT)
    # 获取图像尺寸
    (h, w) = image.shape[:2]
    # 若未指定旋转中心，则将图像中心设为旋转中心
    if center is None:
        center = (w / 2, h / 2)

    # 执行旋转
    M = cv.getRotationMatrix2D(center, angle, scale)
    rotated = cv.warpAffine(image, M, (w, h))

    # 返回旋转后的图像
    out = rotated[k:(h-k), k:(w-k)]
    return out


def add_shader(img):
    img3 = img.copy()
    img3 = rotate(img3, randint(-10, 10))
    h, w = img3.shape
    k = 20
    # img3 = img3[randint(0,k):h-k,randint(0,k):w-k]
    h, w = img3.shape
    # print(h,w)
    g = 10
    k = 255
    r = 0.5+0.5*random.random()
    img3 = circle(img3, 0.1, randi([0, 0], [w, h]), randi(
        [50], [255])[0], randi([g, g, g], [k, k, k]))
    img3 = circle(img3, 0.1, randi(
        [w*2/3, 0], [w, h]), randi([10], [100])[0], randi([g, g, g], [k, k, k]))

    r = 3+2*int(random.random()*2)
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (r, r))
    if random.random() < 0.5:
        img3 = cv.dilate(img3, kernel)
    else:
        img3 = cv.erode(img3, kernel)

    if random.random() > 0.5:
        k = 1/(1+random.random())
    else:
        k = 1+random.random()
    img3 = np.power(img3/float(np.max(img3)), k)
    return img3


# 字体 字体*.ttc的存放路径一般是： /usr/share/fonts/opentype/noto/ 查找指令locate *.ttc
if 1:
    outpath = 'E:/OCR_Line/chars'
    fontfiles = ['huawenxihei.ttf', 'fz-v4.0.ttf']
    pic_per_char = 1000
else:
    outpath = 'E:/OCR_Line/chars1'
    fontfiles = ['fz-v4.0.ttf']
    fontfiles = ['huawenxihei.ttf']
    pic_per_char = 20


def normal2(mu, left, right):
    k = np.random.normal(0, 1, 1)
    if k < 0:
        return -k*left
    else:
        return k*right


def normalint(mu, left, right):
    return int(normal2(mu, left, right))


nums = '0123456789'
numxs = '0123456789X'
ens = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
other = '`~!@#$%^&*()_+={}[]|\\\"'
fuhao='！；。，：￥／？〔｛〕｝＠＃＄％＊'
hans = '的一了是不我这在人有他来个道你大到就上着们那也说么下出地然中看时要没天过可自子能还之以去好后为都会心得里对身手起小想只而和头她无力面己如什现多样已知些声经前神笑事开点方眼发所但气法意情生两家间将很真话成让老被动当用于见回又军把此才几正战明却龙从十三长女光再向实最行高本打风进定魔给问色国同走其种年口边王怎次白全吧儿感果听啊死外体飞分主觉住少比空门因等轻直第脸候相何强张做与重快太应刚位接便量更难二名连水内西马黑兵师入部作吗虽击别放四公完灵并先士处剑算东机火微城变杀叫李带清队斯望竟云原者星山像惊呢海安族金月目加特跟场万林由日任立百使美关命理转思血乎受常离该远数物阵界平冷刻信至突随怪失许五反似找般雷武非通奇绝怕解精暗满双修度红且世半始倒今拉学落攻它传尔音指路越容石表紧冲利哥步合罗够影若伤姐亲功丝弟整息吃势花众帝早哈极青流团根静周即深形近千消结干制皇速帮性楚言备系提教令雪文必谁终站哪化破认级忙往玉底车总仙巨坐男显新斗各克急领切衣杨阳敢收围异达慢件错象服叶办保片喜活朝刀摇准兴条电拿交夜兽管华断尽决苦则八易依语闪露冰足响德记害及兰计元官顿器圣南阿嘴七告球爱敌夫留沉请威每乱况确答陈格视护淡凌脑兄狂宝识题低卫停呼司久未雨脚除孩古鬼字骑房亚代员伙缓居六装复黄呵恐爷单旁米布或跑玩报钱疑睛宇术隐欢章父差续挥句派待九持忽怒蓝佛刘丽类怀您招掉送背甚掌烈希工密细论卡退猛酒运求射另集首北照守晚赶木穿散联铁喝段需支练巴书陆角左群莫防毫民忍客线虎肯痛紫亮宫造展谢股妖激渐禁赵震秦注江取吸枪叹程简梦友继羽右念厉热份压乐靠萧既香引恶止包追寒狠仅争愿雅超刺式号跳波游狼存态炼台属嘿举妹英境灭醒胜抱毕务诉齐拍柔甲峰险娘楼院君观建殿曾逃唐置担阴凡呆眉统层银试弹否约府抓救调索苏顶基松魂图料议严顾礼业汉恩耳败顺余选皮区沙瞬秘树商纷块混贵迷充组呀示船伸舰封助肉排娜期座义升闻室推负扬亡模付芒控警默土承虚妈温凤技费著初麻舞市翻握独普考拥究玄资幸暴野称察按孙查改换箭睡母素较堂品具宗忘副状锋权洞遇丹局味划藏故仍洛历坏弄弱钟哼脱赛药胸菲谈仿爆徐坚拳营须偷骨奥据蒙尊施河透杰晶抬短政产草烦卷幽证牙维森毒摆投吴危飘熟导质胡朋喊释参休劲挡景州印惜欲摸烟幻致妙抗尼枫凝翼永盘寻冥秋智板责浪御尸辈临哦傲娇柳恨横吓硬价探宁际标墙纳田欧奈肩习朱效袋福设戏贝移轰疯科型毛碎叔冒惨扑奋治湖珠饭环咱免迎略雄凭史规伯尘躲犹距盟借骂皱床域降敬避秀挑聚徒鲜源架韩莉撞束岁罪罢哭嘛臣唯啦节惑袭食买腿训稍校姑某阻邪鲁艾裂闭诺尖残尚迅良鱼丁塔泪托洋颗炸逸晓番屋伊仇焰讲荡吼绿旧配圆迹腾假腰搞验彩勇奔颤牛恢占厅趣臂套愣端尤志剩虑妮蛋颜炮卖医舒梅厚闹归圈软弃适浮限抵职帅诸桌镇讶共广范欣病偏值专杂咬增豪触陷雾稳挺择谷附炎巧旋盯夏纪碰扫抢阶曼嗯涌抽胖祖毁岛拜潜优网案慕京项测迪谓列汗操善春烧浑赤逼伟店瞪输富懂侧率杯滚苍瑞孤抖损悄获轮孔养讨吉鸣累凶挂胆浩傻塞陪吐盖姓猜拼琳蛇幕镜材庞盗杜晃扎吟预闷仔纵伦遍爸狗愤纯彻估夺写啸莱折贼灰壁困宋画缘伏脉劫庄补泰介演悠燕摩痕沈创登含研睁婆午池暂廷逐萨凯闲旦岳戒概班误陛街跃斩牌迟谋罩膀村怜替踪霸爵曲乌瞧伴傅惧蒂嘻倍俊歌咒互漂舍采昨碧鹰犯慌糊扭丢羞判竹昏丈拔锐唤莲郎忧侍征疼伍鸟宽剧躯琴监漫席帐赞缩壮悉遭弓貌鼻喃悟忠私绕坦财辰聊掩怨耀吞融洗鼓插凉悲梁织佩笔迫豫朵召吹亦狱佣敏博赫丫茶狐仰盛劳缠忆荣净阁恭废浓磨遥缺菜粗宣躺惯喷晕遗执策脆桑绍奴努耗曹援窗灯佳积唇堆踏扶轩辛熊辉尾屁勒刹粉批乃库翔姬乖趁隆爬码赏怖丰艳拖耐岂诚刃媚慧晨律固盾洁骗社荒滑隔郭戴额旗季惹纸延侯撒颇撤霄锁仪郁痴典园抚滴跪振猎油殊启省供绪洪玛委妻忌拦仁祭泉纹蛮婚擦贴妇牢币唉庆甘宜符倾赌径钻劈堪焦姿堡馆爽矮艘薄零吕扰益奶匆墨朗袍牧耶挣递燃叛亏皆扯户砸肚拒胁柱途婷辆芳肖液巫猫岩董慰烂砍驾欺邦扔漠针醉享莎筑洲恼协盈撑椅郑眨货莹减赢虫愧尝骇康旅俩珍埋餐弯敲吻抛茫侵截悔勾喘氏肃瑟庭牵穴例舌仗哎彼柄夹玲诡笼甜济霍渊嘉霜爹宙魏脖爪哀卓奉腹抹艺泽姆昂踢秒钢瓶疾末挤劝粮核僵倩猪宏寂乘诱聪懒侠潮鸡搭鸿咐瓦弥薇肤勉川匹屈央尺鹏悍耸悬讯溜喂丧怔拾纤魄括乔晴搜掠帕亿孟诗偶琪童载袁覆悦垂均蕾厌霞诀泄宠臭肌词票构饰袖噬衡穷港航驻泥脏辞键慎眸乡逆撕凰奸催缝宿魅凑陵读岸歉授映甩唱崇瑶暖枚渡稀沿贤扩狮斥恋饶返吩捏搂咳斜煞询眯赖笨娅督洒铺齿卑浅尬尴撼促奖染酷课泛屠祸罚契弗刑络汇赐夸埃巡摔郡检疗俗雕祥葛屏予矿俱闯疲呜详录婴滋寸麽麦迈亭枝婉恒涛嫣冠昆妃翅窜矛贪绵朕俏编瘦姜斧繁允裹辱拨樱蝶瞒慈氛咕乾棋雀屑宾後晰掏啪愈措农祝驶誓铃糟捉扮畏韵冯羊咯恰驱淫璃鼠芙烁咽涯梯夕碍县扇崩裁幅愕润岚骷卢猴汤健杖辕哇鼎捕恍箱扣遮髅泡育翠豹溃麟芸铜逍涂献娃评党述饮丛汹棍序湿棒嫁灾挖奏贯冬贫串肥漆喉披跌阔丑弘昌凄卧穆焕酸坛赚割贺砰违捷井役裸筋鞭涨锦匪宴仓骚耻饿瓜肆沐妄巍宛裤驰妥嫩蠢馨桥霆勃廖龟吵审晌款戈仆诧搬漏黎蒋雯灌嘶盔辣疆桃欠饱械抑彭纠勤衫僧购弩签腥蓦揉誉呈鄙窝镖愚腕障培厂坠拢牲脾涩析铠珊尉蓉幼寺黛麒嘲怡瞳趴邀卿蜜链骤毅寨挨刷崖歪嚣频企植伐晋艰龄崔耍竞拟琼谨碗慨丘涉艇逊磊跨浴柯仑坑骄汪舱颠柜趟盒撇谅荷愁摧踩哑绑坤锤乳宰荆拓陌虹叮竖销凛访绽惠鹿沃沾靖堵挽添枯哗绩夷兔姨绳纱腐舟辅巾颈迦搏捂筹呃敖藤梵盼逝矩厨膝沌斌逗咧雁擅咦黯憾旨畅芬狸胳楞摊於冻泣灿躬哲尹淋薛墓籍嫌霉牺拱叉捧哄铭嚷痒膊塌篇歇削涵陡斤匕惶摄儒膛隶嘟寿肢伪励眠瞄辽寇沟哨掀仲逛茵档珑羡哮俘嚎卒汽墅鉴杆裙描攀蜂赔呐储污赋嘀隙吾虏呻炉辩殷嗓狞彷冤旺耿吊颊播偿俯篮溅烤贾缕俄狄擒躁狡嗔乏罕嘎蹲惩谦患渴篷咆歹迁押粒遣沸辨囊鳞版蛛蕴棠狈陶劣蜀逢祺竭潇苗娶炽戮侦讽芝吱宅甫挠邓订玫丸雍厢佐拐叠驳倭兼弦蔡腻苟昭辜坡兮础刮蓬淑梭廊俞琉胎朴遵厮韦禀妍疏浸茹滞嗦咪昊敛湾澜瞎玻溢豆灼斑犬瞥廉锅祈潘壶聂糕匠顽拂魁榜缚彪碌庙鹤妞堕昧蹄兹蔽谎遁肠焚绯腔嗡耽栋岭葬盆蹦茜壳啧妨鞋挪硕携柴矢钰钉阎帘嗤窃蔓冉俺陀沧翘兀申循搅狭侄谭渺姊贱戟津拆彦噗萝赴蚁诈暇棵愉嘱啥妾轨蚩唔浆涡汝锻禅捡衷媒渗吁巢阅孝峡烛氓哟掐租帽泊衍蓄揭妒贸寞毙憋乙喔翰坎革彤舅阱税垃圾滔倚胞寄絮虐颖潭庸喀逻璇笛歆怯瑰旭昔尧填遂祀摘寡嫂惫咚弧沫暮卜枉兆煌蛟澡雇粹栽诛滩裳蚀贞寝胧蝎诅狰桶栈睹售哧韧狙熙臀瀚署蝠舜朦泼绮溪卦栏舔筒册窥歼裴剥旷窟姚犀惕觑囚磁兜擂枕巷绣胀湘捣佑拽乞惟谱抄衙戚钦哩楠蕊垒胃颅琢贡衰喧虞丞岗慑扁骆纽噢剂侃菩剿秃罡咙桓莽菁揽烫嘘祷诞瑜鞘轴孽恕轿萍衬倦芷疚翁淘猾嗜翎陨熬藉窍掷勋咖磕鸦铸棺翩叨倏奢扳乍叙嗽辟畜擎霎倘嫉陋郝搁瘫钥哆茅峻蜘匙趋窄啡绞伺莺邱酥姥菊蛊粘恳醋绷淌炫淮渠娱丐履症捞塑蔑眩衔禽蒸炀涟傀嗖淳挟佘柏帆绰歧渔敞徽缄彬厥讪瞟茂疤霹惭瑾猿迸眶豁宵沼肿卸钩捅婢沮荐肺亨页踹戬忖斐碑芦瓷拘钧雳迭贩屡漩夭鲍儡吏捆扛曰鹫届蝉怠搓漪侮脊栗萱叱淹驼拎娟揍嗅菱槐汁惮襄蟒邻姗账樊衅砖喽瘾抿弛霓览妆澈僻挫矣妩厦喻杏桐倪噩拧燥沦芊揪铲郊蚂帖孕跺蹈冀盐蹭阮婪巅株喇襟婿鞠盏嬉凹敦膜焉跋剪呕颓妓棘桩稚逞僚谍桂嵌勿撩琦穹梧拭焱债脯掘熠朽疫拇窒吭墟顷涣镶邵忿睿峨漾梨邸酬咋奕寥呸饥蝙涅肋峙译漓凸砚剔垫瞻镰噜瞅梳熄眷胤蹬婕绘岔址坊幢蝴猩萌禹唾湛飙揣侥螺驭侣肘熏聘唬垮谊麾讥龚沛嗷箫宪镯琐髓薪毯匿裕骸匈跄瓣懈搀脂葫澎翡膨沁悸踉妲憨苹癸泻黝渣枢酿皓眈玑兢畔筷懊糖伞杭詹综缉尿嚓刁惚瑕奎棉鲸瀑践熔鹅秉傍霖鹃坟澄逮绸隋骏暧媛驯崆锥湃狩媳稻叽姻拙绚扒靡簇饼蚕叩亢赠辖斋咸碟蝇秩诏嚼卵朔胶倔罐炯冶弊讳缭戳搐驴聋禄盲蚊浊鲨唧恙懿铮佬唰镔炙阀凳啃绅锡啼厄鸭锢捶滥匣呛搔偎谜屿伽悯钓鸾煮娴桀拯蜥拣杉谣泳咔煜祟缴笙忑囔冈璐喳炭靴崎忐睬甄泓晖璧烬哽肝痞涕栖堤炒稽棱纲戾驸闺壤赦榕浇厕棚殖舵袅蔚蹙厘稷晒缪虾匀捐颂赎亵棣窘钝懦娲腮橙璋烙瘟芯榻庶茨哉劭葱貂屎悚偌溶绢苑虔飓屯吨娥殆嵩冕骼磋蠕诩旱惺殴梢拚裔婶惋萎觅峭枭砂驿昱缨叭蹿绫谐茧聆帜甸踱葡榴皙汰奄腊昼澳兑诊缜腑敷晦诵邃獠鞑辑蜡庇滕憎悻嗨慷荀咄寓筝逾昕藩啤缸忡璀噪璨祢幡茉粥悴曙聿烨靶颔吒绊憔募洽咏框酋谬萄眺钞溟痹坪挚茄凿恬邹檀褐揖冽崽蟹涎胯瘩抡铛猥菇疙斟塘徊函勺褪遏曦睫庚蒲芽荫徘裘槽鬟嘭睽煎廓稿暄滨嗒缰谕彰蛙窿锵旬珂猝掺嘈缈蟾扈耕攸嵋萤蜴栅曳瑚翱攒蜷崛惬秽芭袱缥魇磅庐鸽礁饵撅缔晏钮琅蔼苛饷葵阙掳晟佗稠懵嗣韬肴渎珀琥娉咫咻恃檐怦浦颁伶祯贷茬吝扼羌瑭隧蘑趾缀簌捺芜殃滢唠噼矫邮掂烽恺竿绒嫡辙啐煽糙戎禾畴枷怅籁瘪吆甬邢栩猖叼攘芥壕昵漱紊汀酣寐锣跷亩黏舷侈鳄靓痊溺篝惘佼娆佯瑛勘仕舆羲虬捻螂攥蛤侏媲荧霏氧漉贬巳珏骁萦桦羿迂券殇蜒骋浙蹂煦臧缎蔷煤雏孰膏坞剌忒糜蓓屹瞠牡衮哒淤祁札躏蹊胄棕矜毗俨肮酱磐呱臻鬓筱铿胴雌硝瀛鞍喋锯暑沪迢玺拈羁蜿衲箍赃伫幺剖呗恤茸钵婧贿闵饕驹辎邑巩蝗崭吮谴躇钳踌蹑噎俐刊矗湮鸠亥踞癫薰弑惦渍诫伎岌褚莞谑铐奠赘蜈诣砌锈桅邯汴旎窖椒鲲燎诶惴窦逵噔岱飕岐隘辫笃疮酌帷玮筛茱捋牟哝缇鄂鸳呦峦瞩汩蜃掣嘤楮餮邬娄瓮蟆敝啷匡顼洼阉唏孺孜烘拷鑫骰掰慵闽睨蚣龌茗冢咣飒咎憧绎靥铅喏皎楷龊黠啄粪杠鸯柬饲窑闸隼菌膳憬羔喵苔淼寰啬酝旖阖捍颍榨缅乒剁杵髯垣伢瞑壑殉诲噤瓒淬筠兖坍笈庵扉奚纶椎宦苞贲梓褒唆琶苇咂毋蜕辐盎袜蕃醇峥螳簸粟谛蹋拗橡馒陕穗邺晁藻橘恻膺弈馋佟枣遐潺啜抉痪撮筏拌霭耷帛郢匍炬娑乓纬嬴鹭驮碾砾谏竺迳娓陇沥玎胥旌墩眦篡睦咛噘阑琵咀羹嫦涧呲曜馅胭诬亘讷侬瘴昙禧丕孵腆濒剐删铳擞簧磷辗匐曝徨粼桨偃楣缤铎锭珈阐婀宓瓯魃卉踝荼槛姒吠槌鹊剃瑙皂蟠谄颌蛰刨锄浏瘸飚卤呤戍馁叟琰垠匾玟炳窈婊杞瓢嬷栓嫖榭殒烹寅礴隅譬绛蒜筐挲蛾炊迥骜猬壹靳蕉腋簪窕鸥俑垢珞芮轼潞稣沣屉纨稼簿笋鳌弼垄魑馀黔涔贽麴绾藐钗劾斓泌拄锏坝纣谧鲤汲丙篆嚏潼龇皑熹渲呓惆祠梗嚅灶圭睚茎跚袒撂螃髻犷渤滤螓赂踵糗蛆篱泯袂岑撬拴嗲晤矶靼凋碳骠渝萃晔醺馈幌赧嗬嘹荤卯麓峒羚鸨俸抠梆噌氤裆菡涸釜僖褥囤舫毓贮盅睐苜婵痉涤硫饺椭氲挛腼镭蹒渭甭瞰笺谪渥翌隗嗫怏绔鳅堑沓溯颐疵霈跤苋琬甥蚌蜻翟癖蝼啾卞咿韶唷蔬毡弋怂霁泞藕鳖掬蟑嗝晾桉咤霾撰桔炕臾哐瞌圃恸岖阂璞猊汐臌獒倡臊嗥剽俭咨笠忏秤忤啵灸蛹畿颉殛妤搪缆琛粲橱淀垛瘤鹦肾觊圳雹褂邕钛挎觎辘纺鹉恿糯鬃榆祗胚揶辄鹞杳咝潋玷豺嗵坳揄螭犸魍撵炜摁捎颦箩汜潦蚯觐肇啕芋镀坷荃翊赈淇琮濑湄骊脓郸篑粱睥犄魉齑徙柿苯骥幔阗哂罔惰辇悼宸纰裟颚旃荦蚓趄趔啻嵘讹垦暹蠡囵斡摒遢轲堰湍蘅诽魈锚袤鼾恪褶恣倨癌漳薯腴洵瑁沽掸耙邋谙犁硅迄嫔玳淅骡嘣袄谲瘠壬孪忻侨脐孱噫痰皿羯沅噶叵蔫愠淄鲛彗癞袈遑骧蜗褛镑涓褴揩镣琊炖禺瘁罂馥郦嶙潢讧夔镂酆涮怵澹孬豚桧臃迩篓偻銮偕泠掖樵徵蔻遽挞臼燧碉摞臆夙麝抒轧蒯瞿仞鄢鏖帚粤椰煊盂峋憩蓁侗阜戛庖佻匮芹蜓榄诋遨夥痣盹泱镳忪姣镐卟噙捱犒犊腺蓟荑傥馊碴洱鸢帼踮蹩跆喟倜泗闾幄濡岷鸶粽蝈闳怩佝芍谀奂冗猷箕隽堇萼忸戌筵濠珩愫倌沂狒淆燮喙诌颛摹锹檄鳍骛椁翦鹧娼弭睑漕贰槿宕砺雉鸪汶赣轶钊橇刽浣钺谗悖獗祚荥丶槟箬繇莆诠浃牒艮麋搡岈唳忾妪霰纾蚱谤呷撸狻蕙昴杈怆贻缮绉鸩砥俪彝橄攫汾膘赳鹄邈滟恁坯邝磬陲沏缙畸蹇铝舀匝佞箐阚钠泾坂腓掇驽迤氢鞅蹶讼哔嫒蘸佥炷哺垩磺钏氅僮皋逅胛遛涿糠翕呔衩诘鳗翳髦锺苓姹浒跖铄敕荔巽舶佶娣濮嗑膻懑颀豸儆貉莓仨藜厩肪圜刎秣钜蜍汨孚邂舄聒呶谒戗莜泸阪椋扪聃舐逖猡邙姝汕剜佰绶罄龅蛀焊昶沱蜚谩砝埔螟衾耘竣蛭恹谆裨赝俾氐湫俟昀滇嗳鸵蒺樽诃涪踊琏淙蹴葩矍溥荏藓潸黜浚煅笆诮谡坨淞铤绥犟楔俦桢猕菠跛鱿恫瘌诰滂葭獐饪殄佃噱醐锉崧戊砧抨牦榷跻槊醍囱祛葳谚逡鎏绺灞绡玖茁诳骓谟咩哓蚤秧馍滓釉棂塾忱町籽柩嗄锟峪夯囫碱酡痍痿绌莘殁兕刍叁奘獬锷崴纭锃酉徇蝮圉馗嶂莒膑莅豢镌胫脶腌蔺鹘痘鄱嚯碇晷绦渚殡龛荻俎碜蛎饯砣亟谔懋闩锲仝瘀稔泫濯哞乜嗟诓驷蓿桠耒糅镫貅犴褓偈溧訇媾栾槁郏葺铩蚨掾靛螯萋侩徉枰鹜褫琚蜮抻襁遴蓑皈虱皖菟觞呖邳颧溉幂阡忝椿窳貔炅舢糨鸷抟沆铣懔腩骅烩盥锴殚砀咭氮孑愎睢逦纂薜嘞狴悌猢爻牍羸撺鹑缃饽嶷蒿蝾稹钹雒仃俅狍梏橐韪囡嵇橹鳕樟轸黢徜酵桎鬣擢肓镏呙璺栉酗铀铡镁酪伥珐璎辂崂炝祉狲榔珲肛芩诿镞啮鼬岫阆檬箔貊饨蜇馄蜢镛寮辍斫蛔诨酊挝璁骢赊铨砒纛楸蟊煲爿谌赁殓聩媸掼罹廿蛐莠钨摺镝怼旮酩暨诟硭蚋赭柠絷歃黩耆蔗闱嘬茯邴缛驺訾轱祆鲧蹉遒僭谯倥孢嵬誊旯妁岬孛泖裾榈荚跎囿悒檑谥辔谶蚺嫱痂虺牯镗煨饴獾瑗闫伉祧诙胪啖钴揆苻錾鼹埠阒笞豕耄撷箸椤痔篙菀嫫蟋唁拮暝岿阕滁衢弁荇逶葆蚪喑蟀隍嘁糸茕埙獭觥晗蝌俚掴磔墒搽滦姘泅湎螫娩涝笮饬菅肱闰骞徕讫傈汞蓐徭鳝荟巯狎鲽璜铆剡猁柚耦逑捭脔汛哙铂挈绻跹琨铖炻珉癣颢裱蕤悱猞赅楹恚衽貘铢钎凼蜉粕钿涞雩鲫傩莳仄斛珙纥筅胰荜焘怄栎洌觜瘙蕲唿缳戕锝埂拊秆讦柑螈锌郗仵恽烊篁搴獍杷脍醴鹂孀邰郜纩骈堞鹬牝湟酃冼氦锒篾缦熨杌硌骝碣轾鏊覃虻膂咴缟偬暾窨鼐帏洮胱铉鲟泵竦虢椽韭佤帧蕨狯屐癯鳃苌侔腚腱圻灏痨眙竽缢辚髭尜趺茔蝰囹洇猱鼋芫赍宥樘睇酶墉嘌罅赡镊椴彀洹盱噻旄犍庹欹珥钙吡撄狃裢镙圩铬馐峄馏搠炔踟鼍仟鹌戡氯苫襦擀箴娈昃荛龈焖诤榛峤犰蟥蹼殳臬妣楫燹缱厣傣狁帔锰圄痢秸嵯樨纡澧畲瘐腭瓴蠹裣郓柞砦杼闼鞒纫芨蛉骐吖瘿笳郧黟舛钾憷稞颏佚苡勐淖澍铰茴褡诜薨濉檫郇骘凫鹗垭罨邗钯搦荠喁枋浔煸矽苕謇垓沔粑蝓躅锨瀣迓杓喈烷矾缁蜊陉堠鄯幛愀瓤蕈踯鲵丨刭濂怫罴坩焙摈氰渑疥肜厶氟鲰疟笫褰痼鲑黍沭缗菸悭疱衿氨艏裎孳庠旒栳茏阄亳卅媪艨邛麂尻樯豌踽鲈艄鲶眭陂榉氖笏廪怛蚍铙殍泶淝菘蒹忉桁疹蝣喱妊萑擘逋埚苒锗唢谰歙烯焯矬疽艟趸蹰眇荩縻亍佧叻箝趿勰嘏桫飨侪疴翥蚝鲠雎毂茑觚阋洄焓跗猗胺鹕笤跸狷钬镪馕疡篦鹈踅棼睾鹳漭烃瞽逄锍哚砬蝥钲陟泷耋酮塄痫蚧埘枸柢潆笄醮彖稗窠萘郯掮尢鲇鼙畦舸帑睃蚜溘蠃岢矸鸱狳祜囟扦枥庾菔蟛嗉擤莪觋醪倬爰蚰轳锱鬻鳏栲渌蛄嬗谘鋈阍杲柘毖禳馔鳐鳟耔辊镍侑娠槎芾豳钤锕孥綦锑洙榫疣砭筮酽愆栀耪薅蜞崤椟糁茇啁氪萁龃墀碘芪莰菏嵊蛩鞯楂缒饣乩捌毽愍瘢铋铧髋鹚儋奁樾滹穑茌骶刖嫜溏螽囝戽橛舂遄坭澶熵镬飧枞柙硎缯蔌鲂冱岙殂艉蠼匏痈鹾尕攮樗皲髡坻伧剀椐瓠讣髁龉彘棹疠薮隈龋朐橼芰圹敉暌饧泔赜黧筚蓖鸬蚬醯钪餍鸹哏甑疋膣螅锞髫宄摭檠盍绂骟棰缡苷蹁铗鞣圯涑箜芎蔟觯鍪镟夤妫熘硼秫阏飑啭枳礅耨茭钡捩柁甯苴铊寤潍牖赀醚阈柽栌肄脘酎鲳柒浠蛳壅枇楱焐秭笥茈蒌讴谵跏镆鬈怙枵痱膈卮圊圮堀忄氚皴绐蟮邡亻庑廨钭挹觳闶檗漯瓿眚芴诂郴鳓俣徼桡楗莨酰雠喾揿笊苎黥僳芏菖蓊袢馑卺垆旆曛汊硒穰粳酾鲷鹣塬曩荪莴蕞蘖怃欤玢蕖镒厝坶屣揎榧芑蓥钣鬲鳔鼗仂婺嫠愦癍癔碓箧缶蘧妗幞沤痦蛑詈钚铴锆丌岘枧柰浜蓼蛞鄣偾藿酢铟锛阊骖鲆檩燔猓蘼趵酲鹩乇囗屙揸碛笸赓鄄铱黻伲缂蜣蜩谝铍韫伛傧揠昝溽胙萏隹麈妯孓溱碚蕺锊镧髀坫崃戢瑷糌缬邶坼梃瞢缧菽豉仳堙忭恂砷筌箪艿郅鲢鳎厍哕廑敫栊癜碹磙笾篪缌苣荨诹诼钒嗍槠欷淦甏胝荞蓍蛱裉辏颞麸娌椹缫翮诒郾铯锂鞫卣圬尥槲氘瘘耩轫迨馓啉圪廴泺熳璩胗苁薹锘阌鳊唑溻瘰砗砩羝茺蓠俳刳唣悝慊瘳茼麇黾嗾嵛杩狨狺痧砜缍芡蓣顸槭溲畹痤眄糍葚郐酹镢隳鹋唛嬖怍朊涫绲胼腧葑蠓踬鞲饫刈垲嵴帻旰畚荭萸蘩颃颟鸲龠呒嗌疸硖绁耧蚴蛸裰謦跣踺逯陔凇堋屦峁廛栝磴篌芘葸薏踔鄞醢铒镲鲮仉侉桄泮燠犭皤粝蹀颡鲞唪嗪掭畈簦绨胍胲舻铘锩鲋崮庋毳箅缣菹虿谇贶陧隰僬匦垧疔肭蝤谖趑轭酐醅醑醛锬劂垅悛戥拶硷笕簟莼蒴藁诖谠谳邾陴鬏龀剞劬勖埸嬲嵝搿柝桴洧浯疝笱肽舯艋芤葙鲔鸸佾墁嫘榇煳硪礞菰蚵趱躜钼鲱匚呋媵崦楝潲獯瘵绀绋莩觫辋镡髂鬯鹨亓劓埕曷洚碲磲粜绱芄酚铪髌鲥俜刿吣垤枘氽溷甍笪胬舨芟蒽銎铵锶骱鹆岣岵怿慝挢掎毹汆沲湔甙罟羼菝讵诎谮赉郄鐾鲎啶塍屺庥掊撙柃檎毵渖砘窭肟蔸蚶蛏躞酯锾饩劢嘧嵫擐湓蝻蠖觖跬阝鳜仡垌氆氇汔篼缑舾莸蛲蠛蠲褊醣骀倮僦凵崾渫瘗箦肼腽臁舳芈荽蒉蛴袼跫遘儇埽庀戆扃搛杪浼滏瓞疃砟硗禊篥簋綮苤踣躐酴钽锸锼阃鞔鲭鹪黪黼榀榘氵浍炱煺犋砻秕籼耜肀肫肷胂莛裥逭钋钌钸髑鳆鼯佴吲帱搌擗桷氡氩溴畋硐糇腠葶袷陬黉龆刂哜埴岜徂惝扌烀眢瞵筇簏芗蝽蟪郫钕钫镉镘颥黹坜埤廾彳楦氙澌畛疳瘅砉砹碡穸缵羟膦菪虼蛘螨裼讠迕钅铈锇锔锪镓镦镱霪骒鹁冁冖哿噍埝弪攉浞潴畀籴罱羰胨胩舴苊茆茛蒇虮蠊裒襞觇轺锿髹鸫哌垸埏墼帙悃揲晡榍榱殪淠漤疬砑缲罘苈萆谂辶鄹酞钔铥镅髟鲡鲦鳇鹇鹱埒岽悫攵桊桕槔溆漶牾畎疖矧稂窀籀腈苄薷褙赙跽钐锖镎饔骣鳙丿坌宀氍沩洫澉瀹爨珧瘊瞍硇筘粢聍舁艚荸蜱钍铞铫骺鲼冂劐哳唼忮恝戋椠瘕瘛瘼祓窆窬篚粞羧脬艽葜蓰觌豇轵醵铌镄阼鞴鲅鲒埯夂恧憝搋氕洳浈濞礤聱艹蟓襻趼迮钆铷镨驵鲣鳘鸺嚆堍嶝巛廒桤橥爝甾痄癀筲箨罾耵舣荬蕹诔赆蹯钇钶铕镤鲚鲻鳢鹛塥庳怊捃揞洎涠滠猸甓瘃瞀砼礓绠耱脎脒脞萜蕻薤衤跞轹锎锓馇鲐鲩丬亠仫卩垡垴岍崞枨毪牮癃眍眵稃筢腙舭菥蜾螬螵衄谫躔郛酤钷锫镩阢鼷劁埭墚摅撖泐滗牿犏猹疰痃痖瘥磉礻箢簖缋缏耥脲膪苠茳莶蒎蒡虍螗贳軎醌醭铹铼镥阽馘鲺鹎鼽傺勹喹彡戤攴瀵灬瘭碥稆筻糈纟耠耢艴苘茚蒗蔹觏赇赕轷辁酏铑铽鲴鼢齄冫耖屮舡鲕绗彐蒈夼疒鳋螋猃荮禚'
charset = nums+ens+fuhao+hans[0:10]
charset = hans[0:3000]
charset = nums+ens+fuhao+hans[0:3000]
#charset = charset[0:10]
img_OpenCV = cv2.imread('01.jpg')

if 0:
    fontdir = 'D:/fonts/ttf/'
    fontfiles = os.listdir(fontdir)
    random.shuffle(fontfiles)
    fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    pic_per_char = 20

if 1:
    fontdir = 'D:/fonts/ttf_han/'
    fontfiles = os.listdir(fontdir)
    random.shuffle(fontfiles)
    #fontfiles = ['huawenxihei.ttf', 'fz-v4.0.ttf']
    pic_per_char = 200
    outpath = 'D:/OCR_Line/hans'

# mser = cv2.MSER_create()
index = 0
numchars=0
print(fontfiles)
labels = []
for fontfile in fontfiles:
    (shotname,extension) = os.path.splitext(fontfile)
    font = ImageFont.truetype(fontdir+fontfile, 48)
    numchars = numchars+1
    for idx, ch in enumerate(charset):
        str = ch
        # print(dir(font))
        size = font.getsize(str)
        print(ch, size)
        path = '%s/%s/%05d_%s' % (outpath, 'images', idx, ch)
        mkdir(path)
        # 需要先把输出的中文字符转换成Unicode编码形式
        # if not isinstance(str, unicode):        str = str.decode('utf8')
        cw = 50
        ch = 50
        pic_per_char2 = 100 - idx//10
        pic_per_char2 = 200
        for i in range(0, pic_per_char2):
            position = (cw, ch)
            w = cw+position[0] + size[0]
            h = ch+position[1] + size[1]
            x = randint(50, 250)+300
            y = randint(50, 300)
            # print(size)
            while 1:
                b = randint(0, 255)
                f = randint(0, 255)
                if abs(b-f)>30:
                    break

            bgColor = (b, b, b)
            fillColor = (f,f,f)
            # fillColor = (randint(0, bgColor[0]), randint(0, bgColor[1]), randint(0, bgColor[2]))
            # img_OpenCV2 = img_OpenCV[y:y+h, x:x+w]
            img_OpenCV2 = np.ones((h,w,3),dtype=np.uint8)
            cv2.rectangle(img_OpenCV2, (0, 0), (w, h), bgColor, -1)
            img_PIL = Image.fromarray(cv2.cvtColor(img_OpenCV2, cv2.COLOR_BGR2RGB))
            draw = ImageDraw.Draw(img_PIL)
            draw.text(position, str, font=font, fill=fillColor)
            img_PIL = img_PIL.rotate(randint(-15, 15))
            # print(dir(font))
            # 使用PIL中的save方法保存图片到本地
            # img_PIL.save('02.jpg', 'jpeg')

            # 转换回OpenCV格式
            # im = cv2.cvtColor(np.asarray(img_PIL), cv2.COLOR_RGB2BGR)
            im = cv2.cvtColor(np.asarray(img_PIL), cv2.COLOR_RGB2GRAY)
            k=5
            if 1:
                if rand()>0.5:
                    if b>f:
                        im = add_erode(im, randint(1,k))
                    else:
                        im = add_dilate(im, randint(1,k))
                else:
                    if b>f:
                        im = add_erode(im, randint(1,k))
                    else:
                        im = add_dilate(im, randint(1,k))

            im = add_noise(im, randint(10,50))
            if 1:
                im1 = add_shader(im)
                k=0.4
                im = im1*k + im*(1-k)
            # im = HSVequalizeHist(im)
            im = motion_blur(im, randint(2, 5), randint(0, 360))
            im = gauss_blur(im, 5, randint(5, 10))
            cw1 = cw
            cw2 = w-cw
            ch1 = ch
            ch2 = h-ch
            if 1:
                cw1 = cw1+normalint(0,-10,5)
                cw2 = cw2+normalint(0,-5,10)
                ch1 = ch1+normalint(0,-20,5)
                ch2 = ch2+normalint(0,-5,20)
            im = im[ch1:ch2, cw1:cw2]
            # gray = im
            # regions, box = mser.detectRegions(gray)
            # print(box)
            if im.shape[0]>0 and im.shape[1]>0:
                im = cv2.resize(im, (28, 28))
                # cv2.imshow("print chinese to image",img_OpenCV)
                # cv2.waitKey()
                fn = '%s/%d_%s.jpg' % (path, index, shotname)
                index = index + 1
                print(h, w, fn)
                # cv2.imwrite(fn, im)
                s = '%s;%d' % (fn, idx)
                labels.append(s)
                cv2.imencode('.jpg', im)[1].tofile(fn)


def savelines(labels, outtxt):
    f = open(outtxt, 'w')
    for s in labels:
        #print(s)
        f.write(s+'\n')
    f.close()

count = len(labels)
count_train = int(count*0.8)
count_test = count - count_train

root = outpath + '/'
outtxt = root+'list.txt'
savelines(labels[:count_train], root+'train.txt')
savelines(labels[count_train:], root+'test.txt')
