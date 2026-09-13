#include "gemini_renderer.h"
#include <algorithm>

namespace {
std::string collapseNewlines(std::string s) {
    std::replace(s.begin(), s.end(), '\n', ' ');
    return s;
}
}

GemRenderResult GeminiRenderer::renderChildren(cmark_node* node) {
    GemRenderResult out;
    for (cmark_node* c = cmark_node_first_child(node); c; c = cmark_node_next(c)) {
        GemRenderResult r = renderNode(c);
        out.text += r.text;
        out.links.insert(out.links.end(), r.links.begin(), r.links.end());
    }
    return out;
}

GemRenderResult GeminiRenderer::renderNode(cmark_node* node) {
    switch (cmark_node_get_type(node)) {

    case CMARK_NODE_TEXT: {
        const char* lit = cmark_node_get_literal(node);
        return { lit ? lit : "", {} };
    }
    case CMARK_NODE_SOFTBREAK:
        return { " ", {} };
    case CMARK_NODE_LINEBREAK:
        return { "\n", {} };

    case CMARK_NODE_STRONG: {
        auto r = renderChildren(node);
        for (auto& ch : r.text) ch = (char)std::toupper((unsigned char)ch);
        return r;
    }
    case CMARK_NODE_EMPH:
    case CMARK_NODE_ITEM:
        return renderChildren(node);

    case CMARK_NODE_CODE: {
        const char* lit = cmark_node_get_literal(node);
        return { "`" + std::string(lit ? lit : "") + "`", {} };
    }

    case CMARK_NODE_THEMATIC_BREAK:
        return { "----\n", {} };

    case CMARK_NODE_PARAGRAPH: {
        auto r = renderChildren(node);
        r.text = collapseNewlines(r.text) + "\n\n";
        return r;
    }
    case CMARK_NODE_HEADING:
        return renderHeading(node);
    case CMARK_NODE_LIST:
        return renderList(node);
    case CMARK_NODE_BLOCK_QUOTE:
        return renderBlockQuote(node);
    case CMARK_NODE_CODE_BLOCK:
        return renderCodeBlock(node);
    case CMARK_NODE_LINK:
        return renderLink(node);
    case CMARK_NODE_IMAGE:
        return renderImage(node);

    default:
        return renderChildren(node);
    }
}

GemRenderResult GeminiRenderer::renderHeading(cmark_node* node) {
    // Gemtext only defines #, ##, ### — deeper markdown headings collapse to ###.
    int level = std::min(3, (int)cmark_node_get_heading_level(node));
    GemRenderResult r = renderChildren(node);
    std::string prefix(level, '#');
    r.text = prefix + " " + collapseNewlines(r.text) + "\n\n";
    return r;
}

GemRenderResult GeminiRenderer::renderList(cmark_node* node) {
    GemRenderResult out;
    for (cmark_node* item = cmark_node_first_child(node); item; item = cmark_node_next(item)) {
        GemRenderResult r = renderNode(item);
        std::string text = collapseNewlines(r.text);
        out.text += "* " + text + "\n";
        out.links.insert(out.links.end(), r.links.begin(), r.links.end());
    }
    out.text += "\n";
    return out;
}

GemRenderResult GeminiRenderer::renderBlockQuote(cmark_node* node) {
    GemRenderResult out;
    for (cmark_node* c = cmark_node_first_child(node); c; c = cmark_node_next(c)) {
        GemRenderResult r = renderNode(c);
        std::string text = collapseNewlines(r.text);
        while (!text.empty() && text.back() == '\n' && text.size() > 1 && text[text.size()-2] == '\n')
            text.pop_back();
        if (!text.empty() && text.back() == '\n') text.pop_back();
        out.text += "> " + text + "\n";
        out.links.insert(out.links.end(), r.links.begin(), r.links.end());
    }
    out.text += "\n";
    return out;
}

GemRenderResult GeminiRenderer::renderCodeBlock(cmark_node* node) {
    const char* lit = cmark_node_get_literal(node);
    const char* info = cmark_node_get_fence_info(node);

    GemRenderResult r;
    r.text = "```" + std::string(info ? info : "") + "\n";
    r.text += (lit ? lit : "");
    if (r.text.back() != '\n') r.text += "\n";
    r.text += "```\n\n";
    return r;
}

GemRenderResult GeminiRenderer::renderLink(cmark_node* node) {
    GemRenderResult r = renderChildren(node);
    const char* url = cmark_node_get_url(node);

    GemLink link;
    link.url = url ? url : "";
    link.label = collapseNewlines(r.text);
    r.links.push_back(link);
    return r;
}

GemRenderResult GeminiRenderer::renderImage(cmark_node* node) {
    GemRenderResult altResult = renderChildren(node);
    std::string alt = altResult.text.empty() ? "Image" : collapseNewlines(altResult.text);
    const char* url = cmark_node_get_url(node);

    GemRenderResult r;
    GemLink link;
    link.url = url ? url : "";
    link.label = alt;
    r.links.push_back(link);
    return r;
}

std::string GeminiRenderer::flushText(std::string text) const {
    return text;
}

std::string GeminiRenderer::flushLinks(const std::vector<GemLink>& links) const {
    std::string out;
    for (auto& l : links)
        out += "=> " + l.url + " " + l.label + "\n";
    if (!links.empty()) out += "\n";
    return out;
}

std::string GeminiRenderer::renderHeader() {
    return "";
}

std::string GeminiRenderer::renderFooter() {
    return "";
}

std::string GeminiRenderer::renderDocument(cmark_node* doc) {
    std::string out;

    out += renderHeader();
    for (cmark_node* c = cmark_node_first_child(doc); c; c = cmark_node_next(c)) {
        GemRenderResult r = renderNode(c);
        out += flushText(r.text);
        out += flushLinks(r.links);
    }
    out += renderHeader();

    return out;
}
