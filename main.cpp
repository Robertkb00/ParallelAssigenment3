#include <iostream>
#include <mutex>


#define THREAD_COUNT 4
#define NUM_GUESTS 500000

std::mutex presents;

std::unique_ptr<std::unordered_set<int>> generateUnorderedSet(int size)
{
  std::unique_ptr<std::vector<int>> vect = std::make_unique<std::vector<int>>();
  
   for (int i = 0; i < size; i++)
        vect->push_back(i);
  
   unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::shuffle(vec->begin(), vec->end(), std::default_random_engine(seed));

   return std::make_unique<std::unordered_set<int>>(vec->begin(), vec->end());

}  
