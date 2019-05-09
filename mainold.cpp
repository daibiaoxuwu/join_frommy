#include "SimSearcher.h"
#include "SimSearcher2.h"
#include <iostream>
#include <ctime>
#include <windows.h>
using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;
	SimSearcher2 searcher2;

	vector<pair<unsigned, unsigned> > resultED, resultED2;
	vector<pair<unsigned, double> > resultJaccard, resultJaccard2;
	bool singleFlag = false;

	int error_q=2; unsigned error_edThreshold = 233; double error_jaccThreshold = 0.035;
//	error_q=3; error_edThreshold = 148; error_jaccThreshold = 0.02; char error_a[]="bab b a a ca b aabaa aababa baabb cb bbbb bb bbaba cbbaab c bb cab b\
//	aa a caba bbabb cabbbabaaa c c cab aba bbabbaa baaa b a c abaab aba baba bb aa c bb cb cabbaab c a abb bbb ba bb abbb bb\
//	cb a bbb c abba a abba c b a aaa a bbabbaaab aa ba bb a";


//jaccard del error in 2
//	error_q=2; error_edThreshold = 77;
//	error_jaccThreshold = 0.070000; char error_a[]="c ab c c ab aaaabbb bbbaab ca c aba b bb baba abbbbbbabb ab b b aab b c\
//	baba c cabbabbba bba b c aaa a cabab c b a cb";
//	singleFlag = true;

//not add right fast terminate:wrong
//	error_q=2; error_edThreshold = 97; error_jaccThreshold = 0.090000; char error_a[]="b bb aa bba a ba ba aa a a ab aaaa a\
//	b ba cab ba ab c c ba aababbb cabaa baa bbb a abb aa aaa ab b b b cbb ca c cbaa cb a cabaaababaabbab abbbbab abb c a aaa\
//	aab cba a ab cb caba a a aaa b c cabaa aaa ca abbaaba bbaababb c cba";singleFlag = true;

//	error_q=2; error_edThreshold = 273; error_jaccThreshold = 0.320000; char error_a[]="cbb baab c bab b aaaa abba c ca aab\
//	bba a a";singleFlag = true;

//    error_q=2; error_edThreshold = 200; error_jaccThreshold = 0.098000; char error_a[]="b b cbaab abbb caabb bb a ab aaa a a\
//    ba b cbaa b b aa a baaaa ca c baa aaaba a cab b babbba bab aa cb ca b ba a a b a caba cb cbaaaaabba ba a b cbab aa bb c\
//    a";singleFlag = true;
//
	char error_a[]= "";

	int count = 0;
	char* a = new char[500];
	unsigned q, edThreshold; double jaccardThreshold;
	for(q = 2; q <= 5; ++ q) {

//	char a[]="bba baa aab bab baa aab abb bab bbb bba bab bab abb bbb aba aaa aaa aba abb bba aaa baa abb aab bbb aab bab aaa bbb aab bbb baa baa bbb bbb bbb baa aab aba aba abb aab aaa aba bab aaa bba abb abb bab abb aba abb bbb bba aaa aaa bab aaa aba abb aba aba aba aba aba";
		char path[] = "D:\\prog10\\Desktop\\Sercher\\sample.txt";
		searcher.createIndex(path, q);
		searcher2.createIndex(path, q);

		for (int times = 0; times < 10000; ++times) {
			printf("q:%d time:%d\n",q,times);
			int length =  1 + rand() % 254;
			for (int j = 0; j <= length;j++) {
				a[j] = (char) (97 + rand() % 3);
				if(a[j]=='c' && j!=0 && j!=length && a[j-1]!=' ') a[j]=' ';
			}
			a[length+1]='\0';
			edThreshold = ((unsigned) rand()) % 300;
			jaccardThreshold = (rand() % 100) / 500.0;

			//test a single error?
			if(singleFlag){
			    count=0;
				edThreshold = error_edThreshold;
				delete[] a;
				a=error_a;
				jaccardThreshold=error_jaccThreshold;
			}



			time_t tv = GetTickCount();
			searcher.searchJaccard(a, jaccardThreshold, resultJaccard);
			printf("jac %d\n", (int)(GetTickCount()-tv)); tv = GetTickCount();
			searcher.searchED(a, edThreshold, resultED);
			printf("ed %d\n", (int)(GetTickCount()-tv)); tv = GetTickCount();
			searcher2.searchJaccard(a, jaccardThreshold, resultJaccard2);
			printf("jac2 %d\n", (int)(GetTickCount()-tv)); tv = GetTickCount();
			searcher2.searchED(a, edThreshold, resultED2);
			printf("ed2 %d\n", (int)(GetTickCount()-tv));
//

            for (int i = 0; i < min(resultJaccard.size(),resultJaccard2.size()); ++i) {
                if (resultJaccard[i].first != resultJaccard2[i].first ||
                    resultJaccard[i].second != resultJaccard2[i].second) {
                    printf("error in resultJaccard[%d]: 1:%d %lf 2:%d %lf\n", i, resultJaccard[i].first,
                           resultJaccard[i].second, resultJaccard2[i].first, resultJaccard2[i].second);
                    if (++count > 50)break;
                }
            }
			if(resultJaccard.size()!=resultJaccard2.size()){
				printf("error in resultJaccard size: 1:%d 2:%d\n", resultJaccard.size(),resultJaccard2.size());
				count++; break;
			}
//	FILE* outFile = fopen("D:\\prog10\\Desktop\\1out.txt","w");
//	for(auto i : resultJaccard){
//		fprintf(outFile, "jac: %d %lf\n",i.first, i.second);
//	}
//    for(auto i : resultED){
//        fprintf(outFile,"ED : %d %d\n",i.first, i.second);
//    }
//            if(count>0) break;
//
            count = 0;
            for (int i = 0; i < min(resultED.size(),resultED2.size()); ++i) {
                if (resultED[i].first != resultED2[i].first || resultED[i].second != resultED2[i].second) {
                    printf("error in resultED[%d]: 1:%d %d 2:%d %d\n", i, resultED[i].first, resultED[i].second,
                           resultED2[i].first, resultED2[i].second);
                    if (++count > 50)break;
                }
            }
			if(resultED.size()!=resultED2.size()){
				printf("\n\nerror in resultED size: 1:%d 2:%d\n", resultED.size(), resultED2.size());
				count++; break;
			}
			if(count>0) break;
			if(singleFlag) break;
		}
		if(count>0) break;
		if(singleFlag) break;
	}
	if(count==0)printf("success!\n"); else
		printf("\n\n\nerror_q=%d; error_edThreshold = %d; error_jaccThreshold = %lf; char error_a[]=\"%s\";singleFlag = true;\n",q,edThreshold,jaccardThreshold,a);
	return 0;
}

