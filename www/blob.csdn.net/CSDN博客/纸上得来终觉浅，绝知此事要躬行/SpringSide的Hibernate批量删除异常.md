# SpringSide的Hibernate批量删除异常 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年03月12日 10:30:40[boonya](https://me.csdn.net/boonya)阅读数：1036








**   问题：使用SpringSide的Hibernate批量删除的时候这样写hql语句是有问题的，如：**


`DELETE ACCOUNT WHERE ID IN (?)`**    异常：参数个数越位，或超出设定长度。**



`batch delete account  parameters size out of   position`



**解决方案：动态加入不定参数的个数。**



```java
/**
	 * 批量删除账号
	 * 
	 * @param projectIds
	 *            账号编号集合
	 * @return
	 */
	public int batchDeleteAccountByIds(long[] accountIds)
	{
		int flag=0;
		try
		{
			Long [] mylong=new Long [accountIds.length];
			StringBuffer sb=new StringBuffer("DELETE Account  where id in ( ");
			for (int i = 0; i < accountIds.length; i++)
			{
				mylong[i]=new Long(accountIds[i]);
				if(i==0)
				{
					sb.append("?");
				}else
				{
					sb.append(",?");
				}
			}
			sb.append(")");
			flag=accountDao.batchExecute(sb.toString(), (Object [])mylong);
		} catch (Exception e)
		{
			Log.getLogger(ProjectDao.class).error("batch delete account error:" + e.getMessage());
		}
		return flag;
	}
```







