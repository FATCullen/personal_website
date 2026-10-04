#include "html_filler.h"

// Default HTML templates for each field, populated with info for each item in the lists from contents.cpp
std::string HTMLFiller::fillProjects() {
    std::string result = "";

    eachTable(content_.at("projects", "projects"), [&](const toml::table& project) {
        result += "<article class=\"project-card\">\n<div class=\"project-card__image\">\n";
        result += "<img src=\"" + str(project["image"]) + "\" alt="">\n</div>\n";
        result += "<div class=\"project-card__body\">\n<span class=\"project-card__title\">" + str(project["title"]) + "</span>\n";
        result += "<p class=\"project-card__desc\">" + str(project["description"]) + "</p>\n";
        result += "<div class=\"project-card__links\">\n";
        eachTable(project["links"], [&](const toml::table& link) {
            result += "<a href=\"" + str(link["url"]) + "\">" + str(link["label"]) + "</a>\n";
        });
        result += "</div>\n</div>\n</article>\n";
    });

    return result;
}

std::string HTMLFiller::fillWorks() {
    std::string result = "";

    eachTable(content_.at("works", "works"), [&](const toml::table& work) {
        result += "<div class=\"entry\">\n";
        result += "<div class=\"entry__logo\"><img src=\"" + str(work["image"]) + "\" alt=\"\" aria-hidden=\"true\"></div>\n";
        result += "<div class=\"entry__body\">\n<div class=\"entry__header\">\n";
        result += "<span class=\"entry__title\">" + str(work["title"]) + "</span>\n";
        result += "<span class=\"entry__dates\">" + str(work["dates"]) + "</span>\n";
        result += "</div>\n<p class=\"entry__desc\">" + str(work["description"]) + "</p>\n";
        result += "</div>\n</div>\n";
    });

    return result;
}

std::string HTMLFiller::fillWebBadges() {
    std::string result = "";

    eachTable(content_.at("webbadges", "webbadges"), [&](const toml::table& badge) {
        result  += "<a class=\"badge\" target=\"_blank\" href=\"" + str(badge["value"]) + "\"><img src=\"" + str(badge["image"]) + "\"></img></a>\n";
    });

    return result;
}

std::string HTMLFiller::fillBlogs() {
    std::string result = "";

    for (const toml::table& blog : content_.blogs()) {
        result += "<li class=\"post-item\" data-title=\"" + str(blog["title"]) + "\" data-tags=\"";
        eachTable(blog["tags"], [&](const toml::table& tag) {
            result += str(tag["tag"]) + " ";
        });
        result += "\" data-date=\"" + str(blog["date"]) + "\">\n";
        result += "<a href=\"/blog/" + str(blog["file_base"]) + ".html\">\n";
        result += "<span class=\"post-item__date\">" + str(blog["date"]) + "</span>\n";
        result += "<span class=\"post-item__title\">" + str(blog["title"]) + "</span>\n";
        result += "<span class=\"post-item__tags\">\n";
        eachTable(blog["tags"], [&](const toml::table& tag) {
            result += "<span class=\"tag\">" + str(tag["tag"]) + "</span>\n";
        });
        result += "</span>\n</a>\n</li>\n";
    }

    return result;
}