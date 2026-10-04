#pragma once
#include "filler.h"

class HTMLFiller : public Filler {
public:
    using Filler::Filler;
private:
    // Override all custom fields for HTML (it uses them all)
    std::string fillProjects() override final;
    std::string fillWorks() override final;
    std::string fillWebBadges() override final;
    std::string fillBlogs() override final;
};