#pragma once
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "../toml/toml.hpp"

using Node = toml::node_view<const toml::node>;

// Helper to grab string value of node
inline std::string str(Node n) {
    return n.value_or(std::string{});
}

// Helper to call function on all tables in array
template <typename F>
void eachTable(Node n, F f) {
    if (const toml::array* arr = n.as_array()) {
        for (const toml::node& el : *arr) {
            if (const toml::table* t = el.as_table()) f(*t);
        }
    }
}

class Content {
public:
    Content(std::filesystem::path contents_dir, std::filesystem::path blogs_dir);
 
    // Toml table of the whole parsed file
    const toml::table& file(const std::string& name) const;
 
    // Grab value at dotted path
    Node at(const std::string& file_name, std::string_view path) const;
 
    // Grab array at dotted path
    const toml::array& list(const std::string& file_name, std::string_view path) const;
 
    // Grab frontmatter from blogs
    const std::vector<toml::table>& blogs() const;
 
private:
    std::filesystem::path contents_dir_;
    std::filesystem::path blogs_dir_;
    mutable std::map<std::string, toml::table> files_;
    mutable std::optional<std::vector<toml::table>> blogs_;
};