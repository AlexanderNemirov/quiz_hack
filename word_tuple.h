#ifndef word_tuple
#define word_tuple

#include<string>
#include<unordered_map>

struct WordTuple{
    WordTuple(std::wstring word, std::unordered_map<wchar_t,unsigned> dict)
    : word_(word), dict_(dict) {}
    WordTuple(const WordTuple & other)
    : word_(other.word_), dict_(other.dict_) {}
    ~WordTuple() {}

    unsigned size() const { return word_.size(); }
    //operators overloading
    bool operator==(const WordTuple & other) { return dict_ == other.dict_; }
    bool operator<(const WordTuple & other) { return word_.size() < other.word_.size(); }
    bool operator!=(const WordTuple & other) { return !(*this == other); }
    WordTuple operator=(const WordTuple & other) {
        if (&other != this) {
            word_ = other.word_;
            dict_ = other.dict_;
        }
        return *this;
    }
    friend WordTuple operator+(const WordTuple & left, const WordTuple & right);
    //get methods
    std::wstring get_word() const { return word_; }
    std::wstring & get_word() { return word_; }
    std::unordered_map<wchar_t,unsigned> get_dict() const { return dict_; }
    std::unordered_map<wchar_t,unsigned> & get_dict() { return dict_; }

    private:
    std::wstring word_;
    std::unordered_map<wchar_t,unsigned> dict_;

    WordTuple() {}
};

WordTuple operator+(const WordTuple & left, const WordTuple & right) {
    WordTuple result{left};
    std::wstring & word = result.get_word();
    word = word + right.get_word();
    std::unordered_map<wchar_t,unsigned> & dict = result.get_dict();
    for (auto &[letter, num] : right.get_dict()) {
        if (dict.contains(letter))
            dict[letter] += num;
        else
            dict[letter] = num;
    }
    return result;
}

#endif