//
// Created by Severin on 05.07.2023.
//

#include "terminal.hpp"

terminal::terminal() {
    setup();
}

terminal::~terminal() {
    shutdown();
}

void terminal::setup() {
    saveOriginal();
    loadRaw();
    char_utils::print(alternate::on());
}

void terminal::render(const std::string& buffer) {
    window = getWindow();

    std::string sequence;

    sequence += ansi::cursor::hide();

    sequence += erase::screen();
    sequence += cursor::home();
    sequence += buffer;
    sequence += ansi::cursor::go(position.row, position.column);

    if (params.cursor_visible)
        sequence += ansi::cursor::show();

    char_utils::print(sequence);
}

void terminal::shutdown() {
    loadOriginal();
    if(!params.cursor_visible)
        char_utils::print(cursor::show());
    char_utils::print(alternate::off());
}

ansi::cursor::position_t terminal::getCursorPosition() {
    char_utils::print(cursor::position());
    return cursor::get();
}

terminal::window_t terminal::getWindow() {
    winsize ws{};

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        char_utils::print(cursor::max());

        ansi::cursor::position_t p = getCursorPosition();
        return {(size_t) p.row, (size_t) p.column};
    }

    return {ws.ws_row, ws.ws_col};
}

void terminal::loadRaw() {
    if (!isatty(STDIN_FILENO))
        throw terminal_exception("not on a tty");

    termios raw = orig_;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
        throw terminal_exception("can't set raw mode");
}

void terminal::loadOriginal() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_);
}

void terminal::saveOriginal() {
    if (tcgetattr(STDIN_FILENO, &orig_) < 0)
        throw terminal_exception("can't get tty settings");
}
