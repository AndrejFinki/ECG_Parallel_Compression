#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <bits/stdc++.h>
using namespace std;

class Compression {

public:

    Compression() = default;

    static const int COMPRESSION_DIFFERENCE_BITS = 3;
    static const int COMPRESSION_DIFFERENCE_LIMIT = ( 1 << ( COMPRESSION_DIFFERENCE_BITS - 1 ) ) - 1;
    static const int UNCOMPRESSED_VALUE_BITS = 10;
    static const int UNCOMPRESSED_VALUE_LIMIT = ( 1 << UNCOMPRESSED_VALUE_BITS ) - 1;

    static void inplace_compress( vector <int> & );
    static void inplace_decompress( vector <int> & );

private:

    static int compress_value( int, int );
    static int decompress_value( int, int );
    static int encode_difference_small( int );
    static int encode_difference_big( int );
    static int decode_difference_small( int, int );
    static int decode_difference_big( int );

};

void Compression::inplace_compress(
    vector <int> &data
) {
    for( int i = data.size() - 1 ; i ; i-- ) {
        data[i] = compress_value( data[i-1], data[i] );
    }
    data[0] = compress_value( -( 1 << 10 ), data[0] );
}

int Compression::compress_value(
    int p,
    int c
) {
    assert( c >= 0 && "Found negative ECG value!" );
    assert( c <= UNCOMPRESSED_VALUE_LIMIT && "Found ECG value that exceedes UNCOMPRESSED_VALUE_LIMIT!" );
    int difference = c - p;
    return abs( difference ) <= COMPRESSION_DIFFERENCE_LIMIT ? encode_difference_small( difference ) : encode_difference_big( c );
}

int Compression::encode_difference_small(
    int difference
) {
    bitset <( COMPRESSION_DIFFERENCE_BITS )> encoded_difference( abs( difference ) );
    encoded_difference.set( COMPRESSION_DIFFERENCE_BITS - 1, difference < 0 ? 1 : 0 );
    return ( int )( encoded_difference.to_ulong() );
}

int Compression::encode_difference_big(
    int current_value
) {
    bitset <( COMPRESSION_DIFFERENCE_BITS + UNCOMPRESSED_VALUE_BITS )> encoded_difference( current_value );
    encoded_difference.set( COMPRESSION_DIFFERENCE_BITS + UNCOMPRESSED_VALUE_BITS - 1 );
    return ( int )( encoded_difference.to_ulong() );
}

void Compression::inplace_decompress(
    vector <int> &data
) {
    data[0] = decompress_value( 0, data[0] );
    for( int i = 1 ; i < data.size() ; i++ ) {
        data[i] = decompress_value( data[i-1], data[i] );
    }
}

int Compression::decompress_value(
    int p,
    int c
) {
    return c >= ( 1 << COMPRESSION_DIFFERENCE_BITS ) ? decode_difference_big( c ) : decode_difference_small( p, c );
}

int Compression::decode_difference_small(
    int p,
    int c
) {
    bitset <( COMPRESSION_DIFFERENCE_BITS )> encoded_difference( c );
    bool sign_bit = encoded_difference.test( COMPRESSION_DIFFERENCE_BITS - 1 );
    encoded_difference.reset( COMPRESSION_DIFFERENCE_BITS - 1 );
    return p + encoded_difference.to_ulong() * ( sign_bit ? -1 : 1 );
}

int Compression::decode_difference_big(
    int value
) {
    bitset <( COMPRESSION_DIFFERENCE_BITS + UNCOMPRESSED_VALUE_BITS )> encoded_difference( value );
    encoded_difference.reset( COMPRESSION_DIFFERENCE_BITS + UNCOMPRESSED_VALUE_BITS - 1 );
    return ( int )( encoded_difference.to_ulong() );
}

#endif