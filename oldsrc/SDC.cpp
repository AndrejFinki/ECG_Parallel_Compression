#include <bits/stdc++.h>
using namespace std;
#define INF (1<<30)
#define LL long long
// #define endl '\n'

/* How many bits should each number be when we're not doing compression. Since ECG values can go up to 1023 and are only positive, 10 bits are needed */
const int BITS_USED_UNCOMPRESSED = 10;

/* What is the assumed starting value for the first ECG value */
const int STARTING_VALUE = 511;

/* Debug flag for debug information */
const bool DEBUG = false;

/* Name of the temporary file that lists all of the datafiles to run the compression on */
const string LS_FILE = "test_cases.tmp";

/* The relative path for the Data directory */
const string DATA_DIR = "../Data/EKG_360";

/* The range of bits to test */
const int LEFT_RANGE = 2, RIGHT_RANGE = 7;

/* What data to output to result */
const bool OUTPUT_VALUES = true;
const bool OUTPUT_TIME = false;

/* Helper functions. */
void program_exit(string);
void open_file( fstream &, const string );
string to_binary_string(int, const int);

/* Driver for a specific test file */
void driver(string);

/* Read the data and compress it, returning the amount of bits it takes */
pair <LL, LL> read_and_compress(string, int);
pair <LL, LL> read_without_compress(string);

/* Functions that help with the output formatting */
void print_header_items();

int main(){ ios::sync_with_stdio(false);

    /* Extract test cases filepaths */
    string cmd = "ls " + DATA_DIR + " > " + LS_FILE;
    system( cmd.c_str() );

    fstream file;
    open_file( file, LS_FILE );

	print_header_items();

    string test;
    while( getline( file, test ) ) driver(test);

}

void driver( string test_path ){

	pair <LL, LL> result = read_without_compress(test_path);

    LL bits_uncompressed = result.first;
    LL bits_compressed_min = bits_uncompressed;

	LL time_uncompressed = result.second;
	LL time_compressed_min = INT_MAX;

    cout << test_path;
	if( OUTPUT_VALUES ) cout << ", " << bits_uncompressed;
	if( OUTPUT_TIME ) cout << ", " << time_uncompressed;
    for( int i = LEFT_RANGE ; i <= RIGHT_RANGE ; i++ ){
		result = read_and_compress(test_path, i);
        LL bits_compressed = result.first;
		LL time_compressed = result.second;
        if( OUTPUT_VALUES ) cout << ", " << bits_compressed;
		if( OUTPUT_TIME ) cout << ", " << time_compressed;
        bits_compressed_min = min( bits_compressed_min, bits_compressed );
		time_compressed_min = min( time_compressed_min, time_compressed );
    }

    double best_reduction = 100 - ( double(bits_compressed_min) * 100.0 / bits_uncompressed );

    if( OUTPUT_VALUES ) cout << ", " << fixed << setprecision(6) << best_reduction << "%";
	if( OUTPUT_TIME ) cout << ", " << time_compressed_min;
	cout << endl;

}

pair <LL, LL> read_without_compress( string test_path ){

    LL TOTAL_BITS = 0;
	auto START_TIME = chrono::high_resolution_clock::now();

    fstream test_in;
    test_in.open( DATA_DIR + "/" + test_path );
    if( !test_in ) program_exit( "Could not access test " + test_path );

    fstream test_out;
    test_out.open( "../Output/" + test_path.substr( 0, test_path.length()-4 ) + ".0.cmp", fstream::in | fstream::out | fstream::trunc );

    function <void(int)> print_value_full = [&](int d) -> void {
		test_out << ( d < 0 ? '1' : '0' );
        test_out << bitset<BITS_USED_UNCOMPRESSED>(abs(d));
        TOTAL_BITS += BITS_USED_UNCOMPRESSED + 1;
    };

    function <void()> process_test_file = [&]() -> void {
        int current_value = STARTING_VALUE;
        int next_value, d;
        while( test_in >> next_value ){
            d = next_value - current_value;
            current_value = next_value;
            print_value_full(d);
        }
    };

    function <int(const string)> get_diff_from_bits = []( const string bits ) -> int {
        int d = stoi(bits.substr(0, INF), nullptr, 2);
        return d;
    };

    function <void()> verify_compression = [&]() -> void {
        string encoded_string, current_bits;

        test_out.clear();
        test_out.seekg(0);
        test_out >> encoded_string;

        int current_value = STARTING_VALUE;
        vector <int> ecg_values;
        for( int i = 0 ; i < encoded_string.length() ; i += BITS_USED_UNCOMPRESSED+1 ){
            current_bits = encoded_string.substr(i+1, BITS_USED_UNCOMPRESSED);

            int d = get_diff_from_bits( current_bits );
            current_value += ( encoded_string[i] == '1' ? -d : d );

            ecg_values.push_back(current_value);
        }

        test_in.clear();
        test_in.seekg(0);

        int test_val, i;
        bool failed = false;

		int test_min_val = (1<<30), test_max_val = -(1<<30);

        for( i = 0 ; test_in >> test_val && i < ecg_values.size() ; i++ ){
			test_min_val = min( test_min_val, test_val );
			test_max_val = max( test_max_val, test_val );
            if( test_val == ecg_values[i] || failed ) continue;
            cerr << "Warning! Values at spot " << i << " don't match! " << test_val << " != " << ecg_values[i] << " in case " << test_path << endl;
        }

		cerr << "MINVAL " << test_min_val << " MAXVAL " << test_max_val << endl;

        if( i != ecg_values.size() || test_in >> test_val ){
            cerr << "Warning! Something didn't reach the end of it's output." << endl;
        }
    };

    process_test_file();
    verify_compression();

    test_in.close();
    test_out.close();

	auto END_TIME = chrono::high_resolution_clock::now();

    return make_pair( TOTAL_BITS, (LL)( chrono::duration_cast<chrono::milliseconds>(END_TIME-START_TIME).count() ) );

}

pair <LL, LL> read_and_compress( string test_path, const int CMP_BITS ){

    LL TOTAL_BITS = 0;
	auto START_TIME = chrono::high_resolution_clock::now();

    fstream test_in;
    test_in.open( DATA_DIR + "/" + test_path );
    if( !test_in ) program_exit( "Could not access test " + test_path );

    fstream test_out;
    test_out.open( "../Output/" + test_path.substr( 0, test_path.length()-4 ) + "." + to_string(CMP_BITS) + ".cmp", fstream::in | fstream::out | fstream::trunc );

    function <bool(int)> can_fit_bits = [&](int d) -> bool {
        int bits = log2( abs(d) )+1;
        return bits < CMP_BITS;
    };

    function <void(int)> print_value_compressed = [&](int d) -> void {
        test_out << ( d < 0 ? '1' : '0' );
        d = abs(d);
        test_out << to_binary_string(d, CMP_BITS-1);
        TOTAL_BITS += CMP_BITS;
    };

    function <void(int)> print_value_full = [&](int d) -> void {
        test_out << '1' << to_binary_string(0, CMP_BITS-1);
		test_out << ( d < 0 ? '1' : '0' );
        test_out << bitset<BITS_USED_UNCOMPRESSED>(abs(d));
        TOTAL_BITS += CMP_BITS + BITS_USED_UNCOMPRESSED + 1;
    };

    function <void()> process_test_file = [&]() -> void {
        int current_value = STARTING_VALUE;
        int next_value, d;
        while( test_in >> next_value ){
            d = next_value - current_value;
            current_value = next_value;
            if( can_fit_bits(d) ) print_value_compressed(d);
            else                  print_value_full(d);
        }
    };

    function <bool(const string)> diff_bits_valid = [&]( const string bits ) -> bool {
        return stoi(bits, nullptr, 2) != (1 << (CMP_BITS-1));
    };

    function <int(const string)> get_value_from_bits = []( const string bits ) -> int {
        return stoi(bits, nullptr, 2);
    };

    function <int(const string)> get_diff_from_bits = []( const string bits ) -> int {
        int d = stoi(bits.substr(1, INF), nullptr, 2);
        if( bits[0] == '1' ) d = -d;
        return d;
    };

    function <void()> verify_compression = [&]() -> void {
        string encoded_string, current_bits;

        test_out.clear();
        test_out.seekg(0);
        test_out >> encoded_string;

        int current_value = STARTING_VALUE;
        vector <int> ecg_values;
        for( int i = 0 ; i < encoded_string.length() ; i += CMP_BITS ){
            current_bits = encoded_string.substr(i, CMP_BITS);

            if( !diff_bits_valid(current_bits) ){
                current_value += ( encoded_string[i+CMP_BITS] == '1' ? -1 : 1 ) * get_value_from_bits( encoded_string.substr(i+CMP_BITS+1, BITS_USED_UNCOMPRESSED) );
                i += BITS_USED_UNCOMPRESSED + 1;
            }else{
                int d = get_diff_from_bits( current_bits );
                current_value += d;
            }

            ecg_values.push_back(current_value);
        }

        test_in.clear();
        test_in.seekg(0);

        int test_val, i;
        bool failed = false;

        for( i = 0 ; test_in >> test_val && i < ecg_values.size() ; i++ ){
            if( test_val == ecg_values[i] || failed ) continue;
            cerr << "Warning! Values at spot " << i << " don't match! " << test_val << " != " << ecg_values[i] << " in case " << test_path << " and cmp=" << CMP_BITS  << endl;
        }

        if( i != ecg_values.size() || test_in >> test_val ){
            cerr << "Warning! Something didn't reach the end of it's output." << endl;
        }
    };

    process_test_file();
    verify_compression();

    test_in.close();
    test_out.close();

    auto END_TIME = chrono::high_resolution_clock::now();

    return make_pair( TOTAL_BITS, (LL)( chrono::duration_cast<chrono::milliseconds>(END_TIME-START_TIME).count() ) );

}

void print_header_items(){

	function <void()> print_time_headers_only = []() -> void {
		cout << "Name, No Compression, ";
		for( int i = LEFT_RANGE ; i <= RIGHT_RANGE ; i++ ) cout << i << " Bits, ";
		cout << "Best Compression Time" << endl;
	};

	function <void()> print_value_headers_only = []() -> void {
		cout << "Name, No Compression, ";
		for( int i = LEFT_RANGE ; i <= RIGHT_RANGE ; i++ ) cout << i << " Bits, ";
		cout << "Best Reduction" << endl;
	};

	function <void()> print_both_headers = []() -> void {
		cout << "Name, No Compression Size, No Compression Time, ";
		for( int i = LEFT_RANGE ; i <= RIGHT_RANGE ; i++ ) cout << i << " Bits Size, " << i << " Bits Time, ";
		cout << "Best Reduction, Best Compression Time" << endl;
	};


	if( !OUTPUT_TIME && !OUTPUT_VALUES ) return;

	if( OUTPUT_TIME && !OUTPUT_VALUES ) print_time_headers_only();

	if( !OUTPUT_TIME && OUTPUT_VALUES ) print_value_headers_only();

	if( OUTPUT_TIME && OUTPUT_VALUES ) print_both_headers();

}

string to_binary_string( int x, const int cmp ){
    string rv = "";
    for( int i = 0 ; i < cmp ; i++ ){
        rv += (x%2) + '0';
        x /= 2;
    } reverse( rv.begin(), rv.end() );
    return rv;
}

void program_exit( string msg = "" ){
    if( msg != "" ) cerr << msg << endl;
    exit(0);
}

void open_file( fstream &file, const string file_path ){
    file.open( file_path );
    if( !file ) program_exit( "Could not open file at path: " + file_path );
}