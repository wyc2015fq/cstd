# java回调函数 - weixin_33985507的博客 - CSDN博客
2013年09月26日 22:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

```java
protected interface Callback<E> {
		E doInCallback(Connection conn, PreparedStatement pstm, ResultSet rs)
				throws Throwable;
	}
	protected <E> E template(Callback<E> callback) throws DataAccessException {
```
```java
// 2.第一步执行
```
```java
PreparedStatement pstm = null;
		ResultSet rs = null;
		try {
```
```java
// 3.进入回调方法入口
			return callback.doInCallback(conn, pstm, rs); // 6.退出执行并取得返回值
		} catch (Throwable e) {
			throw new Exception(e);
		} finally {
```
```java
// 5.返回释放资源
			ConnectionFactory.Close(pstm, rs);
		}
	}
```
```java
//调用方法
```
```java
//执行顺序:  1.执行前
```
```java
Boolean result= template(new Callback<Boolean>() {
			@Override
			public Boolean doInCallback(Connection conn,
					PreparedStatement pstm, ResultSet rs) throws Throwable {
```
```java
// 4.执行回调方法
				pstm = conn.prepareStatement(sql);
				pstm.setObject(1, id);
				int row = pstm.executeUpdate();
				return row != 0 ? true : false;
			}
		});
```
```java
// 7. 从第6步取得返回值赋值给 result
```
