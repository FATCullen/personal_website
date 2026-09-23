#pragma once
#include "filler.h"

class GeminiFiller : public Filler {
private:
    // Only overriding blogs for gemini
    std::string fillBlogs() override final;
};