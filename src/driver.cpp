/* Paper: Benchmarking ECG Compression Based on Successive Differences using Parallelism
 * Author: Andrej Shekerov (andrej.shekerov@students.finki.ukim.mk)
 */
#include <bits/stdc++.h>
using namespace std;

#include "mpi_handler.hpp"
#include "timer.hpp"

int main(
    int argc,
    char ** argv
) {
    MPI_Handler::mpi_init();

    Timer *timer = ( MPI_Handler::get_rank() ? nullptr : new Timer( "ECG Total Compression Time" ) );

    MPI_Handler::run( argc, argv );

    delete timer;

    MPI_Handler::mpi_finalize();
}
