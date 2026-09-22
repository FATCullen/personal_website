#pragma once
#include <string>
#include <fstream>

class Filler {
public:
    void processFile(std::ifstream&, std::ofstream&);

private:
    std::string fillAboutProfessional();
    std::string fillAboutPersonal();
    std::string fillFavouriteLanguages();
    std::string fillFavouriteFrameworks();
    std::string fillSkills();
    std::string fillGPA();

    virtual std::string fillProjects();
    virtual std::string fillWorks();
    virtual std::string fillWebBadges();
    virtual std::string fillBlogs();

    virtual std::string formatText(const std::string);
};