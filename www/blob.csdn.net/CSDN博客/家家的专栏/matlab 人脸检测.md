# matlab 人脸检测 - 家家的专栏 - CSDN博客





2014年09月09日 11:09:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3549








参考网址：http://www.mathworks.com/help/vision/examples/face-detection-and-tracking-using-camshift.html

clear all;

clc;

faceDetector = vision.CascadeObjectDetector();

HOMEIMAGES_array=dir(fullfile('./video/','*.avi'));

namelist = {};

nameindex = 0; i_index=0;

for i = 1:numel(HOMEIMAGES_array)

    temp = HOMEIMAGES_array(i).name;    

    %for subject name

    index_ = find(temp == '_');    

    name = temp(index_(3)+1:end);

    ind = find(ismember(namelist,name));

    if  ~numel(ind)

        nameindex = nameindex + 1;

        namelist{nameindex} = name;

    end    

    %for video frame

    mov=mmread(['./video/',temp]);

   mkdir(['./ytcelebrityface/',temp(1:size(temp,2)-4)]);

    for m=1:mov.nrFramesTotal

        if m<51

            videoFrame=mov.frames(m).cdata;

            %         videoFrame      = step(videoFileReader);

            bbox            = step(faceDetector, videoFrame);

            % Draw the returned bounding box around the detected face.

%             videoOut = insertObjectAnnotation(videoFrame,'rectangle',bbox,'Face');

            if bbox   

                for ibox =1:size(bbox,1)

                    if size(bbox,1)>1

                        videoFace = imresize(imcrop(videoFrame,[bbox(ibox,:)]),[45,45]);

                        imwrite(videoFace,['./videoface/',temp(1:size(temp,2)-4),'/',num2str(m),'more',num2str(ibox),'.jpg'],'jpg');

                    else

                        videoFace = imresize(imcrop(videoFrame,[bbox]),[45,45]);

                        imwrite(videoFace,['./videoface/',temp(1:size(temp,2)-4),'/',num2str(m),'.jpg'],'jpg');

                    end

                end

            end

        end

    end

end





http://www.mathworks.com/help/vision/examples/face-detection-and-tracking-using-camshift.html



