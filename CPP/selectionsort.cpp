
#include <iostream>
using namespace std;

int main(){
    
    char num[6];
    cin >> num;
    char (*ptr)[6] = &num; // first * -> reference, second * -> dereference, alias -> &

    for(int i = 0; i<5; i++){
        for(int j = i; j<5; j++){
            if(num[i] > num[j] ){
                int temp = num[i];
                num[i] = num[j];
                num[j] = temp;
                
                
            }
        }
    }

    cout<< "sorted array: " << *ptr << endl;
    return 0;
}

