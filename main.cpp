#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<chrono>
#include<functional>

#include "word_tuple.h"


//functions called in main
bool ReadFile(const std::string & filename, std::vector<WordTuple> & words, const std::unordered_map<wchar_t,unsigned> & letters);
bool DoMagic(const std::vector<WordTuple> & words, const std::unordered_map<wchar_t,unsigned> & letters,
             std::vector<std::array<std::reference_wrapper<const WordTuple>,5>> & solution, const unsigned letters_count);
void StringToUmap(const std::wstring & word, std::unordered_map<wchar_t,unsigned> & word_map);

int main(int argc, char** args) {
    std::setlocale(LC_ALL,"ru_RU.UTF-8");

    std::string filename = "data/word_rus1000.txt";
    if (argc > 1)
        filename = "data/word_rus" + std::string(args[1]) + ".txt";

    std::wcout << "enter required letters:\n";
    std::wstring letters_in;
    std::wcin >> letters_in;
    std::unordered_map<wchar_t,unsigned> letters;
    StringToUmap(letters_in, letters);
    //filtered words
    std::vector<WordTuple> words;
    std::vector<std::array<std::reference_wrapper<const WordTuple>,5>> solutions;

    if (!ReadFile(filename, words, letters)) {
        std::wcout << "input file was not found!\n";
        return 0;
    }

    std::wcout << "number of filtered words:\n";
    std::wcout << words.size() << L"\n";
    if (words.size() < 5)
        return 0;

    auto start_magic = std::chrono::steady_clock::now();
    if (DoMagic(words, letters, solutions, letters_in.size()))
        for(auto & solution : solutions)
            std::wcout << solution[0].get().get_word() << L"; "
            << solution[1].get().get_word() << L"; "
            << solution[2].get().get_word() << L"; "
            << solution[3].get().get_word() << L"; "
            << solution[4].get().get_word() << L"\n";
    else
        std::wcout << "solution was not found!\n";
    auto end_magic = std::chrono::steady_clock::now();
    auto magic_time = end_magic - start_magic;
    std::wcout << "magic_time: " << std::chrono::duration<double, std::milli> (magic_time).count() << " ms\n";

    return 0;
}

void StringToUmap(const std::wstring & word, std::unordered_map<wchar_t,unsigned> & word_map) {
    for (auto & letter : word) {
        //c++ 20
        if (!word_map.contains(letter))
            word_map[letter] = 0;
        word_map[letter] += 1;
    }
}

bool FilterWord(const std::unordered_map<wchar_t,unsigned> & word_map, const std::unordered_map<wchar_t,unsigned> & letters) {
    bool passed = true;
    for (auto &[letter, num]: word_map)
        if ((!letters.contains(letter)) || (letters.at(letter) < num)) {
            passed = false;
            break;
        }
    return passed;
}

bool ReadFile(const std::string & filename, std::vector<WordTuple> & words, const std::unordered_map<wchar_t,unsigned> & letters) {
    auto start_read = std::chrono::steady_clock::now();
    std::wifstream file_in;
    std::wstringstream file_buffer;
    file_in.imbue(std::locale("ru_RU.UTF-8"));
    file_buffer.imbue(std::locale("ru_RU.UTF-8"));
    file_in.open(filename);
    if(!file_in.is_open())
    return false;
    file_buffer << file_in.rdbuf();
    file_in.close();
    words.reserve(300);
    std::wstring word;
    auto end_read = std::chrono::steady_clock::now();
    auto read_time = end_read - start_read;
    std::wcout << "read_time: " << std::chrono::duration<double, std::milli> (read_time).count() << " ms\n";
    while(std::getline(file_buffer, word)) {
        std::unordered_map<wchar_t,unsigned> word_map;
        StringToUmap(word, word_map);
        if (FilterWord(word_map, letters))
            words.push_back(WordTuple{word, word_map});
    }
    auto end_filter = std::chrono::steady_clock::now();
    auto filter_time = end_filter - end_read;
    std::wcout << "filter_time: " << std::chrono::duration<double, std::milli> (filter_time).count() << " ms\n";
    return true;
}

//check 2:5 words umap
bool IterationCheck(const std::unordered_map<wchar_t,unsigned> & words_map, const std::unordered_map<wchar_t,unsigned> & letters) {
    for (auto &[letter, num] : words_map)
        if (letters.at(letter) < num)
            return false;
    return true;
}

//really ugly code
//check every 5 words from filtered
bool DoMagic(const std::vector<WordTuple> & words, const std::unordered_map<wchar_t,unsigned> & letters,
    std::vector<std::array<std::reference_wrapper<const WordTuple>,5>> & solution, const unsigned letters_count) {
        for (int i0=0; i0 < words.size() - 4; ++i0) {
            for (int i1 = i0+1; i1 < words.size() - 3; ++i1) {
                WordTuple word_tuple_i1 = words[i0] + words[i1];
                if (!IterationCheck(word_tuple_i1.get_dict(),letters)) continue;
                for (int i2 = i1+1; i2 < words.size() - 2; ++i2) {
                    WordTuple word_tuple_i2 = word_tuple_i1 + words[i2];
                    if ((word_tuple_i2.size() >= letters_count) || (!IterationCheck(word_tuple_i2.get_dict(),letters))) continue;
                    for (int i3 = i2+1; i3 < words.size() - 1; ++i3) {
                        WordTuple word_tuple_i3 = word_tuple_i2 + words[i3];
                        if ((word_tuple_i3.size() >= letters_count) || (!IterationCheck(word_tuple_i3.get_dict(),letters))) continue;
                        for (int i4 = i3+1; i4 < words.size(); ++i4) {
                            WordTuple word_tuple_i4 = word_tuple_i3 + words[i4];
                            if (word_tuple_i4.get_dict() == letters) {
                                solution.push_back(std::array<std::reference_wrapper<const WordTuple>, 5>
                                                           {{std::ref(words[i0]), std::ref(words[i1]),
                                                             std::ref(words[i2]), std::ref(words[i3]),
                                                             std::ref(words[i4])}});
                            }
                        }
                    }
                }
            }
        }
        return !solution.empty();
}
