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
    assert(argc == 3);
    std::string file_name(argv[1]);
    std::string out_file(argv[2]);

    Filler* filler = nullptr;

    std::string suffix = "html";
    if (file_name.rfind(suffix) == (file_name.length() - suffix.length())) filler = new HTMLFiller;
    suffix = "gmi";
    if (file_name.rfind(suffix) == (file_name.length() - suffix.length())) filler = new GeminiFiller;
    suffix = "gophermap";
    if (file_name.rfind(suffix) == (file_name.length() - suffix.length())) filler = new GopherFiller;

    if (!filler) {
        std::cout << "ERROR: Unknown template file type";
        return 1;
    }

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

    filler->processFile(file, out);

    free(filler);
}