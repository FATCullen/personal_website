#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include "filler.h"
#include "html_filler.h"
#include "gemini_filler.h"
#include "gopher_filler.h"

int main(int argc, char* argv[]) {
    assert(argc == 3); // Takes two args, one specifying source file and second location to write output file
    std::string file_name(argv[1]);
    std::string out_file(argv[2]);

    Filler* filler = nullptr;

    // Select filler based off of template file extension
    std::string suffix = "html";
    if (file_name.rfind(suffix) == (file_name.length() - suffix.length())) filler = new HTMLFiller;
    suffix = "gmi";
    if (file_name.rfind(suffix) == (file_name.length() - suffix.length())) filler = new GeminiFiller;
    suffix = "gophermap";
    if (file_name.rfind(suffix) == (file_name.length() - suffix.length())) filler = new GopherFiller;

    // If no filler then unknow file type, exit
    if (!filler) {
        std::cout << "ERROR: Unknown template file type";
        return 1;
    }

    // Create ifstream and ofstream
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return 1;
    }
    std::ofstream out(out_file);
    if (!out.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    // Call filler to fill file and write to out
    filler->processFile(file, out);

    // Clean up
    free(filler);

    return 0;
}