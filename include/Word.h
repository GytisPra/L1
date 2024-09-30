#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <json.hpp>
#include <string>

struct Word {
    std::string word = "DEFAULT";
    double wordValue = -1.0;
    int length = -1;
    std::string isPalindrome = "Nežinome";

    bool operator<(const Word& other) const {
        return word <= other.word;
    }

    bool isDefault() const {
        return word == "DEFAULT" && wordValue == -1.0 && length == -1;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Word& Word) {
    os << "word: " << Word.word << ", wordValue: " << Word.wordValue << ", length: " << Word.length;
    return os;
}

inline void to_json(nlohmann::json& j, const Word& Word) {
    j = nlohmann::json{{"word", Word.word}, {"word_value", Word.wordValue}, {"length", Word.length}};
}

inline void from_json(const nlohmann::json& j, Word& Word) {
    j.at("word").get_to(Word.word);
    j.at("word_value").get_to(Word.wordValue);
    j.at("length").get_to(Word.length);
}

#endif