# yii2学习笔记，错误总结，持续更新 - Keith - CSDN博客





2016年02月12日 10:54:53[ke1th](https://me.csdn.net/u012436149)阅读数：1395








**服务器500错误**
- 数据库访问出现错误，查看sql语句是不是出现问题

**Either ‘name’, or ‘model’ and ‘attribute’ properties must be specified**
- 当我进行数据库的查询的时候，如果返回的内容过多，通常会使用asArray将返回的 模型进行数组化，从而减少占用内存空间， 但是如果我们要将返回的数据传递给 视图的时候，就不能数组化了，否则，就会出现这种错误

**ajax请求取不到返回数据**
- ajax是封装到 get\post请求里的， 当我们在要进行if语句判断是ajax、post、get请求的时候，要将ajax请求放在最前。

**数据更新**
- 当使用表单提交的数据进行更新数据库中的数据的时候
- 
对于表单提交的数据，不能用yii\db\ActiveRecord::load()或yii\db\ActiveRechord::attributes 进行赋值，会报错，不知为什么。 只能使用yii\db\ActiveRecord::属性名 = value,进行赋值，如下： 

` 
$session = \Yii::$app->session; 
$res_1 = Work_experience::find()->where([‘uer_id’=>$session[‘user_id’]])->orderBy(‘date_begin’)->all(); 
$source_count = Work_experience::find()->where([‘uer_id’=>$session[‘user_id’]])->orderBy(‘date_begin’)->count(); 

    if(\Yii::$app->request->isAjax){ 

        return  $source_count; 

    }elseif(\Yii::$app->request->isPost){ 
$res_2 = \Yii::$app->request->post(‘Work_experience’);
```
foreach($res_2 as $index => $value){
        if($index < $source_count){
            $res_1[$index]->date_begin = $value['date_begin'];
            $res_1[$index]->date_end = $value['date_end'];
            $res_1[$index]->company=$value['company'];
            $res_1[$index]->department=$value['department'];
            $res_1[$index]->position=$value['position'];
            $res_1[$index]->scenario='submit'; 
        }
        else {
            $res_1[$index] = new Work_experience(['scenario'=>'submit']);
            $res_1[$index]->attributes = $value;
            $res_1[$index]->uer_id = $session['user_id'];
        }
        if($res_1[$index]->validate()){
            $res_1[$index]->save();

        }
    }`
```





