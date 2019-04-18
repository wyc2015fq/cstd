# 使用Hibernate.initialize解决no session的延迟加载问题 - z69183787的专栏 - CSDN博客
2014年02月27日 14:29:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6558
**尝试后发现，需要在session开启的情况下使用：**
出现no session or session was closed这样的异常不多解释了，网上已经一大堆。 
但还是提不推荐把延迟加载特性去掉，而是必要时自己采取办法得到想要的对象。这样做更加灵活和效率高。虽然是避免不了一些代码量。 
之前采取的方案是在session未关闭之前，把其他表对象查询出来并且clone。不过现在无意中发现其实hibernate其实已经提供初始化的方法了，而未必自己去clone了。具体就是利用Hibernate.initialize() 这个方法。 
例子如下：
    /**
     * 获取Prescription中其他扩展属性对象。
     * 
     * @param result[struts2实现分页按钮](http://www.blogjava.net/javagrass/archive/2011/06/archive/2011/06/29/353323.html)
     */
    private void initializeProperties(PaginationSupport<Prescription> result) {
        List<Prescription> items = result.getItems();
        if (items != null && !items.isEmpty()) {
            List<Prescription> persistenceList = new ArrayList<Prescription>(
                    items.size());
            for (Prescription prescription : items) {
                Hibernate.initialize(prescription.getOffice());
                Hibernate.initialize(prescription.getDoctor());
                Hibernate.initialize(prescription.getDoctor().getOffice());
                Hibernate.initialize(prescription.getHospitalDistrict());
                Hibernate.initialize(prescription.getPatient());
                persistenceList.add(prescription);
            }
            result.setItems(persistenceList);
        }
    }
另还有种方法是采取spring的OpenSessionInViewFilter方法，把session关闭延迟到view层。不过我老是没配成功 - -  view层get扩展表对象时还是提示no session。再说了还是觉得这样的消耗有点大，因为把所有查询的session关闭时间都加大了。很多时候只是某些查询需要得到扩展表对象而已，没必 要把全部查询都影响到吧。
