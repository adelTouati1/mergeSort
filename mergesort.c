#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

// function prototypes
void serial_mergesort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
void parallel_mergesort(int A[], int p, int r, int threads);
void* split(void* arg);


const int INSERTION_SORT_THRESHOLD = 100; //based on trial and error

typedef struct list{
    int *A;
    int p;
    int r;
    int threads;
}list;


/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r)
{
	int j;

	for (j=p+1; j<=r; j++) {
		int key = A[j];
		int i = j-1;
		while ((i > p-1) && (A[i] > key)) {
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}


/*
 * parallel_mergesort(int A[], int p, int r, int threads):
 *
 * multithreaded sort the section of the array A[p..r]
 */
void parallel_mergesort(int A[], int p, int r, int threads){
	
	
	if (threads < 2)  {
		int q = (p+r)/2;
                serial_mergesort(A, p, q);
                serial_mergesort(A, q+1, r);
                merge(A, p, q, r);

        }else {
		int pThreads = (threads/4)*2;
		pthread_t firstThread, secondThread;
        	list fristlist, secondList;

                int q = (p+r)/2;

        	fristlist.A = A;
        	fristlist.p = p;
        	fristlist.r = q;

        	secondList.A = A;
        	secondList.p = q+1;
        	secondList.r = r;

        	 if((pThreads/2) == 0){
						fristlist.threads = 1;
                        secondList.threads = 1;
                } else {
                        fristlist.threads = pThreads;
                        secondList.threads = pThreads;
                }

        	pthread_create(&firstThread,NULL,split,(void*)&fristlist);
        	pthread_create(&secondThread,NULL,split,(void*)&secondList);
		
        	pthread_join(firstThread, NULL);
        	pthread_join(secondThread, NULL);
        	merge(A, p, q, r);



	}
}

/*
 * void *split(void* genArg):
 * 
 * function helper to call parallel_mergesort recursivly. 
 * 
 */
void *split(void* genArg){
    list *temp = (list *)genArg;
    parallel_mergesort(temp->A, temp->p, temp->r, temp->threads);
    pthread_exit(NULL);
}


/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r)
{
	if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
			insertion_sort(A,p,r);
	} else {
		int q = (p+r)/2;
		serial_mergesort(A, p, q);
		serial_mergesort(A, q+1, r);
		merge(A, p, q, r);
	}

}

/*
 * merge(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */
void merge(int A[], int p, int q, int r)
{
	int *B = (int *) malloc(sizeof(int) * (r-p+1));

	int i = p;
	int j = q+1;
	int k = 0;
	int l;

	// as long as both lists have unexamined elements 
	//  this loop keeps executing. 
	while ((i <= q) && (j <= r)) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	 /* now only at most one list has unprocessed elements. */

	if (i <= q) {
		 /* copy remaining elements from the first list */
		for (l=i; l<=q; l++) {
			B[k] = A[l];
			k++;
		}
	} else {
		 /* copy remaining elements from the second list */
		for (l=j; l<=r; l++) {
			B[k] = A[l];
			k++;
		}
	}

	/* copy merged output from array B back to array A */
	k=0;
	for (l=p; l<=r; l++) {
		A[l] = B[k];
		k++;
	}

	free(B);
}
