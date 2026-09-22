#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>
#include "cmark.h"
#include "gopher_renderer.h"
#include "gemini_renderer.h"
#include "html_renderer.h"

int main(int argc, char* argv[]) {
    assert(argc == 2);
    std::string file_name(argv[1]);

    std::string suffix = ".md";
    assert(file_name.rfind(suffix) == (file_name.length() - suffix.length()));

    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();

    const char *markdown = content.c_str();
    cmark_node *doc = cmark_parse_document(markdown, strlen(markdown), CMARK_OPT_DEFAULT);

    file_name.erase(file_name.length() - 3);

    GeminiRenderer geminiRenderer;
    content = geminiRenderer.renderDocument(doc);
    std::ofstream geminiFile(file_name + ".gmi");
    if (geminiFile.is_open()) {
        geminiFile << content;
        geminiFile.close();
    } else assert(false);
    

    GopherRenderer gopherRenderer;
    content = gopherRenderer.renderDocument(doc);
    std::ofstream gopherFile(file_name + ".gophermap");
    if (gopherFile.is_open()) {
        gopherFile << content;
        gopherFile.close();
    } else assert(false);

    HTMLRenderer htmlRenderer;
    content = htmlRenderer.renderDocument(doc);
    std::ofstream htmlFile(file_name + ".html");
    if (htmlFile.is_open()) {
        htmlFile << content;
        htmlFile.close();
    } else assert(false);
    
    cmark_node_free(doc);
    return 0;
}