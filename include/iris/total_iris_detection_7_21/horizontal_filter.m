function I5 = horizontal_filter(img,canshu)
% 进行去除睫毛  水平滤波 horizontal rank filter
[ROI_row, ROI_col] = size(img);
f = img;
for i = 1 : ROI_row
    for j = 4 : ROI_col - 4
        b = img(i, j - 3 : j + 3);
        %             [d e] = sort(b);
        [d aa] = sort(b);
        f(i, j) = d(1, 6);
    end
end
I = mat2gray(f);
scaling = 1.0;    sigma = 2;    vert = 1.0;    horz = 8.0;

I4 = canny(I, sigma, scaling, vert, horz, 1.9, 1.5);    % edge detection 4-19
I5 = false(size(I4));

% 5_30 上下眼睑的定位不同
if strcmp(canshu,'top')
    for i = 1 : round(3*ROI_row/5)
        for j = 4 : ROI_col - 4
            if I4(i,j)>0.22
                I5(i,j) = 1;
            else
                I5(i,j) = 0;
            end
        end
    end
elseif strcmp(canshu,'bottom')
    %     for i = 1 : round(3*ROI_row/5)
    for i = round(ROI_row/2) : ROI_row
        for j = 4 : ROI_col - 4
            %             I5(i,j) = 0;
            if I4(i,j)>0.25
                I5(i,j) = 1;
            else
                I5(i,j) = 0;
            end
        end
    end
end

I6 = false(size(I4));
