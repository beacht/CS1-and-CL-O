#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void *malloc(size_t size);

typedef struct point{ //Point struct as required by Restriction 2.
  int x;
  int y;
} point;

point me; //Global variable for my location. The data is filled in the main function. Required by Restriction 5.

point readData(FILE *input) //Required by Restriction 3. This is used to fill a provided point with coordinates.
{
  point temp;
  fscanf(input, "%d %d", &temp.x, &temp.y);
  return temp;
}

double distance(point point1, point point2) //Uses the distance formula to return the distance between any two points.
{
  return sqrt(((point2.x - point1.x)*(point2.x - point1.x)) + ((point2.y - point1.y)*(point2.y - point1.y)));
}

int compareTo(point* ptrPt1, point* ptrPt2) //Determines if the distance between myself and point 1 is greater than, less than, or equal to the distance between myself and point 2. Required by Restriction 4.
{
  if(ptrPt1->x == ptrPt2->x && ptrPt1->y == ptrPt2->y)
    return 0;
  
  double distance1 = distance(*ptrPt1, me);
  double distance2 = distance(*ptrPt2, me);

  if(distance1 < distance2)
    return -1;
  else if(distance1 > distance2)
    return 1;
  else
  {
    if(ptrPt1->x < ptrPt2->x)
      return -1;
    else if(ptrPt1->x > ptrPt2->x)
      return 1;
    else
    {
      if(ptrPt1->y < ptrPt2->y)
        return -1;
      else
        return 1;
    }
  }
}

void insertionSort(point* arr, int l, int r)
{
  point temp;
  int i, j;
  for(i = l; i <= r; i++)
  {
    temp = arr[i];

    for(j = i - 1; j >= 0; j--)
    {
      if((compareTo(&arr[j], &temp) == 1))
        arr[j+1] = arr[j];
      else
        break;
    }
    arr[j+1] = temp;
  }
}

void merge(point* arr, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  point *L = (point*)malloc(n1*sizeof(point));
  point *R = (point*)malloc(n2*sizeof(point));

  for(i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for(j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  i = 0;
  j = 0;
  k = l;
  while(i < n1 && j < n2)
  {
    if((compareTo(&L[i], &R[j])) <= 0)
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

  while(i < n1)
  {
    arr[k] = L[i];
    i++;
    k++;
  }

  while(j < n2)
  {
    arr[k] = R[j];
    j++;
    k++;
  }

  free(L);
  free(R);
}

void mergeSort(point* arr, int l, int r, int threshold) //To follow Restriction 8, it can call Insertion Sort if size becomes <= threshold.
{
  if(l < r)
  {
    int size = r - l + 1;
    if (size <= threshold)
      return insertionSort(arr, l, r);
      

    int m = (l + r)/2;

    mergeSort(arr, l, m, threshold);
    mergeSort(arr, m+1, r, threshold);

    merge(arr, l, m, r);
  }
}

int binarySearch(point infectedArray[], int l, int r, point query) //Binary search function as required by Restriction 6.
{
  if(r >= l)
  {
    int mid = l + (r - l) / 2;

    if((compareTo(&infectedArray[mid], &query)) == 0)
    {
      return mid;
    }

    if((compareTo(&infectedArray[mid], &query)) == 1)
      return binarySearch(infectedArray, l, mid - 1, query);

    return binarySearch(infectedArray, mid + 1, r, query);
  }
  return -1;
}

void sort(point arr[], int size, int threshold) //Wrapper function as required by Restriction 7. Follows the rules outlined by Restriction 1.
{
  if(size <= threshold)
    return insertionSort(arr, 0, size-1);
    
  mergeSort(arr, 0, size-1, threshold);
}


int main(void)
{
  FILE *input = fopen("assignment3input.txt", "r");
  FILE *output = fopen("out.txt", "w");

  int infected, searches, threshold;

  fscanf(input, "%d %d %d %d %d", &me.x, &me.y, &infected, &searches, &threshold);

  point infectedArray[infected]; //Array of all infected points
  point searchArray[searches]; //Array of all points to be searched

  int index = 0;
  for(index = 0; index < infected; index++)
    infectedArray[index] = readData(input); //Fills infected array
  for(index = 0; index < searches; index++)
    searchArray[index] = readData(input); //Fills search array

  sort(infectedArray, infected, threshold); //Sorts the array using Insertion and/or Merge sort as per Restriction 1

  for(index = 0; index < infected; index++)
    fprintf(output, "%d %d\n", infectedArray[index].x, infectedArray[index].y); //Prints the coordinates of each searched point. The following block prints the actual result (that it wasn't found, or that it was found at a given index)

  for(index = 0; index < searches; index++)
  {
    fprintf(output, "%d %d ", searchArray[index].x, searchArray[index].y);
    int n = sizeof(infectedArray) / sizeof(infectedArray[0]);
    point query = searchArray[index];
    int result = binarySearch(infectedArray, 0, n - 1, query);

    if(result == -1)
      fprintf(output, "not found\n");
    else
      fprintf(output, "found at rank %d\n", result + 1);
  }

  fclose(input);
  fclose(output);
  return 0;
}