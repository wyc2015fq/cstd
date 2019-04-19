# IOS Swift语言开发 tableView的重用以及自cell的自适应高度 - =朝晖= - 博客园
# [IOS Swift语言开发 tableView的重用以及自cell的自适应高度](https://www.cnblogs.com/dhcn/p/7130930.html)
http://www.aichengxu.com/[iOS](http://lib.csdn.net/base/ios)/11143168.htm
一.准备数据 （这是一个元组，第一个元素为英雄的名字;第二个元素为英雄头像图片的名字，格式为.PNG,如果为其他的格式，则需要将格式写出来；第三个元素为英雄常用说语句；第四个为英雄介绍。）
data = ([“阿狸”,”阿木木”, “艾瑞莉娅”,”锤石”, “崔斯特”,”厄运小姐”,”弗拉基米尔”, “盖伦”, “赫卡里姆”, “加里奥”,”贾克斯”, “婕拉”, “卡尔玛”,”卡萨丁”,”卡兹克”,”库奇”, “奎因”,”莫甘娜”, “纳尔”,”娜美”],
[“阿狸”,”阿木木”,”艾瑞莉娅”,”锤石”,”崔斯特”,”厄运小姐”,”弗拉基米尔”,”盖伦”,”赫卡里姆”,”加里奥”,”贾克斯”,”婕拉”,”卡尔玛”,”卡萨丁”,”卡兹克”,”库奇”,”奎因”,”莫甘娜”,”纳尔”,”娜美”],
[““慈悲是人类的奢侈之举，但也是责任所在。”——阿狸”,
““我是一只木乃伊，不喜欢说话。”——阿木木”,
““那把剑狂乱飞舞，仿佛蘸血为画。”——艾瑞莉娅”,
““一个残酷成性的幽灵收割者，以折磨生灵和死灵为乐。”——锤石”,
““未来是神秘莫测的，然而我的未来尽在卡牌之中。”——崔斯特”,
““我有两把枪，一把叫射，另一把叫..呵呵。”——厄运小姐”,
““我的孤独无人能及。”——弗拉基米尔”,
““杀敌的最好方法就是连他身边的人一起砍翻。”——盖尔”,
““嘲讽是我的最爱。”——赫卡里姆”,
““活着的新理由：为德玛西亚的意志而战。”——加里奥”,
““我告诉你——英雄联盟中爆发了灯柱形的击痕。”——贾克斯”,
““离花朵越近，就离荆棘越近。”——婕拉”,
““灵性这种东西，外人无法从你身上剥离。要明智地使用它。”——卡尔玛”,
“”虚空就是我的藏身之处。“——卡萨丁”,
““杀戮，吞噬，适应。” ——卡’兹克”,
“”他就是库奇——来自空中的死神。“——库奇”,
““大多数士兵只会依赖于他们的武器，很少有人能真正地与战友唇齿相依。”——奎因”,
“”堕落的天使。“——莫甘娜”,
“”我犯了一个可怕的错误。“——纳尔”,
“”不完成唤潮者的使命，就绝不回去。“——娜美”],
[“九尾妖狐是一个高技能伤害的英雄，她可以不断在团战边缘游走并且凭借自己短CD的技能持续输出伤害。并且漂亮的法师型刺客，声音甜美销魂，身材苗条性感，是造型控的首选之一。”,
“阿木木是联盟之中最具控制力的坦克之一。他的发挥非常依赖队友，一定要紧跟团队来发挥最大效果；他的绝望光环对于其他坦克型英雄非常克制，战斗中可以对对方生命值最高的英雄造成大量伤害。”,
“艾瑞莉娅是一个非常强的团战型英雄，可以担当战士，刺客和坦克多种身份。取得优势艾瑞莉娅可以输出成吨的伤害，而且魔法和物理的混合伤害也让对面英雄非常难处理。”,
“锤石是游戏中最强的辅助之一。有钩人的技能，普攻伤害高，大量的控制技能，在团战中大招可以作为先手技能也可以用于保护输出，技能耗蓝相对降低，在辅助英雄里属性成长较好，难以被gank，配合打野gank很容易，被动技能为吸收灵魂增加护甲和法术强度。”,
“他是一个拥有不少独特技能的AP英雄。抽牌和大招使崔斯特成为一个让人感到恐惧的英雄。魔法回复和造成更多伤害，AOE减速和中等伤害，或者眩晕加上少许伤害之间的选择，甚至能扭转战局，同样也使崔斯特成为一个多面手。”,
“其特点是跑的快、追的快，大招范围广伤害高，好补刀、好收兵，前期易形成装备压制。厄运小姐的攻速加成会在她攻击新目标后延长，从而让她可以游刃有余地将注意力在给多个坏家伙和同样多的悬赏之间来回分配，不论是对单个敌人的伤害还是在团战中的AOE伤害均能打的很足，缺点就是没有任何逃生手段，过于依赖队友的保护。”,
“弗拉基米尔拥有很强的单线能力，特别是在英雄达到9级（Q技能达到5级）后，吸血能力将异常强大。GANK和团战中，吸血鬼都有所作为，尤其是大招和其他英雄的范围伤害技能配合，能造成恐怖的伤害。弗拉基米尔的缺陷则是发育周期过长、对装备的依赖较大等等。”,
“盖伦是一个典型的战士型英雄，他有着不俗的输出能力以及防御能力，可以说是攻守兼备；但是却有着腿短和手短的缺点。盖伦是绝大部分英雄联盟玩家最先接触到的英雄，再加上简单的上手难度和帅气的造型让他深受广大玩家喜爱，成为最能代表英雄联盟的角色之一。”,
“赫卡里姆是一位全新的半肉型战士，技能设定也非常有创意。被动的额外移动速度转化为攻击注定了移速装备对于赫卡里姆的重要性，突进和大招的逆向嘲讽也让他能够轻易的出入于团战。此外，赫卡里姆自带回复技能，也让他能胜任上单与打野。”,
“加里奥是个很全面的坦克，相比其他坦克，他有AOE技能可以很轻松地清掉一波兵，同时也能带来不少伤害，又有护盾能保护己方的DPS，还有团控技能让自己在团战中能为团队的胜利奠定基础，但是由于其手短，冷却时间长，注定是不能上赛场风光。”,
“贾克斯是一个万金油型的英雄，他可以适应任何阵容。取得优势的武器可以输出成吨的伤害，而且魔法和物理的混合伤害也让对面英雄非常难处理。”,
“她有一个独特的攻击、防御系统并以控制植物做为攻击防御的手段。她可以打出多种连招，利用不同种子的相互作用造成不一样的效果。她拥有着强大的爆发能力以及持续输出能力，并且控制手段繁多，反手能力很强，所以她既能胜任常规的AP位，也能够打辅助位。但是婕拉非常脆弱，没有任何逃生能力，并且QE比较考验技能的预判。”,
“天启者卡尔玛是一个非常全能的法师。经过重制之后她可攻可守的灵活性造就了她中单、上单的优势，强大的POKE能力和RE连招可以为自己的队友带来很多优势，也很高因此不论在上路、中路还是下路都有她的一席之地。作为辅助天启者卡尔玛也可以非常优秀并且她的技能机制非常独特。”,
“卡萨丁是后期强大的AP之一，利用大招可以自由进出战场，输出高同时又很难杀死，是个十分令人头疼的英雄。但卡萨丁弱点也很明显，前期线上太过弱势，被针对克制很容易崩盘，逆风局的卡萨丁输出也会很乏力。”,
“作为一个灵活的打野刺客，卡’兹克十分擅长于进行出其不意的偷袭并且在短时间内造成成吨的伤害。另外，卡兹克独特的进化另外能够适应不同的敌对阵容并且最大化他的输出。”,
“库奇拥有极为出色的线上消耗和追杀能力，爆发能力和技能消耗不俗，独特的混伤机制加上自带削减对方双抗的技能，让他在面对任何敌人都游刃有余，缺点逆风较为无力，技能多数为魔法伤害，较为耗蓝，大招装载时间较长在长期消耗后的团战中不占优势。”,
“奎因最大的特点在于可以在两种形态之间进行切换，她自身拥有不错的减速技能和切入技能，并且爆发力也非常出色，如果操作出色，将给对手带来毁灭性的打击。”,
“莫甘娜常常活跃在中单以及辅助位上，她拥有被动技能灵魂吸取以及主动技能暗之禁锢、痛苦腐蚀、黑暗之盾、灵魂镣铐，能够对敌方英雄进行线上最大程度的消耗以及控制，非常棒的主动开团技能以及团控和团战能力，在后期团战中可以起到决定性的作用。”,
“纳尔是一个史前约德尔人，有一张毛茸茸招人喜爱的小脸，以及暴躁的脾气，在陷入极冰陷阱后，纳尔被冰封了起来，直到他的囚笼弱化之后，他才破冰而出，重返自由。在游戏中，纳尔是一个变化无常的上单英雄，可以转换成灵动的远程小型纳尔，以及凶残的近战巨型纳尔。纳尔是一个不消耗法力值的英雄，代替法力值的则是怒气值，在战斗中，他的怒气值会持续的增长。怒气系统的数值与获得的怒气值量还会根据等级变化，但是会在十一级的时候达到成长上限。当纳尔的怒气条充满，他就会变身。”,
“娜美引导着海洋的原始能量，来统御海洋神秘的恢复特性，并号令潮汐本身的狂野能量。娜美是一个非常全能的辅助英雄，软控硬控兼备，并且还有着不错的治疗能力，更重要的是，她强大的控制和减速技能能够很好的帮助队友击杀和逃生。”])
**二.在Xcode中 创建工程（使用MVC的模式）**
[code]（一）创建一个Hero的类
class Hero: NSObject {
var name:String?
var icon:String?
var maxim:String?
var content:String?
}
（二）在ViewController中
1.在viewController中定义成员变量
    var table  = UITableView()
var data:([String],[String],[String],[String])?//元祖
var nameArray:[String]?
var iconArray:[String]?
var maximArray:[String]?
var contentArray:[String]?
var heroArray:[Hero]?
2.在viewDidLoad()方法中将数据分别存在heroArray数组中
//将名字放进数组内
    self.nameArray = []
    for i in 0...19{
        //将数据中的名字放进数组内
        self.nameArray?.append(data!.0[i])
    }
    print("\(nameArray?.count)")
    //将头像放进数组内
    self.iconArray = []
    for i in 0...19{
        //将数据中的头像放进数组内
        self.iconArray?.append(data!.1[i])
    }
    print("\(iconArray?.count)")
    //将格言放进数组内
    self.maximArray = []
    for i in 0...19{
        //将数据中的格言放进数组内
        self.maximArray?.append(data!.2[i])
    }
    print("\(maximArray?.count)")
    //将介绍内容放进数组内
    self.contentArray = []
    for i in 0...19{
        //将数据中的介绍内容放进数组内
        self.contentArray?.append(data!.3[i])
    }
    print("\(contentArray?.count)")
    //将前面几个元素放进heroArray中
    self.heroArray = []
    for i in 0...((self.nameArray?.count)!-1){
        let h = Hero()
        h.name = self.nameArray![i]
        h.icon = self.iconArray![i]
        h.maxim = self.maximArray![i]
        h.content = self.contentArray![i]
        //将数据添加到heroArray数组中（记得一定要添加到数组中）
        self.heroArray?.append(h)
    }
3.创建tableView，并将其放在View中
 self.table.frame = self.view.frame
 self.view.addSubview(self.table)
4.设置代理
  （1）遵循代理UITableViewDataSource,UITableViewDelegate协议
class ViewController: UIViewController,UITableViewDataSource,UITableViewDelegate {
  （2）设置代理（在加入tableView的方法中设置）
   //设置代理
    self.table.delegate = self
    self.table.dataSource  = self
  （3）实现代理方法（主要有四个）
     //实现代理方法
     //确定tableview的分组数（分组是接在前一组的后面，只是当进入到下一组的时候，每一行的下标又会从头开始）
func numberOfSectionsInTableView(tableView: UITableView) -> Int {
    return 1
}
//确定tableView的每一组的行数
func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    print("\((heroArray?.count)!)")
    //此处为heroArry数组的元素个数
    return (heroArray?.count)!
}
  //确定cell(最关键的一个方法)
func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
//因为还没有创建cell，所以先暂时这样写，会报错 
    return cell
}
//返回每一行的高度
func tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat {
    //因为还没有创建cell，所以先暂时这样写，会报错 
}
（三）新创建一个HeroCell类继承UITableViewCell（在其中操作代码）
1.因为有四个部分所以定义四个成员变量
var imgV:UIImageView?//头像图片
var namelabel:UILabel?//英雄的名字
var content label:UILabel?//介绍英雄的内容
var maxim label:UILabel?//英雄常说语言
   2.复写一个init(style: UITableViewCellStyle, reuseIdentifier: String?),并且实例化四个对象，并且放进cell中
   注：复写该方法时会报错根据提示自动补全代码，改正错误
   自动补全的内容为：
 required init?(coder aDecoder: NSCoder) {
    fatalError("init(coder:) has not been implemented")
}
  //复写的init方法
  override init(style: UITableViewCellStyle, reuseIdentifier: String?) {
    super.init(style: style, reuseIdentifier: reuseIdentifier)
    //初始化
    self.imgV = UIImageView()
    self.namelabel = UILabel()
    self.contentlabel = UILabel()
    self.maximlabel = UILabel()
   //放入cell中
    self.addSubview(self.imgV!)
    self.addSubview(self.contentlabel!)
    self.addSubview(self.namelabel!)
    self.addSubview(self.maximlabel!)
}
3.构造一个方法来自定义cell中的内容
首先对String扩展一个方法用来计算文本的高度从而达到自动适应高度
//注：此方法用于计算文本高度，是个固定的语句，网上有很多方法可选，强烈注意:对于新手可能很多人会把这个扩展写类中，这样会报错，要写在类的外面才不会错（我就是犯了这种错）
extension String{
fun stringHeihgtWidth(fontSize:CGFloat,width:CGFloat)->CGFloat{
    let font = UIFont.systemFontOfSize(fontSize)
    let size = CGSizeMake(width,CGFloat.max)
    let paragraphStyle = NSMutableParagraphStyle()
    paragraphStyle.lineBreakMode = .ByWordWrapping
    let attributes = [NSFontAttributeName:font,NSParagraphStyleAttributeName:paragraphStyle.copy()]
    let text = self as NSString
    let rect = text.boundingRectWithSize(size,options: .UsesLineFragmentOrigin,attributes: attributes,context: nil)
    return rect.height
}
}
//自己构造的方法用于设置数据
 func setDataForCell(heroArray: [Hero],indexpath:NSIndexPath){
 //根据字体的号数和文本的宽度计算出相应文本的高度（此时会用到一个对String类扩展的一个方法，方法在前面）
 //根据取出的数据技术出来的maxim的文本高度，15为字体大小（系统默认的字体为17），self.frame.width-140为文本的宽度，可根据需求自己定义
    let maximHeight = heroArray[indexpath.row].maxim?.stringHeihgtWidth(15, width: self.frame.width-140)
   // 根据取出的数据技术出来的content的文本高度，15为字体大小（系统默认的字体为17），self.frame.width-140为文本的宽度，可根据需求自己定义
    let contentHeight = heroArray[indexpath.row].content?.stringHeihgtWidth(15, width: self.frame.width-140)
   //设置四个控件的位置（控件离各边的距离为10，空格键与控件之间的距离也为10）
    self.imgV?.frame = CGRectMake(10, 10, 100, 100)
    self.namelabel?.frame = CGRectMake(20, 120, 80, 30)
    self.maximlabel?.frame = CGRectMake(130, 10, self.frame.width-140, maximHeight!)
    self.contentlabel?.frame = CGRectMake(130, maximHeight!+20, self.frame.width-140, contentHeight!)
    //设置各控件的内容
    self.imgV?.image = UIImage(named: heroArray[indexpath.row].icon!)
    self.namelabel?.text = heroArray[indexpath.row].name
    self.namelabel?.textAlignment = .Center
    self.contentlabel?.text = heroArray[indexpath.row].content
    //设置字体大小
    self.contentlabel?.font = UIFont.systemFontOfSize(15)
    self.maximlabel?.text = heroArray[indexpath.row].maxim
    self.maximlabel?.font = UIFont.systemFontOfSize(15)
    //设置文本内容的行数，等于0，则自动适应文字内容换行
    self.maximlabel?.numberOfLines = 0
    self.contentlabel?.numberOfLines = 0
    //设置字体颜色
    self.namelabel?.textColor  = UIColor.blackColor()
    self.contentlabel?.textColor  = UIColor.blackColor()
    self.maximlabel?.textColor  = UIColor.blackColor()
}
4.设置cell的高度（自己构造的方法）
func getHeightForCell(heroArray:[Hero],indexpath:NSIndexPath)->CGFloat{
    let maximHeight = heroArray[indexpath.row].maxim?.stringHeihgtWidth(15, width: self.frame.width-140)
    let contentHeight = heroArray[indexpath.row].content?.stringHeihgtWidth(15, width: self.frame.width-140)
    //判断如果，头像和名字的高度的和大于maxim和内容的高度的和，则以头像和名字的高度的和为cell的高度
    if  170>maximHeight!+contentHeight!+30{
        return 170
    //相反maxim和内容的高度的和为cell的高度
    }else{
        return maximHeight!+contentHeight!+30
    }
}
   （四）回到ViewController中写代码
   1.在设置代理的方法中注册刚才自定义的cell（有两种注册方法）
   （1）//第一种，cell_id为一个字符窜自定义用于区分取到的是哪一个cell,HeroCell.self为自定义的HeroCell类点self
    self.table.registerClass(HeroCell.self, forCellReuseIdentifier: cell_id)
    注：此种注册方式调用HeroCell中复习的override init(style: UITableViewCellStyle, reuseIdentifier: String?) ，在该方法中执行代码
    （2）//第二种，该种注册在HeroCell中awakeFromNib()方法中执行代码（且在创将HeroCell时必须要生成Xib文件，否则报错）
 self.table.registerNib(UINib.init(nibName: "HoreCell", bundle: nil), forCellReuseIdentifier: "HoreCell_Identifier")
 上面两种任用一种即可,本篇文章使用的第一种
2.在返回cell的方法中得到cell
func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
    //从重用池中找到cell,as! HeroCell要写，否则得不到其中的控件
    let cell = tableView.dequeueReusableCellWithIdentifier(cell_id)as! HeroCell
    //使用cell调用刚才在HeroCell中构造的setDataForCell方法，进而将数据放入cell中，此时也可以直接在该方法中对cell的内容进行定义，但为了代码的间接性，所以直接在HeroCell中设置数据，当有动态改变的数据时可以再该方法中直接设置
    cell.setDataForCell(self.heroArray!, indexpath: indexPath)
    return cell
}
3.设置cell的高
//因为为了自适应高度，所以返回的为直接调用设置的cell的高度
func tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat {
    let herocell = HeroCell()
    //调用HeroCell中的getHeightForCell方法
    return herocell.getHeightForCell(self.heroArray!, indexpath: indexPath)
}
三、附上全部代码
此时代码代码已经全部写完，运行即可，可能会有一些小问题。
下面附上全部的代码：
1.HeroCell类
[code]import UIKit
class Hero: NSObject {
var name:String?
var icon:String?
var maxim:String?
var content:String?
}
2.ViewController中的代码
[code]import UIKit
class ViewController: UIViewController,UITableViewDataSource,UITableViewDelegate {
var table  = UITableView()
var data:([String],[String],[String],[String])?//元祖
var nameArray:[String]?
var iconArray:[String]?
var maximArray:[String]?
var contentArray:[String]?
var heroArray:[Hero]?
override func viewDidLoad() {
    super.viewDidLoad()
    data = (
        ["阿狸",
            "阿木木",
            "艾瑞莉娅",
            "锤石",
            "崔斯特",
            "厄运小姐",
            "弗拉基米尔",
            "盖伦",
            "赫卡里姆",
            "加里奥",
            "贾克斯",
            "婕拉",
            "卡尔玛",
            "卡萨丁",
            "卡兹克",
            "库奇",
            "奎因",
            "莫甘娜",
            "纳尔",
            "娜美"],
        ["阿狸","阿木木","艾瑞莉娅","锤石","崔斯特","厄运小姐","弗拉基米尔","盖伦","赫卡里姆","加里奥","贾克斯","婕拉","卡尔玛","卡萨丁","卡兹克","库奇","奎因","莫甘娜","纳尔","娜美"],
        ["“慈悲是人类的奢侈之举，但也是责任所在。”——阿狸",
            "“我是一只木乃伊，不喜欢说话。”——阿木木",
            "“那把剑狂乱飞舞，仿佛蘸血为画。”——艾瑞莉娅",
            "“一个残酷成性的幽灵收割者，以折磨生灵和死灵为乐。”——锤石",
            "“未来是神秘莫测的，然而我的未来尽在卡牌之中。”——崔斯特",
            "“我有两把枪，一把叫射，另一把叫..呵呵。”——厄运小姐",
            "“我的孤独无人能及。”——弗拉基米尔",
            "“杀敌的最好方法就是连他身边的人一起砍翻。”——盖尔",
            "“嘲讽是我的最爱。”——赫卡里姆",
            "“活着的新理由：为德玛西亚的意志而战。”——加里奥",
            "“我告诉你——英雄联盟中爆发了灯柱形的击痕。”——贾克斯",
            "“离花朵越近，就离荆棘越近。”——婕拉",
            "“灵性这种东西，外人无法从你身上剥离。要明智地使用它。”——卡尔玛",
            "”虚空就是我的藏身之处。“——卡萨丁",
            "“杀戮，吞噬，适应。” ——卡'兹克",
            "”他就是库奇——来自空中的死神。“——库奇",
            "“大多数士兵只会依赖于他们的武器，很少有人能真正地与战友唇齿相依。”——奎因",
            "”堕落的天使。“——莫甘娜",
            "”我犯了一个可怕的错误。“——纳尔",
            "”不完成唤潮者的使命，就绝不回去。“——娜美"],
        ["九尾妖狐是一个高技能伤害的英雄，她可以不断在团战边缘游走并且凭借自己短CD的技能持续输出伤害。并且漂亮的法师型刺客，声音甜美销魂，身材苗条性感，是造型控的首选之一。",
            "阿木木是联盟之中最具控制力的坦克之一。他的发挥非常依赖队友，一定要紧跟团队来发挥最大效果；他的绝望光环对于其他坦克型英雄非常克制，战斗中可以对对方生命值最高的英雄造成大量伤害。",
            "艾瑞莉娅是一个非常强的团战型英雄，可以担当战士，刺客和坦克多种身份。取得优势艾瑞莉娅可以输出成吨的伤害，而且魔法和物理的混合伤害也让对面英雄非常难处理。",
            "锤石是游戏中最强的辅助之一。有钩人的技能，普攻伤害高，大量的控制技能，在团战中大招可以作为先手技能也可以用于保护输出，技能耗蓝相对降低，在辅助英雄里属性成长较好，难以被gank，配合打野gank很容易，被动技能为吸收灵魂增加护甲和法术强度。",
            "他是一个拥有不少独特技能的AP英雄。抽牌和大招使崔斯特成为一个让人感到恐惧的英雄。魔法回复和造成更多伤害，AOE减速和中等伤害，或者眩晕加上少许伤害之间的选择，甚至能扭转战局，同样也使崔斯特成为一个多面手。",
            "其特点是跑的快、追的快，大招范围广伤害高，好补刀、好收兵，前期易形成装备压制。厄运小姐的攻速加成会在她攻击新目标后延长，从而让她可以游刃有余地将注意力在给多个坏家伙和同样多的悬赏之间来回分配，不论是对单个敌人的伤害还是在团战中的AOE伤害均能打的很足，缺点就是没有任何逃生手段，过于依赖队友的保护。",
            "弗拉基米尔拥有很强的单线能力，特别是在英雄达到9级（Q技能达到5级）后，吸血能力将异常强大。GANK和团战中，吸血鬼都有所作为，尤其是大招和其他英雄的范围伤害技能配合，能造成恐怖的伤害。弗拉基米尔的缺陷则是发育周期过长、对装备的依赖较大等等。",
            "盖伦是一个典型的战士型英雄，他有着不俗的输出能力以及防御能力，可以说是攻守兼备；但是却有着腿短和手短的缺点。盖伦是绝大部分英雄联盟玩家最先接触到的英雄，再加上简单的上手难度和帅气的造型让他深受广大玩家喜爱，成为最能代表英雄联盟的角色之一。",
            "赫卡里姆是一位全新的半肉型战士，技能设定也非常有创意。被动的额外移动速度转化为攻击注定了移速装备对于赫卡里姆的重要性，突进和大招的逆向嘲讽也让他能够轻易的出入于团战。此外，赫卡里姆自带回复技能，也让他能胜任上单与打野。",
            "加里奥是个很全面的坦克，相比其他坦克，他有AOE技能可以很轻松地清掉一波兵，同时也能带来不少伤害，又有护盾能保护己方的DPS，还有团控技能让自己在团战中能为团队的胜利奠定基础，但是由于其手短，冷却时间长，注定是不能上赛场风光。",
            "贾克斯是一个万金油型的英雄，他可以适应任何阵容。取得优势的武器可以输出成吨的伤害，而且魔法和物理的混合伤害也让对面英雄非常难处理。",
            "她有一个独特的攻击、防御系统并以控制植物做为攻击防御的手段。她可以打出多种连招，利用不同种子的相互作用造成不一样的效果。她拥有着强大的爆发能力以及持续输出能力，并且控制手段繁多，反手能力很强，所以她既能胜任常规的AP位，也能够打辅助位。但是婕拉非常脆弱，没有任何逃生能力，并且QE比较考验技能的预判。",
            "天启者卡尔玛是一个非常全能的法师。经过重制之后她可攻可守的灵活性造就了她中单、上单的优势，强大的POKE能力和RE连招可以为自己的队友带来很多优势，也很高因此不论在上路、中路还是下路都有她的一席之地。作为辅助天启者卡尔玛也可以非常优秀并且她的技能机制非常独特。",
            "卡萨丁是后期强大的AP之一，利用大招可以自由进出战场，输出高同时又很难杀死，是个十分令人头疼的英雄。但卡萨丁弱点也很明显，前期线上太过弱势，被针对克制很容易崩盘，逆风局的卡萨丁输出也会很乏力。",
            "作为一个灵活的打野刺客，卡'兹克十分擅长于进行出其不意的偷袭并且在短时间内造成成吨的伤害。另外，卡兹克独特的进化另外能够适应不同的敌对阵容并且最大化他的输出。",
            "库奇拥有极为出色的线上消耗和追杀能力，爆发能力和技能消耗不俗，独特的混伤机制加上自带削减对方双抗的技能，让他在面对任何敌人都游刃有余，缺点逆风较为无力，技能多数为魔法伤害，较为耗蓝，大招装载时间较长在长期消耗后的团战中不占优势。",
            "奎因最大的特点在于可以在两种形态之间进行切换，她自身拥有不错的减速技能和切入技能，并且爆发力也非常出色，如果操作出色，将给对手带来毁灭性的打击。",
            "莫甘娜常常活跃在中单以及辅助位上，她拥有被动技能灵魂吸取以及主动技能暗之禁锢、痛苦腐蚀、黑暗之盾、灵魂镣铐，能够对敌方英雄进行线上最大程度的消耗以及控制，非常棒的主动开团技能以及团控和团战能力，在后期团战中可以起到决定性的作用。",
            "纳尔是一个史前约德尔人，有一张毛茸茸招人喜爱的小脸，以及暴躁的脾气，在陷入极冰陷阱后，纳尔被冰封了起来，直到他的囚笼弱化之后，他才破冰而出，重返自由。在游戏中，纳尔是一个变化无常的上单英雄，可以转换成灵动的远程小型纳尔，以及凶残的近战巨型纳尔。纳尔是一个不消耗法力值的英雄，代替法力值的则是怒气值，在战斗中，他的怒气值会持续的增长。怒气系统的数值与获得的怒气值量还会根据等级变化，但是会在十一级的时候达到成长上限。当纳尔的怒气条充满，他就会变身。",
            "娜美引导着海洋的原始能量，来统御海洋神秘的恢复特性，并号令潮汐本身的狂野能量。娜美是一个非常全能的辅助英雄，软控硬控兼备，并且还有着不错的治疗能力，更重要的是，她强大的控制和减速技能能够很好的帮助队友击杀和逃生。"])
    print("\(data!.0.count)+\(data!.1.count)+\(data!.2.count)+\(data!.3.count)")
    //将名字放进数组内
    self.nameArray = []
    for i in 0...19{
        //将数据中的名字放进数组内
        self.nameArray?.append(data!.0[i])
    }
    print("\(nameArray?.count)")
    //将头像放进数组内
    self.iconArray = []
    for i in 0...19{
        //将数据中的头像放进数组内
        self.iconArray?.append(data!.1[i])
    }
    print("\(iconArray?.count)")
    //将格言放进数组内
    self.maximArray = []
    for i in 0...19{
        //将数据中的格言放进数组内
        self.maximArray?.append(data!.2[i])
    }
    print("\(maximArray?.count)")
    //将介绍内容放进数组内
    self.contentArray = []
    for i in 0...19{
        //将数据中的介绍内容放进数组内
        self.contentArray?.append(data!.3[i])
    }
    print("\(contentArray?.count)")
    //将前面几个元素放进heroArray中
    self.heroArray = []
    for i in 0...((self.nameArray?.count)!-1){
        let h = Hero()
        h.name = self.nameArray![i]
        h.icon = self.iconArray![i]
        h.maxim = self.maximArray![i]
        h.content = self.contentArray![i]
        //将数据添加到heroArray数组中
        self.heroArray?.append(h)
    }
    print("\(heroArray?.count)")
    UI()
}
override func didReceiveMemoryWarning() {
    super.didReceiveMemoryWarning()
    // Dispose of any resources that can be recreated.
}
let cell_id = "HeroCell_Identifier"
func UI(){
    self.table.frame = self.view.frame
    self.view.addSubview(self.table)
    //设置代理
    self.table.delegate = self
    self.table.dataSource  = self
    //注册
    self.table.registerClass(HeroCell.self, forCellReuseIdentifier: cell_id)
}
//实现代理方法
func numberOfSectionsInTableView(tableView: UITableView) -> Int {
    return 1
}
func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    print("\((heroArray?.count)!)")
    return (heroArray?.count)!
}
func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
    let cell = tableView.dequeueReusableCellWithIdentifier(cell_id)as! HeroCell
    cell.setDataForCell(self.heroArray!, indexpath: indexPath)
    print("\(self.heroArray![indexPath.row].maxim)")
    print("\(self.heroArray![indexPath.row].name)")
    print("\(self.heroArray![indexPath.row].content)")
    print("\(self.heroArray![indexPath.row].icon)")
    return cell
}
func tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat {
    let herocell = HeroCell()
    return herocell.getHeightForCell(self.heroArray!, indexpath: indexPath)
}
}
3.HeroCell中的代码
[code]import UIKit
class HeroCell: UITableViewCell {
[code]var imgV:UIImageView?
var namelabel:UILabel?
var contentlabel:UILabel?
var maximlabel:UILabel?
override func awakeFromNib() {
    super.awakeFromNib()
    // Initialization code
}
override func setSelected(selected: Bool, animated: Bool) {
    super.setSelected(selected, animated: animated)
    // Configure the view for the selected state
}
override init(style: UITableViewCellStyle, reuseIdentifier: String?) {
    super.init(style: style, reuseIdentifier: reuseIdentifier)
    //初始化
    self.imgV = UIImageView()
    self.namelabel = UILabel()
    self.contentlabel = UILabel()
    self.maximlabel = UILabel()
    self.addSubview(self.imgV!)
    self.addSubview(self.contentlabel!)
    self.addSubview(self.namelabel!)
    self.addSubview(self.maximlabel!)
}
func setDataForCell(heroArray:[Hero],indexpath:NSIndexPath){
    let maximHeight = heroArray[indexpath.row].maxim?.stringHeihgtWidth(15, width: self.frame.width-140)
    let contentHeight = heroArray[indexpath.row].content?.stringHeihgtWidth(15, width: self.frame.width-140)
    self.imgV?.frame = CGRectMake(10, 10, 100, 100)
    self.namelabel?.frame = CGRectMake(20, 120, 80, 30)
    self.maximlabel?.frame = CGRectMake(130, 10, self.frame.width-140, maximHeight!)
    self.contentlabel?.frame = CGRectMake(130, maximHeight!+20, self.frame.width-140, contentHeight!)
    self.imgV?.image = UIImage(named: heroArray[indexpath.row].icon!)
    self.namelabel?.text = heroArray[indexpath.row].name
    self.namelabel?.textAlignment = .Center
    self.contentlabel?.text = heroArray[indexpath.row].content
    self.contentlabel?.font = UIFont.systemFontOfSize(15)
    self.maximlabel?.text = heroArray[indexpath.row].maxim
    self.maximlabel?.font = UIFont.systemFontOfSize(15)
    self.maximlabel?.numberOfLines = 0
    self.contentlabel?.numberOfLines = 0
    self.namelabel?.textColor  = UIColor.blackColor()
    self.contentlabel?.textColor  = UIColor.blackColor()
    self.maximlabel?.textColor  = UIColor.blackColor()
}
func getHeightForCell(heroArray:[Hero],indexpath:NSIndexPath)->CGFloat{
    let maximHeight = heroArray[indexpath.row].maxim?.stringHeihgtWidth(15, width: self.frame.width-140)
    let contentHeight = heroArray[indexpath.row].content?.stringHeihgtWidth(15, width: self.frame.width-140)
    if  170>maximHeight!+contentHeight!{
        return 170
    }else{
        return maximHeight!+contentHeight!+30
    }
}
required init?(coder aDecoder: NSCoder) {
    fatalError("init(coder:) has not been implemented")
}
}
extension String{
func stringHeihgtWidth(fontSize:CGFloat,width:CGFloat)->CGFloat{
let font = UIFont.systemFontOfSize(fontSize)
let size = CGSizeMake(width,CGFloat.max)
let paragraphStyle = NSMutableParagraphStyle()
paragraphStyle.lineBreakMode = .ByWordWrapping
[code]    let attributes = [NSFontAttributeName:font,NSParagraphStyleAttributeName:paragraphStyle.copy()]
    let text = self as NSString
    let rect = text.boundingRectWithSize(size,options: .UsesLineFragmentOrigin,attributes: attributes,context: nil)
    return rect.height
}
}

