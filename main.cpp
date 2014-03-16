#include "iostream"
#include "fstream"
#include  "time.h"
#include "vector"
#include "mem/info_archive.h"
#include <boost/lexical_cast.hpp>


//include local libraries
#include "bwt.h"
#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"
#include "bitarray/rrr.h"
#include "bitarray/rrr2.h"
#include "intarray/sdarray_sml.h"

using namespace mscds;

typedef WatQueryGen<RRR> WavQuery2;
typedef WatBuilderGen<RRR> WavBuilder2;


typedef WatQueryGen<RRR2> WavQuery4;
typedef WatBuilderGen<RRR2> WavBuilder4;

typedef WatQueryGen<SDRankSelectSml> WavQuery3;
typedef WatBuilderGen<SDRankSelectSml> WavBuilder3;

using namespace std;

int main(int argc, char *argv[]){

	if(argc < 5){
		std::cerr << "Error: Not enough arguments" << endl;	
		std::cerr << "Usage: ./bwt -f <sequence_file> -p <pattern_file>[-d][-ds]" << endl;
		return 1;
	}

	bool debug = false;
        std::string seqFile, patFile, data_structure;
	data_structure = "BWT";

	for(unsigned int i= 1; i< argc; i++){
		  if(strcmp(argv[i],"-f") == 0 && (i+1) != argc){
			seqFile = argv[i+1];
			i++;
		  }
		  else if (strcmp(argv[i],"-p") == 0 && (i+1) != argc){
			patFile = argv[i+1];
			i++;
		  }
		  else if (strcmp(argv[i],"-d") == 0){
			debug = true;
		  }
		  else if (strcmp(argv[i],"-ds") == 0 && (i+1) != argc){
			data_structure= argv[i+1];
			i++;
		  }
		  else{
			cout << "Invalid arg: " << argv[i] << endl;
		   	cout << "Usage: ./bwt -f <sequence_file> -p <pattern_file> [-d][-ds]" << endl;
			cout << "\t -d debug mode" << endl ;
			cout << "\t -ds data structure to use:[bwt|RRR|RRR2]" << endl;
	           	exit(0);
	  	  }		  
	}

	using namespace mscds;

	clock_t t1,t2,t3,t4,t5,t6,t7,t8,t9,t10;

	std::string inputSeq, pattern;
	
	// Read input file containing gene sequence
	// std::ifstream inputFile(seqFile,std::ios::in);
	std::ifstream inputFile(seqFile);
	std::ifstream inputFile2(patFile,std::ios::in);
	if(inputFile){
		inputFile.seekg(0, std::ios::end);
              	inputSeq.resize(inputFile.tellg());
                inputFile.seekg(0,std::ios::beg);
                inputFile.read(&inputSeq[0], inputSeq.size());
                inputSeq.erase( std::remove(inputSeq.begin(), inputSeq.end(), '\n'), inputSeq.end() );
                inputFile.close();
        }

        if(inputFile2){
                inputFile2.seekg(0, std::ios::end);
                pattern.resize(inputFile2.tellg());
                inputFile2.seekg(0,std::ios::beg);
                inputFile2.read(&pattern[0], pattern.size());
                pattern.erase( std::remove(pattern.begin(), pattern.end(), '\n'), pattern.end() );
                inputFile2.close();
        }

	//CONFIG
	uint64_t loops = 1;

	cout << "InputFiles read: " << seqFile << " | " << patFile << endl;
	//cout << "Input Strings read: " << inputSeq << endl << pattern << endl;

	vector<uint64_t> input_as_vector;
	
	if( data_structure == "RRR" ){
		
		using boost::lexical_cast;
		using boost::bad_lexical_cast;

		for(string::iterator it = inputSeq.begin(); it < inputSeq.end(); it++){
			input_as_vector.push_back(lexical_cast<uint64_t>(*it));
		}
	
		WavBuilder2 rrr;
		WavQuery2 rrrq;

		t1= clock();
		rrr.build(input_as_vector, &rrrq);
		t2 = clock();

		uint64_t output;
		t3 = clock();
                        for(int count = 1; count <= loops; count++)
				output = rrrq.rank(1,1020);
		t4 = clock();
		cout<< "Wavoutput "<< output << endl;

		float diff1 ((float)t2 - (float)t1);
		float diff1_sec = diff1/CLOCKS_PER_SEC;

		float diff2 ((float)t4 - (float)t3);
		float diff2_sec = diff2/CLOCKS_PER_SEC;

		cout << "diffs: " << diff1_sec <<"  " << diff2_sec/loops << endl;
	}
	else if( data_structure == "BWT" ){
		BWT_Query bq;
		BWT_Builder bd;

		t1 = clock();
			bd.build(inputSeq, &bq, debug); 
			bq.make_count_table(bd.appended, bd.suffix_indices, debug);
		t2 = clock();

		float diff1 ((float)t2 - (float)t1);
		float diff1_sec = diff1/CLOCKS_PER_SEC;

		//size_t = estimate_data_size();
      		cout <<"Ctable size: "<< bq.lesser_char_counts.size() * sizeof(std::map<char,int>) << endl;
		cout <<"OCC size: "<< bq.occtable.size() * sizeof(std::map <char,BWT_Query::countmap>) << endl; 

		if (debug){
			cout << "Example Occ calls"<< endl;
			cout << "------------------------"<< endl;
			cout << "Occ(a,1,2): " << bq.Occ('a',2)<< endl;
			cout << "Occ(t,1,3): " << bq.Occ('t',3)<< endl;
			cout << "Occ(g,1,4): " << bq.Occ('g',4)<< endl;
			cout << "Occ(c,1,1): " << bq.Occ('c',1)<< endl;
			cout << "Occ(g,1,5): " << bq.Occ('g',5)<< endl;
		}

		int pattern_count = 0;
		t3 = clock();	
			cout <<"Pattern to count:"<< pattern << endl;
			for(int count = 1; count <= loops; count++){
				pattern_count = bq.count(pattern,debug);
			}
		t4 = clock(); 

		cout << "Found " << pattern_count << " occurances" << endl;
		float diff2 ((float)t4 - (float)t3); 
		float diff2_sec = diff2/CLOCKS_PER_SEC;

		cout << "Average count time over "<< loops << " "<< diff2_sec/loops << endl;
		cout << (diff2_sec/loops) << endl;

		vector <uint64_t> hits;
		hits.clear();

		t5 = clock();	
	        for(int count = 1; count <= loops; count++)
			hits = bq.locate(pattern, bd.suffix_indices, debug);
		t6 = clock();

		if (debug){
			for(vector<uint64_t>::iterator it = hits.begin();  it != hits.end(); it++)
			cout<< "hit: " << *it << endl;
		}
		cout << "locate.. " << hits.size() << endl;

		float diff3 ((float)t6 - (float)t5);
		float diff3_sec = diff3/CLOCKS_PER_SEC;

		cout << "Average locate time over "<< loops << " " << diff3_sec/loops << endl;
		cout << (diff3_sec/loops) << endl;

                cout <<"Suffix Array index size: "<< bd.suffix_indices.size() * sizeof(uint64_t) << endl;

		vector<int> suffix_indices_small;
	
		for(vector<int>::iterator it = bd.suffix_indices.begin(); it != bd.suffix_indices.end(); it++){
			if ( *it %32 == 0){ suffix_indices_small.push_back(*it);}
		}
		
			
		cout <<"Smaller Suffix array size: "<< suffix_indices_small.size() * sizeof(uint64_t) << endl;

		hits.clear();
		t7 = clock();
		        for(int count = 1; count <= loops; count++)
				hits = bq.locate2(pattern, suffix_indices_small, debug);			
		t8 = clock();

		if(debug){
		      for(vector<uint64_t>::iterator it = hits.begin();  it != hits.end(); it++)
			cout<< "hit: " << *it << endl;
		}

                cout << "locate2.. " << hits.size() << endl;

		float diff4 ((float)t8 - (float)t7);
		float diff4_sec = diff4/CLOCKS_PER_SEC;

		cout << "Average locate2 time over " << loops << " " << diff4_sec/loops << endl;
		cout << (diff4_sec/loops) << endl;

		
		uint64_t Occloops = loops * 100;
		t9 = clock();
			for(uint64_t count = 1; count <= Occloops; count++)
				bq.Occ('1',1020);
		t10 = clock();

                float diff5 ((float)t10 - (float)t9);
                float diff5_sec = diff5/CLOCKS_PER_SEC;

		cout << "bq.Occ('1',1020) " <<  bq.Occ('1',5) << endl;
		
		cout << "Build time: " << diff1_sec << endl;
		cout << "Query time Count: " << diff2_sec/loops << endl;
		cout << "Query time locate: " << diff3_sec/loops  << endl;
		cout << "Query time locate2: " << diff4_sec/loops << endl;
		cout << "Occ('1',5) rank: "<<  diff5_sec/100000000 << endl;
	}
}
