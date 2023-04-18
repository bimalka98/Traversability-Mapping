#include <bits/stdc++.h>

int main() {
    std::vector<int> vect;
    std::cout << "Vector size = " << vect.size() << std::endl;

    if(0 < vect.size()) {
        std::cout << "Vector is not empty" << std::endl;
    } else {
        std::cout << "Vector is empty" << std::endl;
    }

    vect.push_back(10);
    std::cout << "Vector size = " << vect.size() << std::endl;

    
}