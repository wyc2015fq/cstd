# java与easyUI的treegrid交互 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月15日 20:59:31[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：214


当我们从数据库中查出的数据放入了list<map<String,Object>>，然后想以树型展示在前端，我们需要将这个list集合放入一个map中，其中key是 rows 这个是固定的，value就是我们从数据库查出的list集合。其次，我们查出的数据中，有两个字段是必须存在的，一个是每一条数据的唯一标识，比如certificate_code（身份证号码）这个是不重复的，每条数据主键id也符合这个要求，其次还有一个字段为_parentId,这个是固定的，比如Father_certificate_code（父亲的身份证号码），用来说明这条数据的父级数据是谁，这两条数据提供给前端页面，用来分层次。

注意： 根节点 的 _parentId 属性必须为null，或者不传(遍历list，然后remove掉)，否则数据显示不出来

同时前端easyUI 的 treegrid有两个属性 

       idField:'id',                    //指示哪个字段是标识字段(一般用id就好)
       treeField:'certificate_code',                //定义树节点的字段

