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
    Timer *timer = new Timer( "ECG Total Compression Time" );

    MPI_Handler::mpi_init();

    MPI_Handler::run( argc, argv );

    MPI_Handler::mpi_finalize();

    delete timer;
}
