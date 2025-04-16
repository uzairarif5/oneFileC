#include <stdio.h>
#include <stdlib.h>

int pivotIndex(int* nums, int numsSize){
    int i;
    int rightSum;
    int leftSum = 0;
    int pivot = -1;
    for(int possiblePivot=0;possiblePivot<numsSize;possiblePivot++){
        rightSum = 0;
        for (i = numsSize-1; i > possiblePivot; i--) {     
            rightSum += nums[i];
        } 
        if(rightSum == leftSum){
            pivot = possiblePivot;
            break;
        }
        leftSum += nums[possiblePivot];
    }
    return pivot;
}

int main(int argc, char **argv){
    int i;
    int nums[argc-1];
    for (i = 1; i < argc; i++) {     
        nums[i-1] = atoi(argv[i]);
    } 
    printf("%d\n", pivotIndex(nums,argc-1));  
    return 0;
}