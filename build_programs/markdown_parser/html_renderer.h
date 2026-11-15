#pragma once
#include <string>
#include "cmark-gfm.h"

class HTMLRenderer {
public:
    std::string renderDocument(cmark_node* doc);
    
private:
    std::string renderHeader();
    std::string renderFooter();
};