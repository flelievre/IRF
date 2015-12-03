#ifndef __irf_project__utils__
#define __irf_project__utils__

#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <dirent.h>

using namespace std;

namespace utils {
    extern regex hiddenFileRegex;
    
    DIR* dirOpen(string path);
    void dirClose(DIR* dir);
    vector<string> getFilesInDir(string path, string unwanted);
    void createTxt(string name, string label, string scripter, string page , int i, int j);
}

#endif
