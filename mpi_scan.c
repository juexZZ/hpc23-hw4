// mpi scan
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

// seq scan
void scan_seq(long* prefix_sum, const long* A, long n) {
  if (n == 0) return;
  prefix_sum[0] = A[0];
  for (long i = 1; i < n; i++) {
    prefix_sum[i] = prefix_sum[i-1] + A[i];
  }
}

int main(){
    long N = 100000000;
    MPI_init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, nprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nprocs);

    // initialize A on process 0
    long* A, B, B0;
    if (rank == 0){
        A = (long*) malloc(N * sizeof(long));
        for (long i = 0; i < N; i++) A[i] = rand();
        B = (long*) malloc(N * sizeof(long)); // result
        B0 = (long*) malloc(N * sizeof(long)); // result from seq scan
        scan_seq(B0, A, N);
    }
    // split up the vector A and communicate it to the other processors whith MPI_Scatter
    long* A_local = (long*) malloc(N / nprocs * sizeof(long));
    long* A_local_scan = (long*) malloc(N / nprocs * sizeof(long));
    MPI_Scatter(A, N / nprocs, MPI_LONG, A_local, N / nprocs, MPI_LONG, 0, comm); // order is by the rank
    // a local scan on A_local
    A_local_scan[0] = A_local[0];
    for (long i = 1; i < N / nprocs; i++){
        A_local_scan[i] = A_local_scan[i-1] + A_local[i];
    }
    long local_offset = A_local_scan[N / nprocs - 1];
    printf("rank %d has offset %ld\n", rank, local_offset);
    // shares offests using MPI_Allgather
    long* offsets = (long*) malloc(nprocs * sizeof(long));
    MPI_Allgather(&local_offset, 1, MPI_LONG, offsets, 1, MPI_LONG, comm);
    // adds local_offsets from the smaller ranks to correct its local scan
    long corrected_offset = 0;
    for (int j = 0; j < rank; j++){
        corrected_offset += offsets[j];
    }
    for (long i = 0; i < N / nprocs; i++){
        A_local_scan[i] += corrected_offset;
    }
    // Each processor sends its local scan to process 0 using MPI_Gather, for checking
    MPI_Gather(A_local_scan, N / nprocs, MPI_LONG, B, N / nprocs, MPI_LONG, 0, comm);
    // check
    if(rank==0){
        long err = 0;
        for (long i = 0; i < N; i++) err = std::max(err, std::abs(B0[i] - B[i]));
        printf("check on process 0, error = %ld\n", err);
    }
    // free
    if (rank == 0){
        free(A);
        free(B);
        free(B0);
    }
    free(A_local);
    free(A_local_scan);
    free(offsets);
    MPI_Finalize();
    return 0;
}