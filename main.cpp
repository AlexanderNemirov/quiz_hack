#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_set>
#include<locale>
#include<chrono>

bool read_file(const char* filename, std::vector<std::wstring> & all_words);
void filter_words(const std::vector<std::wstring> & all_words, std::vector<std::wstring> & filtered_words,
	const std::unordered_multiset<wchar_t> & letters);
bool check_words(const std::vector<std::wstring> & words, const std::unordered_multiset<wchar_t> & letters);
bool do_magic(const std::vector<std::wstring> & words, const std::unordered_multiset<wchar_t> & letters,
	std::vector<std::vector<std::wstring>> & solution);

int main(){
	auto start = std::chrono::steady_clock::now();

	std::setlocale(LC_ALL,"ru_RU.UTF-8");

	//required letters
	std::unordered_multiset<wchar_t> letters {L'ч',L'ч',L'ч',L'ч',L'ч',L'а',L'а',L'а',L'а',L'с',L'с',L'т',L'т',L'т',L'т',L'т',L'ь',L'е',L'е',L'е',L'е'};
	std::vector<std::wstring> all_words, filtered_words;

	//input file
	if (!read_file("data/word_rus.txt", all_words)){
		std::wcout << "input file was not found!\n";
		return 0;
	}

	filter_words(all_words, filtered_words, letters);

	std::vector<std::vector<std::wstring>> solutions;
	if(do_magic(filtered_words, letters, solutions))
		for(auto solution : solutions)
			std::wcout << solution[0] << L"; "
			<< solution[1] << L"; "
			<< solution[2] << L"; "
			<< solution[3] << L"; "
			<< solution[4] << L"\n";
	else
		std::wcout << "solution was not found!\n";

	auto end = std::chrono::steady_clock::now();
	auto time_exec = end - start;
	std::wcout << "program time execution: " << std::chrono::duration<double, std::milli> (time_exec).count() << " ms\n";

	return 0;
}

bool read_file(const char* filename, std::vector<std::wstring> & all_words){
	std::wifstream file_in;
	file_in.imbue(std::locale("ru_RU.UTF-8"));
	file_in.open(filename);
	bool file_ok = false;
	if(file_in.is_open()){
		all_words.reserve(34010);
		std::wstring line;
		while(std::getline(file_in,line)){
			line.erase(line.end()-1,line.end());
			all_words.push_back(line);
		}
		file_in.close();
		file_ok = true;
	}
	return file_ok;
}

//filter words whether they contain required letters or not
void filter_words(const std::vector<std::wstring> & all_words, std::vector<std::wstring> & filtered_words,
const std::unordered_multiset<wchar_t> & letters){
	for(auto word : all_words){
		bool add = true;
		for(auto word_char : word){
			//c++ 20
			if(!letters.contains(word_char)){
				add = false;
				break;
			}
		}
		if(add)
			filtered_words.push_back(word);
	}
}

//check 5 words
bool check_words(const std::vector<std::wstring> & words, const std::unordered_multiset<wchar_t> & letters){
	std::unordered_multiset<wchar_t> words_set;
	for(auto word : words)
		for(auto word_char : word)
			words_set.insert(word_char);
	return words_set == letters;
}

//check every 5 words from filtered
bool do_magic(const std::vector<std::wstring> & words, const std::unordered_multiset<wchar_t> & letters,
std::vector<std::vector<std::wstring>> & solution){
	for(int i0=0; i0<words.size()-4; ++i0)	
		for(int i1=i0; i1<words.size()-3; ++i1)	
			for(int i2=i1; i2<words.size()-2; ++i2)	
				for(int i3=i2; i3<words.size()-1; ++i3)	
					for(int i4=i3; i4<words.size(); ++i4){
						const std::vector<std::wstring> words_to_check{words[i0],words[i1],words[i2],words[i3],words[i4]};
						if(check_words(words_to_check, letters))
							solution.push_back(words_to_check);
					}
	return !solution.empty();
}
