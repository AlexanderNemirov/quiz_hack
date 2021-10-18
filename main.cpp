#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_set>
#include<locale>
#include<chrono>
#include<functional>


bool read_file(const char* filename, std::vector<std::wstring> & all_words, const std::unordered_multiset<wchar_t> & letters);
bool do_magic(const std::vector<std::wstring> & words, const std::unordered_multiset<wchar_t> & letters,
	std::vector<std::array<std::reference_wrapper<const std::wstring>,5>> & solution);

//no copying !!!!
//no filter ???
int main(){
	std::setlocale(LC_ALL,"ru_RU.UTF-8");
	std::wstring letters_s;
	std::wcin >> letters_s;
	std::wcout << letters_s << L"\n";
	//required letters
	std::unordered_multiset<wchar_t> letters{letters_s.begin(), letters_s.end()};
	//const std::unordered_multiset<wchar_t> letters {L'ч',L'ч',L'ч',L'ч',L'ч',L'а',L'а',L'а',L'а',L'с',L'с',L'т',L'т',L'т',L'т',L'т',L'ь',L'е',L'е',L'е',L'е'};
	auto start = std::chrono::steady_clock::now();
	//wstring -> wchar_t arrays ?
	std::vector<std::wstring> all_words;
	std::vector<std::reference_wrapper<const std::wstring>> filtered_words;
	std::vector<std::array<std::reference_wrapper<const std::wstring>,5>> solutions;

	//input file
	if (!read_file("data/word_rus.txt", all_words, letters)){
		std::wcout << "input file was not found!\n";
		return 0;
	}

	auto start1 = std::chrono::steady_clock::now();
	if (do_magic(all_words, letters, solutions))
		for(auto solution : solutions)
			std::wcout << solution[0].get() << L"; "
			<< solution[1].get() << L"; "
			<< solution[2].get() << L"; "
			<< solution[3].get() << L"; "
			<< solution[4].get() << L"\n";
	else
		std::wcout << "solution was not found!\n";
	auto end = std::chrono::steady_clock::now();
	auto load_filter_time = start1 - start;
	auto magic_time = end - start1;
	std::wcout << "load_filter_time: " << std::chrono::duration<double, std::milli> (load_filter_time).count() << " ms\n";
	std::wcout << "magic_time: " << std::chrono::duration<double, std::milli> (magic_time).count() << " ms\n";

	return 0;
}

bool filter_word(const std::wstring & word, const std::unordered_multiset<wchar_t> & letters){
	bool add = true;
	for(auto word_char : word){
		//c++ 20
		if(!letters.contains(word_char)){
			add = false;
			break;
		}
	}
	return add;
}


bool read_file(const char* filename, std::vector<std::wstring> & all_words, const std::unordered_multiset<wchar_t> & letters){
	std::wifstream file_in;
	file_in.imbue(std::locale("ru_RU.UTF-8"));
	file_in.open(filename);
	bool file_ok = false;
	if(file_in.is_open()){
		all_words.reserve(20);
		std::wstring line;
		//buff -> vec
		//filter_words here ???
		//getline (?)
		while(std::getline(file_in,line)){
			if (filter_word(line,letters))
				all_words.push_back(line);
		}
		file_in.close();
		file_ok = true;
	}
	return file_ok;
}

//check 5 words
bool check_words(std::array<std::reference_wrapper<const std::wstring>, 5> & words, const std::unordered_multiset<wchar_t> & letters){
	std::unordered_multiset<wchar_t> words_set;
	for(auto word : words)
		for(auto word_char : word.get())
			words_set.insert(word_char);
	return words_set == letters;
}

//check every 5 words from filtered
bool do_magic(const std::vector<std::wstring> & words, const std::unordered_multiset<wchar_t> & letters,
std::vector<std::array<std::reference_wrapper<const std::wstring>,5>> & solution){
	for(int i0=0; i0<words.size()-4; ++i0)	
		for(int i1=i0; i1<words.size()-3; ++i1)	
			for(int i2=i1; i2<words.size()-2; ++i2)	
				for(int i3=i2; i3<words.size()-1; ++i3)	
					for(int i4=i3; i4<words.size(); ++i4){
						//vec -> array of refs
						std::array<std::reference_wrapper<const std::wstring>,5>
							words_to_check{ {std::ref(words[i0]),std::ref(words[i1]),std::ref(words[i2]),std::ref(words[i3]),std::ref(words[i4])} };
						if(check_words(words_to_check, letters))
							solution.push_back(words_to_check);
					}
	return !solution.empty();
}
