```
// Comparison with std::sort on a vector of 50 million elements:
// Compiled with clang++ with -O3 optimization.
 
Type: vector<int8_t>
Contains uniformly distributed values from -128 to 127
Radix sort | Time 0.028532 sec, random element: -107
std::sort  | Time 1.48961 sec, random element: 112
Radix sort is 52.2083 times faster

Type: vector<uint8_t>
Contains uniformly distributed values from 0 to 255
Radix sort | Time 0.028995 sec, random element: 162
std::sort  | Time 1.45872 sec, random element: 74
Radix sort is 50.3093 times faster

Type: vector<int16_t>
Contains uniformly distributed values from -32768 to 32767
Radix sort | Time 0.153558 sec, random element: -22616
std::sort  | Time 2.40973 sec, random element: -1000
Radix sort is 15.6927 times faster

Type: vector<uint16_t>
Contains uniformly distributed values from 0 to 65535
Radix sort | Time 0.15496 sec, random element: 26069
std::sort  | Time 2.39205 sec, random element: 65221
Radix sort is 15.4366 times faster

Type: vector<int32_t>
Contains uniformly distributed values from -2.14748e+09 to 2.14748e+09
Radix sort | Time 0.367208 sec, random element: 1.84854e+09
std::sort  | Time 3.43377 sec, random element: 1.25765e+09
Radix sort is 9.35103 times faster

Type: vector<uint32_t>
Contains uniformly distributed values from 0 to 4.29497e+09
Radix sort | Time 0.365617 sec, random element: 2.16502e+09
std::sort  | Time 3.46304 sec, random element: 4.20348e+07
Radix sort is 9.47176 times faster

Type: vector<uint64_t>
Contains uniformly distributed values from 0 to 1.84467e+19
Radix sort | Time 1.09125 sec, random element: 1.23082e+19
std::sort  | Time 3.54483 sec, random element: 9.29472e+17
Radix sort is 3.24841 times faster

Type: vector<int64_t>
Contains uniformly distributed values from -9.22337e+18 to 9.22337e+18
Radix sort | Time 1.09544 sec, random element: 7.3408e+18
std::sort  | Time 3.5584 sec, random element: -2.75899e+18
Radix sort is 3.24839 times faster

Type: vector<float>
Contains uniformly distributed values from 1.17549e-38 to 3.40282e+38
Radix sort | Time 0.401897 sec, random element: 1.03343e+38
std::sort  | Time 3.84228 sec, random element: 2.74816e+38
Radix sort is 9.56037 times faster

Radix sort is a linear time, linear space sorting algorithm.
Unlike most other sorting algorithms, it doesn't use comparisons.
It works with data on the level of individual bytes.
Based on input, it builds byte histograms, based on them,
it builds offset tables for reordering. Then it makes a pass,
which consists of copying and reordering data. One pass per
one byte of element type.
The library implements radix sort for most built in
arithmetic types. This implementation sorts, in ascending order,
a contiguous array of elements of the following types:
- 8/16/32/64 bit signed/unsigned integers
- 32 bit floating point numbers.
For 8 bit types, complexity is O(n) time, O(1) space
For all other types, complexity is O(n) time, O(n) space.
Calling interface:
radix_sort(P, N, M) or radix_sort(P, N), where
P is a raw pointer to data, N is a number of data elements,
M is an optional pointer to memory buffer.
The last argument(M) lets you provide memory buffer externally.
There is no requirement for initialization of said memory buffer.
This buffer should have at least the same size as data being sorted
and will be used for copying data during reordering passes.
If you don't provide the buffer, radix_sort allocates the
buffer on its own and frees it afterwards. Specializations
of radix_sort for 8 bit types don't allocate additional memory and
don't take buffer as the last argument.
```
