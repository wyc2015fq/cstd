# java通过jdbc链接数据库 - bigfacesafdasgfewgf - CSDN博客





2015年01月21日 16:23:48[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：787








**java通过jdbc链接数据库**

    1. 在数据库中建立一个表；




![](https://img-blog.csdn.net/20150121161426067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





敲入mysql语句，建表：注意primary key的设置。




![](https://img-blog.csdn.net/20150121161505083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





吐槽一下，这个表的字段真多。。。

然后我们回到java中，通过jdbc来链接这个数据表。






```java
import java.sql.Connection;  //需要import这两个jar包
import java.sql.DriverManager;  
	
	public static Connection ConnectToDB(){
		Connection conn = null; 
		String sql; 
		int result=0; 
		String url = "jdbc:mysql://192.168.1.134/crawl?useUnicode=true&characterEncoding=UTF-8";  //需要数据库url  
		String username = "zyp" ;   
		String password = "zyp123" ;
		try{
			Class.forName("com.mysql.jdbc.Driver"); // 动态加载mysql驱动
			conn = DriverManager.getConnection(url, username, password); 
			Statement stmt = conn.createStatement();
			sql = "delete from crawl"; 
			stmt.executeUpdate(sql); 
            return conn; 
		}catch (SQLException e){
			System.out.println("数据库连接失败！");
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } 

		return conn; 
	}
	
	public void imsertIntoDB(int questionIndex, String question, String questionCategory, int questionGlanceTimes, Timestamp askTime, String questionDetails, int hasAcceptedAnswer, String acceptedAnswer, int acceptedCommentTimes, int acceptedGoodTimes, int acceptedBadTimes, int acceptedResponserRank, int acceptedResponserRate, String acceptedResponserTeam, String otherAnswer, int otherCommentTimes, int otherGoodTimes, int otherBadTimes, int otherResponserRank, double scoreOfPeopleRank, double scoreOfNumWords, double scoreOfGoodTimes, double totalScore, Connection conn) throws SQLException{
		try{
			
			
			String sql; 
			sql = "insert into crawl(QuestionIndex, Question, QuestionCategory, QuestionGlanceTimes, AskTime, QuestionDetails, HasAcceptedAnswer, AcceptedAnswer, AcceptedCommentTimes, AcceptedGoodTimes, AcceptedBadTimes, AcceptedResponserRank, AcceptedResponserRate, AcceptedResponserTeam, OtherAnswer, OtherCommentTimes, OtherGoodTimes, OtherBadTimes, OtherResponserRank, ScoreOfPeopleRank, ScoreOfNumWords, ScoreOfGoodTimes, TotalScore) "
	    			+ "values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";  //字段占位
			PreparedStatement stmt = conn.prepareStatement(sql);
			stmt.setObject(1, questionIndex); //有多少个字段，就要明确每个？的含义
			stmt.setObject(2, question);
			stmt.setObject(3, questionCategory);
			stmt.setObject(4, questionGlanceTimes);
			stmt.setObject(5, askTime);
			stmt.setObject(6, questionDetails); 
			stmt.setObject(7, hasAcceptedAnswer);
			stmt.setObject(8, acceptedAnswer);
			stmt.setObject(9, acceptedCommentTimes);
			stmt.setObject(10, acceptedGoodTimes);
			stmt.setObject(11, acceptedBadTimes);
			stmt.setObject(12, acceptedResponserRank);
			stmt.setObject(13, acceptedResponserRate);
			stmt.setObject(14, acceptedResponserTeam);
			stmt.setObject(15, otherAnswer);
			stmt.setObject(16, otherCommentTimes);
			stmt.setObject(17, otherGoodTimes);
			stmt.setObject(18, otherBadTimes);
			stmt.setObject(19, otherResponserRank);
			stmt.setObject(20, scoreOfNumWords);
			stmt.setObject(21, scoreOfGoodTimes);
			stmt.setObject(22, scoreOfGoodTimes);
			stmt.setObject(23, totalScore);
			
			System.out.println(question); 
			
			int result; 
	    	result = stmt.executeUpdate(); 
		}catch(SQLException e){
			System.out.println("MySQL操作错误");
			e.printStackTrace();
			throw e; 
		}
		
	}
```



  然后在主函数中先运行connectToDB，返回connection类型的conn，给insertIotoDB传参数conn和其他字段的值，即可。

    需要import数据库的时候，直接每次都调用insertIotoDB函数即可。

    效果如下：

![](https://img-blog.csdn.net/20150121162302600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








    这样以后就会在java中链接数据库了。












