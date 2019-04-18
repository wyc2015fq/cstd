# [转]SSIS Execute SQL Task : Mapping Parameters And Result Sets - weixin_33985507的博客 - CSDN博客
2014年04月29日 14:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
本文转自：[http://www.programmersedge.com/post/2013/03/05/ssis-execute-sql-task-mapping-parameters-and-result-sets.aspx#.U18_6PmSxBk](http://www.programmersedge.com/post/2013/03/05/ssis-execute-sql-task-mapping-parameters-and-result-sets.aspx#.U18_6PmSxBk)
A very common scenario in an ETL process is one in which you need to call out to some configuration tables to figure out some values that it needs to use somewhere downstream in the process. Quite often you also want to pass in some parameters as well. So in this article I will be addressing both of those scenarios. To start, you will just need to drop an Execute SQL Task item onto the Control Flow of an SSIS package and set up the initial configuration so that it is basically given a connection string
![image](http://www.programmersedge.com/pics/SQLTaskEditor1.png)
Now we want to enter in our SQL statement that will contain the parameters. In this case we are using a SQL Server connection and I will be calling a stored procedure. So I will provide ?s for each of the parameters involved. So my statement look like this…
exec Staging.usp_GetTicketQueries ?,?,?
I could also be more verbose if I wanted to
exec Staging.usp_GetTicketQueries  @StartDate=?,@EndDate=?,@SomeOtherValue=?
but it isn’t entirely necessary. Now that I have the statement ready I just need to click on the Parameter Mapping menu item on the left hand side and map my parameters.
![image](http://www.programmersedge.com/pics/SQLTaskEditor2.png)
Notice that there isn’t anything really tricky here. I line out the variables that will be used with the appropriate type and size. The only part that trips a lot of people up is the fourth one which is the Parameter Name. When you initially add an additional parameter to map this column will have a text name in it. Instead what I have done is to change these so that instead it has the 0 based index of the parameters in my statement. Once you have them mapped to the proper index value then everything should be golden.
Now we want to grab that result set that is being sent from the stored proc and map its values to some different variables. So we need to go back to the General settings screen and in the “Result Set” section change the value of the ResultSet property to something other than None in order to be able to make changes to the ResultSet screen later. In this case, I am just returning a single row. So I merely change the value to Single row and I am ready to map the result set.
![image](http://www.programmersedge.com/pics/SQLTaskEditor3.png)
Now switch to the Result Set screen via the menu on the left. Here what you will want to do is to map the actual column names of the result set to the variables that you want them to be placed into. Pretty simple. In the end, you will have something that looks like this.
![image](http://www.programmersedge.com/pics/SQLTaskEditor4.png)
Now you can click OK as you are finished with the process. This is actually a fairly handy thing to know in order to make your ETL processes a little more dynamic.
Cheers!
AJ
