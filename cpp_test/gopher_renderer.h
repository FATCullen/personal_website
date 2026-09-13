#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "cmark.h"

struct GopherLink {
    char type;
    std::string display;
    std::string selector;
    std::string host = "localhost";
    int port = 70;
};

struct RenderResult {
    std::string text;
    std::vector<GopherLink> links;
};

class GopherRenderer {
public:
    explicit GopherRenderer(int mainWidth = 67, int subWidth = 60)
        : mainWidth_(mainWidth), subWidth_(subWidth) {}

    std::string renderDocument(cmark_node* doc);

private:
    int mainWidth_;
    int subWidth_;

    RenderResult renderNode(cmark_node* node, int subWidth, bool isSub);
    RenderResult renderChildren(cmark_node* node, int subWidth, bool isSub);

    RenderResult renderParagraph(cmark_node* node, int subWidth, bool isSub);
    RenderResult renderHeading(cmark_node* node);
    RenderResult renderList(cmark_node* node, int subWidth, bool isSub);
    RenderResult renderBlockQuote(cmark_node* node, int subWidth, bool isSub);
    RenderResult renderCodeBlock(cmark_node* node, int subWidth, bool isSub);
    RenderResult renderLink(cmark_node* node, int subWidth, bool isSub);
    RenderResult renderImage(cmark_node* node);

    std::string flushText(std::string text) const;
    std::string flushLinks(const std::vector<GopherLink>& links) const;
};