# 基于hibernate的分页程序的实现（后台） - z69183787的专栏 - CSDN博客
2012年12月28日 14:33:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1544
分页类：
```java
package com.wondersgroup.workflow.page.util;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
@SuppressWarnings("serial")
public class Page implements Serializable{
	private int totalRows; 		
	private int pageSize=15;
	private int currentPage; 
	private int totalPages; 
	private int startRow; 
	public boolean hasNext=true;
	public boolean hasPrevious=false;
	
	public List<String[]> result = new ArrayList<String[]>();
	
	public Page(){
		super();
	}
	
	public Page(int _totalRows) {
		totalRows = _totalRows;
		totalPages = totalRows / pageSize;
		int mod = totalRows % pageSize;
		if (mod > 0) {
			totalPages++;
		}
		if(pageSize >= totalRows){
			hasNext=false;
			hasPrevious=false;
		}
		currentPage = 1;
		startRow = 0;
	}
	public Page(int _totalRows,int pageSize) {
		totalRows = _totalRows;
		this.pageSize = pageSize;
		totalPages = totalRows / pageSize;
		int mod = totalRows % pageSize;
		if (mod > 0) {
			totalPages++;
		}
		if(pageSize >= totalRows){
			hasNext=false;
			hasPrevious=false;
		}
		currentPage = 1;
		startRow = 0;
	}
	public int getStartRow() {
		return startRow;
	}
	public int getTotalPages() {
		return totalPages;
	}
	public int getCurrentPage() {
		return currentPage;
	}
	public int getPageSize() {
		return pageSize;
	}
	public void setTotalRows(int totalRows) {
		this.totalRows = totalRows;
	}
	public void setStartRow(int startRow) {
		this.startRow = startRow;
	}
	public void setTotalPages(int totalPages) {
		this.totalPages = totalPages;
	}
	public void setCurrentPage(int currentPage) {
		this.currentPage = currentPage;
	}
	public void setPageSize(int pageSize) {
		this.pageSize = pageSize;
	}
	public int getTotalRows() {
		return totalRows;
	}
	public void first() {
		
		currentPage = 1;
		startRow = 0;
		this.hasNext=true;
		this.hasPrevious=false;
		if(pageSize >= totalRows){
			hasNext=false;
			hasPrevious=false;
		}
	}
	public void previous() {
		if(currentPage>1){
			this.hasNext=true;
			this.hasPrevious=false;
			currentPage--;
		}else{
			this.hasNext=true;
			this.hasPrevious=true;
		}
		startRow = (currentPage - 1) * pageSize;
		if(startRow<0) startRow = 0;
	}
	public void next() {
		if(currentPage==(totalPages-1)){
			this.hasNext=false;
			this.hasPrevious=true;
		}else{
			this.hasNext=true;
			this.hasPrevious =true;
		}
		if (currentPage < totalPages) {
			currentPage++;
		}
		
		startRow = (currentPage -1) * pageSize;
	}
	public void last() {
		currentPage = totalPages;
		this.hasNext=false;
		this.hasPrevious=true;
		startRow = (currentPage - 1) * pageSize;
		if(startRow<0) startRow = 0;
		if(pageSize >= totalRows){
			hasNext=false;
			hasPrevious=false;
		}
	}
	public void refresh(int _currentPage) {
		currentPage = _currentPage;
		if(currentPage==1){
			this.hasNext=true;
			this.hasPrevious=false;
		}else if(currentPage==totalPages){
			this.hasNext=false;
			this.hasPrevious=true;
		}else{
			this.hasNext=this.hasPrevious=true;
		}
		startRow = (currentPage - 1) * pageSize;
		if(startRow<0) startRow = 0;
		if (currentPage > totalPages) {
			last();
		}
	}
	public boolean isHasNext() {
		return hasNext;
	}
	public void setHasNext(boolean hasNext) {
		this.hasNext = hasNext;
	}
	public boolean isHasPrevious() {
		return hasPrevious;
	}
	public void setHasPrevious(boolean hasPrevious) {
		this.hasPrevious = hasPrevious;
	}
}
```
为action提供 page构造方法:
```java
package com.wondersgroup.workflow.page.util;
import javax.servlet.http.HttpServletRequest;
public class PageHelp {
	public static Page getPager(HttpServletRequest request, int totalRows) {
		Page pager = new Page(totalRows);
		String currentPage = request.getParameter("currentPage");
		int cPageNo = 1;
		try{
			cPageNo = Integer.parseInt(currentPage);
			if(cPageNo<=0) cPageNo=1;
		}catch(Exception e){}
		pager.refresh(cPageNo);
		
		String pagerMethod = request.getParameter("pageMethod");
		if (pagerMethod != null) {
			if (pagerMethod.equals("first")) {
				pager.first();
			} else if (pagerMethod.equals("previous")) {
				pager.previous();
			} else if (pagerMethod.equals("next")) {
				pager.next();
			} else if (pagerMethod.equals("last")) {
				pager.last();
			} 
			/*else if (pagerMethod.equals("go")) {
				String pageCnt = request.getParameter("pageCnt");
				int pageNo = 1;
				try{
					pageNo = Integer.parseInt(pageCnt);
					if(pageNo<=0) pageNo=1;
				}catch(Exception e){}
				pager.refresh(pageNo);
			}*/
		}
		return pager;
	}
	public static Page getPager(int totalRows) {
		Page pager = new Page(totalRows);
		return pager;
	}
}
```
action中的初始方法：
```java
public void pageResult(String sql) {
		/* 记录总数 */
		int totalRows = (int) queryUtil.getPageService().countBySql(sql);
		page = PageHelp.getPager(request, totalRows);
		//int totalPages = page.getTotalPages();
		//IDs = new ArrayList<ID>();
		
		List<String[]> ps = queryUtil.getPageService().findPaginationInfo(sql, page.getStartRow(), page.getPageSize());
		page.result = ps;
		//return ps;
	}
```
dao中 每页记录以及总记录方法：
```java
package com.wondersgroup.workflow.page.dao.impl;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.annotation.Resource;
import org.hibernate.SQLQuery;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Repository;
import com.wondersgroup.workflow.page.dao.PageDao;
import com.wondersgroup.workflow.util.StringUtil;
@Repository("pageDao")
public class PageDaoImpl implements PageDao{
	private HibernateTemplate hibernateTemplate;
	public HibernateTemplate getHibernateTemplate() {
		return hibernateTemplate;
	}
	//注入hibernateTemplate
	@Resource(name="hbmtpl")
	public void setHibernateTemplate(HibernateTemplate hibernateTemplate) {
		this.hibernateTemplate = hibernateTemplate;
	}
	@SuppressWarnings("unchecked")
	@Override
	public int countBySql(String sql) {
		List list =this.hibernateTemplate.getSessionFactory().getCurrentSession().createSQLQuery(sql).list();
		return list.size();
	}
	@SuppressWarnings("unchecked")
	@Override
	public int countByHql(String hql) {
		List list = this.hibernateTemplate.getSessionFactory().getCurrentSession().createQuery(hql).list();
		return list.size();
	}
	@SuppressWarnings("unchecked")
	@Override
	public List<String[]> findPaginationInfo(String sql, int startRow, int pageSize) {
		List list = new ArrayList();
		List<String[]> finlist = new ArrayList();
		SQLQuery query  = this.hibernateTemplate.getSessionFactory().getCurrentSession().createSQLQuery(sql);
		list = query.setFirstResult(startRow).setMaxResults(pageSize).list();
		Iterator<String[]> it = list.iterator();
		while(it.hasNext()){
			Object[] objArr = (Object[])it.next();
			String[] ret = new String[objArr.length];
			for(int i=0; i<objArr.length; i++){
				ret[i] = StringUtil.getNotNullValueString(objArr[i]);
			}
			finlist.add(ret);
		}
		return finlist;
	}
	
}
```

