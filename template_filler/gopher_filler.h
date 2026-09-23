#pragma once
#include "filler.h"

class GopherFiller : public Filler {
private:
    // Only overriding blogs for gopher
    std::string fillBlogs() override final;

    // Goper plain text needs custom formatting (ie port, host, tabs)
    std::string formatText(const std::string) override final;
};