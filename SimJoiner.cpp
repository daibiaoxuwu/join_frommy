#include "SimJoiner.h"

using namespace std;
inline int my_abs(int a) { return a > 0 ? a : -a; }
inline int my_min(int a, int b) { return a < b ? a : b; }
inline int my_max(int a, int b) { return a > b ? a : b; }
inline int min_3(int x, int y, int z) { return my_min(my_min(x, y), z); }
inline JaccardJoinResult create_JaccardJoinResult(unsigned id1, unsigned id2, double s){
    JaccardJoinResult result;
    result.id1 = id1;
    result.id2 = id2;
    result.s = s;
    return result;
}
inline EDJoinResult create_EDJoinResult(unsigned id1, unsigned id2, unsigned s){
    EDJoinResult result;
    result.id1 = id1;
    result.id2 = id2;
    result.s = s;
    return result;
}


SimJoiner::SimJoiner() {
    word_count = new int[200010];
	char_count = new int[200010];
	lines = new char*[200010];
	same = new int[200010];
	memset(word_count, 0, 200010*sizeof(int));
	memset(char_count, 0, 200010*sizeof(int));
	query_num = new int[500];
	word_sets = new std::vector<int>*[500];
	word_sets_freq = new std::vector<int>*[500];
}

SimJoiner::~SimJoiner() {
    delete[](word_count);
	delete[](char_count);
	delete[](same);
	for(int i = 0; i < line_count && lines[i]!= nullptr; ++ i){
		delete[](lines[i]);
	}
	delete[](lines);
	delete[](query_num);
	delete[](word_sets);
	delete[](word_sets_freq);
}

    
int SimSearcher::createJaccIndex(const char *filename, Trie jacTrie){

	char buf[1024];
	FILE* file = fopen(filename,"r");
	for(line_count=0;fgets(buf,1024,file);++line_count){

		//remove tail \n
		if(buf[strlen(buf)-1]=='\n'){
			buf[strlen(buf)-1]='\0';
		}
		char_count[line_count] = (int)strlen(buf);
		lines[line_count] = new char[258];
		memcpy(lines[line_count],buf,strlen(buf)+1);

		// //ED: insert adjacent q words into edTrie
		// this->q=q;
		// for(int j = 0; buf[j+q-1]!='\0'; ++j){
		// 	edTrie.insert_multiple_unique(line_count, buf + j, q);
		// }

		//jaccard: split by spaces and insert into jacTrie
		char* pch = strtok (buf," \r\n");
		while (pch != nullptr)
		{
			//if is unique
		   	if(jacTrie.insert_multiple_unique(line_count, pch, strlen(pch)))
		   		++word_count[line_count];
			pch = strtok (nullptr, " \r\n");
		}
	}
    return SUCCESS;
}

int SimSearcher::searchJaccard(char *query_split, int id2, Trie jacTrie, double threshold, vector<JaccardJoinResult> &result)
{
	result.clear();

	//copy query into query_split to avoid modifying it in 'strtok'
	memcpy(query_split,query,strlen(query)+1);
	int query_count = 0;

	//split query into words; then find them in jacTrie
	//search function accepts a word and returns a small hash_set
	//containing ids of lines that owns this word.
	//to count multiple words, arrange their pointer in multisets.
	//then count each pointer.
	memset(same, 0, 200010*sizeof(int));

	//count multiple words
	Trie query_trie;
	char* pch = strtok(query_split," \r\n");
	while (pch != nullptr)
	{
		//if this word is unique in query
		if(query_trie.insert_single_line(0, pch, strlen(pch)) == -1) {
			//count number of words
			++query_count;
			//search for this word in jacTrie
			TrieNode* search_result = jacTrie.search(pch, strlen(pch));
			if(search_result != nullptr){
			    //find a same word. add 1 to all lines containing this word.
				for (int it2 : *(search_result->entries)) ++same[it2];
			}
		}
		pch = strtok (nullptr, " \r\n");
	}

	//calculate jaccard with same[]
	for(int i = 0; i < line_count; ++i){
		double jaccard = same[i] / (double)(query_count + word_count[i] - same[i]);
		if(jaccard>=threshold)
            result.emplace_back(create_JaccardJoinResult(i, id2, jacc));
            // result.emplace_back(make_pair(i, jaccard));
	}


	return SUCCESS;
}



int SimJoiner::joinJaccard(const char *filename1, const char *filename2, double threshold, vector<JaccardJoinResult> &result) {
    result.clear();
    Trie jacTrie;
    createJaccIndex(filename1, jacTrie);

    char buf[1024];
	FILE* file = fopen(filename,"r");
	for(line_count=0;fgets(buf,1024,file);++line_count){

		//remove tail \n
		if(buf[strlen(buf)-1]=='\n'){
			buf[strlen(buf)-1]='\0';
		}
        searchJaccard(buf, line_count, jacTrie, threshold, result);
    }
    //CP
    sort(result.begin(), result.end());//TODO: need unique?
    result.resize(unique(result.begin(), result.end()) - result.begin());

    return SUCCESS;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned threshold, vector<EDJoinResult> &result) {
    result.clear();
    return SUCCESS;
}
