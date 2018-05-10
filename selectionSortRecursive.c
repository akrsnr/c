#include <stdio.h>

int getMax(int const * arr, int size) {
    if (size == 1)
        return *arr;

    int ans = getMax(arr + 1, size - 1);
    return *arr > ans ? *arr : ans;
}

int getIndex(int const * arr, int index, int size, int key) {
    if (size < index)
            return -1;

    if (key == arr[index])
        return index;

    return getIndex(arr, index + 1, size, key);

}

void swap(int *a, int *b) {
    int t;

    t  = *b;
    *b = *a;
    *a = t;
}

void selectionRecursive(int * arr, int size) {
    if (size == 1)
        return;

    int maxIndex = getIndex(arr, 0, size, getMax(arr, size));
    swap(&arr[size - 1], &arr[maxIndex]);
    selectionRecursive(arr, size - 1);

}

int main(void) {

    int arr[] = {4, 3, 5, 1, 2};

    selectionRecursive(arr, sizeof(arr) / sizeof(*arr));

    for (int i = 0; i < sizeof(arr) / sizeof(*arr); ++i) {
        printf("%d ", arr[i]);
    }


    return 0;
}
