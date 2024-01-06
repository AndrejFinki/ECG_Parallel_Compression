/* Paper: Benchmarking ECG Compression Based on Successive Differences using Parallelism
 * Author: Andrej Shekerov (andrej.shekerov@students.finki.ukim.mk)
 */
#include <bits/stdc++.h>
using namespace std;

#include "mpi_handler.hpp"
#include "timer.hpp"
#include "compression.hpp"

const string data_dir = "../ECG_Parallel_Compression/data/";
const string output_dir = "../ECG_Parallel_Compression/output/";

int main(
    int argc,
    char ** argv
) {
    const string file_name = argv[1];
    const string file_name_data = data_dir + file_name;
    const string file_name_output = output_dir + file_name;

    MPI_Handler::mpi_init();

    if( !MPI_Handler::get_rank() ) Compression::print_parameters( file_name, MPI_Handler::get_size(), true );

    Timer *timer_compression = ( MPI_Handler::get_rank() ? nullptr : new Timer( "ECG Total Compression Time" ) );

    MPI_Handler::run( file_name_data, file_name_output );

    MPI_Handler::sync();

    delete timer_compression;

    if( !MPI_Handler::get_rank() ) Compression::verify_compression( Data_Handler( file_name_data ).read(), Data_Handler( file_name_output ).read() );

    MPI_Handler::mpi_finalize();
}
