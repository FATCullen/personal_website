#include <sstream>
#include "gopher_filler.h"
#include "contents.h"

std::string GopherFiller::fillBlogs() {
    std::string result = "";

    for (Blog blog : BLOGS) {
        result += "1" + blog.title + " - " + blog.date + "\t/blog/" + blog.file_base + ".gophermap\n";
    }

    return result;
}

std::string GopherFiller::formatText(const std::string text) {
    int width = 67;
    int indent = 2;
    if (text.empty()) return "";

    std::vector<std::string> lines;
    size_t start = 0;
    while (true) {
        size_t pos = text.find('\n', start);
        if (pos == std::string::npos) { lines.push_back(text.substr(start)); break; }
        lines.push_back(text.substr(start, pos - start));
        start = pos + 1;
    }

    if (text.find('\n') != std::string::npos) {
        std::string result;
        for (auto part : lines) result += formatText(part);
        if (!result.empty()) result.pop_back();
        return result;
    }

    int useIndent = ((int)text.size() <= width) ? 0 : indent;

    std::istringstream iss(text);
    std::vector<std::string> words{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    std::string result;
    std::string line(useIndent, ' ');
    for (auto& word : words) {
        if ((int)line.size() + (int)word.size() + 1 <= width) {
            line += word + " ";
        } else {
            std::string trimmed = line;
            while (!trimmed.empty() && std::isspace((unsigned char)trimmed.back())) trimmed.pop_back();
            result += "i" + trimmed + std::string(std::max(0, width - (int)trimmed.size()), ' ') + "\tfake\t(NULL)\t0\n";
            line = word + " ";
        }
    }
    std::string trimmed = line;
    while (!trimmed.empty() && std::isspace((unsigned char)trimmed.back())) trimmed.pop_back();
    result += "i" + trimmed + std::string(std::max(0, width - (int)trimmed.size()), ' ') + "\tfake\t(NULL)\t0\n";
    return result;
}
