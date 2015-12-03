#ifndef __irf_project__extractor__
#define __irf_project__extractor__

#include <opencv2/opencv.hpp>
#include <iostream>
#include <regex>
#include <fstream>
#include "recognize.h"
#include <dirent.h>

using namespace std;
using namespace cv;

class Exctract{
private:
    string in_dir;
    string out_dir;
    string templ_dir;
    string current_file;
    int nb_success;
    Mat templ_cross;
    
public:
    Extract(string in_dir, string out_dir, string templ_dir);
    //~Extractor()
    
    // Extraction
    vector<Point>  identifySquares(string file);
    vector<vector<int>> createGrid(vector<Point> squares, int accuracy);
    void extractFromFile(string name);
    void extractFromFolder();
    
    // Normalization
    vector<Point> identifyCrossCoord(Mat in_form);
    Mat normalize(Mat in_sheet);
};

#endif