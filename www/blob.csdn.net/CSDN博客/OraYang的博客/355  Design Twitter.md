
# 355. Design Twitter - OraYang的博客 - CSDN博客

2018年01月08日 10:24:59[OraYang](https://me.csdn.net/u010665216)阅读数：260所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 355. Design Twitter
---题目

---Design a simplified version of Twitter where users can post tweets, follow/unfollow another user and is able to see the 10 most recent tweets in the user’s news feed. Your design should support the following methods:


---postTweet(userId, tweetId)

---: Compose a new tweet.

---getNewsFeed(userId)

---: Retrieve the 10 most recent tweet ids in the user’s news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent.

---follow(followerId, followeeId)

---: Follower follows a followee.

---unfollow(followerId, followeeId)

---: Follower unfollows a followee.


---Example:

---Twitter twitter = new Twitter();
// User 1 posts a new tweet (id = 5).
twitter.postTweet(1, 5);
// User 1's news feed should return a list with 1 tweet id -> [5].
twitter.getNewsFeed(1);
// User 1 follows user 2.
twitter.follow(1, 2);
// User 2 posts a new tweet (id = 6).
twitter.postTweet(2, 6);
// User 1's news feed should return a list with 2 tweet ids -> [6, 5].
// Tweet id 6 should precede tweet id 5 because it is posted after tweet id 5.
twitter.getNewsFeed(1);
// User 1 unfollows user 2.
twitter.unfollow(1, 2);
// User 1's news feed should return a list with 1 tweet id -> [5],
// since user 1 is no longer following user 2.
twitter.getNewsFeed(1);


---思路

---本题是要实现用户发表推特、关注用户、取消关注、获取用户本身及关注人列表发表的最近10个推特。这道题目发表推特、关注用户及取消关注这些功能实现起来都不难，但是获取用户本身及关注人列表发表的最近10个推特这个实现起来有点麻烦了。很明显，要想实现上述功能，必须要利用特殊的数据结构。在这里我们首先建立一个结构体，该结构体存储用户发表的推特id及时间id信息。其次我们利用hashtable建立用户id与关注人列表之间的映射，然后利用hashtable建立用户id与用户发表的tweets之间的信息。最后我们再利用优先队列（堆数据结构）来得到最近的10个推特。具体逐步思路请看代码实现。

---代码

---class

---Twitter {

---//声明一个Tweet结构体，用来存储用户在time时发表了id

---struct

---Tweet{

---int

---time;

---int

---id;
        Tweet(

---int

---time,

---int

---id):time(time),id(id){}
    };

---//定义两个hashmap，一个用来做用户id与Tweet数组之间的映射，一个用来做用户id与关注者id数组之间的映射

---unordered_map

---<

---int

---,

---vector

---<Tweet>

--->

---tweets;

---unordered_map

---<

---int

---,

---unordered_set

---<

---int

--->

--->

---following;

---int

---time;

---public

---:

---/** Initialize your data structure here. */

---Twitter():time(

---0

---){
    }

---/** Compose a new tweet. */

---void

---postTweet(

---int

---userId,

---int

---tweetId) {
        tweets[userId].emplace_back(time++,tweetId);
    }

---/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */

---vector

---<

---int

--->

---getNewsFeed(

---int

---userId) {

---//获取用户及其关注的用户第一次以及最后一次(最近一次)发表推特的信息,并将地址存储

---vector

---<pair<Tweet*,Tweet*>

---> h;

---//关注的用户

---for

---(

---auto

---& u:following[userId])
        {

---auto

---& t = tweets[u];

---if

---(t.size()>

---0

---)
                h.emplace_back(t.data(),t.data()+t.size()-

---1

---);
        }

---//用户自己

---auto

---& t = tweets[userId];

---if

---(t.size()>

---0

---)
            h.emplace_back(t.data(),t.data()+t.size()-

---1

---);

---//利用优先队列找到前10个发表的推特

---auto

---f = [](

---const

---pair<Tweet*,Tweet*>& x,

---const

---pair<Tweet*,Tweet*>& y){

---return

---x.second->time < y.second->time;
        };
        make_heap(h.begin(),h.end(),f);

---const

---int

---n =

---10

---;

---vector

---<

---int

--->

---res;
        res.reserve(n);

---for

---(

---int

---i=

---0

---;(i<n)&&(!h.empty());i++)
        {
            pop_heap(h.begin(),h.end(),f);

---auto

---& tmp = h.back();
            res.push_back(tmp.second->id);

---if

---(tmp.first==tmp.second--)
                h.pop_back();

---else

---push_heap(h.begin(),h.end(),f);
        }

---return

---res;
    }

---/** Follower follows a followee. If the operation is invalid, it should be a no-op. */

---void

---follow(

---int

---followerId,

---int

---followeeId) {

---if

---(followerId!=followeeId)
        {
            following[followerId].insert(followeeId);
        } 
    }

---/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */

---void

---unfollow(

---int

---followerId,

---int

---followeeId) {
        following[followerId].erase(followeeId);
    }
};

---/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter obj = new Twitter();
 * obj.postTweet(userId,tweetId);
 * vector<int> param_2 = obj.getNewsFeed(userId);
 * obj.follow(followerId,followeeId);
 * obj.unfollow(followerId,followeeId);
 */


