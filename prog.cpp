#include <iostream>

using namespace std;


int main(){

    int *arr = new int[10];

    delete[] arr;
    arr = nullptr;

    cout << arr << endl;

    return 0;
}
