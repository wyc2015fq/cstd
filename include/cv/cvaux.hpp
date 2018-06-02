
#ifndef __CVAUX_HPP__
#define __CVAUX_HPP__

#ifdef __cplusplus

/****************************************************************************************\
*                                      Image class                                       *
\****************************************************************************************/

class CC_EXPORTS CvCamShiftTracker
{
public:
    
    CvCamShiftTracker();
    virtual ~CvCamShiftTracker();
    
    /**** Characteristics of the object that are calculated by track_object method *****/
    float   get_orientation() const // orientation of the object in degrees 
    { return m_box.angle; }
    float   get_length() const // the larger linear size of the object
    { return m_box.size.height; }
    float   get_width() const // the smaller linear size of the object
    { return m_box.size.width; }
    CvPoint2D32f get_center() const // center of the object
    { return m_box.center; }
    CvRect get_window() const // bounding rectangle for the object
    { return m_comp.rect; }
    
    /*********************** Tracking parameters ************************/
    int     get_threshold() const // thresholding value that applied to back project
    { return m_threshold; }

    int     get_hist_dims(int* dims = 0) const // returns number of histogram dimensions and sets
    { return m_hist ? cvGetDims(m_hist->bins, dims) : 0; } 
    
    int     get_min_ch_val(int channel) const // get the minimum allowed value of the specified channel
    { return m_min_ch_val[channel]; }
    
    int     get_max_ch_val(int channel) const // get the maximum allowed value of the specified channel
    { return m_max_ch_val[channel]; }
    
    // set initial object rectangle (must be called before initial calculation of the histogram)
    bool    set_window(CvRect window)
    { m_comp.rect = window; return true; }
    
    bool    set_threshold(int threshold) // threshold applied to the histogram bins
    { m_threshold = threshold; return true; }

    bool    set_hist_bin_range(int dim, int min_val, int max_val);

    bool    set_hist_dims(int c_dims, int* dims);// set the histogram parameters
    
    bool    set_min_ch_val(int channel, int val) // set the minimum allowed value of the specified channel
    { m_min_ch_val[channel] = val; return true; }
    bool    set_max_ch_val(int channel, int val) // set the maximum allowed value of the specified channel
    { m_max_ch_val[channel] = val; return true; }

    /************************ The processing methods *********************************/
    // update object position
    virtual bool  track_object(const IplImage* cur_frame);
    
    // update object histogram
    virtual bool  update_histogram(const IplImage* cur_frame);

    // reset histogram
    virtual void  reset_histogram();

    /************************ Retrieving internal data *******************************/
    // get back project image
    virtual IplImage* get_back_project()
    { return m_back_project; }

    float query(int* bin) const
    { return m_hist ? cvQueryHistValue_nD(m_hist, bin) : 0.f; }
    
protected:

    // internal method for color conversion: fills m_color_planes group
    virtual void color_transform(const IplImage* img); 

    CvHistogram* m_hist;

    CvBox2D    m_box;
    CvConnectedComp m_comp;

    float      m_hist_ranges_data[CC_MAX_DIM][2];
    float*     m_hist_ranges[CC_MAX_DIM];

    int        m_min_ch_val[CC_MAX_DIM];
    int        m_max_ch_val[CC_MAX_DIM];
    int        m_threshold;

    IplImage*  m_color_planes[CC_MAX_DIM];
    IplImage*  m_back_project;
    IplImage*  m_temp;
    IplImage*  m_mask;
};

#endif /* __cplusplus */

#endif /* __CVAUX_HPP__ */


