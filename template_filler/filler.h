#pragma once
#include <string>
#include <fstream>

// Template for a filler, not intended for direct use
class Filler {
public:
    // Public entry point, fill INSERT lines with appropriate contents and write to output file
    void processFile(std::ifstream&, std::ofstream&);

private:
    // Plain text fields
    std::string fillAboutProfessional();
    std::string fillAboutPersonal();
    std::string fillFavouriteLanguages();
    std::string fillFavouriteFrameworks();
    std::string fillSkills();
    std::string fillGPA();

    // List fields (ie vectors of entries)
    virtual std::string fillProjects();
    virtual std::string fillWorks();
    virtual std::string fillWebBadges();
    virtual std::string fillBlogs();

    // Generic plain text formatter helper
    virtual std::string formatText(const std::string);
};