#include "iostream"
#include "vector"

//include local libraries
#include "framework/archive.h"
#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"

using namespace std;

class BWT_Query;

class BWT_Builder {
 public:
        typedef pair<int,string> suffix_pair;

	//void build(const string& input, OutArchive& out);
	void build(const string& input, BWT_Query* out);
	vector<BWT_Builder::suffix_pair> suffix(const string& input);

	void printVector(vector<suffix_pair>& input);
	vector<int> suffix_indices;
	string appended;
};

class BWT_Query {
 public:
	static const uint64_t NOTFOUND = 0xFFFFFFFFFFFFFFFFULL;
	

	/* count the number of times the pattern occur in the input
	*/
	unsigned int count(const std::string& pattern);


	/* return the location of any occurrence of the pattern in the input
	*/
	unsigned int first_occ(const std::string& pattern);

	void make_count_table
                (const std::string& string, std::vector<int>& 
							suffix_indices);
 private:
   std::string bwt;
   typedef pair <char,int> count_pair;
   vector<BWT_Query::count_pair> lesser_char_counts;

   friend class BWT_Builder;
};




