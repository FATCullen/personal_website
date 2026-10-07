#include "html_renderer.h"

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
<body class="scrollable has-fixed-header">
<header class="blog-header">
<nav class="blog-header__nav">
<a href="/blog/" class="blog-header__link">Blog</a>
<a href="/" class="blog-header__link">Portfolio</a>
</nav>
<div>
<button class="icon-btn" aria-label="Navigate to RSS feed" aria-pressed="true">
<a href="/blog/feed.xml" target="_blank">
<span class="icon" aria-hidden="true" style="--icon-url: url('/images/icons/square-rss-solid-full.svg')"></span>
</a>
</button>
<button class="icon-btn" id="theme-toggle" aria-label="Toggle light and dark mode" aria-pressed="true">
<span class="icon" aria-hidden="true" style="--icon-url: url('/images/icons/circle-half-stroke-solid-full.svg')"></span>
</button>
</div>
</header>
<main class="article">)!!!";
    return header;
}

// Boilerplate footer
std::string HTMLRenderer::renderFooter() {
    // R"!!!(
    // <script type="module" src="/main.js" defer></script>
    // </main>
    // <footer class="article-footer">
    // <hr class="article-footer__rule">
    // <p class="article-footer__text">&copy; 2026 Finn Cullen. All rights reserved.</p>
    // </footer>
    // </body>
    // </html>
    // )!!!";
    std::string footer = 
R"!!!(<script type="module" src="/main.js" defer></script>
</main>
</body>
</html>)!!!";
    return footer;
}

// Uses cmark's built in html renderer
std::string HTMLRenderer::renderDocument(cmark_node* doc) {
    std::string out;

    out += renderHeader();
    out += cmark_render_html(doc, CMARK_OPT_DEFAULT);
    out += renderFooter();

    return out;
}
