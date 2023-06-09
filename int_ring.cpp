//int_ring.cpp
// Write a distributed memory program that sends an integer in a ring 
// starting from process 0 to 1 to 2 (and so on). 
// The last process sends the message back to process 0. Perform this loop N times, 
// where N is set in the program or on the command line.
#include <stdlib.h>
#include <mpi.h>

double int_ring(int Nloops, int rank, int left, int right, int& ring_value, MPI_Comm comm){
    //start
    MPI_Barrier(comm);
    double tt = MPI_Wtime();
    //printf("I am processor %d, my left %d, my right %d\n", rank, left, right);
    for (int i=0; i<Nloops; i++){
	MPI_Status status;
	//printf("Process %d iteration %d\n", rank, i);
        if (rank == 0 && i == 0){
            // start at process 0 with value 0
            MPI_Send(&ring_value, 1, MPI_INT, right, 0, comm);
	    //printf("Iteration %d send at process %d with value %d\n", i, rank, ring_value);
            //MPI_Recv(&ring_value, 1, MPI_INT, left, i, comm, &status);
        }
        else{
	    //printf("Process %d waits for receiving from %d at iteration %d\n", rank, left, i);
            MPI_Recv(&ring_value, 1, MPI_INT, left, 0, comm, &status);
	    //printf("Iteration %d Process %d Received %d from Process %d\n", i, rank, ring_value, left);
            ring_value += rank; // add its rank to the value and pass it on
            MPI_Send(&ring_value, 1, MPI_INT, right, 0, comm);
	    //printf("Iteration %d Process %d send %d to Process %d\n", i, rank, ring_value, right);
        }
    }
    if(rank==0){
	MPI_Status status;
	MPI_Recv(&ring_value, 1, MPI_INT, left, 0, comm, &status); // final receive, ring close
    }
    tt = MPI_Wtime() - tt;
    return tt;
}

double array_ring(int Nloops, int rank, int left, int right, long array_size, MPI_Comm comm){
    //start
    char* msg = (char*) malloc(array_size*sizeof(char));
    for (long i = 0; i < array_size; i++) msg[i] = 42;

    MPI_Barrier(comm);
    double tt = MPI_Wtime();
    for (int i=0; i<Nloops; i++){
        MPI_Status status;
	if (rank == 0 && i == 0){
            // start at process 0 with value 0
            MPI_Send(msg, array_size, MPI_CHAR, right, 0, comm);
            //MPI_Recv(&msg, array_size, MPI_CHAR, left, 0, comm, &status);
        }
        else{
            MPI_Recv(msg, array_size, MPI_CHAR, left, 0, comm, &status);
            MPI_Send(msg, array_size, MPI_CHAR, right, 0, comm);
        }
    }
    if(rank==0){
	MPI_Status status;
	MPI_Recv(msg, array_size, MPI_CHAR, left, 0, comm, &status);
    }
    tt = MPI_Wtime() - tt;
    free(msg);
    return tt;
}

int main(int argc, char *argv[]){
    int rank, size;
    int Nloops = atoi(argv[1]);
    MPI_Status status;
    int ring_value = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    char proc_name[MPI_MAX_PROCESSOR_NAME];
    int namelen;
    MPI_Get_processor_name(proc_name, &namelen);
    printf("process %d is at node %s\n", rank, proc_name);
    int left = rank-1;
    int right = rank+1;
    if (left < 0) left = size-1;
    if (right > size-1) right = 0;
    if (!rank) printf("This is proc 0, Nloops = %d, comm size = %d, ring_value = %d\n", Nloops, size, ring_value);
    double tt = int_ring(Nloops, rank, left, right, ring_value, comm);
    if (!rank) printf("This is proc %d, Nloops = %d, comm size = %d, ring_value = %d\n", rank, Nloops, size, ring_value);
    if (!rank) printf("Int ring latency = %e ms\n", tt/(size*Nloops)*1000);
    // array ring
    long array_size = 2000000; //2048*1024;
    double tt_array = array_ring(Nloops, rank, left, right, array_size, comm);
    if(!rank) printf("Array ring get bandwidth = %e GB/s\n*******\n", (size*array_size*Nloops)/tt_array/1024/1024/1024);

    MPI_Finalize();
    return 0;
}
