# JAVA抽奖的算概率法 - z69183787的专栏 - CSDN博客
2015年07月09日 16:13:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4761
php:
data.php处理前端发送的ajax请求，我们才用概率算法，根据设置好的中奖概率，将中奖结果以json的格式输出。关于概率计算的例子可以参照：[PHP+jQuery实现翻板抽奖](http://www.helloweba.com/view-blog-184.html)
```php
$prize_arr = array( 
    '0' => array('id'=>1,'prize'=>'平板电脑','v'=>3), 
    '1' => array('id'=>2,'prize'=>'数码相机','v'=>5), 
    '2' => array('id'=>3,'prize'=>'音箱设备','v'=>10), 
    '3' => array('id'=>4,'prize'=>'4G优盘','v'=>12), 
    '4' => array('id'=>5,'prize'=>'Q币10元','v'=>20), 
    '5' => array('id'=>6,'prize'=>'下次没准就能中哦','v'=>50), 
); 
 
foreach ($prize_arr as $key => $val) { 
    $arr[$val['id']] = $val['v']; 
} 
 
$rid = getRand($arr); //根据概率获取奖项id 
$res['msg'] = ($rid==6)?0:1; //如果为0则没中 
$res['prize'] = $prize_arr[$rid-1]['prize']; //中奖项 
echo json_encode($res); 
 
//计算概率 
function getRand($proArr) { 
    $result = ''; 
 
    //概率数组的总概率精度 
    $proSum = array_sum($proArr); 
 
    //概率数组循环 
    foreach ($proArr as $key => $proCur) { 
        $randNum = mt_rand(1, $proSum); 
        if ($randNum <= $proCur) { 
            $result = $key; 
            break; 
        } else { 
            $proSum -= $proCur; 
        } 
    } 
    unset ($proArr); 
 
    return $result; 
}
```
优先级规则使高等奖尽量在后期抽出
```
```java
import
```
```java
java.util.LinkedList;
```
```java
import
```
```java
java.util.List;
```
```java
public
```
```java
class
```
```java
GetGift {
```
```java
```
```java
// 奖品仓库
```
```java
```
```java
private
```
```java
List<Gift> gifts =
```
```java
new
```
```java
LinkedList<Gift>();
```
```java
```
```java
public
```
```java
GetGift() {
```
```java
```
```java
// 生成一堆奖品放进奖品仓库
```
```java
```
```java
// 一等奖一个优先级1，二等奖两个优先级2。。。20等奖20个优先级20
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
i =
```
```java
1
```
```java
; i <=
```
```java
20
```
```java
; i++) {
```
```java
```
```java
GiftType gt =
```
```java
new
```
```java
GiftType(i +
```
```java
"等奖"
```
```java
, i, i);
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
j =
```
```java
1
```
```java
; j <= i; j++) {
```
```java
```
```java
gifts.add(
```
```java
new
```
```java
Gift(i +
```
```java
"等奖——第"
```
```java
+ j +
```
```java
"号"
```
```java
, gt));
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
// 抽奖
```
```java
```
```java
public
```
```java
synchronized
```
```java
Gift getGift() {
```
```java
```
```java
int
```
```java
randomNumber = (
```
```java
int
```
```java
) (Math.random() * total());
```
```java
```
```java
int
```
```java
priority =
```
```java
0
```
```java
;
```
```java
```
```java
for
```
```java
(Gift g : gifts) {
```
```java
```
```java
priority += g.getType().getPriority();
```
```java
```
```java
if
```
```java
(priority >= randomNumber) {
```
```java
```
```java
// 从奖品库移出奖品
```
```java
```
```java
gifts.remove(g);
```
```java
```
```java
return
```
```java
g;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
// 抽奖次数多于奖品时，没有奖品
```
```java
```
```java
return
```
```java
null
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
/**
```
```java
```
```java
* @param args
```
```java
```
```java
*/
```
```java
```
```java
public
```
```java
static
```
```java
void
```
```java
main(String[] args) {
```
```java
```
```java
GetGift gg =
```
```java
new
```
```java
GetGift();
```
```java
```
```java
// 一共生成210个奖品，抽210次，多抽显示null
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
i =
```
```java
0
```
```java
; i <
```
```java
210
```
```java
; i++) {
```
```java
```
```java
System.out.println(gg.getGift());
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
// 计算总优先级，内部使用
```
```java
```
```java
private
```
```java
int
```
```java
total() {
```
```java
```
```java
int
```
```java
result =
```
```java
0
```
```java
;
```
```java
```
```java
for
```
```java
(Gift g : gifts) {
```
```java
```
```java
result += g.getType().getPriority();
```
```java
```
```java
}
```
```java
```
```java
return
```
```java
result;
```
```java
```
```java
}
```
```java
}
```
```java
// 记录奖品的信息
```
```java
// 如1等奖共1个，优先级为1最难抽
```
```java
class
```
```java
GiftType {
```
```java
```
```java
// 名字（如1等奖）
```
```java
```
```java
private
```
```java
String name;
```
```java
```
```java
// 这种奖品的数量，数量越大越容易抽到
```
```java
```
```java
private
```
```java
int
```
```java
quantity;
```
```java
```
```java
// 这种奖品的优先级，最小为1，数越大越容易抽到
```
```java
```
```java
private
```
```java
int
```
```java
priority;
```
```java
```
```java
public
```
```java
GiftType(String name,
```
```java
int
```
```java
quantity,
```
```java
int
```
```java
priority) {
```
```java
```
```java
this
```
```java
.name = name;
```
```java
```
```java
this
```
```java
.quantity = quantity;
```
```java
```
```java
this
```
```java
.priority = priority;
```
```java
```
```java
}
```
```java
```
```java
public
```
```java
int
```
```java
getPriority() {
```
```java
```
```java
return
```
```java
priority;
```
```java
```
```java
}
```
```java
```
```java
@Override
```
```java
```
```java
public
```
```java
String toString() {
```
```java
```
```java
return
```
```java
"GiftType [name="
```
```java
+ name +
```
```java
", quantity="
```
```java
+ quantity +
```
```java
", priority="
```
```java
+ priority +
```
```java
"]"
```
```java
;
```
```java
```
```java
}
```
```java
}
```
```java
// 奖品
```
```java
class
```
```java
Gift {
```
```java
```
```java
// 每个奖品有唯一id，抽奖不会重复，格式为"16等奖——第8号"
```
```java
```
```java
private
```
```java
String id;
```
```java
```
```java
// 这个奖品的类别
```
```java
```
```java
private
```
```java
GiftType type;
```
```java
```
```java
public
```
```java
Gift(String id, GiftType type) {
```
```java
```
```java
this
```
```java
.id = id;
```
```java
```
```java
this
```
```java
.type = type;
```
```java
```
```java
}
```
```java
```
```java
public
```
```java
GiftType getType() {
```
```java
```
```java
return
```
```java
type;
```
```java
```
```java
}
```
```java
```
```java
@Override
```
```java
```
```java
public
```
```java
String toString() {
```
```java
```
```java
return
```
```java
"Gift [id="
```
```java
+ id +
```
```java
", type="
```
```java
+ type +
```
```java
"]"
```
```java
;
```
```java
```
```java
}
```
```java
}
```
```
