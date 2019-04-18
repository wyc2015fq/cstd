# Hibernate 使用 Criteria 分页 - z69183787的专栏 - CSDN博客
2014年01月09日 15:00:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4002
```java
public Page findListByPage(HashMap<String, String> filter, String sortWay) {
    Page page = null;
    List list = new ArrayList();
    Session session = getHibernateTemplate().getSessionFactory().openSession();
    Transaction tran = session.beginTransaction();
    try {
      Criteria query = session.createCriteria(DwDataExchangeStore.class);
      if (!filter.isEmpty()) {
        for (Iterator i = filter.keySet().iterator(); i.hasNext(); )
        {
          String paramName = (String)i.next();
          if ((paramName.equalsIgnoreCase("id")) || (paramName.equalsIgnoreCase("dataType"))) {
            query.add(Restrictions.eq(paramName, filter.get(paramName)));
          }
          else if (paramName.equalsIgnoreCase("valid")) {
            query.add(Restrictions.eq(paramName, Long.valueOf(Long.parseLong((String)filter.get(paramName)))));
          }
          else if (paramName.contains("Time")) {
            SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
            String fieldName = paramName.substring(0, paramName.length() - 1);
            String sDate = (String)filter.get(paramName);
            boolean isStartTime = paramName.endsWith("s");
            if (isStartTime)
              query.add(Restrictions.ge(fieldName, dateFormat.parse(sDate + " 00:00:00")));
            else {
              query.add(Restrictions.le(fieldName, dateFormat.parse(sDate + " 23:59:59")));
            }
          }
          else if (paramName.contains("Date")) {
            String fieldName = paramName.substring(0, paramName.length() - 1);
            String sDate = (String)filter.get(paramName);
            boolean isStartTime = paramName.endsWith("s");
            if (isStartTime)
              query.add(Restrictions.ge(fieldName, sDate));
            else {
              query.add(Restrictions.le(fieldName, sDate));
            }
          }
          else if ((!paramName.equalsIgnoreCase("pageNo")) && (!paramName.equalsIgnoreCase("pageSize"))) {
            query.add(Restrictions.like(paramName, (String)filter.get(paramName), MatchMode.ANYWHERE));
          }
        }
      }
      if (sortWay.equalsIgnoreCase("asc"))
        query.addOrder(Order.asc("createTime"));
      else {
        query.addOrder(Order.desc("createTime"));
      }
      int totalCount = ((Integer)query.setProjection(Projections.rowCount()).uniqueResult()).intValue();
      query.setProjection(null);
      query.setResultTransformer(CriteriaSpecification.ROOT_ENTITY);
      int pageNo = 0;
      int pageSize = 0;
      if ((filter.get("pageNo") != null) && (Integer.parseInt((String)filter.get("pageNo")) > 0)) {
        pageNo = Integer.parseInt((String)filter.get("pageNo"));
        pageSize = filter.get("pageSize") == null ? 10 : Integer.parseInt((String)filter.get("pageSize"));
        query.setFirstResult(pageSize * (pageNo - 1));
        query.setMaxResults(pageSize);
      }
      list = query.list();
      if (pageNo == 0)
        page = new Page(0, pageSize, totalCount, pageSize, list);
      else {
        page = new Page(pageSize * (pageNo - 1), pageSize, totalCount, pageSize, list);
      }
      page.setResult(list);
      tran.commit();
    } catch (Exception e) {
      tran.rollback();
      e.printStackTrace();
    } finally {
      session.flush();
      session.close();
    }
    return page;
  }
```
