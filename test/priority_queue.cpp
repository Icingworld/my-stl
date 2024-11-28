#include "../src/priority_queue.h"
#include <iostream>

int main()
{
    stl::priority_queue<int> pq;
    for (int i = 0; i < 10; ++i) {
        pq.push(i);
    }
    for (int i = 10; i > 0; --i) {
        pq.push(i);
    }
    
    for (int i = 0; i < 20; ++i) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;

    return 0;
}