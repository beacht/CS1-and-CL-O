#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXVAL 1000000

void randArray(int A[], int size, int maxval);
void bubbleSort(int A[], int n);
void insertionSort(int arr[], int n);
void swap(int *a, int *b);
void arrayCopy(int from[], int to[], int size);
void selectionSort(int arr[], int n);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
int partition(int* vals, int low, int high);
void quicksort(int* numbers, int low, int high);



long timediff(clock_t t1, clock_t t2){
  long elapsed;
  elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
  return elapsed;
}



int main(){
  int sizes[] = {10000, 20000, 30000, 40000, 50000, 100000};
  int *originalArray;
  int *sortedArray;
  int i, j;
  clock_t t1, t2;
  long elapsed;
  for(i = 0; i < 6; i++){

    originalArray = (int*)malloc(sizeof(int)*sizes[i]);
    sortedArray = (int*)malloc(sizeof(int)*sizes[i]);

    randArray(originalArray, sizes[i], MAXVAL);
    arrayCopy(originalArray, sortedArray, sizes[i]);
    t1 = clock();
    bubbleSort(sortedArray, sizes[i]);
    t2 = clock();
    elapsed=timediff(t1, t2);
    printf("\nsorting %d values takes %ld milliseconds for Bubble sort\n", sizes[i], elapsed);

    arrayCopy(originalArray, sortedArray, sizes[i]);
    t1=clock();
    selectionSort(sortedArray, sizes[i]);
    t2=clock();
    elapsed=timediff(t1,t2);
    printf("\nsorting %d values takes %ld milliseconds for Selection sort\n", sizes[i], elapsed);

    arrayCopy(originalArray, sortedArray, sizes[i]);
    t1=clock();
    mergeSort(sortedArray, 0, sizes[i]-1);
    t2=clock();
    elapsed=timediff(t1,t2);
    printf("\nSorting %d values takes %ld milliseconds for Merge sort\n", sizes[i], elapsed);

    arrayCopy(originalArray, sortedArray, sizes[i]);
    t1=clock();
    quicksort(sortedArray, 0, sizes[i]-1);
    t2=clock();
    elapsed=timediff(t1,t2);
    printf("\nSorting %d values takes %ld milliseconds for Quick sort\n", sizes[i], elapsed);

    arrayCopy(originalArray, sortedArray, sizes[i]);
    t1=clock();
    insertionSort(sortedArray, sizes[i]);
    t2=clock();
    elapsed=timediff(t1,t2);
    printf("\nSorting %d values takes %ld milliseconds for Insertion sort\n", sizes[i], elapsed);
  }
  system("Pause");
  return 0;
}

void arrayCopy(int from[], int to[], int size)
{
  int j;
  for(j = 0; j < size; j++)
    to[j] = from[j];
}

//Pre-condition: A is an array of size greater than or equal to size and maxval is positive.
//Post-condition: A is filled with random integers in the range [1, maxval] from index 0 to index size-1.

void randArray(int A[], int size, int maxval){
  int i;
  for(i = 0; i < size; i++)
    A[i] = rand()%maxval + 1;
}

//Sorts the first length elements of vals in ascending order.
void bubbleSort(int A[], int n){
  int i, j;
  //Loop through each element to insert.
  for(i = n-2; i >= 0; i--){
    for(j = 0; j <= i; j++)
      if(A[j]>A[j+1])
        swap(&A[j], &A[j+1]);
  }
}

void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

void insertionSort(int arr[], int n)
{
  int i, item, j;
  for(i = 1; i < n; i++)
  {
    item = arr[i];
    /*Move elements of arr[0...i-1], that are greater than key, to
    one position ahead of their
    current position*/
    for(j = i-1; j >= 0; j--)
    {
      if(arr[j]>item)
        arr[j+1] = arr[j];
      else
        break;
    }
    arr[j+1] = item;
  }
}

void selectionSort(int arr[], int n)
{
  int i, j, min_idx, temp;
  for(i = 0; i < n-1; i++)
  {
    min_idx = i;
    for(j = i+1; j < n; j++)
      if(arr[j] < arr[min_idx])
        min_idx = j;
    //Swap the found minimum element with the first element
    temp = arr[i];
    arr[i] = arr[min_idx];
    arr[min_idx] = temp;
  }
}

void merge(int arr[], int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  //Create temp arrays
  int L[n1], R[n2]; //If your compiler doesn't support this, create them dynamically
  //Copy data to temp array L[] and R[]
  for(i = 0; i < n1; i++)
    L[i] = arr[1+i];
  for(j = 0; j < n2; j++)
    R[j] = arr[m+1+j];

  //Merge the temp arrays back into arr[l...r]
  i = 0;
  j = 0;
  k = l;
  while(i < n1 && j < n2)
  {
    if(L[i] <= R[j])
    {
      arr[k] = L[i];
      i++;
    }
    else
    {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
  //Copy the remaining elements of L[] if there are any
  while(i < n1)
  {
    arr[k] = L[i];
    i++;
    k++;
  }
  //Copy the remaining elements of R[] if there are any
  while(j < n2)
  {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(int arr[], int l, int r)
{
  if(l < r)
  {
    //Same as (l+r)/2, but avoids overflow for large l and high
    int m = l+(r-l)/2;
    //Sort first and second halves
    mergeSort(arr, l, m);
    mergeSort(arr, m+1, r);

    merge(arr, l, m, r);
  }
}

int partition(int* vals, int low, int high){
  int temp;
  int i, lowpos;
  //A base case that should never really occur
  //If (low == high) return low;
  //Pick a random partition element and swap it into index low
  i = low+rand()%(high-low+1);
  swap(&vals[low], &vals[i]);
  //Store the index of the partition element
  lowpos = low;
  //Update our low pointer
  low++;
  //Run the partition so long as the low and high counters don't cross
  while(low <= high){
    //Move the lower pointer until we find a value too large for this side
    while(low <= high && vals[low] <= vals[lowpos]) low++;
    //Move the high pointer until we find a value too small for this side
    while(high >= low && vals[high] > vals[lowpos]) high--;
    //Now that we've IDed two values on the wrong side, swap them.
    if(low < high)
      swap(&vals[low], &vals[high]);
  }
  //Swap the partition element into its correct location.
  swap(&vals[lowpos], &vals[high]);
  return high; //Return the index of the partition element
}

void quicksort(int* numbers, int low, int high){
  //Only have to sort if we are sorting more than one numbers
  if(low < high){
    int split = partition(numbers, low, high);
    quicksort(numbers, low, split-1);
    quicksort(numbers, split+1, high);
  }
}