#include "gopher_filler.h"
#include "contents.h"

std::string GopherFiller::fillBlogs() {
    std::string result = "";

    for (Blog blog : BLOGS) {
        result += "1" + blog.title + " - " + blog.date + "\t/blog/" + blog.file_base + ".gophermap\n";
    }

    return result;
}