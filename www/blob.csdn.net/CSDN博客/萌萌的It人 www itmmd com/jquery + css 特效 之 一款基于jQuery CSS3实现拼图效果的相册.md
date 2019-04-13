
# jquery + css 特效 之 一款基于jQuery-CSS3实现拼图效果的相册 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月22日 13:31:24[Jlins](https://me.csdn.net/dyllove98)阅读数：2625


今天我们要来分享一款很酷的jQuery相册插件，首先相册中的图片会以一定的角度倾斜放置在页面上，点击图片缩略图就可以展开图片，并且图片是由所有缩略图拼接而成，图片展开和收拢的动画效果也非常不错。当然图片倾斜需要CSS3支持。效果图如下：
![](http://pic.w2bc.com/upload/201501/20/201501202103173034.png)

本章主要参照了如下文章进行学习
[jquery](http://www.itmmd.com/tag/jquery.html):http://www.itmmd.com/tag/jquery.html
[jQuery教程(31)-jQuery插件开发之全局函数插件](http://www.itmmd.com/201501/534.html)
[jQuery教程(30)-jQuery插件开发之自定义选择符](http://www.itmmd.com/201501/533.html)
[jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
[jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
[jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)

实现的代码。
html代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<divid="im_wrapper"class="im_wrapper"><divstyle="background-position: 0px 0px;"><imgsrc="images/thumbs/1.jpg"alt=""/></div><divstyle="background-position: -125px 0px;"><imgsrc="images/thumbs/2.jpg"alt=""/></div><divstyle="background-position: -250px 0px;"><imgsrc="images/thumbs/3.jpg"alt=""/></div><divstyle="background-position: -375px 0px;"><imgsrc="images/thumbs/4.jpg"alt=""/></div><divstyle="background-position: -500px 0px;"><imgsrc="images/thumbs/5.jpg"alt=""/></div><divstyle="background-position: -625px 0px;"><imgsrc="images/thumbs/6.jpg"alt=""/></div><divstyle="background-position: 0px -125px;"><imgsrc="images/thumbs/7.jpg"alt=""/></div><divstyle="background-position: -125px -125px;"><imgsrc="images/thumbs/8.jpg"alt=""/></div><divstyle="background-position: -250px -125px;"><imgsrc="images/thumbs/9.jpg"alt=""/></div><divstyle="background-position: -375px -125px;"><imgsrc="images/thumbs/10.jpg"alt=""/></div><divstyle="background-position: -500px -125px;"><imgsrc="images/thumbs/11.jpg"alt=""/></div><divstyle="background-position: -625px -125px;"><imgsrc="images/thumbs/12.jpg"alt=""/></div><divstyle="background-position: 0px -250px;"><imgsrc="images/thumbs/13.jpg"alt=""/></div><divstyle="background-position: -125px -250px;"><imgsrc="images/thumbs/14.jpg"alt=""/></div><divstyle="background-position: -250px -250px;"><imgsrc="images/thumbs/15.jpg"alt=""/></div><divstyle="background-position: -375px -250px;"><imgsrc="images/thumbs/16.jpg"alt=""/></div><divstyle="background-position: -500px -250px;"><imgsrc="images/thumbs/17.jpg"alt=""/></div><divstyle="background-position: -625px -250px;"><imgsrc="images/thumbs/18.jpg"alt=""/></div><divstyle="background-position: 0px -375px;"><imgsrc="images/thumbs/19.jpg"alt=""/></div><divstyle="background-position: -125px -375px;"><imgsrc="images/thumbs/20.jpg"alt=""/></div><divstyle="background-position: -250px -375px;"><imgsrc="images/thumbs/21.jpg"alt=""/></div><divstyle="background-position: -375px -375px;"><imgsrc="images/thumbs/22.jpg"alt=""/></div><divstyle="background-position: -500px -375px;"><imgsrc="images/thumbs/23.jpg"alt=""/></div><divstyle="background-position: -625px -375px;"><imgsrc="images/thumbs/24.jpg"alt=""/></div></div><divid="im_loading"class="im_loading"></div><divid="im_next"class="im_next"></div><divid="im_prev"class="im_prev"></div><div></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
js代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
(function($, sr) {vardebounce =function(func, threshold, execAsap) {vartimeout;returnfunctiondebounced() {varobj =this, args =arguments;functiondelayed() {if(!execAsap)
                            func.apply(obj, args);
                        timeout=null;
                    };if(timeout)
                        clearTimeout(timeout);elseif(execAsap)
                        func.apply(obj, args);
                    timeout= setTimeout(delayed, threshold || 100);
                };
            }//smartresizejQuery.fn[sr] =function(fn) {returnfn ?this.bind('resize', debounce(fn)) :this.trigger(sr); };
        })(jQuery,'smartresize');</script>
    <script type="text/javascript">$(function() {//check if the user made the//mistake to open it with IEvarie =false;if($.browser.msie)
                ie=true;//flag to control the click eventvarflg_click =true;//the wrappervar$im_wrapper = $('\#im_wrapper');//the thumbsvar$thumbs = $im_wrapper.children('div');//all the imagesvar$thumb_imgs = $thumbs.find('img');//number of imagesvarnmb_thumbs =$thumbs.length;//image loading statusvar$im_loading = $('\#im_loading');//the next and previous buttonsvar$im_next = $('\#im_next');var$im_prev = $('\#im_prev');//number of thumbs per linevarper_line = 6;//number of thumbs per columnvarper_col = Math.ceil(nmb_thumbs /per_line)//index of the current thumbvarcurrent = -1;//mode = grid | singlevarmode = 'grid';//an array with the positions of the thumbs//we will use it for the navigation in single modevarpositionsArray =[];for(vari = 0; i < nmb_thumbs; ++i)
                positionsArray[i]=i;//preload all the images$im_loading.show();varloaded = 0;
            $thumb_imgs.each(function() {var$this= $(this);
                $('<img/>').load(function() {++loaded;if(loaded == nmb_thumbs * 2)
                        start();
                }).attr('src', $this.attr('src'));
                $('<img/>').load(function() {++loaded;if(loaded == nmb_thumbs * 2)
                        start();
                }).attr('src', $this.attr('src').replace('/thumbs', ''));
            });//starts the animationfunctionstart() {
                $im_loading.hide();//disperse the thumbs in a griddisperse();
            }//disperses the thumbs in a grid based on windows dimentionsfunctiondisperse() {if(!flg_click)return;
                setflag();
                mode= 'grid';//center point for first thumb along the width of the windowvarspaces_w = $(window).width() / (per_line + 1);//center point for first thumb along the height of the windowvarspaces_h = $(window).height() / (per_col + 1);//let's disperse the thumbs equally on the page$thumbs.each(function(i) {var$thumb = $(this);//calculate left and top for each thumb,//considering how many we want per linevarleft = spaces_w * ((i % per_line) + 1) - $thumb.width() / 2;vartop = spaces_h * (Math.ceil((i + 1) / per_line)) - $thumb.height() / 2;//lets give a random degree to each thumbvarr = Math.floor(Math.random() * 41) - 20;/*now we animate the thumb to its final positions;
                    we also fade in its image, animate it to 115x115,
                    and remove any background image    of the thumb - this
                    is not relevant for the first time we call disperse,
                    but when changing from single to grid mode*/if(ie)varparam ={'left': left + 'px','top': top + 'px'};elsevarparam ={'left': left + 'px','top': top + 'px','rotate': r + 'deg'};
                    $thumb.stop()
                        .animate(param,700,function() {if(i == nmb_thumbs - 1)
                                setflag();
                        })
                        .find('img')
                        .fadeIn(700,function() {
                            $thumb.css({'background-image': 'none'});
                            $(this).animate({'width': '115px','height': '115px','marginTop': '5px','marginLeft': '5px'},150);
                        });
                });
            }//controls if we can click on the thumbs or not//if theres an animation in progress//we don't want the user to be able to clickfunctionsetflag() {
                flg_click= !flg_click
            }/*when we click on a thumb, we want to merge them
            and show the full image that was clicked.
            we need to animate the thumbs positions in order
            to center the final image in the screen. The
            image itself is the background image that each thumb
            will have (different background positions)
            If we are currently seeing the single image,
            then we want to disperse the thumbs again,
            and with this, showing the thumbs images.*/$thumbs.bind('click',function() {if(!flg_click)return;
                setflag();var$this= $(this);
                current= $this.index();if(mode == 'grid') {
                    mode= 'single';//the source of the full imagevarimage_src = $this.find('img').attr('src').replace('/thumbs', '');
                    $thumbs.each(function(i) {var$thumb = $(this);var$image = $thumb.find('img');//first we animate the thumb image//to fill the thumbs dimentions$image.stop().animate({'width': '100%','height': '100%','marginTop': '0px','marginLeft': '0px'},150,function() {//calculate the dimentions of the full imagevarf_w = per_line * 125;varf_h = per_col * 125;varf_l = $(window).width() / 2 - f_w / 2varf_t = $(window).height() / 2 - f_h / 2/*set the background image for the thumb
                            and animate the thumbs postions and rotation*/if(ie)varparam ={'left': f_l + (i % per_line) * 125 + 'px','top': f_t + Math.floor(i / per_line) * 125 + 'px'};elsevarparam ={'rotate': '0deg','left': f_l + (i % per_line) * 125 + 'px','top': f_t + Math.floor(i / per_line) * 125 + 'px'};
                            $thumb.css({'background-image': 'url(' + image_src + ')'}).stop()
                                .animate(param,1200,function() {//insert navigation for the single modeif(i == nmb_thumbs - 1) {
                                        addNavigation();
                                        setflag();
                                    }
                                });//fade out the thumb's image$image.fadeOut(700);
                        });
                    });
                }else{
                    setflag();//remove navigationremoveNavigation();//if we are on single mode then disperse the thumbsdisperse();
                }
            });//removes the navigation buttonsfunctionremoveNavigation() {
                $im_next.stop().animate({'right': '-50px' }, 300);
                $im_prev.stop().animate({'left': '-50px' }, 300);
            }//add the navigation buttonsfunctionaddNavigation() {
                $im_next.stop().animate({'right': '0px' }, 300);
                $im_prev.stop().animate({'left': '0px' }, 300);
            }//User clicks next button (single mode)$im_next.bind('click',function() {if(!flg_click)return;
                setflag();++current;var$next_thumb = $im_wrapper.children('div:nth-child(' + (current + 1) + ')');if($next_thumb.length > 0) {varimage_src = $next_thumb.find('img').attr('src').replace('/thumbs', '');vararr = Array.shuffle(positionsArray.slice(0));
                    $thumbs.each(function(i) {//we want to change each divs background image//on a different point of timevart = $(this);
                        setTimeout(function() {
                            t.css({'background-image': 'url(' + image_src + ')'});if(i == nmb_thumbs - 1)
                                setflag();
                        }, arr.shift()* 20);
                    });
                }else{
                    setflag();--current;return;
                }
            });//User clicks prev button (single mode)$im_prev.bind('click',function() {if(!flg_click)return;
                setflag();--current;var$prev_thumb = $im_wrapper.children('div:nth-child(' + (current + 1) + ')');if($prev_thumb.length > 0) {varimage_src = $prev_thumb.find('img').attr('src').replace('/thumbs', '');vararr = Array.shuffle(positionsArray.slice(0));
                    $thumbs.each(function(i) {vart = $(this);
                        setTimeout(function() {
                            t.css({'background-image': 'url(' + image_src + ')'});if(i == nmb_thumbs - 1)
                                setflag();
                        }, arr.shift()* 20);
                    });
                }else{
                    setflag();++current;return;
                }
            });//on windows resize call the disperse function$(window).smartresize(function() {
                removeNavigation()
                disperse();
            });//function to shuffle an arrayArray.shuffle =function(array) {for(varj, x, i =array.length; i;
                    j= parseInt(Math.random() *i),
                    x= array[--i], array[i] = array[j], array[j] =x
                );returnarray;
            };
        });![复制代码](http://common.cnblogs.com/images/copycode.gif)


