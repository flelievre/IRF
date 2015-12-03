#include "extract.h"
#include <cmath>

// Theorical data of crosses position and angle between them
const cv::Point top_right_cross(2205, 472);
const cv::Point bottum_left_cross(256, 3235);
const double angle = 54.801136731;


/**
 * \brief   Find the coordinates of the two cross on the sheet
 * \param   in_form     The matrix containing the image
 * \return  A vector of two points
 */
vector<Point> Extract::identifyCrossCoord(Mat in_form) {
    
    vector<Point> res;
    Point top_right_cross;
    Point bottom_left_cross;
    
    Mat cross;
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    
    // Find top right cross position
    Rect t_r_corner(in_form.cols*3/4, 0, in_form.cols/4, in_form.rows/4);
    
    matchTemplate(in_form(t_r_corner), this->cross_template, cross, CV_TM_SQDIFF);
    minMaxLoc(cross, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    
    top_right_cross = minLoc;
    
    top_right_cross.x += this->cross_template.cols/2 + in_form.cols*3/4;
    top_right_cross.y += this->cross_template.rows/2;
    
    // Find bottom left cross position
    Rect b_l_corner(0, in_form.rows*3/4, in_form.cols/4, in_form.rows/4);
    
    matchTemplate(in_form(b_l_corner), this->cross_template, cross, CV_TM_SQDIFF);
    minMaxLoc(cross, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    
    bottom_left_cross = minLoc;
    bottom_left_cross.x += this->cross_template.cols/2;
    bottom_left_cross.y += this->cross_template.rows/2 + in_form.rows*3/4;
    
    res.push_back(top_right_cross);
    res.push_back(bottom_left_cross);
    
    return res;
}

/**
 * \brief   Normalizes the sheet in size and rotation
 * \param   in_form     The raw sheet to normalize
 * \return  The normalized sheet
 */
Mat Extract::normalize(Mat in_form) {

    Mat normalized_sheet;
    vector<Point> cross_coordinates = this->identifyCrossCoord(in_form);
    Point tr_cross = cross_coordinates[0];
    Point bl_cross = cross_coordinates[1];
    
    // Rotate the TR-BL line around BL to match the normalized orientation
    double dx_cross = tr_cross.x - bl_cross.x;
    double dy_cross = bl_cross.y - tr_cross.y;
    double cross_angle = atan(dy_cross/dx_cross) * 180.0 / 3.14;
    
    Mat rot = getRotationMatrix2D(bl_cross, angle-cross_angle, 1.0);
    warpAffine(in_form, normalized_sheet, rot, in_form.size());
    
    // Align this form's BL cross with the normalized BL
    Point delta_bl_nm = bottum_left_cross - bl_cross;
    
    Mat translate = (Mat_<double>(2,3) << 1, 0, delta_bl_nm.x, 0, 1, delta_bl_nm.y);
    warpAffine(normalized_sheet, normalized_sheet, translate, in_form.size());
    
    // Resize the image to position the TR cross correctly
    double nm_dx_cross = top_right_cross.x - bottum_left_cross.x;
    double nm_dy_cross = bottum_left_cross.y - top_right_cross.y;
    double scale_factor_x = nm_dx_cross/dx_cross;
    double scale_factor_y = nm_dy_cross/dy_cross;
    
    Mat resize = getRotationMatrix2D(bl_cross, 0.0, (scale_factor_x+scale_factor_y)*0.5);
    warpAffine(normalized_sheet, normalized_sheet, resize, in_form.size());
    
    // Vertically crop the image to the region between the crosses
    Rect crop_roi(0, top_right_cross.y, normalized_sheet.cols, nm_dy_cross);
    
    return normalized_sheet(crop_roi);
}
