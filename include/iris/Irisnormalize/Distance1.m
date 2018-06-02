%%计算同类的汉明距离
function dist=Distance1(A,M)

[a b s]=size(A);

dist = zeros(s,s);

for i=1:(s-1)
    for j=(i+1):s
        dist(j,i)=HDR(A(:,:,i),M(:,:,i),A(:,:,j),M(:,:,j));
    end
end

