# PHP老师没教过你的那些知识点 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李虎头](http://www.jobbole.com/members/kphcdr) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**另类的写法有惊喜**
我们在阅读某些源代码的时候会发现有一种另类的写法，比如

PHP
```
//异常写法
if(false == $result)
//正常写法
if($result == false)
```
其实这是一种很聪明的写法，它可以很大程度上杜绝手误造成的严重bug。 假如我们在正常方法中把 `==` 误写成 `=` ，那么`if($result = false)` 就会恒定返回true，导致在实际运行中产生未知的Bug，如果使用异常的写法出现手误变成时`if(false = $false)` ,编辑器会抛出一个error，我们就可以及时发现并修复问题。
### ‘<?php’开始前不要有空行，php脚本结尾的 ‘?>’ 尽量省略
假如有一个文件在 `?>` 后有一个空白行，那么你在使用 `session_start()` 等方法时，会返回一个警告`headers already sent by xxx` ，这是因为 `session_start()` 要求使用前不能有任何输出。 同理 `<?php` 前的空行也会存在同样的问题。
### 字符串和数字在一起时，要留神

PHP
```
var_dump('a'.0); //返回error，语法错误
```
解决的方案比较简单，只要在`.`连接左右加上空格就可以防止语法错误。

PHP
```
var_dump ('abc' == 0); //输出true
var_dump ('1abc' == 0); //输出false
```
这个例子中输出true是不是让你大吃一惊，其实php中将字符串和数字做比较时，会把字符串强制转换为int性，转换规则为 `如果字符串是数字开头，那么取开头数字作为转换结果，如果开头不是数字那么就为0`
### 字符串的运算，你惹不起

PHP
```
for ($i = 'a'; $i <= 'z'; ++$i) {
    echo "$i ";
}
```
你以为会输出从a到z？那你就错了，亮瞎你的输出结果如下：
> 
a b c d e f g h i j k l m n o p q r s t u v w x y z aa ab ac ad ae af ag ah ai aj ak al am an ao ap aq ar as at au av aw ax ay az ba bb bc bd be bf bg bh bi bj bk bl bm bn bo bp bq br bs bt bu bv bw bx by bz ca cb cc cd ce cf cg ch ci cj ck cl cm cn co cp cq cr cs ct cu cv cw cx cy cz da db dc dd de df dg dh di dj dk dl dm dn do dp dq dr ds dt du dv dw dx dy dz ea eb ec ed ee ef eg eh ei ej ek el em en eo ep eq er es et eu ev ew ex ey ez fa fb fc fd fe ff fg fh fi fj fk fl fm fn fo fp fq fr fs ft fu fv fw fx fy fz ga gb gc gd ge gf gg gh gi gj gk gl gm gn go gp gq gr gs gt gu gv gw gx gy gz ha hb hc hd he hf hg hh hi hj hk hl hm hn ho hp hq hr hs ht hu hv hw hx hy hz ia ib ic id ie if ig ih ii ij ik il im in io ip iq ir is it iu iv iw ix iy iz ja jb jc jd je jf jg jh ji jj jk jl jm jn jo jp jq jr js jt ju jv jw jx jy jz ka kb kc kd ke kf kg kh ki kj kk kl km kn ko kp kq kr ks kt ku kv kw kx ky kz la lb lc ld le lf lg lh li lj lk ll lm ln lo lp lq lr ls lt lu lv lw lx ly lz ma mb mc md me mf mg mh mi mj mk ml mm mn mo mp mq mr ms mt mu mv mw mx my mz na nb nc nd ne nf ng nh ni nj nk nl nm nn no np nq nr ns nt nu nv nw nx ny nz oa ob oc od oe of og oh oi oj ok ol om on oo op oq or os ot ou ov ow ox oy oz pa pb pc pd pe pf pg ph pi pj pk pl pm pn po pp pq pr ps pt pu pv pw px py pz qa qb qc qd qe qf qg qh qi qj qk ql qm qn qo qp qq qr qs qt qu qv qw qx qy qz ra rb rc rd re rf rg rh ri rj rk rl rm rn ro rp rq rr rs rt ru rv rw rx ry rz sa sb sc sd se sf sg sh si sj sk sl sm sn so sp sq sr ss st su sv sw sx sy sz ta tb tc td te tf tg th ti tj tk tl tm tn to tp tq tr ts tt tu tv tw tx ty tz ua ub uc ud ue uf ug uh ui uj uk ul um un uo up uq ur us ut uu uv uw ux uy uz va vb vc vd ve vf vg vh vi vj vk vl vm vn vo vp vq vr vs vt vu vv vw vx vy vz wa wb wc wd we wf wg wh wi wj wk wl wm wn wo wp wq wr ws wt wu wv ww wx wy wz xa xb xc xd xe xf xg xh xi xj xk xl xm xn xo xp xq xr xs xt xu xv xw xx xy xz ya yb yc yd ye yf yg yh yi yj yk yl ym yn yo yp yq yr ys yt yu yv yw yx yy yz
### 时刻小心精度丢失的情况

PHP
```
$f = 0.58;
echo intval($f * 100); // 输出57
sprintf("%.2f",$f * 100); //输出58.00
```
精度丢失的问题在其他语言中有会有，但是在PHP这种弱类型的语言中更加需要注意。
其实上述说到的问题在PHP手册中都已经提及到了，手册翻百遍打码如有神，一定的细心加一定的经验，你就能够在开发的路上一路填坑一路前行。
