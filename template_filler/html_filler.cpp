#include "html_filler.h"
#include "contents.h"

std::string HTMLFiller::fillProjects() {
    std::string result = "";

    for (Project project : PROJECTS) {
        result += "<article class=\"project-card\">\n<div class=\"project-card__image\">\n";
        result += "<img src=\"" + project.img + "\" alt="">\n</div>\n";
        result += "<div class=\"project-card__body\">\n<span class=\"project-card__title\">" + project.title + "</span>\n";
        result += "<p class=\"project-card__desc\">" + project.description + "</p>\n";
        result += "<div class=\"project-card__links\">\n";
        for (Link link : project.links) result += "<a href=\"" + link.value + "\">" + link.display + "</a>\n";
        result += "</div>\n</div>\n</article>\n";
    }

    return result;
}

std::string HTMLFiller::fillWorks() {
    std::string result = "";

    for (Work work : WORKS) {
        result += "<div class=\"entry\">\n";
        result += "<div class=\"entry__logo\"><img src=\"" + work.img + "\" alt=\"\" aria-hidden=\"true\"></div>\n";
        result += "<div class=\"entry__body\">\n<div class=\"entry__header\">\n";
        result += "<span class=\"entry__title\">" + work.title + "</span>\n";
        result += "<span class=\"entry__dates\">" + work.dates + "</span>\n";
        result += "</div>\n<p class=\"entry__desc\">" + work.description + "</p>\n";
        result += "</div>\n</div>\n";
    }

    return result;
}

std::string HTMLFiller::fillWebBadges() {
    std::string result = "";

    for (WebBadge badge : WEB_BADGES) {
        result  += "a class=\"badge\" target=\"_blank\" href=\"" + badge.value + "\"><img src=\"" + badge.img + "\"></img></a>\n";
    }

    return result;
}

std::string HTMLFiller::fillBlogs() {
    std::string result = "";

    for (Blog blog : BLOGS) {
        result += "<li class=\"post-item\" data-title=\"" + blog.title + "\" data-tags=\"";
        for (std::string tag : blog.tags) result += tag + " ";
        result += "\" data-date=\"" + blog.date + "\">\n";
        result += "<a href=\"/blog/" + blog.file_base + "\">\n";
        result += "<span class=\"post-item__date\">" + blog.date + "</span>\n";
        result += "<span class=\"post-item__title\">" + blog.title + "</span>\n";
        result += "<span class=\"post-item__tags\">\n";
        for (std::string tag : blog.tags) result += "<span class=\"tag\">" + tag + "</span>\n";
        result += "</span>\n</a>\n</li>\n";
    }

    return result;
}