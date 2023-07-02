#include <iostream>

#include <fmt/format.h>

#define iuchar inline unsigned char

namespace ansi {
    namespace styles {
        namespace set {
            iuchar bold = 1;
            iuchar faint = 2;
            iuchar italic = 3;
            iuchar underline = 4;
            iuchar blinking = 5;
            iuchar reverse = 6;
            iuchar invisible = 7;
            iuchar strikethrough = 8;
        }
        namespace reset {
            iuchar bold = 22;
            iuchar faint = 22;
            iuchar italic = 23;
            iuchar underline = 24;
            iuchar blinking = 25;
            iuchar reverse = 26;
            iuchar invisible = 27;
            iuchar strikethrough = 28;
        }
    }
    namespace colors {
        namespace small {
            namespace fg {
                iuchar black = 30;
                iuchar red = 31;
                iuchar green = 32;
                iuchar yellow = 33;
                iuchar blue = 34;
                iuchar magenta = 35;
                iuchar cyan = 36;
                iuchar white = 37;
            }
            namespace bg {
                iuchar black = 40;
                iuchar red = 41;
                iuchar green = 42;
                iuchar yellow = 43;
                iuchar blue = 44;
                iuchar magenta = 45;
                iuchar cyan = 46;
                iuchar white = 47;
            }

            iuchar reset = 0;
        }
        namespace big {

        }
        namespace rgb {

        }
    }

    std::string color(unsigned char color) {
        return fmt::format("{}[{}m", "\u001b", color);
    }

    std::string style(unsigned char style) {
        return fmt::format("{}[{}m", "\u001b", style);
    }
}

int main() {
    using namespace ansi;

    std::cout << color(colors::small::fg::red) << style(styles::set::italic) << "Hello, World!"
              << color(colors::small::reset) << style(styles::reset::italic) << std::endl;
    return 0;
}
