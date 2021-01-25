#include <iostream>
#include <cstdlib>

template <typename T>
class TVector {
public:
    TVector() {
        Data = (T*)calloc(10, sizeof(T));
        Capacity = 10;
        CurSize = 0;
    }

    TVector(int size) {
        Data = (T*)calloc(size, sizeof(T));
        Capacity = size;
        CurSize = size;
    }

    ~TVector() {
        free(Data);
    }

    void PushBack(T elem) {
        if (CurSize == Capacity) {
            Extend();
        }
        Data[CurSize++] = elem;
    }

    int Size() {
        return CurSize;
    }

    T& operator[](int id) {
        return Data[id];
    }

private:
    T* Data;
    int Capacity;
    int CurSize;

    // doubles capacity
    void Extend() {
        Data = (T*)realloc(Data, 2 * Capacity * sizeof(T));
        Capacity *= 2;
    }
};

struct TItem {
    int key;
    char value[65];
};

void CountingSort(TVector <TItem>& data, TVector <TItem>& result, int max_key) {
    TVector <int> counts(max_key + 1);
    for (int i = 0; i < data.Size(); ++i) { // counting how many times each key appears in data
        counts[data[i].key]++;
    }
    for (int i = 1; i <= max_key; ++i) { // counting how many items have keys <= than counts[i]
        counts[i] += counts[i - 1];
    }
    for (int i = data.Size() - 1; i >= 0; --i) { // creating sorted vector
        result[--counts[data[i].key]] = data[i];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TVector <TItem> data;
    TItem elem;
    int max_key = -1;
    while (std::cin >> elem.key >> elem.value) {
        if (elem.key > max_key) { // searching for upper bound of keys
            max_key = elem.key;
        }
        data.PushBack(elem);
    }
    TVector <TItem> result(data.Size());
    CountingSort(data, result, max_key);
    for (int i = 0; i < result.Size(); ++i) {
        std::cout.fill('0');
        std::cout.width(6);
        std::cout << result[i].key << " " << result[i].value << "\n";
    }
    return 0;
}
