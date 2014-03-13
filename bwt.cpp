#include "iostream"
#include "vector"
#include "algorithm"
#include "map"

//include headers from local libraries

#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"
#include "bwt.h"
#include "framework/archive.h"
#include "bitarray/rrr.h"
#include "bitarray/rrr2.h"
#include "intarray/sdarray_sml.h"

using namespace mscds;

using namespace std;


/* Function to write the ouput object to disk
void BWT_Builder::build(const std::string& input, OutArchive& out){

}
*/

void BWT_Builder::build(const std::string& input, BWT_Query *out, bool debug){

	out->bwt.clear();
	//cout << "Input string: " << input << endl;

	//Append end of string char
	BWT_Builder::appended = input + "$";
	if (debug) { cout << "Appended with $: " << appended << endl; }

	vector<BWT_Builder::suffix_pair> suffixes = suffix(appended);
	
	//sort the suffixes using custom sort predicate
	sort(suffixes.begin(), suffixes.end(),
	 [](const pair<int,string> &left, const pair<int,string> &right)->bool{
			return left.second < right.second;  }
	   );		

	
	if(debug){
		cout << "Sorted Suffixes:" << endl;
		printVector(suffixes);
	        cout << "---------"<< endl;
	}

	//Extract just the suffix indices from the sorted suffix strings
	transform(suffixes.begin(), suffixes.end(), back_inserter(BWT_Builder::suffix_indices), 
 	 [](const suffix_pair&p){return p.first;});

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
	
	if (debug) { cout << "BWT Transform: " << out->bwt << endl; }
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

	//printVector(suffixes);

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
		(const std::string& input, std::vector<int>& suff_ind, bool debug){
	
	// Build C table
	char last_seen_char =' ';
	int last_seen_char_count = 0;

	if (debug){
		cout << "Char-Count & Occ tables" << endl;
		cout << "---------------"<< endl;
	}

   for(vector<int>::iterator iter = suff_ind.begin(); 
				iter != suff_ind.end(); ++iter ){
	if(last_seen_char != input[*iter] ){
		lesser_char_counts[input[*iter]]=
				 last_seen_char_count;
		last_seen_char = input[*iter];
		if (debug){
			cout << last_seen_char <<" ";
			cout << last_seen_char_count << endl;
		}

	// Build Occ table
	for(unsigned int index_k = 1; index_k <= bwt.length();
						++index_k){
		string bwt_sub = bwt.substr(0,index_k);
		occtable[input[*iter]][index_k] = std::count(bwt_sub.begin(),
						bwt_sub.end(),input[*iter] );
		if(debug){
			cout << bwt_sub << endl;
			cout << "occ("<< input[*iter] <<",1,"<< index_k << ") "<< 
					occtable[input[*iter]][index_k] << endl;
		}
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
uint64_t BWT_Query::count(const std::string& pattern, bool debug){
	
	std::pair<int,int> range = findRange(pattern,debug);
	int range_end = range.second;
	int range_start = range.first;
	
	if (range_end < range_start )
		return NOTFOUND;
	else
		return (range_end-range_start+1);
}


std::pair<int,int> BWT_Query::findRange(const std::string& pattern, bool debug){

	// initialised to index of last character in pattern
        int phase = pattern.length();
        int range_start = 1;
        int range_end = bwt.length();
        
        // Start the search from the last character of the pattern
        while( (range_start <= range_end) && (phase >= 1) ){
                char c= pattern[phase-1];
                range_start = lesser_char_counts[c]
                                + Occ(c,range_start-1)+1;
                range_end = lesser_char_counts[c]
                                + Occ(c,range_end);
		if( debug ){
	          cout<<"Start_Ptr "<< range_start << "|" << "End_ptr " <<
        	   range_end << "|"<< "Iteration " << phase << "|" << c << endl;
		}
                phase--;
        }   
	    
        return make_pair(range_start,range_end);
}


// Return the location of any occurrence of the pattern in the input
vector<uint64_t> BWT_Query::locate(const std::string& pattern, 
				std::vector<int>& suff_ind, bool debug){
	uint64_t firstHit = -1;
	vector<uint64_t> allHits;
	
	std::pair<int,int> range = findRange(pattern,debug);

	if(range.second < range.first )
		{ allHits.push_back(0xFFFFFFFFFFFFFFFFULL); return allHits; }

	for(unsigned int i = range.first; i <= range.second; i++){
		if (firstHit == -1){
			firstHit= suff_ind[i-1];
		}
		allHits.push_back(suff_ind[i-1]);
		if(debug){ cout<< "Locate pattern found at Offset: " <<  suff_ind[i-1]  << endl;}
	}

	return allHits;
}


// Return the location of any occurrence of the pattern in the input
vector<uint64_t> BWT_Query::locate2(const std::string& pattern,
                                std::vector<int>& suff_ind_small, bool debug){
	//TODO parametrise First Hit
	uint64_t first_hit = -1;
	vector<uint64_t> allHits;

        std::pair<int,int> range = findRange(pattern, debug);

        if(range.second < range.first )
                { allHits.push_back(0xFFFFFFFFFFFFFFFFULL); return allHits; }

        for(unsigned int i = range.first; i <= range.second; i++){
                unsigned int v = 0, j = i;
                char c = bwt[j-1];
	        while ( j%2 !=0 ){
	                //cout << "j:v value: " << c << " " << j << ":" << v << endl;
                        j = lesser_char_counts[c] + Occ(c,j);
			c = bwt[j-1];
			v++;
		}
               	if (first_hit == -1){
			first_hit= suff_ind_small[j-1]+v;
		}
	
                //cout << "j:v value: " << c << " " << j << ":" << v << endl;
		allHits.push_back(suff_ind_small[j-1]+v);
		if(debug){ cout << "Locate2: Pattern found at Offset: " << suff_ind_small[j-1]+v << endl; }
        }
        return allHits;
}
