%%计算同类的汉明距离
function dist=Range1(A,M)

[a s]=size(A);

dist = zeros(s,s);

for i=1:(s-1)
    for j=(i+1):s
        dist(j,i)=Hamm(A(:,i),M(:,i),A(:,j),M(:,j));
    end
end