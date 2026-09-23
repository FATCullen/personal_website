#include "gopher_renderer.h"
#include "text_tools.h"
#include <cctype>

// Calls render on all children of a node, returns the result
// Most formatting rules first call this on their own node to get inner content, then apply their own formatting to that
RenderResult GopherRenderer::renderChildren(cmark_node* node, int subWidth, bool isSub) {
    RenderResult out;
    for (cmark_node* c = cmark_node_first_child(node); c; c = cmark_node_next(c)) {
        RenderResult r = renderNode(c, subWidth, isSub);
        out.text += r.text;
        out.links.insert(out.links.end(), r.links.begin(), r.links.end());
    }
    return out;
}

// Renders a node, calls a different formatting rule depending on the type of markdown element (via cmark ast)
RenderResult GopherRenderer::renderNode(cmark_node* node, int subWidth, bool isSub) {
    switch (cmark_node_get_type(node)) {

    case CMARK_NODE_TEXT: {
        const char* lit = cmark_node_get_literal(node);
        return { lit ? lit : "", {} };
    }
    case CMARK_NODE_SOFTBREAK:
        return { " ", {} };
    case CMARK_NODE_LINEBREAK:
        return { "\n", {} };
    case CMARK_NODE_CODE: {
        const char* lit = cmark_node_get_literal(node);
        return { "`" + std::string(lit ? lit : "") + "`", {} };
    }
    case CMARK_NODE_THEMATIC_BREAK:
        return { gtxt::hline(mainWidth_ - 2, "O="), {} };

    case CMARK_NODE_STRONG: {
        auto r = renderChildren(node, subWidth, isSub);
        for (auto& ch : r.text) ch = (char)std::toupper((unsigned char)ch);
        return r;
    }
    case CMARK_NODE_EMPH:
    case CMARK_NODE_ITEM:
        return renderChildren(node, subWidth, isSub);

    case CMARK_NODE_PARAGRAPH:
        return renderParagraph(node, subWidth, isSub);
    case CMARK_NODE_HEADING:
        return renderHeading(node);
    case CMARK_NODE_LIST:
        return renderList(node, subWidth, isSub);
    case CMARK_NODE_BLOCK_QUOTE:
        return renderBlockQuote(node, subWidth, isSub);
    case CMARK_NODE_CODE_BLOCK:
        return renderCodeBlock(node, subWidth, isSub);
    case CMARK_NODE_LINK:
        return renderLink(node, subWidth, isSub);
    case CMARK_NODE_IMAGE:
        return renderImage(node);

    default:
        return renderChildren(node, subWidth, isSub);
    }
}

// indented, and wrap at char limit
RenderResult GopherRenderer::renderParagraph(cmark_node* node, int subWidth, bool isSub) {
    auto r = renderChildren(node, subWidth, isSub);
    r.text = gtxt::breakUp(r.text, isSub ? subWidth : mainWidth_, 2);
    if (!isSub) r.text += "\n";
    return r;
}

// Heading, either enderlined or boxed depending on level
RenderResult GopherRenderer::renderHeading(cmark_node* node) {
    int level = cmark_node_get_heading_level(node);
    RenderResult inner = renderChildren(node, mainWidth_, false);

    RenderResult r;
    r.links = inner.links;
    if (level == 1) {
        std::string boxed = gtxt::box(inner.text, (int)inner.text.size() + 4);
        r.text = gtxt::padCenter(boxed, mainWidth_);
    } else {
        r.text = gtxt::underline(inner.text);
    }
    return r;
}

// List, indented with bullet or number before each item
RenderResult GopherRenderer::renderList(cmark_node* node, int subWidth, bool isSub) {
    bool ordered = cmark_node_get_list_type(node) == CMARK_ORDERED_LIST;
    int index = ordered ? cmark_node_get_list_start(node) : 0;

    RenderResult out;
    for (cmark_node* item = cmark_node_first_child(node); item; item = cmark_node_next(item)) {
        std::string label = ordered ? (std::to_string(index) + ".") : std::string("-");
        int itemWidth = subWidth - (3 + (int)label.size());

        RenderResult r = renderNode(item, itemWidth, true);
        while (r.text.back() == '\n') r.text.pop_back();
        
        out.text += gtxt::bulletize(r.text, label);
        out.links.insert(out.links.end(), r.links.begin(), r.links.end());
        ++index;
    }
    if (!isSub) {
        out.text = gtxt::rightPad(out.text, subWidth);
        out.text = gtxt::padCenter(out.text, mainWidth_);
    }
    return out;
}

// Block quote, indented with | along left edge
RenderResult GopherRenderer::renderBlockQuote(cmark_node* node, int subWidth, bool isSub) {
    RenderResult out;
    for (cmark_node* c = cmark_node_first_child(node); c; c = cmark_node_next(c)) {
        RenderResult r = renderNode(c, subWidth - 4, true);
        out.text += gtxt::blockIndent(r.text);
        out.links.insert(out.links.end(), r.links.begin(), r.links.end());
    }
    if (!isSub) {
        out.text = gtxt::rightPad(out.text, subWidth);
        out.text = gtxt::padCenter(out.text, mainWidth_);
    }
    return out;
}

// Code block, contained within a box
RenderResult GopherRenderer::renderCodeBlock(cmark_node* node, int subWidth, bool isSub) {
    const char* lit = cmark_node_get_literal(node);
    RenderResult r;
    r.text = gtxt::breakUp(lit ? lit : "", subWidth - 4, 0);
    r.text = gtxt::box(r.text, subWidth);
    if (!isSub) {
        r.text = gtxt::rightPad(r.text, subWidth);
        r.text = gtxt::padCenter(r.text, mainWidth_);
    }
    return r;
}

// Adds a link with html gopher tag
RenderResult GopherRenderer::renderLink(cmark_node* node, int subWidth, bool isSub) {
    RenderResult r = renderChildren(node, subWidth, isSub);
    const char* url = cmark_node_get_url(node);

    GopherLink link;
    link.type = 'h';
    link.display = "\"" + r.text + "\"";
    link.selector = std::string("URL:") + (url ? url : "");
    r.links.push_back(link);
    return r;
}

// Adds a link with an image tag
RenderResult GopherRenderer::renderImage(cmark_node* node) {
    RenderResult altResult = renderChildren(node, mainWidth_, false);
    std::string alt = altResult.text.empty() ? "Image" : altResult.text;
    const char* url = cmark_node_get_url(node);

    RenderResult r;
    GopherLink link;
    link.type = 'I';
    link.display = alt;
    link.selector = url ? url : "";
    r.links.push_back(link);
    return r;
}

// Applies default gopher info line formatting to text
std::string GopherRenderer::flushText(std::string text) const {
    std::string out;
    std::stringstream ss(text);
    std::string line;

    while (std::getline(ss, line)) {
        out += "i";
        out += line;
        out += "\tfake\t(NULL)\t0\r\n";
    }
    return out;
}

// Outputs links with appropriate tag and port info
std::string GopherRenderer::flushLinks(const std::vector<GopherLink>& links) const {
    std::string out;
    for (auto& l : links) {
        out += std::string(1, l.type) + l.display + "\t" + l.selector
             + "\t" + l.host + "\t" + std::to_string(l.port) + "\n";
    }
    return out;
}

// Button to return to main page
std::string GopherRenderer::renderHeader() {
    return "1Home\t/\n";
}
// Empty footer
std::string GopherRenderer::renderFooter() {
    return "";
}
// Main entry point, adds defualt header + footer and each fromatted node
// Links are flushed after each node returns (so for example, after each paragraph)
std::string GopherRenderer::renderDocument(cmark_node* doc) {
    std::string out;

    out += renderHeader();
    for (cmark_node* c = cmark_node_first_child(doc); c; c = cmark_node_next(c)) {
        RenderResult r = renderNode(c, subWidth_, false);
        out += flushText(r.text);
        out += flushLinks(r.links);
    }
    out += renderFooter();

    return out;
}