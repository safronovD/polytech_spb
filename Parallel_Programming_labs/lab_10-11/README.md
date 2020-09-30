## Usage

make run proc_count=4

## 

Productivity = Performance / Complexity

Performance = 1 / Execution_time

Complexity = Program_codelines

```
MPI_Reduce_scatter complexity = 24
my_reduce_scatter complexity = 40
```

Tables legend : ↓ N_trials, → N_proc

MPI_Reduce_scatter Execution_time: 

|||||
|---|---|---|---|
| -      |2     |3     |4
|1000    |0.0007|0.0015|0.0019
|10000   |0.0059|0.0167|0.0175
|100000  |0.0668|0.1571|0.1688
|1000000 |0.6550|1.6143|1.6943

my_reduce_scatter Execution_time:

|||||
|---|---|---|---|
| -      |2     |3     |4
|1000    |0.0012|0.0014|0.0029
|10000   |0.0061|0.0139|0.0208
|100000  |0.0695|0.1459|0.2060
|1000000 |0.6621|1.5018|2.1668

For 1000000 trials

Performance :

|Processors|MPI_Reduce_scatter|my_reduce_scatter|ratio|
|---|---|---|---|
|2  |1.5267|1.5103|1.0108|
|3  |0.6194|0.6658|0.9303|
|4  |0.5902|0.4615|1.2788|

|Processors|MPI_Reduce_scatter|my_reduce_scatter|ratio|
|---|---|---|---|
|2  |0.0636|0.0377|1.6870|
|3  |0.0258|0.0166|1.5542|
|4  |0.0245|0.0115|2.1304|
