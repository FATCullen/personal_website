#include <iostream>
#include <string>
#include <fstream>
#include "cmark.h"
#include "gopher_renderer.h"

int main() {
     std::ifstream file("test.md");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    const char *markdown = content.c_str();
    cmark_node *doc = cmark_parse_document(markdown, strlen(markdown), CMARK_OPT_DEFAULT);
    GopherRenderer renderer;
    std::string gopher = renderer.renderDocument(doc);
    
    std::cout << gopher << std::endl;
    
    cmark_node_free(doc);
    return 0;
}