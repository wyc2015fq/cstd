function img_result = glassRemoval(I)
img_result = I;
img_gray=I;
[M N]=size(img_gray);
aa=reshape(img_gray,1,1*M*N);
thm=floor(0.05*M*N);
bb=sort(aa,'descend');
R=zeros(M,N);
th=bb(1,thm);

for i=30:M-30
    for j=30:N-30
        if img_gray(i,j)<th
            R(i,j)=0;
        else
            R(i,j)=1;
            % R(i-2:i+2,j-2:j+2)=1;
        end
    end
end
L=bwlabel(R,4);
point_num=max(max(L));
for p_num=1:point_num
    [col row]=find(L==p_num);
    po_num=length(find(L==p_num));

    rr=round(sqrt(po_num)/2);
    avg_p(1,:)=[round(sum(sum(col))/po_num),round(sum(sum(row))/po_num)];
    R(avg_p(1,1)-rr-2:avg_p(1,1)+rr+2,avg_p(1,2)-rr-2:avg_p(1,2)+2+rr)=1;
end
%b=strel('square',5);
%R=imdilate(R,b);
[aa_c bb_c]=find(R==1);
rnum=size(aa_c,1);
for i=1:rnum
    x0=bb_c(i,1);
    y0=aa_c(i,1);

    for j=x0-1:-1:1
        if((R(y0,j)==0)&(R(y0,j+1)==0)&(R(y0,j+2)==1))
            xl=j;
            break;
        else
            continue;
        end
    end

    for j=x0+1:N
        if((R(y0,j)==0)&(R(y0,j-1)==0)&(R(y0,j-2)==1))
            xr=j;
            break;
        else
            continue;
        end
    end

    for k=y0-1:-1:1
        if((R(k,x0)==0)&(R(k+1,x0)==0)&(R(k+2,x0)==1))
            yt=k;
            break;
        else
            continue;
        end
    end

    for k=y0+1:M
        if((R(k,x0)==0)&(R(k-1,x0)==0)&(R(k-2,x0)==1))
            yd=k;
            break;
        else
            continue;
        end
    end
    a1=double(img_gray(y0,xl));
    a2=double(img_gray(y0,xr));
    a3=double(img_gray(yt,x0));
    a4=double(img_gray(yd,x0));
    %bb(1,i)= ( a1*(xr-x0)+a2*(x0-xl))/(2*(xr-xl)) +(a3*(yd-y0)+a4*(y0-yt))/(2*(yd-yt));
    %R(y0,x0)=0;
    %( a1*(xr-x0)+a2*(x0-xl))/(2*(xr-xl)) +(a3*(yd-y0)+a4*(y0-yt))/(2*(yd-yt))
    img_result(y0,x0)=( a1*(xr-x0)+a2*(x0-xl))/(2*(xr-xl)) +(a3*(yd-y0)+a4*(y0-yt))/(2*(yd-yt));
    %  ( img_gray(y0,xl)*(xr-x0)+img_gray(y0,xr)*(x0-xl))/(2*(xr-xl)) +(img_gray(yt,x0)*(yd-y0)+img_gray(yd,x0)*(y0-yt))/(2*(yd-yt))
end