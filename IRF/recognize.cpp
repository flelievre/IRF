#include "recognize.h"

/**
 * \brief   PictogramIdentifier constructor
 * \param   templ_dir     The folder containing the template pictograms
 */
Recognize::Recognize(string templ_dir){
    this->templ_dir = templ_dir;
    this->templ_it = utils::getFilesInDir(templ_dir, "sheet_cross.png");
}

/**
 * \brief   Identify the given printed pictogram
 * \param   img     A matrix containing a single printed pictogram
 * \return  The name of the recognized pictogram
 */
string Recognize::recognizePicto(Mat img){
    Mat res;
    
    int best_value = 0;
    int best_template_match = 0;

    Mat templ;
	 for(int i=0; i < this->templ_it.size(); i++){
        // Load current template
        templ = imread(this->templ_dir + this->templ_it[i]);
        // Create the result matrix
        int res_cols =  img.cols - templ.cols + 1;
        int res_rows = img.rows - templ.rows + 1;
        res.create(res_cols, res_rows, CV_32FC1);
        
        // Do the Matching and Normalize
        matchTemplate(img, templ, res, CV_TM_CCOEFF);
        
        double minVal, maxVal;
        Point minLoc, maxLoc;
        minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
        
        // Update best match
        if(i == 0) {
            best_value = maxVal;
            best_template_match = i;
        }
        else if(maxVal > best_value) {
            best_value = maxVal;
            best_template_match = i;
        }   
    }
    string templ_diround = this->templ_it[best_template_match];
    regex templateFileRegex("([a-zA-Z]+).png");
    match_results<string::const_iterator> resultTemplateFile;
    
    regex_match(templ_diround, resultTemplateFile, templateFileRegex);
    string parsed_templ_diround = resultTemplateFile[1];
    
    return parsed_templ_diround;
}