#pragma once
#include <string>
#include <vector>
#include "cmark.h"

struct GemLink {
    std::string url;
    std::string label;
};

struct GemRenderResult {
    std::string text;
    std::vector<GemLink> links;
};

class GeminiRenderer {
public:
    std::string renderDocument(cmark_node* doc);

private:
    GemRenderResult renderNode(cmark_node* node);
    GemRenderResult renderChildren(cmark_node* node);

    GemRenderResult renderParagraph(cmark_node* node);
    GemRenderResult renderHeading(cmark_node* node);
    GemRenderResult renderList(cmark_node* node);
    GemRenderResult renderBlockQuote(cmark_node* node);
    GemRenderResult renderCodeBlock(cmark_node* node);
    GemRenderResult renderLink(cmark_node* node);
    GemRenderResult renderImage(cmark_node* node);

    std::string flushText(std::string text) const;
    std::string flushLinks(const std::vector<GemLink>& links) const;

    std::string renderHeader();
    std::string renderFooter();
};