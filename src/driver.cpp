/* Paper: Benchmarking ECG Compression Based on Successive Differences using Parallelism
 * Author: Andrej Shekerov (andrej.shekerov@students.finki.ukim.mk)
 */
#include <bits/stdc++.h>
using namespace std;

#include "mpi_handler.hpp"

int main( int argc, char ** argv )
{
    MPI_Handler::mpi_init();

    MPI_Handler::run( argc, argv );

    MPI_Handler::mpi_finalize();
}

/*  TODO: Automate file compression, decompression and verification.
        *) For each file we run, we should automatically compress the data, decompress the data, and then verify the compression using the verifier
        *) Allow for a set of files to be run at once, instead of specifying which file runs at each program start
        *) We should also allow running the compression algorithm on the same file with different parameters for the COMPRESSION_DIFFERENCE_BITS value

    TODO: Benchmark execution time.
        *) Execution time should be measured for each test, for each set of parameters, and for each step of the process (compression, decompression)
*/