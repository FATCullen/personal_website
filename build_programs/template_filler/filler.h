#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "content.h"

class Filler {
public:
    explicit Filler(const Content& content) : content_(content) {}
    virtual ~Filler() = default;

    // Public entry point, fill INSERT lines with appropriate contents and write to output file
    void processFile(std::ifstream& file, std::ofstream& out);

protected:
    const Content& content_;

    // Structured sections: not supported by default, override per format
    virtual std::string fillProjects();
    virtual std::string fillWorks();
    virtual std::string fillWebBadges();
    virtual std::string fillBlogs();

    // Formatting for plain string (for example, info line formatting in gopher)
    virtual std::string formatText(std::string text);
    virtual std::string formatList(const std::vector<std::string>& items);

private:
    std::string expand(const std::string& placeholder);
    std::string fillText(const std::string& file, const std::string& path);
};