#ifndef word_tuple
#define word_tuple

#include<string>
#include<unordered_map>
#include<vector>

struct WordTuple{
    WordTuple(std::wstring word, std::unordered_map<wchar_t,unsigned> dict)
    : word_(word) {
        vec_.reserve(dict.size());
        for (auto & letter_key : dict)
            vec_.push_back(letter_key);
    }
    WordTuple(const WordTuple & other)
    : word_(other.word_), vec_(other.vec_) {}
    ~WordTuple() {}

    unsigned size() const { return vec_.size(); }
    //operators overloading
    bool operator==(const WordTuple & other) { return vec_ == other.vec_; }
    bool operator<(const WordTuple & other) { return word_.size() < other.word_.size(); }
    bool operator!=(const WordTuple & other) { return !(*this == other); }
    WordTuple operator=(const WordTuple & other) {
        if (&other != this) {
            word_ = other.word_;
            vec_ = other.vec_;
        }
        return *this;
    }
    friend WordTuple operator+(const WordTuple & left, const WordTuple & right);
    //get methods
    std::wstring get_word() const { return word_; }
    std::wstring & get_word() { return word_; }
    std::vector<std::pair<wchar_t,unsigned>> get_vec() const { return vec_; }
    std::vector<std::pair<wchar_t,unsigned>> & get_vec() { return vec_; }

    private:
    std::wstring word_;
    std::vector<std::pair<wchar_t,unsigned>> vec_;

    WordTuple() {}
};

WordTuple operator+(const WordTuple & left, const WordTuple & right) {
    WordTuple result{right};
    result.word_ = result.word_ + left.word_;
    for (auto & let_num : left.vec_) {
        auto vec_let = std::find_if(result.vec_.begin(), result.vec_.end(), [&](const auto & l) {return l.first == let_num.first;});
        if (vec_let != result.vec_.end())
            vec_let->second += let_num.second;
        else
            result.vec_.push_back(let_num);
    }
    return result;
}

#endif