#include <stdio.h>
#include <stdlib.h>

int* runningSum(int* nums, int numsSize){
    for (int i = 1; i < numsSize; i++) {     
        nums[i] += nums[i-1];
    } 
    return nums;
}

int main(int argc, char **argv){
    int i;
    int nums[argc];
    for (i = 1; i < argc; i++) {     
        nums[i-1] = atoi(argv[i]); 
    } 
    int *result = runningSum(nums,argc-1);
    for (i = 0; i < argc-1; i++) {     
        printf("%d\n", result[i]);     
    } 
    return 0;
}