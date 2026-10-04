#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "filler.h"
#include "html_filler.h"
#include "gemini_filler.h"
#include "gopher_filler.h"

namespace fs = std::filesystem;

// Helper for detecting file type
bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Helper to select filler based on file extension
static std::unique_ptr<Filler> makeFiller(const std::string& name, const Content& content) {
    if (endsWith(name, "html"))      return std::make_unique<HTMLFiller>(content);
    if (endsWith(name, "gmi"))       return std::make_unique<GeminiFiller>(content);
    if (endsWith(name, "gophermap")) return std::make_unique<GopherFiller>(content);
    return nullptr;
}

// Fill one template into its output location
static bool processTemplate(const fs::path& in, const fs::path& out, const Content& content) {
    try {
        fs::create_directories(out.parent_path());
 
        // Select filler, or copy over directly if no filling rule
        auto filler = makeFiller(in.filename().string(), content);
        if (!filler) { 
            fs::copy_file(in, out, fs::copy_options::overwrite_existing);
            return true;
        }
 
        // Create in and out streams
        std::ifstream file(in);
        if (!file.is_open()) {
            std::cerr << "ERROR: failed to open input file " << in << "\n";
            return false;
        }
        std::ofstream outfile(out);
        if (!outfile.is_open()) {
            std::cerr << "ERROR: failed to open output file " << out << "\n";
            return false;
        }
 
        // Attempt to process
        try {
            filler->processFile(file, outfile);
            return true;
        } catch (const toml::parse_error& e) {
            std::cerr << "ERROR: " << in.string() << ": TOML parse failure: " << e << "\n";
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << in.string() << ": " << e.what() << "\n";
        }
 
        // Failed, remove bad file
        outfile.close();
        fs::remove(out);
        return false;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << in.string() << ": " << e.what() << "\n";
        return false;
    }
}
 
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0]
                  << " <templates_dir> <build_dir> <contents_dir> <blogs_dir>\n";
        return 1;
    }
    fs::path templates_dir = argv[1];
    fs::path build_dir     = argv[2];
 
    if (!fs::is_directory(templates_dir)) {
        std::cerr << "ERROR: templates directory not found: " << templates_dir << "\n";
        return 1;
    }
 
    // Initialize content
    Content content(argv[3], argv[4]);
 
    // Collect first so processing order is deterministic
    std::vector<fs::path> inputs;
    for (const auto& entry : fs::recursive_directory_iterator(templates_dir)) {
        if (entry.is_regular_file()) inputs.push_back(entry.path());
    }
    std::sort(inputs.begin(), inputs.end());
 
    // Proccess all files
    int failures = 0;
    for (const fs::path& in : inputs) {
        fs::path out = build_dir / fs::relative(in, templates_dir);
        if (!processTemplate(in, out, content)) ++failures;
    }
 
    // Finish
    std::cout << "Processed " << inputs.size() << " templates, " << failures << " failed\n";
    return failures == 0 ? 0 : 1;
}
