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
const int runs_per_file = 25;

int main(
    int argc,
    char ** argv
) {
    MPI_Handler::mpi_init();

    if( !MPI_Handler::get_rank() ) Compression::print_parameters( "*", MPI_Handler::get_size(), true );

    vector <string> ecg_files = Data_Handler::get_files_in_dir( data_dir );

    for( const string &ecg_file : ecg_files ) {

        const string file_name = ecg_file;
        const string file_name_data = data_dir + file_name;
        const string file_name_output = output_dir + file_name;

        int total_time = 0;

        for( int run = 1 ; run <= runs_per_file ; run++ ) {

            MPI_Handler::sync();

            vector <Timer *> run_timers = MPI_Handler::run( file_name_data, file_name_output );

            MPI_Handler::sync();

            if( !MPI_Handler::get_rank() ) {
                for( Timer * t : run_timers ) {
                    total_time += t->check();
                    delete t;
                }
            }

            if( !MPI_Handler::get_rank() ) Compression::verify_compression( Data_Handler( file_name_data ).read(), Data_Handler( file_name_output ).read() );
        
        }

        if( !MPI_Handler::get_rank() ) cout << file_name << " - Average time per run: " << total_time / runs_per_file << " ms." << endl << endl;

    }

    MPI_Handler::mpi_finalize();
}
