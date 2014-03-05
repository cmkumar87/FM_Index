#include "iostream"
#include "vector"
#include "map"

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
	
	std::pair<int,int> findRange(const std::string& pattern, bool debug);

	/* count the number of times the pattern occur in the input
	*/
	uint64_t count(const std::string& pattern);


	/* return the location of any occurrence of the pattern in the input */
	uint64_t locate(const std::string& pattern, std::vector<int>);

	void make_count_table
                (const std::string& string, std::vector<int>& suffix_indices);
	unsigned int Occ(char c, int prefix_end_index);

 private:
   std::string bwt;

   typedef std::map <char,int> char_count_map;
   char_count_map lesser_char_counts;

   typedef std::map <unsigned int,unsigned int> countmap;
   typedef std::map <char, countmap> table;
   table occtable;

   typedef char_count_map::iterator cIter;

   friend class BWT_Builder;
};




