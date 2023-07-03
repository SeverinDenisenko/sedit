//
// Created by Severin on 03.07.2023.
//

#ifndef SEDIT_ANSI_HPP
#define SEDIT_ANSI_HPP

#include <fmt/format.h>

namespace ansi {
    inline const char* esc = "\u001b";

    class code {
    public:
        explicit code(unsigned char value) : value(value) {}

        unsigned char operator()() const { return value; }

    private:
        unsigned char value;
    };

    namespace cursor {
        [[deprecated]] std::string home() {
            return fmt::format("{}[{}", esc, "H");
        }

        std::string go(unsigned int line, unsigned int column) {
            return fmt::format("{}[{};{}{}", esc, line, column, "f");
        }

        std::string up(unsigned int lines = 1) {
            return fmt::format("{}[{}{}", esc, lines, "A");
        }

        std::string down(unsigned int lines = 1) {
            return fmt::format("{}[{}{}", esc, lines, "B");
        }

        std::string left(unsigned int columns = 1) {
            return fmt::format("{}[{}{}", esc, columns, "D");
        }

        std::string right(unsigned int columns = 1) {
            return fmt::format("{}[{}{}", esc, columns, "C");
        }
    }
    namespace erase {
        std::string line() {
            return fmt::format("{}[{}{}", esc, 2, "K");
        }

        std::string screen() {
            return fmt::format("{}[{}{}", esc, 2, "J");
        }
    }
    namespace styles {
        namespace set {
            class code : public ansi::code {
            public:
                explicit code(unsigned char value) : ansi::code(value) {}
            };

            code bold(1);
            code faint(2);
            code italic(3);
            code underline(4);
            code blinking(5);
            code reverse(6);
            code invisible(7);
            code strikethrough(8);
        }
        namespace reset {
            class code : public ansi::code {
            public:
                explicit code(unsigned char value) : ansi::code(value) {}
            };

            code bold(22);
            code faint(22);
            code italic(23);
            code underline(24);
            code blinking(25);
            code reverse(26);
            code invisible(27);
            code strikethrough(28);
        }
    }
    namespace colors {
        namespace small {
            namespace fg {
                class code : public ansi::code {
                public:
                    explicit code(unsigned char value) : ansi::code(value) {}
                };

                code black(30);
                code red(31);
                code green(32);
                code yellow(33);
                code blue(34);
                code magenta(35);
                code cyan(36);
                code white(37);
                code reset(0);
            }
            namespace bg {
                class code : public ansi::code {
                public:
                    explicit code(unsigned char value) : ansi::code(value) {}
                };

                code black(40);
                code red(41);
                code green(42);
                code yellow(43);
                code blue(44);
                code magenta(45);
                code cyan(46);
                code white(47);
                code reset(0);
            }
        }
        namespace big {
            class code : public ansi::code {
            public:
                explicit code(unsigned char value) : ansi::code(value) {}
            };

            code Black(0);
            code Maroon(1);
            code Green(2);
            code Olive(3);
            code Navy(4);
            code Purple(5);
            code Teal(6);
            code Silver(7);
            code Grey(8);
            code Red(9);
            code Lime(10);
            code Yellow(11);
            code Blue(12);
            code Fuchsi(13);
            code Aqua(14);
            code White(15);
            code Grey0(16);
            code NavyBlue(17);
            code DarkBlue(18);
            code Blue3(19);
            code Blue3A(20);
            code Blue1(21);
            code DarkGreen(22);
            code DeepSkyBlue4(23);
            code DeepSkyBlue4B(24);
            code DeepSkyBlue4A(25);
            code DodgerBlue3(26);
            code DodgerBlue2(27);
            code Green4(28);
            code SpringGreen4(29);
            code Turquoise4(30);
            code DeepSkyBlue3(31);
            code DeepSkyBlue3A(32);
            code DodgerBlue1(33);
            code Green3(34);
            code SpringGreen3(35);
            code DarkCyan(36);
            code LightSeaGreen(37);
            code DeepSkyBlue2(38);
            code DeepSkyBlue1(39);
            code Green3A(40);
            code SpringGreen3A(41);
            code SpringGreen2(42);
            code Cyan3(43);
            code DarkTurquoise(44);
            code Turquoise2(45);
            code Green1(46);
            code SpringGreen2A(47);
            code SpringGreen1(48);
            code MediumSpringGreen(49);
            code Cyan2(50);
            code Cyan1(51);
            code DarkRed(52);
            code DeepPink4(53);
            code Purple4(54);
            code Purple4A(55);
            code Purple3(56);
            code BlueViolet(57);
            code Orange4(58);
            code Grey37(59);
            code MediumPurple4(60);
            code SlateBlue3(61);
            code SlateBlue3A(62);
            code RoyalBlue1(63);
            code Chartreuse4(64);
            code DarkSeaGreen4(65);
            code PaleTurquoise4(66);
            code SteelBlue(67);
            code SteelBlue3(68);
            code CornflowerBlue(69);
            code Chartreuse3(70);
            code DarkSeaGreen4A(71);
            code CadetBlue(72);
            code CadetBlueA(73);
            code SkyBlue3(74);
            code SteelBlue1(75);
            code Chartreuse3A(76);
            code PaleGreen3(77);
            code SeaGreen3(78);
            code Aquamarine3(79);
            code MediumTurquoise(80);
            code SteelBlue1A(81);
            code Chartreuse2(82);
            code SeaGreen2(83);
            code SeaGreen1(84);
            code SeaGreen1A(85);
            code Aquamarine1(86);
            code DarkSlateGray2(87);
            code DarkRedA(88);
            code DeepPink4B(89);
            code DarkMagenta(90);
            code DarkMagentaA(91);
            code DarkViolet(92);
            code PurpleA(93);
            code Orange4A(94);
            code LightPink4(95);
            code Plum4(96);
            code MediumPurple3(97);
            code MediumPurple3A(98);
            code SlateBlue1(99);
            code Yellow4(100);
            code Wheat4(101);
            code Grey53(102);
            code LightSlateGrey(103);
            code MediumPurple(104);
            code LightSlateBlue(105);
            code Yellow4A(106);
            code DarkOliveGreen3(107);
            code DarkSeaGreen(108);
            code LightSkyBlue3(109);
            code LightSkyBlue3A(110);
            code SkyBlue2(111);
            code Chartreuse2A(112);
            code DarkOliveGreen3A(113);
            code PaleGreen3A(114);
            code DarkSeaGreen3(115);
            code DarkSlateGray3(116);
            code SkyBlue1(117);
            code Chartreuse1(118);
            code LightGreen(119);
            code LightGreenA(120);
            code PaleGreen1(121);
            code Aquamarine1A(122);
            code DarkSlateGray1(123);
            code Red3(124);
            code DeepPink4A(125);
            code MediumVioletRed(126);
            code Magenta3(127);
            code DarkVioletA(128);
            code PurpleB(129);
            code DarkOrange3(130);
            code IndianRed(131);
            code HotPink3(132);
            code MediumOrchid3(133);
            code MediumOrchid(134);
            code MediumPurple2(135);
            code DarkGoldenrod(136);
            code LightSalmon3(137);
            code RosyBrown(138);
            code Grey63(139);
            code MediumPurple2A(140);
            code MediumPurple1(141);
            code Gold3(142);
            code DarkKhaki(143);
            code NavajoWhite3(144);
            code Grey69(145);
            code LightSteelBlue3(146);
            code LightSteelBlue(147);
            code Yellow3(148);
            code DarkOliveGreen3B(149);
            code DarkSeaGreen3A(150);
            code DarkSeaGreen2(151);
            code LightCyan3(152);
            code LightSkyBlue1(153);
            code GreenYellow(154);
            code DarkOliveGreen2(155);
            code PaleGreen1A(156);
            code DarkSeaGreen2A(157);
            code DarkSeaGreen1(158);
            code PaleTurquoise1(159);
            code Red3A(160);
            code DeepPink3(161);
            code DeepPink3A(162);
            code Magenta3B(163);
            code Magenta3A(164);
            code Magenta2(165);
            code DarkOrange3A(166);
            code IndianRedA(167);
            code HotPink3A(168);
            code HotPink2(169);
            code Orchid(170);
            code MediumOrchid1(171);
            code Orange3(172);
            code LightSalmon3A(173);
            code LightPink3(174);
            code Pink3(175);
            code Plum3(176);
            code Violet(177);
            code Gold3A(178);
            code LightGoldenrod3(179);
            code Tan(180);
            code MistyRose3(181);
            code Thistle3(182);
            code Plum2(183);
            code Yellow3A(184);
            code Khaki3(185);
            code LightGoldenrod2(186);
            code LightYellow3(187);
            code Grey84(188);
            code LightSteelBlue1(189);
            code Yellow2(190);
            code DarkOliveGreen1(191);
            code DarkOliveGreen1A(192);
            code DarkSeaGreen1A(193);
            code Honeydew2(194);
            code LightCyan1(195);
            code Red1(196);
            code DeepPink2(197);
            code DeepPink1(198);
            code DeepPink1A(199);
            code Magenta2A(200);
            code Magenta1(201);
            code OrangeRed1(202);
            code IndianRed1(203);
            code IndianRed1A(204);
            code HotPink(205);
            code HotPinkA(206);
            code MediumOrchid1A(207);
            code DarkOrange(208);
            code Salmon1(209);
            code LightCoral(210);
            code PaleVioletRed1(211);
            code Orchid2(212);
            code Orchid1(213);
            code Orange1(214);
            code SandyBrown(215);
            code LightSalmon1(216);
            code LightPink1(217);
            code Pink1(218);
            code Plum1(219);
            code Gold1(220);
            code LightGoldenrod2A(221);
            code LightGoldenrod2B(222);
            code NavajoWhite1(223);
            code MistyRose1(224);
            code Thistle1(225);
            code Yellow1(226);
            code LightGoldenrod1(227);
            code Khaki1(228);
            code Wheat1(229);
            code Cornsilk1(230);
            code Grey100(231);
            code Grey3(232);
            code Grey7(233);
            code Grey11(234);
            code Grey15(235);
            code Grey19(236);
            code Grey23(237);
            code Grey27(238);
            code Grey30(239);
            code Grey35(240);
            code Grey39(241);
            code Grey42(242);
            code Grey46(243);
            code Grey50(244);
            code Grey54(245);
            code Grey58(246);
            code Grey62(247);
            code Grey66(248);
            code Grey70(249);
            code Grey74(250);
            code Grey78(251);
            code Grey82(252);
            code Grey85(253);
            code Grey89(254);
            code Grey93(255);
        }
        namespace rgb {
            std::string bg(unsigned char r, unsigned char g, unsigned char b) {
                return fmt::format("{}[48;2;{};{};{}m", esc, r, g, b);
            }

            std::string fg(unsigned char r, unsigned char g, unsigned char b) {
                return fmt::format("{}[38;2;{};{};{}m", esc, r, g, b);
            }
        }
    }

    std::string style(styles::set::code style, colors::small::bg::code bg, colors::small::fg::code fg) {
        return fmt::format("{}[{};{};{}m", esc, style(), bg(), fg());
    }

    std::string style(styles::reset::code style, colors::small::bg::code bg, colors::small::fg::code fg) {
        return fmt::format("{}[{};{};{}m", esc, style(), bg(), fg());
    }

    std::string style(styles::set::code style) {
        return fmt::format("{}[{}m", esc, style());
    }

    std::string style(styles::reset::code style) {
        return fmt::format("{}[{}m", esc, style());
    }

    std::string color(colors::small::bg::code color) {
        return fmt::format("{}[{}m", esc, color());
    }

    std::string color(colors::small::fg::code color) {
        return fmt::format("{}[{}m", esc, color());
    }

    std::string color(colors::small::bg::code bg, colors::small::fg::code fg) {
        return fmt::format("{}[{};{}m", esc, bg(), fg());
    }

    std::string color(colors::big::code fg, colors::big::code bg) {
        return fmt::format("{}[38;5;{}m{}[48;5;{}m", esc, fg(), esc, bg());
    }

    std::string color(colors::big::code fg) {
        return fmt::format("{}[38;5;{}m", esc, fg());
    }

}

#endif //SEDIT_ANSI_HPP
