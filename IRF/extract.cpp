#include "extract.h"

const int ROWS = 7;
const int COLS = 5;

const Rect first_pic_zone(200, 246, 250, 250);
const int pict_y_padding = 348;

const Rect first_hand_pic_zone(610,248,250,250);
const Point hand_pic_padding(348,348);
const Rect hand_pic_cropping(10,16,16,16);

/**
 * \brief   Extractor constructor
 * \param   input_f     The input folder where we retrieve the usersheets
 * \param   output_f    The output folder where we put the pictograms exctracted
 */
Extract::Extract(string in_dir, string out_dir, string templ_dir){
    this->in_dir = in_dir;
    this->out_dir = out_dir;
    this->templ_dir = templ_dir;
    this->nb_success = 0;
    this->templ_cross = imread(templ_dir + "sheet_cross.png");
    if(this->templ_cross.empty())
        cout << "Cross template not found...Please place it into the input directory." << endl;
}

/**
 * \brief   Extracts the pictograms of a given usersheet
 * \param   filename    The name of the file containing the usersheet
 */
void Extract::extractFromFile(string name) {
    Recognize recognizer = *new Recognize(this->templ_dir);
    this->current_file = name;
    
    // Normalize the input image
    Mat input_sheet = imread(this->in_dir + name);
    Mat normalized_sheet = this->normalize(input_sheet);
    
    // Prepare input file regex
    regex fileRegex("([0-9][0-9][0-9])([0-9][0-9]).png");
    match_results<string::const_iterator> resultFile;
    
    // Retrieve scripter and page number
    if(!regex_match(name, resultFile, fileRegex))
        cout << "Invalid filename" << endl;
    
    string label_name;
    string scripter = resultFile[1];
    string page = resultFile[2];
    
    // Ignore the page 22 or 23 of each scripter because it's not a valid input file
    if ( (page.compare("22") == 0) || (page.compare("23") == 0) )
        return;
    
    Rect current_printed_pictogram_zone = first_pic_zone;
    Rect current_drawn_pictogram_zone = first_hand_pic_zone;
    
    for(int i=0; i < ROWS; i++) {
        
        current_printed_pictogram_zone.y = first_pic_zone.y + i*pict_y_padding;
        current_drawn_pictogram_zone.y = first_hand_pic_zone.y + i*hand_pic_padding.y;
        Mat p = normalized_sheet(current_printed_pictogram_zone);
        label_name = recognizer.recognizePicto(p);
        // If we can'tidentify the label, skip this row
        if(label_name == ""){
			continue;
		}
        
        // Get the drawn pictograms
        for (int j=0; j < COLS; j++) {
            
            current_drawn_pictogram_zone.x = first_hand_pic_zone.x + j*hand_pic_padding.x;
            
            Rect current_drawn_pictogram_zone_cropped = current_drawn_pictogram_zone;
            
            // Crop
            current_drawn_pictogram_zone_cropped.x += hand_pic_cropping.x;
            current_drawn_pictogram_zone_cropped.y += hand_pic_cropping.y;
            current_drawn_pictogram_zone_cropped.width -= hand_pic_cropping.width;
            current_drawn_pictogram_zone_cropped.height -= hand_pic_cropping.height;
            
            // Write output files
            string name = this->out_dir + label_name + "_" + scripter + "_" + page + "_" + to_string((_ULonglong)i) + "_" + to_string((_ULonglong)j);
            imwrite(outputName + ".png", normalized_sheet(current_drawn_pictogram_zone_cropped));
            fileManager::createTxt(name, label_name, scripter, page , i, j);

        }
    }
    this->nb_success++;
	
}

/**
 * \brief   Extracts all the pictograms from sheets contained in input folder
 */
void Extract::extractFromInputFolder() {
    
    DIR* dir = fileManager::dirOpen(this->in_dir);
    struct dirent* readFile = NULL;

    while ((readFile = readdir(dir)) != NULL) {
		 if(!regex_match(readFile->d_name, fileManager::hiddenFileRegex)) {
            cout << "### Handling file : " << readFile->d_name << endl;
            this->extractFromFile(readFile->d_name);
        }
		
    }
    
	
    fileManager::dirClose(dir);
    cout << "Sheets partially or completly extracted : " << this->nb_success << endl;
}
