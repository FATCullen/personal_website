#include "gemini_filler.h"

std::string GeminiFiller::fillBlogs() {
    std::string result;
 
    // Format to Gemini link syntax '=> link label'
    for (const toml::table& blog : content_.blogs()) {
        result += "=> /blog/" + str(blog["file_base"]) + ".gmi " +
                  str(blog["title"]) + " - " + str(blog["date"]) + "\n";
    }
 
    return result;
}