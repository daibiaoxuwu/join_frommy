#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include <vector>
#include "Trie.h"

template <typename IDType, typename SimType>
struct JoinResult {
    IDType id1;
    IDType id2;
    SimType s;
    
    bool operator < (const JoinResult &jr) const {
	    return id1 < jr.id1 || (id1 == jr.id1 && id2 < jr.id2);
	}
	bool operator == (const JoinResult &jr) const {
		return id1 == jr.id1 && id2 == jr.id2;
	}
};

typedef JoinResult<unsigned, double> JaccardJoinResult;
typedef JoinResult<unsigned, unsigned> EDJoinResult;

const int SUCCESS = 0;
const int FAILURE = 1;

class SimJoiner {
    // Trie jacTrie, edTrie;
	//count number of word in each line
	int* word_count;
	int* char_count;
	char** lines;
    int* query_num;
    std::vector<int> **word_sets;
    std::vector<int> **word_sets_freq;

	//hash of lines
	int* same;

	//count number of lines
	int q;
	int line_count;

	int calculate_ED(const char *query, char* line, int threshold);

public:
    SimJoiner();
    ~SimJoiner();

    int joinJaccard(const char *filename1, const char *filename2, double threshold, std::vector<JaccardJoinResult> &result);
    int joinED(const char *filename1, const char *filename2, unsigned threshold, std::vector<EDJoinResult> &result);
};

#endif
