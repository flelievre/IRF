#ifndef __irf_project__pictogram_identifier__
#define __irf_project__pictogram_identifier__

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "fileManager.h"




#include <stdlib.h> 




using namespace std;
using namespace cv;

class Recognize{
private:
    string templ_dir;
    vector<string> templ_it;
    
public:
    Recognize(string templ_dir);
    string recognizePicto(Mat img);
};

#endif
