function [img_warp xx yy] = keyimgWarp(R1,T1,R2,T2,img_key,x,y,w_3D)
                img_warp=zeros(size(img_key,1),size(,2));
                ww_3d=w_3D(1,:);
                sp0=ww_3d(1,1:3)-ww_3d(1,4:6);
                sp1=ww_3d(1,1:3)-ww_3d(1,7:9);

                n=cross(sp0,sp1)';
               d=-dot(n,ww_3d(1,1:3));
  
                
                u0=136;
                v0=113;
                kx=363;
                ky=361;
                A=[kx 0 u0
                    0 ky v0
                    0 0 1];
                Rk=R1;
                Tk=T1;
                Rp=R2;
                Tp=T2;

                H=A*(Rp*Rk'-(-Rp*Rk'*Tk+Tp)*(Rk*n)'/(d-Tk'*(Rk*n)))*pinv(A);
                tt=(H)*[y 
                    x
                    1];
                xx=round(tt(1)/tt(3));
                yy=round(tt(2)/tt(3));
                if (xx<1)
                    xx=1;
                    
                end
                if(yy<1)
                    yy=1;
                end
               
                
         for k=x-7:x+7
            for l=y-7:y+7
                    tt=(H)*[l 
                    k
                    1];
                xxx=round(tt(1)/tt(3));
                yyy=round(tt(2)/tt(3));
                if (xxx<1)
                    xxx=1;
                end
                if(yyy<1)
                    yyy=1;
                end  
                 img_warp(round(yyy),round(xxx))=img_key(k,l);
            end
        end
                
                
                
                
                
                
                
%                 iH=pinv(H);
%                 
%           for k=yy-15:yy+15
%             for l=xx-15:xx+15
%                   tt=iH*[l 
%                     k
%                     1];
%                 xxx=tt(1)/tt(3);
%                 yyy=tt(2)/tt(3);
%                 if xxx<1
%                     xxx=1;
%                 else if xxx>320
%                         xxx=320;
%                     end
%                 end
%                 if yyy<1
%                     yyy=1;
%                 else if yyy>240
%                         yyy=240;
%                     end
%                 end
% %                 dst = imWarp(src, H, w, h)
%                 img_warp(k,l)=img_key(round(yyy),round(xxx));
%             end
          end
         % img_warp=mat2gray(img_warp);
          