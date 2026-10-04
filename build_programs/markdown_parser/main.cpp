#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "cmark.h"
#include "gopher_renderer.h"
#include "gemini_renderer.h"
#include "html_renderer.h"

namespace fs = std::filesystem;

// Frees the cmark AST
struct DocDeleter {
    void operator()(cmark_node* n) const { cmark_node_free(n); }
};
using DocPtr = std::unique_ptr<cmark_node, DocDeleter>;

// Helper to trim whitespce from right side of string
static std::string rtrim(std::string s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\t')) s.pop_back();
    return s;
}

// Read file into string
static std::string readFile(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("failed to open input file");
    std::stringstream ss;
    ss << file.rdbuf();
    if (file.bad()) throw std::runtime_error("failed while reading input file");
    return ss.str();
}

// Write string to file
static void writeFile(const fs::path& path, const std::string& content) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("failed to open output file " + path.string());
    out << content;
    out.close();
    if (!out) throw std::runtime_error("failed while writing " + path.string());
}

// Removes frontmatter from md file
static std::string stripFrontmatter(std::string text, const fs::path& path) {
    if (text.compare(0, 3, "\xEF\xBB\xBF") == 0) text.erase(0, 3); // UTF-8 BOM

    // Search for opening frontmatter
    std::istringstream in(text);
    std::string line;
    if (!std::getline(in, line) || rtrim(line) != "+++") {
        std::cerr << "WARNING: " << path.string() << ": no frontmatter found\n";
        return text;
    }

    // Search for closing frontmatter
    bool closed = false;
    while (std::getline(in, line)) {
        if (rtrim(line) == "+++") { closed = true; break; }
    }
    if (!closed) throw std::runtime_error("missing closing '+++' frontmatter delimiter");

    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

// Render one input .md file to .html, .gmi, and .gophermap
static bool processBlog(const fs::path& in, const fs::path& out_dir) {
    const std::string stem = in.stem().string();
    std::vector<fs::path> written;

    try {
        std::string markdown = stripFrontmatter(readFile(in), in);

        // Parse document to AST
        DocPtr doc(cmark_parse_document(markdown.data(), markdown.size(), CMARK_OPT_DEFAULT));
        if (!doc) throw std::runtime_error("cmark failed to parse the document");

        // Parse AST to desired formats (.html, .gmi, .gophermap)
        std::vector<std::pair<std::string, std::string>> outputs; // extension, rendered text
        { GeminiRenderer r; outputs.emplace_back(".gmi",       r.renderDocument(doc.get())); }
        { GopherRenderer r; outputs.emplace_back(".gophermap", r.renderDocument(doc.get())); }
        { HTMLRenderer   r; outputs.emplace_back(".html",      r.renderDocument(doc.get())); }

        // Write parsing results to output folder
        for (const auto& [ext, text] : outputs) {
            fs::path out = out_dir / (stem + ext);
            written.push_back(out);
            writeFile(out, text);
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << in.string() << ": " << e.what() << "\n";
        for (const fs::path& p : written) {
            std::error_code ec;
            fs::remove(p, ec);
        }
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <blogs_dir> <output_dir>\n";
        return 1;
    }
    fs::path blogs_dir = argv[1];
    fs::path out_dir   = argv[2];

    if (!fs::is_directory(blogs_dir)) {
        std::cerr << "ERROR: blogs directory not found: " << blogs_dir << "\n";
        return 1;
    }

    // Collect all files in input directory
    std::vector<fs::path> inputs;
    try {
        fs::create_directories(out_dir);
        for (const auto& entry : fs::directory_iterator(blogs_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".md")
                inputs.push_back(entry.path());
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
    std::sort(inputs.begin(), inputs.end());

    // Proccess all files
    int failures = 0;
    for (const fs::path& in : inputs) {
        if (!processBlog(in, out_dir)) ++failures;
    }

    // Output result
    std::cout << "Rendered " << inputs.size() << " posts, " << failures << " failed\n";
    return failures == 0 ? 0 : 1;
}