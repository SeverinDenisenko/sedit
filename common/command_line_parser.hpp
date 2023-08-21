//
// Created by Severin on 08.07.2023.
//

#ifndef SEDIT_COMMAND_LINE_PARSER_HPP
#define SEDIT_COMMAND_LINE_PARSER_HPP

#include <fstream>
#include <sstream>
#include <vector>

class command_line_parser {
public:
    class command_line_parser_exception: std::runtime_error {
    public:
        explicit command_line_parser_exception(const std::string& message): std::runtime_error(message) {}
    };

    template <typename T>
    [[nodiscard]] T lexical_cast(const std::string& str) const
    {
        T res;
        std::istringstream stream;
        stream.str(str);
        stream >> res;

        if(stream.fail())
            throw command_line_parser_exception("can't parse string");

        return res;
    }

    template <>
    [[nodiscard]] std::string lexical_cast(const std::string& str) const
    {
        return str;
    }

    command_line_parser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i)
            tokens.emplace_back(argv[i]);
    }

    template <typename T>
    [[nodiscard]] T get(const std::string& option) const {
        auto itr = std::find(tokens.begin(), tokens.end(), option);
        if (itr != tokens.end() && ++itr != tokens.end())
            return lexical_cast<T>(*itr);

        throw command_line_parser_exception("can't find argument");
    }

    template <typename T>
    [[nodiscard]] T get(size_t option) const {
        if (option < tokens.size())
            return lexical_cast<T>(tokens[option]);

        throw command_line_parser_exception("can't find argument");
    }

    [[nodiscard]] bool contains(const std::string& option) const {
        return std::find(tokens.begin(), tokens.end(), option) != tokens.end();
    }

    [[nodiscard]] bool empty() const{
        return tokens.empty();
    }

private:
    std::vector<std::string> tokens;
};

#endif //SEDIT_COMMAND_LINE_PARSER_HPP
