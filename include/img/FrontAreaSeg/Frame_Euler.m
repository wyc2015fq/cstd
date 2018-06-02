% 求两帧的距离，Frame1和Frame2必须为结构体，且其域名为Centroid
% 返回的矩阵d是两两之间的距离
function d=Frame_Euler(Frame1,Frame2)
N1=length(Frame1);
N2=length(Frame2);
d=zeros(N2,N1);
for i=1:N2
    for j=1:N1
        d(i,j)=norm(Frame2(i).Centroid-Frame1(j).Centroid);
    end
end
end