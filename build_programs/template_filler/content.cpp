#include "content.h"
 
#include <algorithm>
#include <fstream>
#include <stdexcept>
 
namespace fs = std::filesystem;

// Helper to trim whitespace from right of string
std::string rtrim(std::string s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\t')) s.pop_back();
    return s;
}

// Helper to grab values for frontmatter, parses for content between two opening --- and returns toml table
toml::table parseFrontmatter(const fs::path& p) {
    std::ifstream f(p);
    if (!f.is_open()) throw std::runtime_error("Could not open " + p.string());
 
    std::string line;
    if (!std::getline(f, line) || rtrim(line) != "+++")
        throw std::runtime_error(p.string() + ": missing opening '+++' frontmatter delimiter");
 
    std::string frontmatter;
    bool closed = false;
    while (std::getline(f, line)) {
        if (rtrim(line) == "+++") { closed = true; break; }
        frontmatter += line + "\n";
    }
    if (!closed)
        throw std::runtime_error(p.string() + ": missing closing '+++' frontmatter delimiter");
 
    return toml::parse(frontmatter, p.string());
}

Content::Content(fs::path contents_dir, fs::path blogs_dir)
    : contents_dir_(std::move(contents_dir)), blogs_dir_(std::move(blogs_dir)) {}
 
const toml::table& Content::file(const std::string& name) const {
    auto it = files_.find(name);
    if (it == files_.end()) {
        fs::path path = contents_dir_ / (name + ".toml");
        it = files_.emplace(name, toml::parse_file(path.string())).first;
    }
    return it->second;
}
 
Node Content::at(const std::string& file_name, std::string_view path) const {
    Node node = toml::at_path(file(file_name), path);
    if (!node)
        throw std::runtime_error(file_name + ".toml: missing '" + std::string(path) + "'");
    return node;
}

const toml::array& Content::list(const std::string& file_name, std::string_view path) const {
    if (const toml::array* arr = at(file_name, path).as_array()) return *arr;
    throw std::runtime_error(file_name + ".toml: '" + std::string(path) + "' is not an array");
}

const std::vector<toml::table>& Content::blogs() const {
    if (!blogs_) {
        std::vector<toml::table> result;
        if (fs::is_directory(blogs_dir_)) {
            for (const auto& entry : fs::directory_iterator(blogs_dir_)) {
                if (!entry.is_regular_file() || entry.path().extension() != ".md") continue;
                toml::table meta = parseFrontmatter(entry.path());
                meta.insert_or_assign("file_base", entry.path().stem().string());
                result.push_back(std::move(meta));
            }
        }
        // Sort by date
        std::sort(result.begin(), result.end(), [](const toml::table& a, const toml::table& b) {
            std::string da = str(a["date"]), db = str(b["date"]);
            if (da != db) return da > db;
            return str(a["file_base"]) < str(b["file_base"]);
        });
        blogs_ = std::move(result);
    }
    return *blogs_;
}