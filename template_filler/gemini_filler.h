#pragma once
#include "filler.h"

class GeminiFiller : public Filler {
private:
    std::string fillBlogs() override final;
};