
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

struct Add{
	Add(int toAdd) : _toAdd(toAdd) {}
	int operator()(int x){
		return x + _toAdd;
	}
	private:
	int _toAdd;
};

int main(){
	Add add_42(42); // state 저장
	Add add_13(13);
	int result_42 = add_42(2);
        std::cout << result_42 << " ";
	int result_13 = add_13(2);
        std::cout << result_13 << " ";
	std::vector<int> v(10, 10);   
    for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
        std::cout << *iter << " ";
    }
	std::transform(v.begin(), v.end(), v.begin(), add_42);
    for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
        std::cout << *iter << " ";
    }
}