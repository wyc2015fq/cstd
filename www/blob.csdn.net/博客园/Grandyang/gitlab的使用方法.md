# gitlab的使用方法 - Grandyang - 博客园







# [gitlab的使用方法](https://www.cnblogs.com/grandyang/p/4001278.html)





Git global setup: git全局建立
git config --global user.name "Your Name"
git config --global user.email "your.name@gmail.come"

Create Repository: 建立仓库
mkdir your_file
cd your_file
git init
touch README
git add README
git commit -m "first commit"
git remote add origin git@gitlab.com:yourname/your_file.git
git push -u origin master

Existing Git Repo? 已经存在git仓库
cd existing_git_repo
git remote add origin git@gitlab.com:yourname/your_file.git
git push -u origin master




Add existing repo to gitlab:
git init
touch README
git add README
git commit -m "first commit"
git remote add origin git@gitlab.com:yourname/your_file.git
git push -u origin master


If error "fatal: remote origin already exists" occurs:

Seems like remote origin is already there somehow and you want to change its uri to another value. You can't add origin since its already there but you can update it. Instead of `git remote add` command, you can run:

```
git remote set-url origin git@gitlab.com:yourname/your_file.git
```














