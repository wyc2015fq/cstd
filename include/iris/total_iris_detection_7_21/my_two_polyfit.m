% 最小二乘曲线拟合y=a+bx+cx^2
function h = my_two_polyfit(x,y) 

% x = a(1,:);
% y = a(2,:);
m = length(x);  						% // m为数据个数
d = zeros(1,m);
for i = 1 : m
    d(1) = d(1) + x(i);
    a(1, 2) = d(1);
    d(2) = d(2) + x(i) ^ 2;
    a(1, 3) = d(2);
    d(3) = d(3) + y(i);
    a(1, 4) = d(3);
    d(4) = d(4) + x(i) ^ 3;
    a(2, 3) = d(4);
    d(5) = d(5) + x(i) * y(i);
    a(2, 4) = d(5);
    d(6) = d(6) + x(i) ^ 4;
    a(3, 3) = d(6);
    d(7) = d(7) + x(i) ^ 2 * y(i);
    a(3, 4) = d(7);
end

a(1, 1) = m;
a(2, 1) = a(1, 2);
a(2, 2) = a(1, 3);
a(3, 1) = a(1, 3);
a(3, 2) = a(2, 3);
n = 3; 			%//n为行数
m = 4;			%//m为列数
for k = 1 : n - 1
    for i = k + 1 : n
        for j = k + 1 : m
            a(i, j) = a(i, j) - a(i, k) * a(k, j) / a(k, k);
        end
    end
end

z(n) = a(n, m) / a(n, n);

for k = n - 1 : -1 : 1
    z(k) = 0;
    for j = n : -1 : k + 1
        z(k) = z(k) + a(k, j) * z(j);
    end
    z(k) = (a(k, m) - z(k)) / a(k, k);
end
% xs(1) = z(1);
% xs(2) = z(2);
% xs(3) = z(3);
h(1) = z(3);
h(2) = z(2);
h(3) = z(1);

