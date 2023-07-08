//
// Created by Severin on 05.07.2023.
//

#include "ansi.hpp"

#include <regex>

#include <fmt/format.h>

std::string ansi::alternate::on() {
    return fmt::format("{}[?1049h", esc);
}

std::string ansi::alternate::off() {
    return fmt::format("{}[?1049l", esc);
}

std::string ansi::cursor::home() {
    return fmt::format("{}[H", esc);
}

std::string ansi::cursor::go(unsigned int row, unsigned int column) {
    return fmt::format("{}[{};{}f", esc, row, column);
}

std::string ansi::cursor::up(unsigned int lines) {
    return fmt::format("{}[{}A", esc, lines);
}

std::string ansi::cursor::down(unsigned int lines) {
    return fmt::format("{}[{}B", esc, lines);
}

std::string ansi::cursor::left(unsigned int columns) {
    return fmt::format("{}[{}D", esc, columns);
}

std::string ansi::cursor::right(unsigned int columns) {
    return fmt::format("{}[{}C", esc, columns);
}

std::string ansi::cursor::max() {
    return right(999) + down(999);
}

std::string ansi::cursor::hide() {
    return fmt::format("{}[?25l", esc);
}

std::string ansi::cursor::show() {
    return fmt::format("{}[?25h", esc);
}

std::string ansi::cursor::position() {
    return fmt::format("{}[6n", esc);
}

ansi::cursor::position_t ansi::cursor::get() {
    position_t result;
    std::string response;

    for (int i = 0; i < 32; ++i) {
        char c;
        try {
            c = char_utils::get();
        } catch (char_utils::io_exception& ex) {
            break;
        }

        response += c;
        if (c == 'R')
            break;
    }

    std::regex regex("[0-9]+");
    auto begin = std::sregex_iterator(response.begin(), response.end(), regex);
    auto end = std::sregex_iterator();

    if (std::distance(begin, end) != 1)
        throw std::runtime_error("can't get cursor position");

    std::sregex_iterator iter = begin;
    result.row = std::stoi(iter->str());
    ++iter;
    result.column = std::stoi(iter->str());

    return result;
}

std::string ansi::erase::line() {
    return fmt::format("{}[2K", esc);
}

std::string ansi::erase::screen() {
    return fmt::format("{}[2J", esc);
}

std::string ansi::colors::rgb::bg(unsigned char r, unsigned char g, unsigned char b) {
    return fmt::format("{}[48;2;{};{};{}m", esc, r, g, b);
}

std::string ansi::colors::rgb::fg(unsigned char r, unsigned char g, unsigned char b) {
    return fmt::format("{}[38;2;{};{};{}m", esc, r, g, b);
}

std::string
ansi::style(ansi::styles::set::code style, ansi::colors::small::bg::code bg, ansi::colors::small::fg::code fg) {
    return fmt::format("{}[{};{};{}m", esc, style(), bg(), fg());
}

std::string
ansi::style(ansi::styles::reset::code style, ansi::colors::small::bg::code bg, ansi::colors::small::fg::code fg) {
    return fmt::format("{}[{};{};{}m", esc, style(), bg(), fg());
}

std::string ansi::style(ansi::styles::set::code style) {
    return fmt::format("{}[{}m", esc, style());
}

std::string ansi::style(ansi::styles::reset::code style) {
    return fmt::format("{}[{}m", esc, style());
}

std::string ansi::color(ansi::colors::small::bg::code color) {
    return fmt::format("{}[{}m", esc, color());
}

std::string ansi::color(ansi::colors::small::fg::code color) {
    return fmt::format("{}[{}m", esc, color());
}

std::string ansi::color(ansi::colors::small::bg::code bg, ansi::colors::small::fg::code fg) {
    return fmt::format("{}[{};{}m", esc, bg(), fg());
}

std::string ansi::color(ansi::colors::big::code fg, ansi::colors::big::code bg) {
    return fmt::format("{}[38;5;{}m{}[48;5;{}m", esc, fg(), esc, bg());
}

std::string ansi::color(ansi::colors::big::code fg) {
    return fmt::format("{}[38;5;{}m", esc, fg());
}
