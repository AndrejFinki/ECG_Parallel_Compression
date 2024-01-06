/* Paper: Benchmarking ECG Compression Based on Successive Differences using Parallelism
 * Author: Andrej Shekerov (andrej.shekerov@students.finki.ukim.mk)
 */
#include <bits/stdc++.h>
using namespace std;

#include "mpi_handler.hpp"
#include "timer.hpp"
#include "compression.hpp"

int main(
    int argc,
    char ** argv
) {
    const string file_name = argv[1];
    const string file_name_data = "../ECG_Parallel_Compression/data/" + file_name;
    const string file_name_output = "../ECG_Parallel_Compression/output/" + file_name;

    MPI_Handler::mpi_init();

    if( !MPI_Handler::get_rank() ) Compression::print_parameters( argv[1], MPI_Handler::get_size() );

    Timer *timer_compression = ( MPI_Handler::get_rank() ? nullptr : new Timer( "ECG Total Compression Time" ) );

    MPI_Handler::run( file_name_data, file_name_output );

    delete timer_compression;

    MPI_Handler::mpi_finalize();
}
