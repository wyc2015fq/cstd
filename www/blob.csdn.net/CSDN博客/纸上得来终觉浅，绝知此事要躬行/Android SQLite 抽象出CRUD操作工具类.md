# Android SQLite 抽象出CRUD操作工具类 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月16日 14:39:28[boonya](https://me.csdn.net/boonya)阅读数：634








1, 定义统一的Dao接口




```java
public interface IDao<T> {

	public abstract T getSingleById(int id);

	public abstract List<T> getAll();

	public abstract boolean update(T video);

	public abstract int deleteByIds(Collection<Integer> ids);

	public abstract boolean delete(T video);

	public abstract boolean add(T video);

	public int updateBySQL(String statement, String... arguments);

	public List<T> getListByFieldAndOrderBy(Map<String, Object> fieldValues,
			Map<String, Boolean> orderBy);
}
```

2, 定义抽象Dao 


```java
public abstract class AbstractDao<T> implements IDao<T> {
	private Dao<T, Integer> dao;

	public AbstractDao(Context context, Class<T> clazz) {
		try {
			dao = DBHelp.getInstance(context).getDao(clazz);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	@Override
	public T getSingleById(int id) {
		if (dao == null)
			return null;
		try {
			return dao.queryForId(id);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return null;
	}

	@Override
	public List<T> getListByFieldAndOrderBy(Map<String, Object> fieldValues,
			Map<String, Boolean> orderBy) {
		if (dao == null)
			return null;
		try {
			QueryBuilder<T, Integer> qb = dao.queryBuilder();
			if (orderBy != null) {
				for (Map.Entry<String, Boolean> entry : orderBy.entrySet()) {
					qb.orderBy(entry.getKey(), entry.getValue());
				}
			}
			if (fieldValues != null) {
				Where<T, Integer> where = qb.where();
				for (Map.Entry<String, Object> entry : fieldValues.entrySet()) {
					where.eq(entry.getKey(), entry.getValue());
				}
			}
			return qb.query();

			// return dao.queryForFieldValuesArgs(fieldValues);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return null;
	}

	@Override
	public List<T> getAll() {
		if (dao == null)
			return null;
		try {
			return dao.queryForAll();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return null;
	}

	public List<T> getAllOrderBy(String columnName, boolean ascending) {
		if (dao == null)
			return null;
		try {
			return dao.queryBuilder().orderBy(columnName, ascending).query();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return null;
	}

	@Override
	public boolean update(T t) {
		if (dao == null)
			return false;
		try {
			return dao.update(t) == 1;
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}

	public int updateBySQL(String statement, String... arguments) {
		if (dao == null)
			return 0;
		try {
			return dao.updateRaw(statement, arguments);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return 0;
	}

	@Override
	public int deleteByIds(Collection<Integer> ids) {
		if (dao == null)
			return 0;
		try {
			return dao.deleteIds(ids);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return 0;
	}

	public boolean deleteAll(String table) {
		if (dao == null)
			return false;
		try {
			return dao.executeRaw("DELETE FROM " + table) > 0;
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}

	@Override
	public boolean delete(T video) {
		if (dao == null)
			return false;
		try {
			return dao.delete(video) == 1;
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}

	@Override
	public boolean add(T t) {
		if (dao == null)
			return false;
		try {
			return dao.createIfNotExists(t) != null;
			// return dao.create(video) == 1;
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}

}
```
3, 在逻辑bean里使用很简单:



```java
public class QuestionDao extends AbstractDao<Question> {

	public QuestionDao(Context context) {
		super(context, Question.class);
	}
}
```

4, 使用的时候就很简单了:




```java
QuestionDao dao = new QuestionDao();

dao.add(T);

...
```
这样就可以很简单的做CRUD操作了....
            


