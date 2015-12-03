#include "extractor.h"

string input_learning_db = "input_learning_db";
string learning_db_extracted = "learning_db_extracted";
string template_pictograms = "template_pictograms";

int main (int argc, const char * argv[]) {
    
    Extractor extractor = *new Extractor(input_learning_db + "/",
                                         learning_db_extracted + "/",
                                         template_pictograms + "/");
    extractor.extractFromInputFolder();

    return 0;
}