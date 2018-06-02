function Test()
    clear all
    rgb=imread('c:\222.jpg');%需要处理的图片
    m=size(rgb,1);
    n=size(rgb,2);
    rr=zeros(m,n);
    gg=zeros(m,n);
    bb=zeros(m,n);
    for i=1:m
        for j=1:n
            rr(i,j)=logm(double(rgb(i,j,1))+eps);
            gg(i,j)=logm(double(rgb(i,j,2))+eps);
            bb(i,j)=logm(double(rgb(i,j,3))+eps);
        end
    end
    rr=rr/max(max(rr(:)));
    gg=gg/max(max(gg(:)));
    bb=bb/max(max(bb(:)));
    rrr= retinex_frankle_mccann(rr, 4);
    ggg= retinex_frankle_mccann(gg, 4);
    bbb= retinex_frankle_mccann(bb, 4);
    for i=1:m
        for j=1:n
            rrr(i,j)=round(exp(rrr(i,j)*5.54));
            ggg(i,j)=round(exp(ggg(i,j)*5.54));
            bbb(i,j)=round(exp(bbb(i,j)*5.54));
        end
    end
    rgb=cat(3,uint8(rrr),uint8(ggg),uint8(bbb));
    rgb=max(min(rgb,255),0);
    imshow(rgb);
end

function [ Retinex ] = retinex_frankle_mccann( L, nIterations )
    global RR IP OP NP Maximum
    RR = L;
    Maximum = max(L(:));                                 % maximum color value in the image
    [nrows, ncols] = size(L);
    shift = 2^(fix(log2(min(nrows, ncols)))-1);          % initial shift
    OP = Maximum*ones(nrows, ncols);                     % initialize Old Product
    while (abs(shift) >= 1)
        for i = 1:nIterations
            CompareWith(0, shift);                         % horizontal step
            CompareWith(shift, 0);                         % vertical step
        end
        shift = -shift/2;                                 % update the shift
    end
    Retinex = NP;
end

function CompareWith(s_row, s_col)
    global RR IP OP NP Maximum
    IP = OP;
    if (s_row + s_col > 0)
        IP((s_row+1):end, (s_col+1):end) = OP(1:(end-s_row), 1:(end-s_col)) + ...
            RR((s_row+1):end, (s_col+1):end) - RR(1:(end-s_row), 1:(end-s_col));
    else
        IP(1:(end+s_row), 1:(end+s_col)) = OP((1-s_row):end, (1-s_col):end) + ...
            RR(1:(end+s_row),1:(end+s_col)) - RR((1-s_row):end, (1-s_col):end);
    end
    IP(IP > Maximum) = Maximum;                          % The Reset operation
    NP = (IP + OP)/2;                                    % average with the previous Old Product
    OP = NP;                                             % get ready for the next comparison
end
