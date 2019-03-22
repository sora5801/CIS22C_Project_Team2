/*
	Credit to : Phuoc Le
*/

#pragma once
#include <iostream>

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
	array, and places all greater (greater than pivot)
   to left of pivot and all smaller elements to right
   of pivot */
   //we can do it inside quicksort function but separate them is more clearly
template <class Type> int partition(Type * arr, int low, int high)
{
	Type pivot = arr[high]; //last element
	int i = low - 1; // will be index of greater element (greater than pivot)

	//pass greater elements of pivot to the left and smaller elements to the right
	for (int j = low; j < high; j++)
	{
		//current element is greater or equal pivot
		if (arr[j] <= pivot)
		{
			i++; // increment index of greater element
			std::swap(arr[i], arr[j]); //swap the greater element to the left
		}
	}
	//swap pivot to the right position of it
	std::swap(arr[i + 1], arr[high]);
	//return pivot position
	return i + 1;
}

/*quicksort main function
arr is array will be sorted
low is starting index
high is ending index*/
template <class Type> void quickSortRecursive(Type * arr, int low, int high)
{
	if (low < high)
	{
		//pivot is partitioning index, pivot is in its right position
		int pivot = partition(arr, low, high);
		//sort elements before pivot
		quickSortRecursive(arr, low, pivot - 1);
		//sort elements after pivot
		quickSortRecursive(arr, pivot + 1, high);
	}
}