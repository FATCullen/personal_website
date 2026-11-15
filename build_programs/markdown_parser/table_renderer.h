#pragma once
#include <string>
#include <vector>
#include "cmark-gfm.h"

// Render table to ASCII
std::string renderTextTable(cmark_node* node,
    const std::vector<std::vector<std::string>>& rows, int maxWidth);
