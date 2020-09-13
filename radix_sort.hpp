#include <cstdint>
#include <climits>

#ifndef RADIX_SORT_H
#define RADIX_SORT_H

/*
    MIT License

    Copyright (c) 2020 Max Tikhomirov

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
 
const unsigned values_in_byte = 1U << CHAR_BIT;

const unsigned lsb_mask = values_in_byte - 1;

#define byte0(value) ((value) & lsb_mask)
#define byte1(value) (((value) >> CHAR_BIT) & lsb_mask)
#define byte2(value) (((value) >> (2 * CHAR_BIT)) & lsb_mask)
#define byte3(value) (((value) >> (3 * CHAR_BIT)) & lsb_mask)
#define byte4(value) (((value) >> (4 * CHAR_BIT)) & lsb_mask)
#define byte5(value) (((value) >> (5 * CHAR_BIT)) & lsb_mask)
#define byte6(value) (((value) >> (6 * CHAR_BIT)) & lsb_mask)
#define byte7(value) ((value) >> (7 * CHAR_BIT))

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
