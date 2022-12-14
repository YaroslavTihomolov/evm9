#include <iostream>

const int repeats = 5;

int *Init(int offset, int elements) {
    int *array = new int[offset * elements]{0};
    const int elements_size = offset / elements;
    for (int i = 0; i < elements_size; i++) {
        int index = i;
        for (int j = 1; j < elements; j++) {
            array[index] = index + offset;
            index += offset;
        }
        array[index] = (i + 1) % elements_size;
    }
    return array;
}

void Detour(const int *array, const int size, int elements) {
    union ticks {
        long long t64;
        struct s32 {
            long th, tl;
        } t32;
    } start{}, end{};
    long long min = LLONG_MAX;
    int k = 0;
    const int elements_size = size / elements;
    for (int j = 0; j < repeats; j++) {
        asm volatile ("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl)::"memory");
        for (int i = 0; i < elements_size * elements; i++)
            k = array[k];
        asm volatile ("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl)::"memory");
        long long tact = end.t64 - start.t64;
        min = min < tact ? min : tact;
    }
    std::cout << "Count of banks:" << elements;
    std::cout << "       Time taken:" << min / (size) << "\n";
    delete[]array;
}


int main() {
    int const cache_size = 16 * 1024 * 1024;//64MB
    int *array;
    for (int i = 1; i <= 34; i++) {
        array = Init(cache_size, i);
        Detour(array, cache_size, i);
    }
    return 0;
}
