%%计算不同类的汉明距离
function dist=Range2(A1,M1,A2,M2)

[a s]=size(A1);

dist = zeros(s,s);

for i=1:s
    for j=1:s
        dist(i,j)=Hamm(A1(:,i),M1(:,i),A2(:,j),M2(:,j));
    end
end


