#include "node_processing.h"
#include <cstdlib>
#include <string>

// Helper to find nodes of a given type
std::vector<cmark_node*> findNodes(cmark_node* doc, cmark_node_type type) {
    std::vector<cmark_node*> nodes;
    cmark_iter* iter = cmark_iter_new(doc);

    // Iterate through all nodes, pull out any that match given type
    while (cmark_iter_next(iter) != CMARK_EVENT_DONE) {
        cmark_node* node = cmark_iter_get_node(iter);
        if (cmark_iter_get_event_type(iter) == CMARK_EVENT_ENTER &&
            cmark_node_get_type(node) == type) {
            nodes.push_back(node);
        }
    }
    
    cmark_iter_free(iter);
    return nodes;
}

// Adds images alt text underneath as a caption
static void addImageCaption(cmark_node* image, std::vector<NodeChange>& changes) {
    // Render to HTML
    char* rendered = cmark_render_html(image, CMARK_OPT_DEFAULT);
    std::string html(rendered);
    std::free(rendered);

    // Grab alt text
    const auto start = html.find(" alt=\"");
    if (start == std::string::npos) return;
    const auto end = html.find('"', start + 6);
    const std::string caption = html.substr(start + 6, end - start - 6);
    if (caption.empty()) return;

    // Identify node type
    cmark_node* parent = cmark_node_parent(image);
    const bool standalone = cmark_node_get_type(parent) == CMARK_NODE_PARAGRAPH &&
        cmark_node_first_child(parent) == image && !cmark_node_next(image);
    cmark_node* original = standalone ? parent : image;
    cmark_node* replacement = cmark_node_new(
        standalone ? CMARK_NODE_CUSTOM_BLOCK : CMARK_NODE_CUSTOM_INLINE);

    // If image is standalone, make it a figure, if nested make image with caption
    const std::string markup = standalone
        ? "<figure>" + html + "<figcaption>" + caption + "</figcaption></figure>"
        : "<span class=\"captioned-image\">" + html +
          "<span class=\"image-caption\">" + caption + "</span></span>";

    // Change the node, and log the change to be reverted later
    cmark_node_set_on_enter(replacement, markup.c_str());
    cmark_node_replace(original, replacement);
    changes.push_back({original, replacement});
}

// Adds captions to all images
void addImageCaptions(cmark_node* doc, std::vector<NodeChange>& changes) {
    for (cmark_node* image : findNodes(doc, CMARK_NODE_IMAGE)) {
        addImageCaption(image, changes);
    }
}

// Reverses all changes, so future operations have unaltered node tree.  Goes in reverse order, in case of nesting
void restoreNodeChanges(std::vector<NodeChange>& changes) {
    // Loop through changes in reverse
    for (auto it = changes.rbegin(); it != changes.rend(); ++it) {
        cmark_node_replace(it->replacement, it->original);
        cmark_node_free(it->replacement);
    }
    changes.clear();
}
