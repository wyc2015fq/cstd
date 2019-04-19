# Ruby on Rails 开发笔记 - zwvista - 博客园
## [Ruby on Rails 开发笔记](https://www.cnblogs.com/zwvista/p/10331477.html)
### 安装 Ruby on Rails
[Install Rails: A step-by-step guide](http://installrails.com/)
### 创建应用
```
# 创建新的应用程序
$ rails new blog
$ cd blog
# 启动服务器
$ bin/rails server
# http://localhost:3000 可访问网页
```
### 官方示例
[Rails 入门](https://ruby-china.github.io/rails-guides/getting_started.html)
### 控制器生成器
使用 generate controller 命令（控制器生成器）来自动生成控制器（controller），视图（view） 以及动作（action）
```
# 生成 Welcome 控制器以及它的 index 动作 
$ bin/rails generate controller Welcome index
```
主要生成下面两个文件
- app/controllers/welcome_controller.rb
控制器文件
- app/views/welcome/index.html.erb
嵌入式Ruby文件
- http://localhost:3000/welcome/index
Rails 将把该请求映射为 Welcome 控制器的 index 动作
### 路由
通过修改 config/routes.rb 文件来设定路由
```
Rails.application.routes.draw do
  get 'welcome/index'
  resources :articles
  root 'welcome#index'
end
```
使用 routes 命令来确认路由
```
$ bin/rails routes
       Prefix Verb   URI Pattern                  Controller#Action
welcome_index GET    /welcome/index(.:format)     welcome#index
     articles GET    /articles(.:format)          articles#index
              POST   /articles(.:format)          articles#create
  new_article GET    /articles/new(.:format)      articles#new
 edit_article GET    /articles/:id/edit(.:format) articles#edit
      article GET    /articles/:id(.:format)      articles#show
              PATCH  /articles/:id(.:format)      articles#update
              PUT    /articles/:id(.:format)      articles#update
              DELETE /articles/:id(.:format)      articles#destroy
         root GET    /                            welcome#index
```
### Articles 控制器及其模型
```
# 生成 Articles 控制器
$ bin/rails generate controller Articles
```
使用 generate model 命令（模型生成器）来自动生成模型（model）
```
# 生成 Article 模型
$ bin/rails generate model Article title:string text:text
$ bin/rails db:migrate
```
### 控制器类代码
```
class ArticlesController < ApplicationController
  def index
    @articles = Article.all
  end
  def show
    @article = Article.find(params[:id])
  end
  def new
    @article = Article.new
  end
  def edit
    @article = Article.find(params[:id])
  end
  def create
    @article = Article.new(article_params)
    if @article.save
      redirect_to @article
    else
      render 'new'
    end
  end
  def update
    @article = Article.find(params[:id])
    if @article.update(article_params)
      redirect_to @article
    else
      render 'edit'
    end
  end
  def destroy
    @article = Article.find(params[:id])
    @article.destroy
    redirect_to articles_path
  end
  
private
  def article_params
    params.require(:article).permit(:title, :text)
  end
end
```
下面是该控制器类所包含的方法以及它们所对应的路由
- index（列表）GET /articles
- show（显示）GET /articles/:id
- new（新建）GET /articles/new
- edit（编辑）GET /articles/:id/edit
- create（创建）PATCH/POST /articles
- update（更新）PUT /articles/:id
- destroy（删除）DELETE /articles/:id
### 参考链接
[Ruby on Rails](https://rubyonrails.org/)
