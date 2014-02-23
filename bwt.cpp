#include "iostream"
#include "vector"
#include "algorithm"
#include "map"

//include headers from local libraries

#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"
#include "bwt.h"
#include "framework/archive.h"

using namespace std;


/* Function to write the ouput object to disk
void BWT_Builder::build(const std::string& input, OutArchive& out){

}
*/


void BWT_Builder::build(const std::string& input, BWT_Query *out){

	out->bwt.clear();
	cout << "Input string: " << input << endl;

	//Append end of string char
	BWT_Builder::appended = input + "$";
	cout << "Appended with $: " << appended << endl;

	vector<BWT_Builder::suffix_pair> suffixes = suffix(appended);

	
	//sort the suffixes using custom sort predicate
	sort(suffixes.begin(), suffixes.end(),
		[](const pair<int,string> &left, const pair<int,string> &right)->bool{
			return left.second < right.second;
		});		

	
	cout << "Sorted Suffixes:" << endl;
	printVector(suffixes);

	//Extract just the suffix indices from the sorted suffix strings
	transform(suffixes.begin(), suffixes.end(), back_inserter(BWT_Builder::suffix_indices), [](const suffix_pair&p){return p.first;});

	// Create bwt from sorted suffix indices
	for (vector<int>::iterator iter = BWT_Builder::suffix_indices.begin();
		iter != BWT_Builder::suffix_indices.end(); ++iter ){

		if( *iter == 0 ) {
			out->bwt.push_back('$');
		}
		else {
		 	out->bwt.push_back(appended[*iter - 1]);
		}
	}
	cout << "---------"<< endl;
	cout << "BWT Transform: " << out->bwt << endl;

}


/*
  Finds all the suffixes for the string.
  Returns suffixes with their indices.
*/
vector<BWT_Builder::suffix_pair> 
	BWT_Builder::suffix (const string& input){

	vector<BWT_Builder::suffix_pair> suffixes;

	for (unsigned int index = 0; index < input.length(); index++){
	   suffixes.push_back(suffix_pair(index,input.substr(index)));
	}

	printVector(suffixes);

	return suffixes;
}


void BWT_Builder::printVector(vector<BWT_Builder::suffix_pair>& input){
       for (vector<suffix_pair>::iterator iter = input.begin();
                                        iter != input.end(); ++iter){
            int index = (*iter).first; 
	    cout << index << ":";  

	    string suffix =(*iter).second;
	    cout << suffix << endl;
        }  

}


void BWT_Query::make_count_table
		(const std::string& input, std::vector<int>& suff_ind){
	
	// Build C table
	char last_seen_char =' ';
	int last_seen_char_count = 0;

	cout << "Char-Count table" << endl;
	cout << "---------------"<< endl;
   for(vector<int>::iterator iter = suff_ind.begin(); 
				iter != suff_ind.end(); ++iter ){
	if(last_seen_char != input[*iter] ){
		lesser_char_counts[input[*iter]]=
				 last_seen_char_count;
		last_seen_char = input[*iter];

		cout << last_seen_char <<" ";
		cout << last_seen_char_count << endl;

	// Build Occ table
	for(unsigned int index_k = 0; index_k < bwt.length();
						++index_k){
		string bwt_sub = bwt.substr(0,index_k+1);
		occtable[input[*iter]][index_k] = std::count(bwt_sub.begin(),
						bwt_sub.end(),input[*iter] );
	//	cout << "occ("<< input[*iter] <<",0,"<< index_k << ") "<< 
	//				occtable[input[*iter]][index_k] << endl;
	   }
	}

	last_seen_char_count++;
	}
	
}


/* returns number of occurances of character c in BWT of 
   string [1,k]
*/
unsigned int BWT_Query::Occ(char c, int prefix_end_index){
	return occtable[c][prefix_end_index];
}


/* count the number of times the pattern occur in the input
*/
unsigned int BWT_Query::count(const std::string& pattern){
	
	//Start the search from the last character of the pattern
	// c is initialised to character from pattern 
	char c = pattern.back();

	int phase = pattern.length();
	int range_start = lesser_char_counts[c];
	range_start+=1;
	int range_end = lesser_char_counts[c+1];

	while( (range_start <= range_end) && (phase >= 2) ){
		c= pattern[phase - 1];
		range_start = lesser_char_counts[c]
				+ Occ(c,range_start-1)+1;
		range_end = lesser_char_counts[c]
				+ Occ(c,range_end);
		phase--;
	}

	if (range_end < range_start )
		return NOTFOUND;
	else
		return (range_end-range_start+1);

}

// return the location of any occurrence of the pattern in the input
unsigned int BWT_Query::first_occ(const std::string& pattern){

}


/*
	BitArray b = BitArrayBuilder::create(4);
	b.setbit(0, true);

	
	vector<uint64_t> v(10,5);
	cout << "Created vector of size:" << v.size() << endl;

	for (long index = 0; index < 3; index ++){
		cout << "Vector element " << index << v.at(index) << endl;
	}
*/

