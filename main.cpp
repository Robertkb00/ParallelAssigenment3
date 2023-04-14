#include <iostream>
#include <mutex>
#include <memory>
#include <chrono>


#define THREADS 4
#define GUESTS 500000

std::mutex mutex;

std::unique_ptr<std::unordered_set<int>> generateUnorderedSet(int size)
{
  std::unique_ptr<std::vector<int>> vect = std::make_unique<std::vector<int>>();
  
   for (int i = 0; i < size; i++)
        vect->push_back(i);
  
   unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::shuffle(vec->begin(), vec->end(), std::default_random_engine(seed));

   return std::make_unique<std::unordered_set<int>>(vec->begin(), vec->end());

}

void tasks(LinkedList* list, std::unordered_set<int>* giftBag, std::unordered_set<int>* cards)
{
    while (cards->size() < GUESTS) {
        int task = Util::generateRandomNumber(1, 3);

        switch (task)
        {
            // Task 1: Takes a gift and adds it to the list
            case 1:
            {
                mutex.lock();

                if (giftBag->empty() || giftBag->begin() == giftBag->end())
                {
                    mutex.unlock();
                    continue;
                }

                // Remove the first item from the unordered set and
                // add it to the linked list
                std::unordered_set<int>::iterator iter = giftBag->begin();
                int num = *iter;
                giftBag->erase(iter);
                mutex.unlock();

                list->insert(num);
                break;
            }
            //Task 2: Writes a 'Thank You' card, removes present from list
            case 2:
            {
                if (list->empty())
                {
                    continue;
                }

                int guest = list->removeHead();

                if (guest == INT_MIN)
                {
                    continue;
                }

//#ifdef VERBOSE
                //std::cout << "Thank you guest " << guest << "! "
//                           << "(guests remaining: " << list->size() << ")"
//                           << std::endl;
// #endif

                mutex.lock();
                cards->insert(guest);
                mutex.unlock();
                break;
            }
            //Task 3: Check gift for tag
            case 3:
            {
                int randomGuest = Util::generateRandomNumber(0, GUESTS - 1);
                // Silences the unused warning when compiling when VERBOSE isn't defined
                bool found __attribute__((unused)) = list->contains(randomGuest);

// #ifdef VERBOSE
//                 std::cout << "Minotaur: guest with ID "
//                           << randomGuest
//                           << " was " << (found ? "found" : "not found")
//                           << " (cards written: " << cards->size() << "/" << NUM_GUESTS << ")"
//                           << std::endl;
// #endif
                break;
            }
        }
    }
}

int presents()
{
    std::unique_ptr<LinkedList> list = std::make_unique<LinkedList>();
    std::unique_ptr<std::unordered_set<int>> cards = std::make_unique<std::unordered_set<int>>();
    std::thread threads[THREADS] = {};

    std::cout << "Generating " << GUESTS << " gifts..." << std::endl;

    std::unique_ptr<std::unordered_set<int>> giftBag = generateUnorderedSet(GUESTS);

    for (int i = 0; i < THREADS; i++) 
    {
        threads[i] = std::thread(completeTask, list.get(), giftBag.get(), cards.get());
    }

    std::cout << "Running " << THREADS << " threads..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}

int main()
{
  std::cout << "Testing";
  presents();  
}
