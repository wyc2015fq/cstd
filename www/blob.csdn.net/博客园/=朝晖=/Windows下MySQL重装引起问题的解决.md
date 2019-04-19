# Windows下MySQL重装引起问题的解决 - =朝晖= - 博客园
# [Windows下MySQL重装引起问题的解决](https://www.cnblogs.com/dhcn/p/7100206.html)
解决了Plugin 'InnoDB' init function returned error问题和error1405那个安全设置密码登陆问题，我个人觉得关键点在于删除C:/Documents and Settings/All Users/Application Data/MySQL/MySQL Server 5.1/目录下的数据文件，当然，我不可能在重现问题求证了，不过看此贴的人可以尝试一下只做此步是否能解决问题，当然还有删除MYSQL安装目录中的残留文件步骤也比较重要
Cite From：[http://forums.mysql.com/read.php?22,144428,250105#msg-250105](http://forums.mysql.com/read.php?22,144428,250105#msg-250105)
Re: Plugin 'InnoDB' init function returned error.
Posted by: [**Richard Lee**](http://forums.mysql.com/profile.php?22,2208130) ()
Date: February 27, 2009 10:21AM
For Windows OS: 
OK, I struggled with this for ages. There are various partial fixes that did not fix it for me on the web, but in the end the following worked: 
If you've installed it, then uninstall (an earlier version that had been removed could have left crap that is causing an issue, so best to start off clean). Do this as follows: 
- Add/Remove Programs and remove MySQL. 
- Delete the Programs File/MySQL folder (it gets left behind) 
- Delete the data file (where ever you stored it) #:/MySQL DataFiles > This is assuming you have no data to as yet to worry about!!!! 
- Run RegEdit and delete all the MySQL Keys (search and F3) and the half a dozen or so CLSIDs that coe up with just a single non-defailt key for MySQL. You can leave ADOBE/ keys that mention it if you have dreamweaver etc. As allways back up you registry first before choppig it! 
OK. Now run te install and go through the config wizard until it fails at the start up. Then close it. 
Now open up Services and open MySQL properties - I set it to run under the Administrator account rather than System/ANOther to ensure it had permissions. 
Next, go to your new MySQL DataFile and delete 'ibdata1'. 
Then go to 'C:/Documents and Settings/All Users/Application Data/MySQL/MySQL Server 5.1/data' and delete all tje 'ib_logfile' prefixed files (should be 0 and 1) - useful to notice the .err log here to which is a text file. 
Then go back to Services and manually start it and all should be fine. 
Hopefully I have saved you a few hourse peeing around with Windows - MySQL Devs, about time this bug was fixed eh? Been hanging around a long while now and the web is chok full of unanswered or half answered requests for help. Remember - some of us have no choice but to use Windows.

