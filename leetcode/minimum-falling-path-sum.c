#include <stdio.h>

int getMinFromArray(int* arr, int arrSize){
    int min = arr[0];
    if(arrSize>1) for(int j=1;j<arrSize;j++) if(arr[j]<min) min = arr[j];
    return min;
}

int smallestFromThree(int num1, int num2, int num3){
    if(num1<=num2 && num1<=num3) return num1;
    if(num2<=num1 && num2<=num3) return num2;
    return num3;
}

int smallestFromTwo(int num1, int num2){
    return (num1<num2) ? num1 : num2;
}

int main(){
    int matrix[3][3] = {{-19,57},{-40,-5}};
    int matrixSize = 2;
    int matrixColSize[1] = {2};

    if(matrixColSize[0] == 1){return getMinFromArray(matrix[0], matrixSize);}
    for(int j=matrixColSize[0]-1;j>=0;j--){
        for(int i=0;i<matrixSize;i++){
            if(i==0) matrix[j][i] = matrix[j][i] + smallestFromTwo(matrix[j+1][i], matrix[j+1][i+1]);
            if(i==matrixSize-1) matrix[j][i] = matrix[j][i] + smallestFromTwo(matrix[j+1][i], matrix[j+1][i-1]);
            if(i>0&&i<matrixSize-1) matrix[j][i] = matrix[j][i] + smallestFromThree(matrix[j+1][i], matrix[j+1][i-1],matrix[j+1][i+1]);
        }
    }
    printf("Ans: %d",getMinFromArray(matrix[0],matrixSize));
    return 0;
}
