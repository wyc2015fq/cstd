# hibernatesupportDao 注入 sessionfactory hibernateTemplate - z69183787的专栏 - CSDN博客
2014年01月08日 12:05:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1287
@Autowired(required=false)
    public void setSessionFactory0(@Qualifier(value="sessionFactory")SessionFactory sessionFactory) {
        super.setSessionFactory(sessionFactory);
    }
