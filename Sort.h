
/*
  MIT Licence
Author -> Brett Rogers

Reference Coding Train Quick Sort Youtube Video

*/


#include <cstdlib>
#include <iostream>
#include <vector>


namespace OrganizeList
{
  struct Sort 
  {

    Sort() {}

    void Swap(std::vector<int> &array, int a, int b) 
    {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
    }

  int Partition(std::vector<int> &array, int start, int end) 
  {
    int pivotIndex = start;
    int pivotValue = array[end];
    for (int i = start; i < end; i++) {
      if (array[i] < pivotValue) {
        Swap(array, i, pivotIndex);
        pivotIndex++;
      }
    }
    Swap(array, pivotIndex, end);
    return pivotIndex;
  }

  void QuickSort(std::vector<int> &array, int start, int end) 
  {

    if (start >= end) {
      return;
    }
    int index = Partition(array, start, end);
    QuickSort(array, start, index - 1);
    QuickSort(array, index + 1, end);
  }
};

} 
