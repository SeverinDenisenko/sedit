//
// Created by Severin on 08.07.2023.
//

#ifndef SEDIT_INPUT_PARSER_HPP
#define SEDIT_INPUT_PARSER_HPP

#include <fstream>
#include <sstream>
#include <vector>

class input_parser {
public:
    class input_parser_exception: std::runtime_error {
    public:
        explicit input_parser_exception(const std::string& message): std::runtime_error(message) {}
    };

    template <typename T>
    T lexical_cast(const std::string& str)
    {
        T res;
        std::istringstream stream;
        stream.str(str);
        stream >> res;

        if(stream.fail())
            throw input_parser_exception("can't parse string");

        return res;
    }

    input_parser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i)
            tokens.emplace_back(argv[i]);
    }

    [[nodiscard]] const std::string& get(const std::string& option) const {
        auto itr = std::find(tokens.begin(), tokens.end(), option);
        if (itr != tokens.end() && ++itr != tokens.end()) {
            return *itr;
        }

        throw input_parser_exception("can't find argument");
    }

    template <typename T>
    [[nodiscard]] T get(const std::string& option) const {
        auto itr = std::find(tokens.begin(), tokens.end(), option);
        if (itr != tokens.end() && ++itr != tokens.end())
            return lexical_cast<T>(*itr);

        throw input_parser_exception("can't find argument");
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

#endif //SEDIT_INPUT_PARSER_HPP
