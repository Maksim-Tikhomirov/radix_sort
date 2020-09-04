#include "radix_sort.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string>
#include <cassert>
#include <random>
#include <type_traits>

using namespace std;

template <typename T>
void call_radix_sort(vector<T> & main)
{
    radix_sort(main.data(), main.size());
}

template <typename T>
void call_std_sort(vector<T> & v)
{
   std::sort(v.begin(), v.end());
}

template <typename T>
bool sorted_and_equal(const vector<T> & sorted, vector<T>& original)
{
    assert(sorted.size() == original.size());

    std::sort(original.begin(), original.end());

    for(unsigned i = 0; i < sorted.size(); ++i)
        if (sorted[i] != original[i])
            return false;

    return true;
}

template <typename T>
class random_values
{
private:

    mt19937 generator;

    using Distribution = typename conditional<is_integral<T>::value,
            uniform_int_distribution<T>,
            uniform_real_distribution<T>>::type;

    Distribution distribution;

public:

    random_values()
    {
        random_device rd;
        generator = mt19937(rd());
        distribution = Distribution(numeric_limits<T>::min(), numeric_limits<T>::max());
    }

    T operator()()
    {
        return distribution(generator);
    }
};

template <typename T, typename F>
double test(string function_name, F sorting_function, const unsigned num_of_elements, T)
{
    vector<T> main(num_of_elements);

    random_values<T> random_value;

    generate(main.begin(), main.end(), random_value);

    vector<T> copy = main;

    clock_t start = clock();

    sorting_function(main);

    double total = (clock() - start) / double(CLOCKS_PER_SEC);

    cout << function_name << " | Time " << total <<
         " sec";

    if(!main.empty())
        cout << ", random element: " << (long double)main[rand() % main.size()];

    cout << endl;

    // Verification of sorting.
    // Proves that sorted array is equal to original array, sorted with std::sort.
    if(!sorted_and_equal(main, copy))
    {
        cout << function_name << ": data is not sorted" << endl;
        exit(1);
    }

    return total;
}

template<typename T>
struct TypeData;

#define REGISTER_TYPE_NAME(X) template <> struct TypeData<X> \
    { static const char* name; } ; const char* TypeData<X>::name = #X

REGISTER_TYPE_NAME(int8_t);
REGISTER_TYPE_NAME(uint8_t);
REGISTER_TYPE_NAME(int16_t);
REGISTER_TYPE_NAME(uint16_t);
REGISTER_TYPE_NAME(int32_t);
REGISTER_TYPE_NAME(uint32_t);
REGISTER_TYPE_NAME(uint64_t);
REGISTER_TYPE_NAME(int64_t);
REGISTER_TYPE_NAME(float);

template <typename T>
void run_test(const unsigned number_of_elements)
{
    cout << "Type: vector<" << TypeData<T>::name << ">" << endl;

    const long double min_value = numeric_limits<T>::min();
    const long double max_value = numeric_limits<T>::max();

    cout << "Contains uniformly distributed values from " << min_value << " to " << max_value << endl;

    const double radix_time = test("Radix sort", call_radix_sort<T>, number_of_elements, T());

    const double std_sort_time = test("std::sort ", call_std_sort<T>, number_of_elements, T());

    cout << "Radix sort is " <<
        max(std_sort_time, radix_time) / min(std_sort_time, radix_time) <<
        " times " << (std_sort_time > radix_time ? "faster" : "slower") << endl << endl;
}

template <typename T>
void run_tests(const unsigned number_of_elements)
{
    run_test<T>(number_of_elements);
}

template <typename T1, typename T2, typename ... Ts>
void run_tests(const unsigned number_of_elements)
{
    run_test<T1>(number_of_elements);
    run_tests<T2, Ts...>(number_of_elements);
}

int main()
{
    const unsigned num_of_elements = 50000000U;

    cout << "Number of elements: " << num_of_elements << endl;

    run_tests<int8_t,
            uint8_t,
            int16_t,
            uint16_t,
            int32_t,
            uint32_t,
            uint64_t,
            int64_t,
            float
            >(num_of_elements);
}