#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<chrono>
#include<functional>

#include "word_tuple.h"

typedef std::unordered_map<wchar_t,unsigned> UmapLets;
typedef std::vector<std::array<std::reference_wrapper<const WordTuple>,5>> VecCombinations;
typedef std::vector<std::pair<wchar_t,unsigned>> VecLets;
typedef std::vector<WordTuple> VecWords;

//functions called in main
bool ReadFile(const std::string & filename, VecWords & words, const UmapLets & letters);
void StringToUmap(const std::wstring & word, UmapLets & word_map);
bool GenMain(unsigned index_start, unsigned index_end, const VecWords & words,
             const UmapLets & letters,
             VecCombinations & solution);

int main(int argc, char** args) {
    std::setlocale(LC_ALL,"ru_RU.UTF-8");

    std::string filename = "data/word_rus1000.txt";
    if (argc > 1)
        filename = "data/word_rus" + std::string(args[1]) + ".txt";

    std::wcout << "enter required letters:\n";
    std::wstring letters_in;
    std::wcin >> letters_in;
    UmapLets letters;
    StringToUmap(letters_in, letters);
    //filtered words
    VecWords words;
    VecCombinations solutions;

    if (!ReadFile(filename, words, letters)) {
        std::wcout << "input file was not found!\n";
        return 0;
    }

    std::wcout << "number of filtered words: " << words.size() << L"\n";
    if (words.size() < 5)
        return 0;

    auto start_magic = std::chrono::steady_clock::now();
    if (GenMain(0, words.size()-4, words, letters, solutions)) {
        std::wcout << "number of solutions: " << solutions.size() << L"\n";
        for (auto &solution: solutions)
            std::wcout << solution[0].get().get_word() << L"; "
                       << solution[1].get().get_word() << L"; "
                       << solution[2].get().get_word() << L"; "
                       << solution[3].get().get_word() << L"; "
                       << solution[4].get().get_word() << L"\n";
    }
    else
        std::wcout << "solution was not found!\n";
    auto end_magic = std::chrono::steady_clock::now();
    auto magic_time = end_magic - start_magic;
    std::wcout << "magic_time: " << std::chrono::duration<double, std::milli> (magic_time).count() << " ms\n";

    return 0;
}

void StringToUmap(const std::wstring & word, UmapLets & word_map) {
    for (auto & letter : word) {
        //c++ 20
        if (!word_map.contains(letter))
            word_map[letter] = 0;
        word_map[letter] += 1;
    }
}

bool FilterWord(const UmapLets & word_map, const UmapLets & letters) {
    bool passed = true;
    for (auto &[letter, num]: word_map)
        if ((!letters.contains(letter)) || (letters.at(letter) < num)) {
            passed = false;
            break;
        }
    return passed;
}

bool ReadFile(const std::string & filename, VecWords & words, const UmapLets & letters) {
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
        UmapLets word_map;
        StringToUmap(word, word_map);
        if (FilterWord(word_map, letters))
            words.push_back(WordTuple{word, word_map});
    }
    auto end_filter = std::chrono::steady_clock::now();
    auto filter_time = end_filter - end_read;
    std::wcout << "filter_time: " << std::chrono::duration<double, std::milli> (filter_time).count() << " ms\n";
    return true;
}

//last check
bool FinalCheck(const VecLets & words_vec, const UmapLets & letters) {
    for (auto &[letter, num] : words_vec)
        if (letters.at(letter) != num)
            return false;
    return true;
}

//check 2:4 words umap
bool IterCheck(const VecLets & words_vec, const UmapLets & letters, const unsigned len) {
      for (unsigned i=0; i<len; ++i)
          if (letters.at(words_vec[i].first) < words_vec[i].second)
              return false;
    return true;
}

void Gen(unsigned index_start, std::vector<unsigned> & indices, const WordTuple & prev_words,
         const VecWords & words, const UmapLets & letters, VecCombinations & solution) {
    if (indices.size() == 5) {
        if ((prev_words.size() == letters.size()) && (FinalCheck(prev_words.get_vec(),letters))) {
            solution.push_back({{std::ref(words[indices[0]]),
                                 std::ref(words[indices[1]]),
                                 std::ref(words[indices[2]]),
                                 std::ref(words[indices[3]]),
                                 std::ref(words[indices[4]])}});
        }
    }
    else
        for (int index=index_start; index<words.size(); ++index) {
            WordTuple next_words = prev_words + words[index];
            if ((next_words.size() > letters.size()) || (!IterCheck(next_words.get_vec(), letters, words[index].size()))) continue;
            indices.push_back(index);
            Gen(index+1, indices, next_words, words, letters, solution);
            indices.pop_back();
        }
    return;
}

bool GenMain(unsigned index_start, unsigned index_end, const VecWords & words,
            const UmapLets & letters, VecCombinations & solution) {
    std::vector<unsigned> indices;
    indices.reserve(5);
    for (unsigned index=index_start; index<index_end; ++index) {
        indices.push_back(index);
        Gen(index+1, indices, words[index], words, letters, solution);
        indices.pop_back();
    }
    return !solution.empty();
}