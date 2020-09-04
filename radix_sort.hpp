#include <cstdint>
#include <climits>

#ifndef RADIX_SORT_H
#define RADIX_SORT_H

/*
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
*/

/* Radix sort is a linear time, linear space sorting algorithm.
 * Unlike most other sorting algorithms, it doesn't use comparisons.
 * It works with data on the level of individual bytes.
 * Based on input, it builds byte histograms, based on them,
 * it builds offset tables for reordering. Then it makes a pass,
 * which consists of copying and reordering data. One pass per
 * one byte of element type.
 * The library implements radix sort for most built in
 * arithmetic types. This implementation sorts, in ascending order,
 * a contiguous array of elements of the following types:
 * - 8/16/32/64 bit signed/unsigned integers
 * - 32 bit floating point numbers.
 * For 8 bit types, complexity is O(n) time, O(1) space
 * For all other types, complexity is O(n) time, O(n) space.
 * Calling interface:
 * radix_sort(P, N, M) or radix_sort(P, N), where
 * P is a raw pointer to data, N is a number of data elements,
 * M is an optional pointer to memory buffer.
 * The last argument(M) lets you provide memory buffer externally.
 * There is no requirement for initialization of said memory buffer.
 * This buffer should have at least the same size as data being sorted
 * and will be used for copying data during reordering passes.
 * If you don't provide the buffer, radix_sort allocates the
 * buffer on its own and frees it afterwards. Specializations
 * of radix_sort for 8 bit types don't allocate additional memory and
 * don't take buffer as the last argument.
 */

const unsigned values_in_byte = 1U << CHAR_BIT;

#define byte0(value) ((value) & 255)
#define byte1(value) (((value) >> 8) & 255)
#define byte2(value) (((value) >> 16) & 255)
#define byte3(value) (((value) >> 24) & 255)
#define byte4(value) (((value) >> 32) & 255)
#define byte5(value) (((value) >> 40) & 255)
#define byte6(value) (((value) >> 48) & 255)
#define byte7(value) ((value) >> 56)

#define COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist, extract_byte_f) \
    { \
        unsigned unroll_count = count >> 2; \
        T* i = src; \
        for (; unroll_count; --unroll_count, i += 4) \
        { \
            auto p0 = bitwise_transform(*i); \
            auto p1 = bitwise_transform(*(i+1)); \
            auto p2 = bitwise_transform(*(i+2)); \
            auto p3 = bitwise_transform(*(i+3)); \
            \
            dst[++hist[extract_byte_f(p0)]] = *i; \
            dst[++hist[extract_byte_f(p1)]] = *(i+1); \
            dst[++hist[extract_byte_f(p2)]] = *(i+2); \
            dst[++hist[extract_byte_f(p3)]] = *(i+3); \
        } \
        for (; i != src_end; ++i) \
        { \
            auto p = bitwise_transform(*i); \
            dst[++hist[extract_byte_f(p)]] = *i; \
        } \
    }

inline void radix_sort_calculate_offset_table_16(unsigned* hist0, unsigned* hist1)
{
    unsigned offset0 = 0;
    unsigned offset1 = 0;
 
     for (unsigned int i = 0; i < values_in_byte; i += 4)
    {
        unsigned temp_offset = hist0[i] + offset0;
        hist0[i] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+1] + offset0; 
        hist0[i+1] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+2] + offset0; 
        hist0[i+2] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+3] + offset0; 
        hist0[i+3] = offset0 - 1; 
        offset0 = temp_offset;

        temp_offset = hist1[i] + offset1; 
        hist1[i] = offset1 - 1; 
        offset1 = temp_offset;
        
        temp_offset = hist1[i+1] + offset1; 
        hist1[i+1] = offset1 - 1; 
        offset1 = temp_offset;
        
        temp_offset = hist1[i+2] + offset1; 
        hist1[i+2] = offset1 - 1; 
        offset1 = temp_offset;
        
        temp_offset = hist1[i+3] + offset1; 
        hist1[i+3] = offset1 - 1; 
        offset1 = temp_offset;
    }
}

inline void radix_sort_calculate_offset_table_32(unsigned* hist0, unsigned* hist1,
                                                 unsigned* hist2, unsigned* hist3)
{
    unsigned offset0 = 0;
    unsigned offset1 = 0; 
    unsigned offset2 = 0;
    unsigned offset3 = 0;

    for (unsigned i = 0; i < values_in_byte; i += 4)
    {
        unsigned temp_offset = hist0[i] + offset0;
        hist0[i] = offset0 - 1;
        offset0 = temp_offset;

        temp_offset = hist0[i+1] + offset0;
        hist0[i+1] = offset0 - 1;
        offset0 = temp_offset;

        temp_offset = hist0[i+2] + offset0;
        hist0[i+2] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+3] + offset0;
        hist0[i+3] = offset0 - 1;
        offset0 = temp_offset;

        temp_offset = hist1[i] + offset1;
        hist1[i] = offset1 - 1;
        offset1 = temp_offset;

        temp_offset = hist1[i+1] + offset1;
        hist1[i+1] = offset1 - 1;
        offset1 = temp_offset;

        temp_offset = hist1[i+2] + offset1;
        hist1[i+2] = offset1 - 1;
        offset1 = temp_offset;

        temp_offset = hist1[i+3] + offset1;
        hist1[i+3] = offset1 - 1;
        offset1 = temp_offset;

        temp_offset = hist2[i] + offset2;
        hist2[i] = offset2 - 1;
        offset2 = temp_offset;

        temp_offset = hist2[i+1] + offset2;
        hist2[i+1] = offset2 - 1;
        offset2 = temp_offset;

        temp_offset = hist2[i+2] + offset2;
        hist2[i+2] = offset2 - 1;
        offset2 = temp_offset;

        temp_offset = hist2[i+3] + offset2;
        hist2[i+3] = offset2 - 1;
        offset2 = temp_offset;

        temp_offset = hist3[i] + offset3;
        hist3[i] = offset3 - 1;
        offset3 = temp_offset;

        temp_offset = hist3[i+1] + offset3;
        hist3[i+1] = offset3 - 1;
        offset3 = temp_offset;

        temp_offset = hist3[i+2] + offset3;
        hist3[i+2] = offset3 - 1;
        offset3 = temp_offset;

        temp_offset = hist3[i+3] + offset3;
        hist3[i+3] = offset3 - 1;
        offset3 = temp_offset;
    }
}

inline void radix_sort_calculate_offset_table_64(unsigned* hist0, unsigned* hist1,
                                                 unsigned* hist2, unsigned* hist3,
                                                 unsigned* hist4, unsigned* hist5,
                                                 unsigned* hist6, unsigned* hist7)
{
    unsigned offset0 = 0;
    unsigned offset1 = 0;
    unsigned offset2 = 0;
    unsigned offset3 = 0;
    unsigned offset4 = 0;
    unsigned offset5 = 0;
    unsigned offset6 = 0;
    unsigned offset7 = 0;

    for (unsigned int i = 0; i < values_in_byte; i += 4)
    {
        unsigned temp_offset = hist0[i] + offset0; 
        hist0[i] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+1] + offset0; 
        hist0[i+1] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+2] + offset0; 
        hist0[i+2] = offset0 - 1; 
        offset0 = temp_offset;
        
        temp_offset = hist0[i+3] + offset0; 
        hist0[i+3] = offset0 - 1; 
        offset0 = temp_offset;

        temp_offset = hist1[i] + offset1; 
        hist1[i] = offset1 - 1; 
        offset1 = temp_offset;
        
        temp_offset = hist1[i+1] + offset1; 
        hist1[i+1] = offset1 - 1; 
        offset1 = temp_offset;
        
        temp_offset = hist1[i+2] + offset1; 
        hist1[i+2] = offset1 - 1; 
        offset1 = temp_offset;
        
        temp_offset = hist1[i+3] + offset1; 
        hist1[i+3] = offset1 - 1; 
        offset1 = temp_offset;

        temp_offset = hist2[i] + offset2; 
        hist2[i] = offset2 - 1; 
        offset2 = temp_offset;
        
        temp_offset = hist2[i+1] + offset2; 
        hist2[i+1] = offset2 - 1; 
        offset2 = temp_offset;
        
        temp_offset = hist2[i+2] + offset2; 
        hist2[i+2] = offset2 - 1; 
        offset2 = temp_offset;
        
        temp_offset = hist2[i+3] + offset2; 
        hist2[i+3] = offset2 - 1; 
        offset2 = temp_offset;

        temp_offset = hist3[i] + offset3; 
        hist3[i] = offset3 - 1; 
        offset3 = temp_offset;
        
        temp_offset = hist3[i+1] + offset3; 
        hist3[i+1] = offset3 - 1; 
        offset3 = temp_offset;
        
        temp_offset = hist3[i+2] + offset3; 
        hist3[i+2] = offset3 - 1; 
        offset3 = temp_offset;
        
        temp_offset = hist3[i+3] + offset3; 
        hist3[i+3] = offset3 - 1; 
        offset3 = temp_offset;

        temp_offset = hist4[i] + offset4; 
        hist4[i] = offset4 - 1; 
        offset4 = temp_offset;
        
        temp_offset = hist4[i+1] + offset4; 
        hist4[i+1] = offset4 - 1; 
        offset4 = temp_offset;
        
        temp_offset = hist4[i+2] + offset4; 
        hist4[i+2] = offset4 - 1; 
        offset4 = temp_offset;
        
        temp_offset = hist4[i+3] + offset4; 
        hist4[i+3] = offset4 - 1; 
        offset4 = temp_offset;

        temp_offset = hist5[i] + offset5; 
        hist5[i] = offset5 - 1; 
        offset5 = temp_offset;
        
        temp_offset = hist5[i+1] + offset5; 
        hist5[i+1] = offset5 - 1; 
        offset5 = temp_offset;
        
        temp_offset = hist5[i+2] + offset5; 
        hist5[i+2] = offset5 - 1; 
        offset5 = temp_offset;
        
        temp_offset = hist5[i+3] + offset5; 
        hist5[i+3] = offset5 - 1; 
        offset5 = temp_offset;

        temp_offset = hist6[i] + offset6; 
        hist6[i] = offset6 - 1; 
        offset6 = temp_offset;
        
        temp_offset = hist6[i+1] + offset6; 
        hist6[i+1] = offset6 - 1; 
        offset6 = temp_offset;
        
        temp_offset = hist6[i+2] + offset6; 
        hist6[i+2] = offset6 - 1; 
        offset6 = temp_offset;
        
        temp_offset = hist6[i+3] + offset6; 
        hist6[i+3] = offset6 - 1; 
        offset6 = temp_offset;

        temp_offset = hist7[i] + offset7; 
        hist7[i] = offset7 - 1; 
        offset7 = temp_offset;
        
        temp_offset = hist7[i+1] + offset7; 
        hist7[i+1] = offset7 - 1; 
        offset7 = temp_offset;
        
        temp_offset = hist7[i+2] + offset7; 
        hist7[i+2] = offset7 - 1; 
        offset7 = temp_offset;
        
        temp_offset = hist7[i+3] + offset7; 
        hist7[i+3] = offset7 - 1; 
        offset7 = temp_offset;
    }
}

template <typename T, typename F>
void radix_sort_16_impl(T* src, unsigned count, T* dst, F bitwise_transform)
{
    unsigned histograms[2][values_in_byte] = { 0 };

    unsigned* hist0 = histograms[0];
    unsigned* hist1 = histograms[1];

    T* src_end = src + count;
    T* dst_end = dst + count;

    unsigned unroll_count = count >> 2;

    T* i = src;

    for (; unroll_count; --unroll_count, i += 4)
    {
        auto element0 = bitwise_transform(*i);
        auto element1 = bitwise_transform(*(i+1));
        auto element2 = bitwise_transform(*(i+2));
        auto element3 = bitwise_transform(*(i+3));

        hist0[byte0(element0)]++;
        hist1[byte1(element0)]++;

        hist0[byte0(element1)]++;
        hist1[byte1(element1)]++;

        hist0[byte0(element2)]++;
        hist1[byte1(element2)]++;

        hist0[byte0(element3)]++;
        hist1[byte1(element3)]++;
    }

    for (; i != src_end; ++i)
    {
        auto h = bitwise_transform(*i);

        hist0[byte0(h)]++;
        hist1[byte1(h)]++;
    }

    radix_sort_calculate_offset_table_16(hist0, hist1);

    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist0, byte0);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist1, byte1);
}

template <typename T, typename F>
inline void radix_sort_32_impl(T* src, const unsigned count, T* dst, F bitwise_transform)
{
    unsigned histograms[4][values_in_byte] = { 0 };

    unsigned* hist0 = histograms[0];
    unsigned* hist1 = histograms[1];
    unsigned* hist2 = histograms[2];
    unsigned* hist3 = histograms[3];

    T* src_end = src + count;
    T* dst_end = dst + count;

    unsigned unroll_count = count >> 2;

    T* i = src;

    for (; unroll_count; --unroll_count, i += 4)
    {
        auto element0 = bitwise_transform(*(i));
        auto element1 = bitwise_transform(*(i+1));
        auto element2 = bitwise_transform(*(i+2));
        auto element3 = bitwise_transform(*(i+3));

        hist0[byte0(element0)]++;
        hist1[byte1(element0)]++;
        hist2[byte2(element0)]++;
        hist3[byte3(element0)]++;

        hist0[byte0(element1)]++;
        hist1[byte1(element1)]++;
        hist2[byte2(element1)]++;
        hist3[byte3(element1)]++;

        hist0[byte0(element2)]++;
        hist1[byte1(element2)]++;
        hist2[byte2(element2)]++;
        hist3[byte3(element2)]++;

        hist0[byte0(element3)]++;
        hist1[byte1(element3)]++;
        hist2[byte2(element3)]++;
        hist3[byte3(element3)]++;
    }

    for (; i != src_end; ++i)
    {
        auto h = bitwise_transform(*i);

        hist0[byte0(h)]++;
        hist1[byte1(h)]++;
        hist2[byte2(h)]++;
        hist3[byte3(h)]++;
    }

    radix_sort_calculate_offset_table_32(hist0, hist1, hist2, hist3);

    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist0, byte0);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist1, byte1);
    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist2, byte2);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist3, byte3);
}

template <typename T, typename F>
inline void radix_sort_64_impl(T* src, unsigned count, T* dst, F bitwise_transform)
{
    unsigned histograms[8][values_in_byte] = { 0 };

    unsigned* hist0 = histograms[0];
    unsigned* hist1 = histograms[1];
    unsigned* hist2 = histograms[2];
    unsigned* hist3 = histograms[3];
    unsigned* hist4 = histograms[4];
    unsigned* hist5 = histograms[5];
    unsigned* hist6 = histograms[6];
    unsigned* hist7 = histograms[7];

    T* src_end = src + count;
    T* dst_end = dst + count;

    unsigned unroll_count = count >> 2;

    T* i = src;

    for (; unroll_count; --unroll_count, i += 4)
    {
        auto element0 = bitwise_transform(*i);
        auto element1 = bitwise_transform(*(i + 1));
        auto element2 = bitwise_transform(*(i + 2));
        auto element3 = bitwise_transform(*(i + 3));

        hist0[byte0(element0)]++;
        hist1[byte1(element0)]++;
        hist2[byte2(element0)]++;
        hist3[byte3(element0)]++;
        hist4[byte4(element0)]++;
        hist5[byte5(element0)]++;
        hist6[byte6(element0)]++;
        hist7[byte7(element0)]++;

        hist0[byte0(element1)]++;
        hist1[byte1(element1)]++;
        hist2[byte2(element1)]++;
        hist3[byte3(element1)]++;
        hist4[byte4(element1)]++;
        hist5[byte5(element1)]++;
        hist6[byte6(element1)]++;
        hist7[byte7(element1)]++;

        hist0[byte0(element2)]++;
        hist1[byte1(element2)]++;
        hist2[byte2(element2)]++;
        hist3[byte3(element2)]++;
        hist4[byte4(element2)]++;
        hist5[byte5(element2)]++;
        hist6[byte6(element2)]++;
        hist7[byte7(element2)]++;

        hist0[byte0(element3)]++;
        hist1[byte1(element3)]++;
        hist2[byte2(element3)]++;
        hist3[byte3(element3)]++;
        hist4[byte4(element3)]++;
        hist5[byte5(element3)]++;
        hist6[byte6(element3)]++;
        hist7[byte7(element3)]++;
    }

    for (; i != src_end; ++i)
    {
        auto h = bitwise_transform(*i);

        hist0[byte0(h)]++;
        hist1[byte1(h)]++;
        hist2[byte2(h)]++;
        hist3[byte3(h)]++;
        hist4[byte4(h)]++;
        hist5[byte5(h)]++;
        hist6[byte6(h)]++;
        hist7[byte7(h)]++;
    }

    radix_sort_calculate_offset_table_64(hist0, hist1, hist2, hist3, hist4, hist5, hist6, hist7);

    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist0, byte0);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist1, byte1);
    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist2, byte2);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist3, byte3);

    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist4, byte4);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist5, byte5);
    COPY_WITH_REORDERING_UNROLLED(src, src_end, dst, hist6, byte6);
    COPY_WITH_REORDERING_UNROLLED(dst, dst_end, src, hist7, byte7);

}

#undef COPY_WITH_REORDERING_UNROLLED
#undef byte0
#undef byte1
#undef byte2
#undef byte3
#undef byte4
#undef byte5
#undef byte6
#undef byte7

void radix_sort(uint8_t * src, const unsigned count)
{    
    unsigned histogram[values_in_byte] = { 0 };

    for(unsigned i = 0; i < count; ++i)
        ++histogram[src[i]];

    unsigned write_index = 0;

    for(unsigned value = 0; value < values_in_byte; ++value)
    {
        const unsigned element_count = histogram[value];

        for (unsigned i = 0; i < element_count; ++i, ++write_index)
            src[write_index] = uint8_t(value);
    }
}

void radix_sort(int8_t * src, unsigned count)
{
    unsigned histogram[values_in_byte] = { 0 };

    for(unsigned i = 0; i < count; ++i)
        ++histogram[int32_t(src[i]) + int32_t(128)];

    unsigned write_index = 0;

    for(int32_t value = 0; value < values_in_byte; ++value)
    {
        const unsigned element_count = histogram[value];

        const int8_t write_value = int8_t(value - int32_t(128));

        for (unsigned i = 0; i < element_count; ++i, ++write_index)
            src[write_index] = write_value;
    }
}

void radix_sort(uint16_t* src, unsigned count, uint16_t* temp)
{
    auto identity = [](uint16_t v) -> uint16_t
    {
        return v;
    };

    radix_sort_16_impl(src, count, temp, identity);
}

void radix_sort(int16_t* src, unsigned count, int16_t* temp)
{
    auto flip_sign = [](int16_t v) -> uint16_t
    {
        return  *reinterpret_cast<uint16_t*>(&v) ^ (uint16_t(1) << 15);
    };

    radix_sort_16_impl(src, count, temp, flip_sign);
}

void radix_sort(uint32_t * src, unsigned count, uint32_t* temp)
{
    auto identity = [](uint32_t v) -> uint32_t
    {
        return v;
    };

    radix_sort_32_impl(src, count, temp, identity);
}

void radix_sort(int32_t * src, unsigned count, int32_t* temp)
{
    auto flip_sign = [](int32_t v) -> uint32_t
    {
        return  *reinterpret_cast<uint32_t*>(&v) ^ (uint32_t(1) << 31);
    };

    radix_sort_32_impl(src, count, temp, flip_sign);
}

void radix_sort(uint64_t* src, unsigned count, uint64_t* temp)
{
    auto identity = [](uint64_t v) -> uint64_t
    {
        return v;
    };

    radix_sort_64_impl(src, count, temp, identity);
}

void radix_sort(int64_t * src, unsigned count, int64_t* temp)
{
    auto flip_sign = [](int64_t v) -> uint64_t
    {
        return  *reinterpret_cast<uint64_t*>(&v) ^ (uint64_t(1) << 63);
    };

    radix_sort_64_impl(src, count, temp, flip_sign);
}

void radix_sort(float* src, unsigned count, float* temp)
{
    auto flip = [](float v) -> uint32_t
    {
        // if floats are guaranteed to be >= 0,
        // you can do this(will be a bit faster):
        // return *reinterpret_cast<uint32_t*>(&v);
        // otherwise:
        uint32_t as_uint = *reinterpret_cast<uint32_t*>(&v);
        uint32_t mask = -int32_t(as_uint >> 31) | (uint32_t(1) << 31);
        return as_uint ^ mask;
    };

    radix_sort_32_impl(src, count, temp, flip);
}

template <typename T>
void radix_sort(T* src, const unsigned count)
{
    T * temp = new T[count];
    radix_sort(src, count, temp);
    delete[] temp;
}

#endif //RADIX_SORT_H