#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<chrono>
#include<functional>


bool ReadFile(const std::string & filename, std::vector<std::wstring> & words, const std::unordered_map<wchar_t,unsigned> & letters);
bool DoMagic(const std::vector<std::wstring> & words, const std::unordered_map<wchar_t,unsigned> & letters,
             std::vector<std::array<std::reference_wrapper<const std::wstring>,5>> & solution, unsigned letters_count);
void StringToUmap(const std::wstring & word_list, std::unordered_map<wchar_t,unsigned> & word_map);

int main(int argc, char** args){
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
    std::vector<std::wstring> words;
    std::vector<std::array<std::reference_wrapper<const std::wstring>,5>> solutions;

	if (!ReadFile(filename, words, letters)){
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
			std::wcout << solution[0].get() << L"; "
			<< solution[1].get() << L"; "
			<< solution[2].get() << L"; "
			<< solution[3].get() << L"; "
			<< solution[4].get() << L"\n";
	else
		std::wcout << "solution was not found!\n";
	auto end_magic = std::chrono::steady_clock::now();
	auto magic_time = end_magic - start_magic;
	std::wcout << "magic_time: " << std::chrono::duration<double, std::milli> (magic_time).count() << " ms\n";

	return 0;
}

void StringToUmap(const std::wstring & word_list, std::unordered_map<wchar_t,unsigned> & word_map){
    for (auto & letter : word_list) {
        //c++ 20
        if (!word_map.contains(letter))
            word_map[letter] = 0;
        word_map[letter] += 1;
    }
}

bool FilterWord(const std::wstring & word, const std::unordered_map<wchar_t,unsigned> & letters){
    std::unordered_map<wchar_t,unsigned> word_map;
    StringToUmap(word, word_map);
	bool passed = true;
    for (auto &[letter, num]: word_map)
        if ((!letters.contains(letter)) || (letters.at(letter) < num)) {
            passed = false;
            break;
        }
    return passed;
}

bool ReadFile(const std::string & filename, std::vector<std::wstring> & words, const std::unordered_map<wchar_t,unsigned> & letters){
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
    std::wstring line;
    auto end_read = std::chrono::steady_clock::now();
    auto read_time = end_read - start_read;
    std::wcout << "read_time: " << std::chrono::duration<double, std::milli> (read_time).count() << " ms\n";
    while(std::getline(file_buffer,line)){
        if (FilterWord(line, letters))
            words.push_back(line);
    }
    auto end_filter = std::chrono::steady_clock::now();
    auto filter_time = end_filter - end_read;
    std::wcout << "filter_time: " << std::chrono::duration<double, std::milli> (filter_time).count() << " ms\n";
	return true;
}

//check 5 words
bool CheckWords(std::wstring & words_combined, const std::unordered_map<wchar_t,unsigned> & letters){
    std::unordered_map<wchar_t,unsigned> words_map;
    StringToUmap(words_combined, words_map);
    return words_map == letters;
}

//really ugly code
//check every 5 words from filtered
//3rd, 4th, 5th for loops include length check
bool DoMagic(const std::vector<std::wstring> & words, const std::unordered_map<wchar_t,unsigned> & letters,
    std::vector<std::array<std::reference_wrapper<const std::wstring>,5>> & solution, const unsigned letters_count){
        for (int i0=0; i0<words.size()-4; ++i0) {
            for (int i1 = i0+1; i1 < words.size() - 3; ++i1) {
                std::wstring words_i0i1 = words[i0] + words[i1];
                for (int i2 = i1+1; i2 < words.size() - 2; ++i2) {
                    std::wstring words_i0i1i2 = words_i0i1 + words[i2];
                    if (words_i0i1i2.size() >= letters_count) continue;
                    for (int i3 = i2+1; i3 < words.size() - 1; ++i3) {
                        std::wstring words_i0i1i2i3 = words_i0i1i2 + words[i3];
                        if (words_i0i1i2i3.size() >= letters_count) continue;
                        for (int i4 = i3+1; i4 < words.size(); ++i4) {
                            std::wstring words_to_check = words_i0i1i2i3 + words[i4];
                            if (words_to_check.size() != letters_count) continue;
                            if (CheckWords(words_to_check, letters)) {
                                solution.push_back(std::array<std::reference_wrapper<const std::wstring>, 5>
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
