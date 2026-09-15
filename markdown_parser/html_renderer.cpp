#include "html_renderer.h"

std::string HTMLRenderer::renderHeader() {
    std::string header = R"!!!(
<!DOCTYPE html>
<html lang="en" class="no-js">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Blog — FATCullen</title>
<script>document.documentElement.classList.remove('no-js');</script>
<link rel="stylesheet" href="/styles.css">
<link rel="stylesheet" href="/blog/blog.css">
<link rel="stylesheet" href="/blog/article.css">
</head>
<body class="scrollable has-fixed-header">
<header class="blog-header">
<nav class="blog-header__nav">
<a href="/blog/" class="blog-header__link" aria-current="page">Blog</a>
<a href="/" class="blog-header__link">Portfolio</a>
</nav>
<button class="icon-btn" id="theme-toggle" aria-label="Toggle light and dark mode" aria-pressed="false" disabled>
<span class="icon" aria-hidden="true" style="--icon-url: url('/images/icons/circle-half-stroke-solid-full.svg')"></span>
</button>
</header>
<main class="article">
    )!!!";
    return header;
}

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
    std::string footer = R"!!!(
<script type="module" src="/main.js" defer></script>
</main>
</body>
</html>
    )!!!";
    return footer;
}

std::string HTMLRenderer::renderDocument(cmark_node* doc) {
    std::string out;

    out += renderHeader();
    out += cmark_render_html(doc, CMARK_OPT_DEFAULT);
    out += renderHeader();

    return out;
}
