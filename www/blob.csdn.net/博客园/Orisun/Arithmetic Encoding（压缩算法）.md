# Arithmetic Encoding（压缩算法） - Orisun - 博客园







# [Arithmetic Encoding（压缩算法）](https://www.cnblogs.com/zhangchaoyang/articles/1837182.html)





关于算术编码的介绍请见CSDN上Gjack的博客。[http://blog.csdn.net/Gjack/archive/2010/01/15/5193754.aspx](http://blog.csdn.net/Gjack/archive/2010/01/15/5193754.aspx)

#include<iostream>
#include<map>
#include<string>
#include<cassert>
#include<utility>




using namespace std;

int main(int argc,char **argv)
{
    double High=1.0;
    double Low=0.0;
    string original="CADACDB";

    //create a map to store all chars and it's occurrence frequency
    map<char,double> char_freq;
    char_freq.insert(make_pair('A',0.1));
    char_freq.insert(make_pair('B',0.4));
    char_freq.insert(make_pair('C',0.2));
    char_freq.insert(make_pair('D',0.3));

    //compute HighRanges and LowRanges for each char
    map<char,pair<double,double> > char_range;
    map<char,double>::const_iterator iter=char_freq.begin();
    double floor=Low;
    while(iter!=char_freq.end())
    {
        char_range[iter->first].first=floor;
        floor+=iter->second;
        char_range[iter->first].second=floor;
        ++iter;
    }

    //the core code to compress
    for(string::size_type i=0;i<original.size();i++)
    {
        char temp=original[i];
        assert(char_freq.count(temp));

        double Range=High-Low;
        High=Low+Range*char_range[temp].second;
        Low=Low+Range*char_range[temp].first;
    }
    double result=(Low+High)/2;
    cout<<"Compression Result: "<<result<<endl;

    //the core code to decompress
    Low=0.0;
    High=1.0;
    bool goon=true;
    cout<<"Original Text: ";
    do{
        map<char,pair<double,double> >::const_iterator itr=char_range.begin();
        while(itr!=char_range.end())
        {
            double Range=High-Low;
            if((result-Low)/Range>itr->second.first && (result-Low)/Range<itr->second.second)
            {
                cout<<itr->first;            
                High=Low+Range*itr->second.second;
                Low=Low+Range*itr->second.first;
                break;
            }
            ++itr;
        }
        if(itr==char_range.end())
            goon=false;
    }while(goon);
    cout<<endl;

    return 0;
}












