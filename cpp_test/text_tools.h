#pragma once
#include <string>

namespace gtxt {
std::string hline(int width, const std::string& chars = "-");
std::string breakUp(const std::string& text, int width, int indent);
std::string padCenter(const std::string& text, int width);
std::string rightPad(const std::string& text, int width);
std::string box(const std::string& text, int width);
std::string bulletize(const std::string& text, const std::string& bullet);
std::string blockIndent(const std::string& text);
std::string underline(const std::string& text, const std::string& chars = "-");
}