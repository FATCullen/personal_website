#pragma once
#include <string>
#include "cmark-gfm.h"

class HTMLRenderer {
public:
    std::string renderDocument(cmark_node* doc, cmark_llist* extensions);
    
private:
    std::string renderHeader();
    std::string renderFooter();
};