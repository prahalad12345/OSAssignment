#include <iostream>
#include <algorithm>
using namespace std;

int main(){
    for(int i=1;i<=100;i++){
       
            string cs="./my_matrix_multiply -a i -b o -t "+to_string(i);
            int sys=system(cs.c_str());
        
    }
}