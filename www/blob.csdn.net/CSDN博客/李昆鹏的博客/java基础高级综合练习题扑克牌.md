
# java基础高级综合练习题扑克牌 - 李昆鹏的博客 - CSDN博客


2018年03月26日 12:18:36[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：315


**———————–java基础高级综合练习题扑克牌—————–**
题目要求：
1.创建一副有序的扑克牌，
2.随机洗牌
3.随机抽取一张牌
4.对扑克牌进行自定义排序
5.对扑克牌按牌色分组
————创建一张牌的类——————-
package cn.tx.poker;
/**
* 一张扑克牌的类
* @作者 李昆鹏
*
*/
public class APoker implements Comparable{
`/**
     * 花色ID
     */
    private int colorId;
    /**
     * 花色
     */
    private String color;
    /**
     * 扑克牌ID
     */
    private int pokerValueId;
    /**
     * 扑克牌
     */
    private String pokerValue;
    public int getColorId() {
        return colorId;
    }
    public void setColorId(int colorId) {
        this.colorId = colorId;
    }
    public String getColor() {
        return color;
    }
    public void setColor(String color) {
        this.color = color;
    }
    public int getPokerValueId() {
        return pokerValueId;
    }
    public void setPokerValueId(int pokerValueId) {
        this.pokerValueId = pokerValueId;
    }
    public String getPokerValue() {
        return pokerValue;
    }
    public void setPokerValue(String pokerValue) {
        this.pokerValue = pokerValue;
    }
    public APoker(int colorId, String color, int pokerValueId, String pokerValue) {
        super();
        this.colorId = colorId;
        this.color = color;
        this.pokerValueId = pokerValueId;
        this.pokerValue = pokerValue;
    }
    @Override
    public String toString() {
        return "\n一张扑克牌 [ 扑克牌的种类 =" + color + ","+  " 扑克牌=" + pokerValue + "]";
    }
    @Override
    public int compareTo(APoker o) {
        int val = this.colorId - o.colorId;
        if(val == 0)
            val = this.pokerValueId - o.pokerValueId;
        return val;
    }`}
—————–创建一组牌的类———————
package cn.tx.poker;
import java.util.ArrayList;
import java.util.List;
/**
* 创建一组扑克牌的类
* @作者 李昆鹏
*
*/
public class PokerGroup {
`/**
 * 组的颜色ID
 */
private int colorGroupId;
/**
 * 组的颜色
 */
private String colorGroup;
/**
 * 一组扑克牌集合
 */
private List<APoker> pokerList = new ArrayList<APoker>();
/**
 * 一组扑克牌的数量
 */
private int pokerGroupCount;
public int getColorGroupId() {
    return colorGroupId;
}
public void setColorGroupId(int colorGroupId) {
    this.colorGroupId = colorGroupId;
}
public String getColorGroup() {
    return colorGroup;
}
public void setColorGroup(String colorGroup) {
    this.colorGroup = colorGroup;
}
public List<APoker> getPokerList() {
    return pokerList;
}
public void setPokerList(List<APoker> pokerList) {
    this.pokerList = pokerList;
}
public int getPokerGroupCount() {
    return pokerGroupCount;
}
public void setPokerGroupCount(int pokerGroupCount) {
    this.pokerGroupCount = pokerGroupCount;
}
@Override
public String toString() {
    return "\nPokerGroup [ 一组牌的颜色=" + colorGroup + ", 组中的牌=" + pokerList
            +  "]";
}
public PokerGroup(int colorGroupId, String colorGroup) {
    super();
    this.colorGroupId = colorGroupId;
    this.colorGroup = colorGroup;
}`}
———————–创建一副牌的类————————
package cn.tx.poker;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
/**
* 一副扑克牌
*
* @作者 李昆鹏
*
*/
public class Pokers {
/**
* 一副牌的牌色属性
*/
public static final String[] COLORS = { “王”, “♠”, “♦”, “♣”, “♥” };
/**
* 一副牌的属性
*/
public static final String[] POKERS = { “大王”, “小王”, “A”, “2”, “3”, “4”, “5”, “6”, “7”, “8”, “9”, “10”, “J”, “Q”,
“K” };
/**
* 一张扑克牌
*/
private APoker apoker;
`/**
 * 一副扑克牌的集合
 */
private List<APoker> alist;
/**
 * 一组牌的集合
 */
private List<PokerGroup> pg;
/**
 * 一副牌的数量
 */
private int pokerCount;
public APoker getApoker() {
    return apoker;
}
public void setApoker(APoker apoker) {
    this.apoker = apoker;
}
public List<APoker> getAlist() {
    return alist;
}
public void setAlist(List<APoker> alist) {
    this.alist = alist;
}
public int getPokerCount() {
    return pokerCount;
}
public void setPokerCount(int pokerCount) {
    this.pokerCount = pokerCount;
}
/**
 * 将一副牌分组
 */
public List<PokerGroup> pokerGrouping() {
    pg = new ArrayList<PokerGroup>();
    for (APoker a : alist) {
        PokerGroup p = findGroup(a.getColorId());
        if (p == null) {
            p = new PokerGroup(a.getColorId(), a.getColor());
            p.getPokerList().add(a);
            p.setPokerGroupCount(p.getPokerList().size());
            pg.add(p);
        } else {
            p.getPokerList().add(a);
        }
    }
    return pg;
}
/**
 * 根据牌色查找对应的分组
 */
public PokerGroup findGroup(int colorId) {
    PokerGroup p = null;
    for (PokerGroup a : pg) {
        if (colorId == a.getColorGroupId())
            p = a;
    }
    return p;
}
/**
 * 创建一副扑克牌 先按花色排序，再按大小排序
 */
public List<APoker> foundPokers() {
    alist = new ArrayList<APoker>();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 15; j++) {
            if (i == 0 && j == 0 || i == 0 && j == 1) {
                APoker poker = new APoker(i, findColor(i), j, findPoker(j));
                alist.add(poker);
            } else if (i > 0 && j > 1) {
                APoker poker = new APoker(i, findColor(i), j, findPoker(j));
                alist.add(poker);
            }
        }
    }
    return alist;
}
/**
 * 对一副牌洗牌
 */
public List<APoker> shufflePoker() {
    Collections.shuffle(alist);
    return alist;
}
/**
 * 随机抽取一张扑克牌
 */
public APoker randomPoker() {
    Random r = new Random();
    return alist.get(r.nextInt(54));
}
/**
 * 对扑克牌排序
 */
public List<APoker> sortPoker() {
    Collections.sort(alist);
    return alist;
}
/**
 * 根据花色ID找花色
 */
public static String findColor(int colorID) {
    return COLORS[colorID];
}
/**
 * 根据扑克牌ID查找扑克牌
 */
public static String findPoker(int pokerID) {
    return POKERS[pokerID];
}`}
—————-测试类——————–
package cn.tx.poker;
public class Test {
`public static void main(String[] args) {
    Pokers p = new Pokers();
    //创建一副扑克牌
    p.setAlist(p.foundPokers());
    System.out.println("创建一副扑克牌");
    System.out.println(p.foundPokers());
    System.out.println("***********************************");
    //随机洗牌
    System.out.println("随机洗牌");
    System.out.println(p.shufflePoker());
    System.out.println("***********************************");
    //随机抽取一张牌
    System.out.println("随机抽取一张牌");
    System.out.println(p.randomPoker());
    System.out.println("***********************************");
    //对一副牌按花色进行排序
    System.out.println("对一副牌按花色进行排序");
    System.out.println(p.sortPoker());
    System.out.println("***********************************");
    //对扑克牌分组
    System.out.println("对扑克牌分组");
    System.out.println(p.pokerGrouping());
}`}
—结果—：
创建一副扑克牌
[
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=大王],
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=小王],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=K]]
---
随机洗牌
[
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=大王],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=小王],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=6]]
---
随机抽取一张牌
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=3]
---
对一副牌按花色进行排序
[
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=大王],
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=小王],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=K],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=K]]
---
对扑克牌分组
[
PokerGroup [ 一组牌的颜色=王, 组中的牌=[
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=大王],
一张扑克牌 [ 扑克牌的种类 =王, 扑克牌=小王]]],
PokerGroup [ 一组牌的颜色=♠, 组中的牌=[
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♠, 扑克牌=K]]],
PokerGroup [ 一组牌的颜色=♦, 组中的牌=[
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♦, 扑克牌=K]]],
PokerGroup [ 一组牌的颜色=♣, 组中的牌=[
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♣, 扑克牌=K]]],
PokerGroup [ 一组牌的颜色=♥, 组中的牌=[
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=A],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=2],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=3],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=4],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=5],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=6],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=7],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=8],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=9],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=10],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=J],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=Q],
一张扑克牌 [ 扑克牌的种类 =♥, 扑克牌=K]]]]

