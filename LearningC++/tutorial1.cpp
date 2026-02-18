#include <iostream>
#include <array>
#include <vector>
#include <unordered_map>
#include <string>

int main() {

    //vector closest to list in python
    std::vector<int> vec = {10,20,30};
    std::array<int, 5> arr = {10,2, 5};

    //append = push_back
    for(auto i : vec){
        std::cout << i << "\n";
    }

    std::unordered_map<std::string, int> dict;
    dict["sally"] = 0;
    dict.insert({"sallys", 0});

    for(const auto& [key,val] : dict) {
        std::cout << key << " " << val << "\n"; 
    }

    return 0;
}
