#pragma once
#include <vector>
#include "cmark.h"

// Logs a change to a node, so it can be reverted later
struct NodeChange {
    cmark_node* original;
    cmark_node* replacement;
};

// Functions to find and apply alterations to certain html blocks
std::vector<cmark_node*> findNodes(cmark_node* doc, cmark_node_type type);
void addImageCaptions(cmark_node* doc, std::vector<NodeChange>& changes);
void restoreNodeChanges(std::vector<NodeChange>& changes);
