#include "text_tools.h"
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cctype>

namespace {

std::vector<std::string> splitLines(const std::string& s) {
    std::vector<std::string> lines;
    size_t start = 0;
    while (true) {
        size_t pos = s.find('\n', start);
        if (pos == std::string::npos) { lines.push_back(s.substr(start)); break; }
        lines.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return lines;
}

std::string rtrim(std::string s) {
    while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
    return s;
}

}

namespace gtxt {

std::string hline(int width, const std::string& chars) {
    if (chars.empty() || width <= 0) return "\n";
    std::string out;
    while ((int)out.size() + (int)chars.size() <= width) out += chars;
    int remaining = width - (int)out.size();
    if (remaining > 0) out += chars.substr(0, remaining);
    return out + "\n";
}

std::string breakUp(const std::string& text, int width, int indent) {
    if (text.empty()) return "";

    if (text.find('\n') != std::string::npos) {
        std::string result;
        for (auto& part : splitLines(text)) result += breakUp(part, width, indent);
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
            std::string trimmed = rtrim(line);
            result += trimmed + std::string(std::max(0, width - (int)trimmed.size()), ' ') + "\n";
            line = word + " ";
        }
    }
    std::string trimmed = rtrim(line);
    result += trimmed + std::string(std::max(0, width - (int)trimmed.size()), ' ') + "\n";
    return result;
}

std::string padCenter(const std::string& text, int width) {
    if (text.empty()) return "";
    std::string result;
    for (auto& line : splitLines(text)) {
        int padding = std::max(0, width - (int)line.size());
        int left = (padding / 2) + (padding % 2);
        int right = padding / 2;
        result += std::string(left, ' ') + line + std::string(right, ' ') + "\n";
    }
    return result;
}

std::string rightPad(const std::string& text, int width) {
    if (text.empty()) return "";
    auto lines = splitLines(text);
    std::string result;
    for (size_t i = 0; i < lines.size(); ++i) {
        int padding = std::max(0, width - (int)lines[i].size());
        result += lines[i] + std::string(padding, ' ');
        if (i + 1 < lines.size()) result += "\n";
    }
    return result;
}

std::string box(const std::string& text, int width) {
    if (text.empty()) return "";
    std::string border = "+" + std::string(std::max(0, width - 2), '-') + "+\n";
    std::string result = border;
    for (auto& line : splitLines(text)) result += "| " + line + " |\n";
    result += border;
    return result;
}

std::string bulletize(const std::string& text, const std::string& bullet) {
    if (text.empty()) return "";
    std::string result;
    for (auto& line : splitLines(text)) result += "    " + line + "\n";
    int padLen = std::max(0, 3 - (int)bullet.size());
    std::string prefix = std::string(padLen, ' ') + bullet;
    return result.size() >= 3 ? prefix + result.substr(3) : prefix + result;
}

std::string blockIndent(const std::string& text) {
    if (text.empty()) return "";
    std::string result;
    for (auto& line : splitLines(text)) result += " |  " + line + "\n";
    return result;
}

std::string underline(const std::string& text, const std::string& chars) {
    return "    " + text + "\n    " + hline((int)text.size(), chars);
}

}