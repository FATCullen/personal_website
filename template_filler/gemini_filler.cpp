#include "gemini_filler.h"
#include "contents.h"

std::string GeminiFiller::fillBlogs() {
    std::string result = "";

    for (Blog blog : BLOGS) {
        result += "=> /blog/" + blog.file_base + ".gmi " + blog.title + " - " + blog.date + "\n";
    }

    return result;
}