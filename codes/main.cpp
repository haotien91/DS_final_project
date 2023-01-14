#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<algorithm>

#include "Search_parse.h"

using namespace std;

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		transform(s.begin(), s.end(), s.begin(), ::tolower); //! 所有字元轉為小寫
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}

// parsing query.txt

void parse_query(vector<vector<string>> &parsed, string query) {
	fstream fi;
	fi.open(query, ios::in);
	string tmp;
	vector<string> tmp_string;

	while(getline(fi, tmp)){
		tmp_string = split(tmp, " ");
		// for (auto &word : tmp_string){
		// 	cout << word << endl;
		// }
		parsed.push_back(tmp_string);
	}
	fi.close();
}

// SUFFIX compare
int suffix_compare(SuffixTree *trie, const string sub_str) {
	int find = 0;

	return find;
}

// EXACT compare
int exact_compare(SuffixTree *trie, const string sub_str) {
	int find = 0;

	return find;
}
// PREFIX compare
int prefix_compare(SuffixTree *trie, const string sub_str) {
	int find = 0;

	return find;
}


int main(int argc, char *argv[])
{
    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	cout << data_dir << endl;

	// Read File & Parser Example

	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;

	// store every matched title
	vector<vector<string>> Title_Cor_Search;		
	// separate input by different lines
	vector<vector<string>> parsed;
	// pass by reference
	parse_query(parsed, query);
		

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	fi.open("data/0.txt", ios::in);

    // GET TITLENAME
	getline(fi, title_name);

    // GET TITLENAME WORD ARRAY
    tmp_string = split(title_name, " ");

	//! COLLECTING ALL CONTENT (INCLUDING TITLE)
	// vector<string> title = word_parse(tmp_string);
	vector<string> content = word_parse(tmp_string);

	// for(auto &word : title){
	// 	cout << word << endl;
	// }
	
    // GET CONTENT LINE BY LINE
	while(getline(fi, tmp)){

        // GET CONTENT WORD VECTOR
		tmp_string = split(tmp, " ");

		// PARSE CONTENT
		// vector<string> content = word_parse(tmp_string);
		for (auto it : word_parse(tmp_string)) {
			content.push_back(it);
		}
		// for(auto &word : content){
		// 	cout << word << endl;
		// }
		//......
	}
    // CLOSE FILE
	fi.close();

	// creating suffix tree
	auto suffixTree = SuffixTree();

	for (auto &word : content) {
		suffixTree.NewSuffix(word + "$");
	}

	// sub_str for comparison
	string sub_str;
	

	// search by every line
	for (int check = 0; check < parsed.size(); check++) {
		int And_Or = -1; // Justify the operator is "AND" : 0 or "OR" : 1
		int last_value = 0; // Represents the last true-value of matching

		for (auto element : parsed[check]) {
			switch (element[0]) {
				// suffix search
				case '*' :
					cout << "SUFFIX: ";
					sub_str = element.substr(1, element.length() - 2);
					int find = suffix_compare(&suffixTree, sub_str);

					// Operation
					if (And_Or == 0) {
						// AND
						last_value = last_value && find;
					}
					else if (And_Or == 1) {
						// OR
						last_value = last_value || find;
					}
					else {
						// initial
						last_value = find;
					}

					cout << sub_str + " ";
				continue;
				// exact search
				case '\"' :
					cout << "EXACT: ";
					sub_str = element.substr(1, element.length() - 2);
					int find = suffix_compare(&suffixTree, sub_str);

					// Operation
					if (And_Or == 0) {
						// AND
						last_value = last_value && find;
					}
					else if (And_Or == 1) {
						// OR
						last_value = last_value || find;
					}
					else {
						// initial
						last_value = find;
					}

					cout << sub_str + " ";
				continue;
				// AND
				case '+':
					cout << "AND ";
					And_Or = 0;
				continue;
				// OR
				case '/':
					cout << "OR ";
					And_Or = 1;
				continue;
				// prefix search
				default :
					cout << "PREFIX: ";
					sub_str = element;
					int find = suffix_compare(&suffixTree, sub_str);

					// Operation
					if (And_Or == 0) {
						// AND
						last_value = last_value && find;
					}
					else if (And_Or == 1) {
						// OR
						last_value = last_value || find;
					}
					else {
						// initial
						last_value = find;
					}

					cout << sub_str + " ";
				continue;
			}
		}
		if (last_value == 1) {
			Title_Cor_Search[check].push_back(title_name);
		}
		cout << endl;
	}


}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
