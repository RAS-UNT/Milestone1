
#ifndef SORTER_H
#define SORTER_H

#include <vector>

namespace Sorter {

template <typename T, typename Compare>
void merge(std::vector<T>& a, std::vector<T>& tmp,
           int lo, int mid, int hi, Compare before) {
    int i = lo;
    int j = mid;
    int k = lo;

    while (i < mid && j < hi) {
        if (before(a[j], a[i])) tmp[k++] = a[j++];
        else                    tmp[k++] = a[i++];
    }

    while (i < mid) tmp[k++] = a[i++];
    while (j < hi)  tmp[k++] = a[j++];

    for (k = lo; k < hi; k++) a[k] = tmp[k];
}

template <typename T, typename Compare>
void mergeSortRange(std::vector<T>& a, std::vector<T>& tmp,
                    int lo, int hi, Compare before) {
    if (hi - lo < 2) return;

    int mid = lo + (hi - lo) / 2;
    mergeSortRange(a, tmp, lo, mid, before);
    mergeSortRange(a, tmp, mid, hi, before);
    merge(a, tmp, lo, mid, hi, before);
}

template <typename T, typename Compare>
void mergeSort(std::vector<T>& a, Compare before) {
    if (a.size() < 2) return;

    std::vector<T> tmp(a);
    mergeSortRange(a, tmp, 0, (int)a.size(), before);
}

}

#endif
