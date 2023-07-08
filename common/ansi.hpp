//
// Created by Severin on 03.07.2023.
//

#ifndef SEDIT_ANSI_HPP
#define SEDIT_ANSI_HPP

#include "char_utils.hpp"

namespace keys {
    inline const char esc = 27;
    inline const char backspace = 127;
    inline const char newline = 13;
}

namespace ansi {
    using namespace keys;

    class code {
    public:
        explicit code(unsigned char value) : value(value) {}

        unsigned char operator()() const { return value; }

    private:
        unsigned char value;
    };

    namespace alternate {
        std::string on();

        std::string off();
    }

    namespace cursor {
        struct position_t {
            int row {1};
            int column {1};
        };

        std::string home();

        std::string go(unsigned int row, unsigned int column);

        std::string up(unsigned int lines = 1);

        std::string down(unsigned int lines = 1);

        std::string left(unsigned int columns = 1);

        std::string right(unsigned int columns = 1);

        std::string max();

        std::string hide();

        std::string show();

        std::string position();

        position_t get();
    }
    namespace erase {
        std::string line();

        std::string screen();
    }
    namespace styles {
        namespace set {
            class code : public ansi::code {
            public:
                explicit code(unsigned char value) : ansi::code(value) {}
            };

            inline code bold(1);
            inline code faint(2);
            inline code italic(3);
            inline code underline(4);
            inline code blinking(5);
            inline code reverse(6);
            inline code invisible(7);
            inline code strikethrough(8);
        }
        namespace reset {
            class code : public ansi::code {
            public:
                explicit code(unsigned char value) : ansi::code(value) {}
            };

            inline code bold(22);
            inline code faint(22);
            inline code italic(23);
            inline code underline(24);
            inline code blinking(25);
            inline code reverse(26);
            inline code invisible(27);
            inline code strikethrough(28);
        }
    }
    namespace colors {
        namespace small {
            namespace fg {
                class code : public ansi::code {
                public:
                    explicit code(unsigned char value) : ansi::code(value) {}
                };

                inline code black(30);
                inline code red(31);
                inline code green(32);
                inline code yellow(33);
                inline code blue(34);
                inline code magenta(35);
                inline code cyan(36);
                inline code white(37);
                inline code reset(0);
            }
            namespace bg {
                class code : public ansi::code {
                public:
                    explicit code(unsigned char value) : ansi::code(value) {}
                };

                inline code black(40);
                inline code red(41);
                inline code green(42);
                inline code yellow(43);
                inline code blue(44);
                inline code magenta(45);
                inline code cyan(46);
                inline code white(47);
                inline code reset(0);
            }
        }
        namespace big {
            class code : public ansi::code {
            public:
                explicit code(unsigned char value) : ansi::code(value) {}
            };

            inline code Black(0);
            inline code Maroon(1);
            inline code Green(2);
            inline code Olive(3);
            inline code Navy(4);
            inline code Purple(5);
            inline code Teal(6);
            inline code Silver(7);
            inline code Grey(8);
            inline code Red(9);
            inline code Lime(10);
            inline code Yellow(11);
            inline code Blue(12);
            inline code Fuchsi(13);
            inline code Aqua(14);
            inline code White(15);
            inline code Grey0(16);
            inline code NavyBlue(17);
            inline code DarkBlue(18);
            inline code Blue3(19);
            inline code Blue3A(20);
            inline code Blue1(21);
            inline code DarkGreen(22);
            inline code DeepSkyBlue4(23);
            inline code DeepSkyBlue4B(24);
            inline code DeepSkyBlue4A(25);
            inline code DodgerBlue3(26);
            inline code DodgerBlue2(27);
            inline code Green4(28);
            inline code SpringGreen4(29);
            inline code Turquoise4(30);
            inline code DeepSkyBlue3(31);
            inline code DeepSkyBlue3A(32);
            inline code DodgerBlue1(33);
            inline code Green3(34);
            inline code SpringGreen3(35);
            inline code DarkCyan(36);
            inline code LightSeaGreen(37);
            inline code DeepSkyBlue2(38);
            inline code DeepSkyBlue1(39);
            inline code Green3A(40);
            inline code SpringGreen3A(41);
            inline code SpringGreen2(42);
            inline code Cyan3(43);
            inline code DarkTurquoise(44);
            inline code Turquoise2(45);
            inline code Green1(46);
            inline code SpringGreen2A(47);
            inline code SpringGreen1(48);
            inline code MediumSpringGreen(49);
            inline code Cyan2(50);
            inline code Cyan1(51);
            inline code DarkRed(52);
            inline code DeepPink4(53);
            inline code Purple4(54);
            inline code Purple4A(55);
            inline code Purple3(56);
            inline code BlueViolet(57);
            inline code Orange4(58);
            inline code Grey37(59);
            inline code MediumPurple4(60);
            inline code SlateBlue3(61);
            inline code SlateBlue3A(62);
            inline code RoyalBlue1(63);
            inline code Chartreuse4(64);
            inline code DarkSeaGreen4(65);
            inline code PaleTurquoise4(66);
            inline code SteelBlue(67);
            inline code SteelBlue3(68);
            inline code CornflowerBlue(69);
            inline code Chartreuse3(70);
            inline code DarkSeaGreen4A(71);
            inline code CadetBlue(72);
            inline code CadetBlueA(73);
            inline code SkyBlue3(74);
            inline code SteelBlue1(75);
            inline code Chartreuse3A(76);
            inline code PaleGreen3(77);
            inline code SeaGreen3(78);
            inline code Aquamarine3(79);
            inline code MediumTurquoise(80);
            inline code SteelBlue1A(81);
            inline code Chartreuse2(82);
            inline code SeaGreen2(83);
            inline code SeaGreen1(84);
            inline code SeaGreen1A(85);
            inline code Aquamarine1(86);
            inline code DarkSlateGray2(87);
            inline code DarkRedA(88);
            inline code DeepPink4B(89);
            inline code DarkMagenta(90);
            inline code DarkMagentaA(91);
            inline code DarkViolet(92);
            inline code PurpleA(93);
            inline code Orange4A(94);
            inline code LightPink4(95);
            inline code Plum4(96);
            inline code MediumPurple3(97);
            inline code MediumPurple3A(98);
            inline code SlateBlue1(99);
            inline code Yellow4(100);
            inline code Wheat4(101);
            inline code Grey53(102);
            inline code LightSlateGrey(103);
            inline code MediumPurple(104);
            inline code LightSlateBlue(105);
            inline code Yellow4A(106);
            inline code DarkOliveGreen3(107);
            inline code DarkSeaGreen(108);
            inline code LightSkyBlue3(109);
            inline code LightSkyBlue3A(110);
            inline code SkyBlue2(111);
            inline code Chartreuse2A(112);
            inline code DarkOliveGreen3A(113);
            inline code PaleGreen3A(114);
            inline code DarkSeaGreen3(115);
            inline code DarkSlateGray3(116);
            inline code SkyBlue1(117);
            inline code Chartreuse1(118);
            inline code LightGreen(119);
            inline code LightGreenA(120);
            inline code PaleGreen1(121);
            inline code Aquamarine1A(122);
            inline code DarkSlateGray1(123);
            inline code Red3(124);
            inline code DeepPink4A(125);
            inline code MediumVioletRed(126);
            inline code Magenta3(127);
            inline code DarkVioletA(128);
            inline code PurpleB(129);
            inline code DarkOrange3(130);
            inline code IndianRed(131);
            inline code HotPink3(132);
            inline code MediumOrchid3(133);
            inline code MediumOrchid(134);
            inline code MediumPurple2(135);
            inline code DarkGoldenrod(136);
            inline code LightSalmon3(137);
            inline code RosyBrown(138);
            inline code Grey63(139);
            inline code MediumPurple2A(140);
            inline code MediumPurple1(141);
            inline code Gold3(142);
            inline code DarkKhaki(143);
            inline code NavajoWhite3(144);
            inline code Grey69(145);
            inline code LightSteelBlue3(146);
            inline code LightSteelBlue(147);
            inline code Yellow3(148);
            inline code DarkOliveGreen3B(149);
            inline code DarkSeaGreen3A(150);
            inline code DarkSeaGreen2(151);
            inline code LightCyan3(152);
            inline code LightSkyBlue1(153);
            inline code GreenYellow(154);
            inline code DarkOliveGreen2(155);
            inline code PaleGreen1A(156);
            inline code DarkSeaGreen2A(157);
            inline code DarkSeaGreen1(158);
            inline code PaleTurquoise1(159);
            inline code Red3A(160);
            inline code DeepPink3(161);
            inline code DeepPink3A(162);
            inline code Magenta3B(163);
            inline code Magenta3A(164);
            inline code Magenta2(165);
            inline code DarkOrange3A(166);
            inline code IndianRedA(167);
            inline code HotPink3A(168);
            inline code HotPink2(169);
            inline code Orchid(170);
            inline code MediumOrchid1(171);
            inline code Orange3(172);
            inline code LightSalmon3A(173);
            inline code LightPink3(174);
            inline code Pink3(175);
            inline code Plum3(176);
            inline code Violet(177);
            inline code Gold3A(178);
            inline code LightGoldenrod3(179);
            inline code Tan(180);
            inline code MistyRose3(181);
            inline code Thistle3(182);
            inline code Plum2(183);
            inline code Yellow3A(184);
            inline code Khaki3(185);
            inline code LightGoldenrod2(186);
            inline code LightYellow3(187);
            inline code Grey84(188);
            inline code LightSteelBlue1(189);
            inline code Yellow2(190);
            inline code DarkOliveGreen1(191);
            inline code DarkOliveGreen1A(192);
            inline code DarkSeaGreen1A(193);
            inline code Honeydew2(194);
            inline code LightCyan1(195);
            inline code Red1(196);
            inline code DeepPink2(197);
            inline code DeepPink1(198);
            inline code DeepPink1A(199);
            inline code Magenta2A(200);
            inline code Magenta1(201);
            inline code OrangeRed1(202);
            inline code IndianRed1(203);
            inline code IndianRed1A(204);
            inline code HotPink(205);
            inline code HotPinkA(206);
            inline code MediumOrchid1A(207);
            inline code DarkOrange(208);
            inline code Salmon1(209);
            inline code LightCoral(210);
            inline code PaleVioletRed1(211);
            inline code Orchid2(212);
            inline code Orchid1(213);
            inline code Orange1(214);
            inline code SandyBrown(215);
            inline code LightSalmon1(216);
            inline code LightPink1(217);
            inline code Pink1(218);
            inline code Plum1(219);
            inline code Gold1(220);
            inline code LightGoldenrod2A(221);
            inline code LightGoldenrod2B(222);
            inline code NavajoWhite1(223);
            inline code MistyRose1(224);
            inline code Thistle1(225);
            inline code Yellow1(226);
            inline code LightGoldenrod1(227);
            inline code Khaki1(228);
            inline code Wheat1(229);
            inline code Cornsilk1(230);
            inline code Grey100(231);
            inline code Grey3(232);
            inline code Grey7(233);
            inline code Grey11(234);
            inline code Grey15(235);
            inline code Grey19(236);
            inline code Grey23(237);
            inline code Grey27(238);
            inline code Grey30(239);
            inline code Grey35(240);
            inline code Grey39(241);
            inline code Grey42(242);
            inline code Grey46(243);
            inline code Grey50(244);
            inline code Grey54(245);
            inline code Grey58(246);
            inline code Grey62(247);
            inline code Grey66(248);
            inline code Grey70(249);
            inline code Grey74(250);
            inline code Grey78(251);
            inline code Grey82(252);
            inline code Grey85(253);
            inline code Grey89(254);
            inline code Grey93(255);
        }
        namespace rgb {
            std::string bg(unsigned char r, unsigned char g, unsigned char b);

            std::string fg(unsigned char r, unsigned char g, unsigned char b);
        }
    }

    std::string style(styles::set::code style, colors::small::bg::code bg, colors::small::fg::code fg);

    std::string style(styles::reset::code style, colors::small::bg::code bg, colors::small::fg::code fg);

    std::string style(styles::set::code style);

    std::string style(styles::reset::code style);

    std::string color(colors::small::bg::code color);

    std::string color(colors::small::fg::code color);

    std::string color(colors::small::bg::code bg, colors::small::fg::code fg);

    std::string color(colors::big::code fg, colors::big::code bg);

    std::string color(colors::big::code fg);

}

#endif //SEDIT_ANSI_HPP
