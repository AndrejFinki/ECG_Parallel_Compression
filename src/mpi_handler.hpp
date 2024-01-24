#ifndef MPI_HANDLER_H
#define MPI_HANDLER_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "ecg_process.hpp"

class MPI_Handler {

public:

    MPI_Handler() = default;

    static void run( const string &, const string & );
    static void mpi_init();
    static void mpi_finalize();
    static int get_rank();
    static int get_size();
    static void sync();

};

void MPI_Handler::run(
    const string &file_name_data,
    const string &file_name_output
) {
    ECG_Process *process = new ECG_Process_Method_1( MPI_Handler::get_rank() );
    process->set_input( file_name_data );
    process->set_output( file_name_output );

    switch( MPI_Handler::get_rank() )
    {
        case 0: {
            vector <int> compressed_data = process->main_process( process->get_input(), MPI_Handler::get_size() );
            process->write_output( &compressed_data ); 
            break;
        }

        default: {
            process->secondary_process();
        }
    }

    delete process;
}

void MPI_Handler::mpi_init()
{
    int argc = 1;
    char *argv;
    char **argv1 = &argv;
    MPI_Init( &argc, &argv1 );
}

void MPI_Handler::mpi_finalize()
{
    MPI_Finalize();
}

int MPI_Handler::get_rank()
{
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    return rank;
}

int MPI_Handler::get_size()
{
    int size;
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    return size;
}

void MPI_Handler::sync()
{
    MPI_Barrier( MPI_COMM_WORLD );
}

#endif