#pragma once
#include "filler.h"

class HTMLFiller : public Filler {
private:
    std::string fillProjects() override final;
    std::string fillWorks() override final;
    std::string fillWebBadges() override final;
    std::string fillBlogs() override final;
};