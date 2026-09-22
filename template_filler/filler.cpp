#include "filler.h"
#include "contents.h"

void Filler::processFile(std::ifstream& file, std::ofstream& out) {    
    std::string line;
    std::string prefix = "INSERT_";
    while (std::getline(file, line)) {
        if (line.size() >= prefix.size() && line.compare(0, prefix.size(), prefix) == 0){
            if      (line == "INSERT_ABOUT_PROFESSIONAL") 
                out << fillAboutProfessional() + "\n";
            else if (line == "INSERT_ABOUT_PERSONAL") 
                out << fillAboutPersonal() + "\n";
            else if (line == "INSERT_FAVOURITE_LANGUAGES") 
                out << fillFavouriteLanguages() + "\n";
            else if (line == "INSERT_FAVOURITE_FRAMEWORKS") 
                out << fillFavouriteFrameworks() + "\n";
            else if (line == "INSERT_SKILLS") 
                out << fillSkills() + "\n";
            else if (line == "INSERT_GPA") 
                out << fillGPA() + "\n";
            else if (line == "INSERT_PROJECTS") 
                out << fillProjects() + "\n";
            else if (line == "INSERT_WORKS") 
                out << fillWorks() + "\n";
            else if (line == "INSERT_WEB_BADGES") 
                out << fillWebBadges() + "\n";
            else if (line == "INSERT_BLOGS") 
                out << fillBlogs() + "\n";
        }
        else out << line + "\n";
    }

    out.close();
}

std::string Filler::fillAboutProfessional() {
    return formatText(ABOUT_PROFESSIONAL);
}
std::string Filler::fillAboutPersonal() {
    return formatText(ABOUT_PERSONAL);
}
std::string Filler::fillFavouriteLanguages() {
    return formatText(FAVOURITE_LANGUAGES);
}
std::string Filler::fillFavouriteFrameworks() {
    return formatText(FAVOURITE_FRAMEWORKS);
}
std::string Filler::fillSkills() {
    return formatText(SKILLS);
}
std::string Filler::fillGPA() {
    return formatText(GPA);
}
std::string Filler::fillProjects() {
    return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION";
}
std::string Filler::fillWorks() {
    return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION";
}
std::string Filler::fillWebBadges() {
    return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION";
}
std::string Filler::fillBlogs() {
    return "FILLER FOR THIS WEBSITE FORMAT DOES NOT SUPPORT THIS OPTION";
}

std::string Filler::formatText(std::string text) {
    return text;
}