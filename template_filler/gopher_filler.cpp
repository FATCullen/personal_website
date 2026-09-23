#include <sstream>
#include <iterator>
#include "gopher_filler.h"
#include "contents.h"

std::string GopherFiller::fillBlogs() {
    std::string result = "";

    for (Blog blog : BLOGS) {
        result += "1" + blog.title + " - " + blog.date + "\t/blog/" + blog.file_base + ".gophermap\n";
    }

    return result;
}

// Apply default gopher info text formatting
// Wraps text to fit to 67 char gophermap width
// and add i tag boilerplate
std::string GopherFiller::formatText(const std::string text) {
    int width = 67; // Default width of 67 chars
    int indent = 2;
    if (text.empty()) return "";

    int useIndent = ((int)text.size() <= width) ? 0 : indent;

    // Split text into words
    std::istringstream iss(text);
    std::vector<std::string> words{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    // Initialize text with indent (tab at start of paragraph)
    std::string result;
    std::string line(useIndent, ' ');
    // Add each word, wrapping when a line is full
    for (auto& word : words) {
        if ((int)line.size() + (int)word.size() + 1 <= width) {
            line += word + " ";
        } else {
            std::string trimmed = line;
            while (!trimmed.empty() && std::isspace((unsigned char)trimmed.back())) trimmed.pop_back();
            // Gopher info text formatting
            // i \t title \t path \t host \t port \n
            result += "i" + trimmed + std::string(std::max(0, width - (int)trimmed.size()), ' ') + "\tfake\t(NULL)\t0\n";
            line = word + " ";
        }
    }
    // Add any leftover
    std::string trimmed = line;
    while (!trimmed.empty() && std::isspace((unsigned char)trimmed.back())) trimmed.pop_back();
    result += "i" + trimmed + std::string(std::max(0, width - (int)trimmed.size()), ' ') + "\tfake\t(NULL)\t0\n";
    return result;
}
