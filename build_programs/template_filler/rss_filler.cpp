#include "rss_filler.h"

#include <ctime>

// Helper to convert yyyy-mm-dd time to RFC 822 for RSS
std::string convertToRfc822(const std::string& yyyymmdd) {
    std::tm tm = {};
    int year = 0, month = 0, day = 0;
    // Scan year, month day from input
    if (sscanf(yyyymmdd.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        return "";
    }
    
    // Set up an populate std::tm struct
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = 1;
    std::mktime(&tm);

    // Format result to RFC 822
    char buffer[50];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S +0000", &tm);
    
    return std::string(buffer);
}

std::string RSSFiller::fillRSSItems() {
    std::string result;
 
    // Format to RSS feed item
    for (const toml::table& blog : content_.blogs()) {
        result += "<item>\n";
        result += "<title>" + str(blog["title"]) + "</title>\n";
        result += "<pubDate>" + convertToRfc822(str(blog["date"])) + "</pubDate>\n";
        result += "<link>https://fatcullen.com/blog/" + str(blog["file_base"]) + ".html</link>\n";
        result += "<guid>https://fatcullen.com/blog/" + str(blog["file_base"]) + ".html</guid>\n";
        eachTable(blog["tags"], [&](const toml::table& tag) {
            result += "<category>" + str(tag["tag"]) + "</category>\n";
        });
        result += "</item>\n";
    }
 
    return result;
}