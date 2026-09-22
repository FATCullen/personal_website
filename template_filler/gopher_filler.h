#pragma once
#include "filler.h"

class GopherFiller : public Filler {
private:
    std::string fillBlogs() override final;

    std::string formatText(const std::string) override final;
};