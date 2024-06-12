/* Paper: Benchmarking ECG Compression Based on Successive Differences using Parallelism
 * Author: Andrej Shekerov (andrej.shekerov@students.finki.ukim.mk)
 */
#include <bits/stdc++.h>
using namespace std;

#include "../headers/mpi_handler.hpp"
#include "../headers/timer.hpp"
#include "../headers/compression.hpp"

const string data_dir = "../ECG_Parallel_Compression/data/";
const string output_dir = "../ECG_Parallel_Compression/output/";
const int runs_per_file = 12;

int main(
    int argc,
    char ** argv
) {
    MPI_Handler::mpi_init();

    if( !MPI_Handler::get_rank() )
        Compression::print_parameters( "*", MPI_Handler::get_size(), true );

    vector <string> ecg_files = Data_Handler::get_files_in_dir( data_dir, "files_" + to_string( MPI_Handler::get_rank() ) );

    for( const string &ecg_file : ecg_files ) {
        const string file_name = ecg_file;
        const string file_name_data = data_dir + file_name;
        const string file_name_output = output_dir + file_name;
        vector <int> timer_list;
        
        for( int run = 1 ; run <= runs_per_file ; run++ ) {
            Timer *t = nullptr;
            Data_Handler *input_dh = nullptr;
            Data_Handler *output_dh = nullptr;

            if( !MPI_Handler::get_rank() ) {
                input_dh = new Data_Handler( file_name_data );
                output_dh = new Data_Handler( file_name_output );
                input_dh->read();
                while( input_dh->data()->size() % MPI_Handler::get_size() )
                    input_dh->data()->pop_back();
                t = new Timer( "Global Timer" );
            }
            
            MPI_Handler::sync();

            MPI_Handler::run( input_dh->data(), output_dh->data() );

            MPI_Handler::sync();

            if( !MPI_Handler::get_rank() ) {
                timer_list.push_back( t->check() );
                Compression::verify_compression( input_dh->data(), output_dh->data() );
                delete t;
                delete input_dh;
                delete output_dh;
            }   
        }

        if( !MPI_Handler::get_rank() ) {
            sort( timer_list.begin(), timer_list.end() );
            int timer_sum = 0, timers_taken = timer_list.size();
            for( const int &i : timer_list )
                timer_sum += i;
            if( timer_list.size() > 2 ) {
                timer_sum -= timer_list[0] + timer_list[ timers_taken - 1 ];
                timers_taken -= 2;
            }
            cout << file_name << " - Average time per run: " << timer_sum / timers_taken << " ms." << endl << endl;
        }
    }

    MPI_Handler::mpi_finalize();
}
