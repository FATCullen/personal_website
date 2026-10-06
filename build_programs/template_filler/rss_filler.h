#pragma once
#include "filler.h"

class RSSFiller : public Filler {
public:
    using Filler::Filler;
private:
    // Only overriding RSS Items for RSS
    std::string fillRSSItems() override final;
};