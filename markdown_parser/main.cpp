#include <iostream>
#include <string>
#include <fstream>
#include "cmark.h"
#include "gopher_renderer.h"
#include "gemini_renderer.h"
#include "html_renderer.h"

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

    GeminiRenderer geminiRenderer;
    std::string gemini = geminiRenderer.renderDocument(doc);
    GopherRenderer gopherRenderer;
    std::string gopher = gopherRenderer.renderDocument(doc);
    HTMLRenderer htmlRenderer;
    std::string html = htmlRenderer.renderDocument(doc);
    
    std::cout << gemini << std::endl;
    std::cout << gopher << std::endl;
    std::cout << html << std::endl;
    
    cmark_node_free(doc);
    return 0;
}