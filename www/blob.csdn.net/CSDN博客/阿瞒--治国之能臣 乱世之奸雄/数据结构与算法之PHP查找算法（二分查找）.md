# 数据结构与算法之PHP查找算法（二分查找） - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2019年03月26日 11:56:10[baofenny](https://me.csdn.net/jiaminbao)阅读数：31
个人分类：[数据结构与算法](https://blog.csdn.net/jiaminbao/article/category/8795800)
二分查找又称折半查找，只对有序的数组有效。
优点是比较次数少，查找速度快，平均性能好，占用系统内存较少；
缺点是要求待查表为有序表，且插入删除困难。
因此，折半查找方法适用于不经常变动而查找频繁的有序列表。
一、算法思想
首先，将表中间位置记录的值与查找值比较，如果两者相等，则查找成功；否则利用中间位置将表分成前、后两个子表，如果中间位置的值大于查找值，则进一步查找前一子表，否则进一步查找后一子表。
重复以上过程，直到找到满足条件的记录，使查找成功，或直到子表不存在为止，此时查找不成功。
具体做法如下：
(1) 将数组的第一个位置设置为下边界(0)。
(2) 将数组最后一个元素所在的位置设置为上边界(数组的长度减 1)。
(3) 若下边界等于或小于上边界，则做如下操作：
a. 将中点设置为floor((上边界+下边界)/2)。
b. 如果中点的元素小于查询的值，则将下边界设置为中点元素所在下标加1。
c. 如果中点的元素大于查询的值，则将上边界设置为中点元素所在下标减1。
d. 否则中点元素即为要查找的数据，可以进行返回。
二、PHP的二分查找算法实现
1、递归方式
// 数组为升序排序
function binarySearch($arr, $search) {
    $start = 0;
    $end = count($arr) - 1;
    while ($start <= $end) {
        $mid = floor(($start + $end) / 2);
        // echo "当前的中点：" . $mid;   // 用于显示每次重新计算后得到的中点
        if ($search == $arr[$mid]) {
            return $mid;
        } elseif ($search < $arr[$mid]) { // 所查值小于中间值，则在$mid左边的值中查找
            $end = $mid - 1;
        } else { // 所查值大于中间值，则在$mid右边的值中查找
            $start = $mid + 1;
        }
    }
    return -1; // 查找失败
}
2、非递归方式
// 数组为升序排序
function binSearch($arr, $start, $end, $search) {
    if ($start <= $end) {
        $mid = floor(($start + $end) / 2);
        if ($search == $arr[$mid]) {
            return $mid;
        } elseif ($search < $arr[$mid]) {
            return binSearch($arr, $start, $mid - 1, $search);
        } else {
            return binSearch($arr, $mid + 1, $end, $search);
        }
    }
    return -1;
}
**三、计算连续重复次数**
**function** countDuplicate($arr, $search) {
    $count = 0;
    $position = binarySearch($arr, $search);
**if** ($position > -1) {
        $count++;
**for** ($i = $position - 1; $i > 0; $i--) {
**if** ($arr[$i] == $search) {
                $count++;
            } **else** {
**break**;
            }
        }
**for** ($i = $position + 1; $i < count($arr); $i++) {
**if** ($arr[$i] == $search) {
                $count++;
            } **else** {
**break**;
            }
        }
    }
**return** $count;
}
// 测试
$arr = array(1, 2, 4, 4, 4, 39, 4, 4, 4, 4, 78, 33, 69);
$cnt = countDuplicate($arr, 4);
echo $cnt;    // 4
我们可以看到，示例中数组有3个连续的4和4个连续的4，而结果为4。
这是因为这个函数一开始调用 binSearch( ) 函数来查找指定的值。如果在数组中能找到这个值，这个函数就通过两个循环来统计这个值连续重复出现的次数。
第一个循环向下遍历数组，统计找到的值出现的次数，当下一个值与要查找的值不匹配时则停止计数。
第二个循环向上遍历数组，统计找到的值出现的次数，当下一个值与要查找的值不匹配时则停止计数。
