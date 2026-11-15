#include "table_renderer.h"
#include "cmark-gfm-core-extensions.h"
#include "../tabulate/tabulate.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>

// Normalize whitespace before wrapping
static std::string normalizeCell(const std::string& text) {
    std::istringstream input(text);
    std::string word, result;
    while (input >> word) {
        if (!result.empty()) result += ' ';
        result += word;
    }
    return result;
}

// Wrap individual cell.  Split word only when too long for collumn
static std::string wrapCell(const std::string& text, int width) {
    std::istringstream input(text);
    std::string word, line, output;
    auto flush = [&]() {
        if (!output.empty()) output += '\n';
        output += line;
        line.clear();
    };

    while (input >> word) {
        if (!line.empty() && (int)line.size() + 1 + (int)word.size() > width) flush();
        // Split word
        while ((int)word.size() > width) {
            bool hyphen = width > 1;
            size_t split = width - (hyphen ? 1 : 0);
            // Add hyphen if split
            line = word.substr(0, split) + (hyphen ? "-" : "");
            flush();
            word.erase(0, split);
        }
        if (!word.empty()) {
            if (!line.empty()) line += ' ';
            line += word;
        }
    }
    if (!line.empty() || output.empty()) flush();
    return output;
}

// Render table to ASCII.  Almost all of the logic is for determining collumn width, to keep table readable while fitting on screen
std::string renderTextTable(cmark_node* node,
    const std::vector<std::vector<std::string>>& input, int maxWidth) {
    if (input.empty()) return "";
    const int count = cmark_gfm_extensions_get_table_columns(node);
    if (!count) return "";
    auto rows = input;
    for (auto& row : rows) {
        row.resize(count);
        for (auto& cell : row) cell = normalizeCell(cell);
    }
    const uint8_t* alignments = cmark_gfm_extensions_get_table_alignments(node);
    std::vector<int> maximum(count, 1), widths(count, 1);
    for (int c = 0; c < count; ++c)
        for (const auto& row : rows)
            maximum[c] = std::max(maximum[c], (int)row[c].size());
    
    // Drop cell padding before squeezing the text into narrower columns.
    const int natural = std::accumulate(maximum.begin(), maximum.end(), 0);
    const int padding = natural + 3 * count + 1 <= maxWidth ? 1 : 0;
    const int budget = std::max(count, maxWidth - (2 * padding + 1) * count - 1);

    // Cache wrapping heights for every usable width
    std::vector<std::vector<std::vector<int>>> heights(count);
    for (int c = 0; c < count; ++c) {
        maximum[c] = std::min(maximum[c], budget);
        heights[c].resize(maximum[c] + 1);
        for (int w = 1; w <= maximum[c]; ++w) {
            for (const auto& row : rows) {
                std::string wrapped = wrapCell(row[c], w);
                heights[c][w].push_back(1 + (int)std::count(wrapped.begin(), wrapped.end(), '\n'));
            }
        }
    }

    // Minimize total row height first, then total cell height as a tie-breaker
    long long weight = 1;
    for (int c = 0; c < count; ++c)
        for (int height : heights[c][1]) weight += height;
    auto score = [&](const std::vector<int>& candidate) {
        long long total = 0, cells = 0;
        for (size_t r = 0; r < rows.size(); ++r) {
            int height = 1;
            for (int c = 0; c < count; ++c) {
                int h = heights[c][candidate[c]][r];
                height = std::max(height, h);
                cells += h;
            }
            total += height;
        }
        return total * weight + cells;
    };

    int spare = budget - std::accumulate(widths.begin(), widths.end(), 0);
    while (spare > 0) {
        long long current = score(widths), bestGain = 0;
        int bestColumn = -1, bestCost = 1;
        for (int c = 0; c < count; ++c) {
            for (int cost = 1; cost <= spare && widths[c] + cost <= maximum[c]; ++cost) {
                auto candidate = widths;
                candidate[c] += cost;
                long long gain = current - score(candidate);
                if (gain * bestCost > bestGain * cost) {
                    bestGain = gain;
                    bestColumn = c;
                    bestCost = cost;
                }
            }
        }
        if (bestColumn < 0) break;
        widths[bestColumn] += bestCost;
        spare -= bestCost;
    }

    // Improve the initial allocation by transferring space between columns
    while (true) {
        auto best = widths;
        long long bestScore = score(widths);
        for (int from = 0; from < count; ++from) {
            for (int to = 0; to < count; ++to) {
                if (from == to) continue;
                int limit = std::min(widths[from] - 1, maximum[to] - widths[to]);
                for (int amount = 1; amount <= limit; ++amount) {
                    auto candidate = widths;
                    candidate[from] -= amount;
                    candidate[to] += amount;
                    long long value = score(candidate);
                    if (value < bestScore) {
                        bestScore = value;
                        best = candidate;
                    }
                }
            }
        }
        if (best == widths) break;
        widths = best;
    }

    // Create table
    tabulate::Table table;
    for (const auto& row : rows) {
        tabulate::Table::Row_t cells;
        for (int c = 0; c < count; ++c)
            cells.push_back(wrapCell(row[c], widths[c]));
        table.add_row(cells);
    }

    // Epply padding and font alignment
    table.format().padding_left(padding).padding_right(padding);
    for (int c = 0; c < count; ++c) {
        auto alignment = tabulate::FontAlign::left;
        if (alignments && alignments[c] == 'c') alignment = tabulate::FontAlign::center;
        if (alignments && alignments[c] == 'r') alignment = tabulate::FontAlign::right;
        table.column(c).format().width(widths[c] + 2 * padding).font_align(alignment);
    }

    std::ostringstream output;
    output << table;
    return output.str() + "\n";
}

