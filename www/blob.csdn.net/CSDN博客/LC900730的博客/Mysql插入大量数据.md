# Mysql插入大量数据 - LC900730的博客 - CSDN博客
2017年08月21日 18:47:17[lc900730](https://me.csdn.net/LC900730)阅读数：279
## 预处理插入大数据
```
//去掉最后一个逗号问题
//可根据情况调整
$chu=(int)($count/5000);//取整
$yu=$count%5000;  //余数
for($i=0;$i<$chu;$i++){
    $value='';
    for($j=$i*5000;$j<($i+1)*5000;$j++){
        //拼接values的值
        $values.='("'.$codeModel[$j]['rid'].'","'.$codeModel[$j]['cid'].'"),';
        }
        $sql="insert into w_code(rid,cid) values".$values;
        Yii::app()->db->createCommand($sql)->execute();
    }
}
//处理剩下的数据
for($k=$chu*2000;$k<$chu*2000+$yu;$k++){
    $values.='("'.$rs[$k]['taskprogressid'].'","'.$rs[$k]['pic'].'"),';
    $sql="insert into w_code(rid,cid) values".$values;
    Yii::app()->db->createCommand($sql)->execute();
}
```
