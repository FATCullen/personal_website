#pragma once
#include "filler.h"

class GopherFiller : public Filler {
private:
    std::string fillBlogs() override final;
};