#pragma once
#include <string>
#include <vector>

struct Link {
    std::string display;
    std::string value;
};

struct Project {
    std::string img;
    std::string title;
    std::string description;
    std::vector<Link> links;
};

struct Work {
    std::string img;
    std::string title;
    std::string dates;
    std::string description;
};

struct WebBadge {
    std::string img;
    std::string value;
};

const extern std::string ABOUT_PROFESSIONAL;
const extern std::string ABOUT_PERSONAL;

const extern std::string FAVOURITE_LANGUAGES;
const extern std::string FAVOURITE_FRAMEWORKS;
const extern std::string SKILLS;

const extern std::string GPA;

const extern std::vector<Project> PROJECTS;

const extern std::vector<Work> WORKS;

const extern std::vector<WebBadge> WEB_BADGES;

