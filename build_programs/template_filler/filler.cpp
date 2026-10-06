#include "filler.h"

#include <map>
#include <sstream>
#include <stdexcept>


struct TextSource {
    const char* file;
    const char* path;
};

const std::map<std::string, TextSource> PlainTextInserts = {
    {"INSERT_ABOUT_PROFESSIONAL",   {"site", "about.professional"}},
    {"INSERT_ABOUT_PERSONAL",       {"site", "about.personal"}},
    {"INSERT_FAVOURITE_LANGUAGES",  {"site", "favourite.languages"}},
    {"INSERT_FAVOURITE_FRAMEWORKS", {"site", "favourite.frameworks"}},
    {"INSERT_SKILLS",               {"site", "favourite.skills"}},
    {"INSERT_GPA",                  {"site", "about.gpa"}},
};

void Filler::processFile(std::ifstream& file, std::ofstream& out) {
    const std::string prefix = "INSERT_";
    std::string line;
    while (std::getline(file, line)) {
        if (line.compare(0, prefix.size(), prefix) == 0)
            out << expand(line) << "\n";
        else
            out << line << "\n";
    }
    out.close();
}

std::string Filler::expand(const std::string& placeholder) {
    auto it = PlainTextInserts.find(placeholder);
    if (it != PlainTextInserts.end()) return fillText(it->second.file, it->second.path);

    if (placeholder == "INSERT_PROJECTS")   return fillProjects();
    if (placeholder == "INSERT_WORKS")      return fillWorks();
    if (placeholder == "INSERT_WEB_BADGES") return fillWebBadges();
    if (placeholder == "INSERT_BLOGS")      return fillBlogs();
    if (placeholder == "INSERT_RSS_ITEMS")  return fillRSSItems();

    throw std::runtime_error("Unknown placeholder: " + placeholder);
}

// Fills text field, strings go through formatText, arrays of strings through formatList.
std::string Filler::fillText(const std::string& file, const std::string& path) {
    Node node = content_.at(file, path);

    if (auto s = node.value<std::string>()) return formatText(*s);

    if (const toml::array* arr = node.as_array()) {
        std::vector<std::string> items;
        for (const toml::node& el : *arr) {
            if (auto s = el.value<std::string>()) items.push_back(*s);
        }
        return formatList(items);
    }

    throw std::runtime_error(std::string(file) + ".toml: '" + path +
                             "' must be a string, or array of strings");
}

// Not defined in base (see deriving fillers)
std::string Filler::fillProjects()  { return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION"; }
std::string Filler::fillWorks()     { return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION"; }
std::string Filler::fillWebBadges() { return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION"; }
std::string Filler::fillBlogs()     { return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION"; }
std::string Filler::fillRSSItems()  { return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION"; }

// Default rendering is just plain text
std::string Filler::formatText(std::string text) {
    return text;
}

// Just seperates values by strings
std::string Filler::formatList(const std::vector<std::string>& items) {
    std::string result;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) result += ", ";
        result += items[i];
    }
    return formatText(result);
}