# hpc23-hw4

Juexiao Zhang, jz4725
---------------------

## Q1

process 0 is at node cs488.hpc.nyu.edu

process 1 is at node cs489.hpc.nyu.edu

pingpong latency: 1.110422e-03 ms

pingpong bandwidth: 1.002979e+01 GB/s

See `slurm\_hwq1.out` for the HPC output.

## Q2

process 0 is at node cs493.hpc.nyu.edu
process 1 is at node cs494.hpc.nyu.edu
process 3 is at node cs496.hpc.nyu.edu
process 2 is at node cs495.hpc.nyu.edu
N = 10
Int ring latency = 3.865903e-01 ms
Array ring get bandwidth = 1.120697e+01 GB/s

N = 100

Int ring latency = 3.218137e-02 ms
Array ring get bandwidth = 1.169816e+01 GB/s

Latency acutally gets better from 10 to 100. Speculate that some overhead in communication is optimized in the system if communication pattern is fixed, or the overhead is fixed for fixed (repreated) communication pattern and got amortized when calculating the latency.

The output can be found at `slurm\_hwq2q3.out`

## Q3

I picked the first task: scan with MPI. The output can be found at `slurm\_hwq2q3.out`, where I printed the partial(local) offset of each processors and checked the result against sequential version and got 0 error.

## Q4

I will be doing the project with Yiwei on implementing a sparse attention mechanism (proposed by OpenAI for effciently modeling long sequence in Transformer for large NN models) with OpenMP and MPI.

We have pitched our project at your office the other day.process 0 is at node cs493.hpc.nyu.edu

This is proc 0, Nloops = 100, comm size = 4, ring_value = 0
process 1 is at node cs494.hpc.nyu.edu
process 2 is at node cs495.hpc.nyu.edu
process 3 is at node cs496.hpc.nyu.edu
This is proc 0, Nloops = 100, comm size = 4, ring_value = 600
Int ring latency = 3.218137e-02 ms
Array ring get bandwidth = 1.169816e+01 GB/s
