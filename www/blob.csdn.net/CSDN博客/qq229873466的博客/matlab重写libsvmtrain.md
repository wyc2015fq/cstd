# matlab重写libsvmtrain - qq229873466的博客 - CSDN博客

2017年03月18日 20:00:51[qq229873466](https://me.csdn.net/qq229873466)阅读数：225


就挑出libsvm中关于svm_c的核心部分重写，其实就是B集的选择和梯度迭代。

```
function [ w, b ] = svm_train( data )
%% global
global Cp;
global Cn;
global Q;
global grab;
global alpha;
global y;
%% parameters
Cp = 5;
Cn = 5;
%%
x = data(:, 2:end);
y = data(:, 1);
L = length(y);

alpha = zeros(L, 1);
%%
Q = (y* y') .* (x * x');
p = -1*ones(L, 1);
grab = p;

iter = 0;
max_iter = 1e6;
while iter < max_iter
    [i, j, flag] = select_B;
    if flag == 1
       break; 
    end

    iter = iter + 1;
    old_alpha_B = [alpha(i);alpha(j)];
    update_alpha(i, j);
    delta_B = [alpha(i);alpha(j)] - old_alpha_B;
    grab = grab + [Q(:,i) Q(:,j)] * delta_B;
end

%% object value
v = 1/2 * alpha' * (grab + p);
b = -calculate_rho;
w = x'*(alpha.*y);
end

function [i, j, flag] = select_B
    global Q;
    global grab;
    global alpha;
    global y;
    flag = 0;
    i = -1;
    j = -1;
    L = length(y);
    m = -inf;
    for t = 1 : L
        if (alpha(t) < get_C(y(t)) && y(t) == 1) || ...
                (alpha(t) > 0 && y(t) == -1)
            max_i = -y(t) * grab(t);
            if m <= max_i
                m = max_i;
                i = t;
            end
        end
    end
    M = inf;
    min_temp = inf;
    for t = 1 : L
        if t == i
           continue; 
        end
        if (alpha(t) < get_C(y(t)) && y(t) == -1) || ...
                (alpha(t) > 0 && y(t) == 1)
            min_j = -y(t) * grab(t);
            if M >= min_j
                M = min_j;
            end
            
            if min_j < m
                a_ts = Q(i,i) - 2*y(i)*y(t)*Q(i,t) + Q(t,t);
                if a_ts <= 0
                    a_ts = 1e-12;
                end
                b_ts = m + y(t) * grab(t);
                min_j2 = -b_ts^2 / a_ts;
                if min_temp > min_j2
                    min_temp = min_j2;
                    j = t;
                end
            end
        end
    end
    
    if m - M < 1e-5 || j == -1
       flag = 1; 
    end
end

function update_alpha(i, j)
    global Q;
    global grab;
    global alpha;
    global y;
    C_i = get_C(i);
    C_j = get_C(j);
    if y(i) ~= y(j)
        a = Q(i,i) + 2*Q(i,j) + Q(j,j);
        if a <= 0
            a = 1e-12;
        end
        diff = alpha(i) - alpha(j);
        alpha(i) = alpha(i) + (-grab(i)-grab(j))/a;
        alpha(j) = alpha(j) + (-grab(i)-grab(j))/a;
        if diff > 0
            if alpha(j) < 0
                alpha(j) = 0;
                alpha(i) = diff;
            end
        else
            if alpha(i) < 0
                alpha(i) = 0;
                alpha(j) = -diff;
            end
        end
        
        if diff > C_i - C_j; 
            if alpha(i) > C_i
                alpha(i) = C_i;
                alpha(j) = C_i - diff;
            end
        else
            if alpha(j) > C_j
                alpha(j) = C_j;
                alpha(i) = C_j + diff;
            end
        end
    else
         a = Q(i,i) - 2*Q(i,j) + Q(j,j);
        if a <= 0
            a = 1e-12;
        end
        sum = alpha(i) + alpha(j);
        alpha(i) = alpha(i) + (-grab(i)+grab(j))/a;
        alpha(j) = alpha(j) + (grab(i)-grab(j))/a;
        if sum > C_i
            if alpha(i) > C_i
                alpha(i) = C_i;
                alpha(j) = sum - C_i;
            end
        else
            if alpha(j) < 0
                alpha(j) = 0;
                alpha(i) = sum;
            end
        end
        
        if sum > C_j; 
            if alpha(j) > C_j
                alpha(j) = C_j;
                alpha(i) = sum - C_j;
            end
        else
            if alpha(i) < 0
                alpha(i) = 0;
                alpha(j) = sum;
            end
        end
    end
end

function [rho] = calculate_rho
    global y;
    global grab;
    global alpha;
    nr_free = 0;
    ub = inf;
    lb = -inf;
    sum_free = 0;
    L = length(y);
    for i = 1 : L
        yG = y(i) * grab(i);
        if alpha(i) >= get_C(y(i))
            if y(i) == -1
                ub = min(ub, yG);
            else
                lb = max(lb, yG);
            end
        elseif alpha(i) <= 0
            if y(i) == 1
                ub = min(ub, yG);
            else
                lb = max(lb, yG);
            end  
        else
            nr_free = nr_free + 1;
            sum_free = sum_free + yG;
        end
    end
    
    if nr_free>0 
		rho = sum_free/nr_free;
	else
		rho = (ub+lb)/2;
    end
end

function [C] = get_C(y)
global Cp;
global Cn;
    if y == 1
        C = Cp;
    else
        C = Cn;   
    end
end
```

然后简单的测试。

```
data = [1 3 4;
        1 4 5;
        1 2 3;
        1 1 4;
        -1 5 8;
        -1 9 10;
        -1 8 5];
[w, b] = svm_train(data);

x = data(:,2:end);
y = data(:,1);

hold on;
grid on;
for i = 1 : length(y)
    if y(i) == 1
        plot(x(i,1),x(i,2),'ro');
    else
        plot(x(i,1),x(i,2),'bo');
    end
end
X = 0:0.1:10;
Y = -(w(1).*X+b)./w(2);
plot(X,Y);
```

