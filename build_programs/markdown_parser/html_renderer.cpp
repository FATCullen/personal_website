#include "html_renderer.h"
#include "node_processing.h"

// Boilerplate header (with nav buttons and UI light/dark toggle)
std::string HTMLRenderer::renderHeader() {
    std::string header = 
R"!!!(<!DOCTYPE html>
<html lang="en" class="no-js">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Blog — FATCullen</title>
<script>document.documentElement.classList.remove('no-js');</script>
<link rel="stylesheet" href="/styles.css">
<link rel="stylesheet" href="/blog/blog.css">
<link rel="stylesheet" href="/blog/article.css">

<link rel="icon" type="image/png" href="/favicon/favicon-96x96.png" sizes="96x96" />
<link rel="icon" type="image/svg+xml" href="/favicon/favicon.svg" />
<link rel="shortcut icon" href="/favicon/favicon.ico" />
<link rel="apple-touch-icon" sizes="180x180" href="/favicon/apple-touch-icon.png" />
<link rel="manifest" href="/favicon/site.webmanifest" />
</head>
<body>
  <header id="site-header">
    <input class="nav-toggle" type="checkbox" id="nav-toggle" aria-label="Toggle navigation menu" aria-controls="site-nav">
    <label class="menu-toggle icon-btn" for="nav-toggle" aria-hidden="true">☰</label>
    <nav id="site-nav" aria-label="Main navigation">
      <a href="/" class="icon-btn">Home</a>
      <a href="/experience.html" class="icon-btn">Experience</a>
      <a href="/projects.html" class="icon-btn">Projects</a>
      <a href="/blog/" class="icon-btn">Blog</a>
    </nav>
    <a class="site-title" href="/">FATCullen</a>
    <div class="header-actions">
      <a class="icon-btn" href="/blog/feed.xml" aria-label="RSS feed">
        <span class="icon" aria-hidden="true" style="--icon-url: url('/images/icons/square-rss-solid-full.svg')"></span>
      </a>
      <button class="icon-btn" id="theme-toggle" aria-label="Toggle light and dark mode" aria-pressed="false" disabled>
        <span class="icon" aria-hidden="true" style="--icon-url: url('/images/icons/circle-half-stroke-solid-full.svg')"></span>
      </button>
    </div>
  </header>
<div class="page-content">
  <main id="main-content" class="article" tabindex="-1">)!!!";
    return header;
}

// Boilerplate footer
std::string HTMLRenderer::renderFooter() {
    std::string footer = 
R"!!!(<script type="module" src="/main.js" defer></script>
</main>
  </div>
</body>
</html>)!!!";
    return footer;
}

std::string HTMLRenderer::renderDocument(cmark_node* doc) {
    std::vector<NodeChange> changes;
    addImageCaptions(doc, changes);

    char* rendered = cmark_render_html(doc, CMARK_OPT_DEFAULT);
    std::string out = renderHeader() + rendered + renderFooter();

    std::free(rendered);
    restoreNodeChanges(changes);
    return out;
}
