#include "linkedlist.h"
#include <iostream>
#include <algorithm>
#include <mutex>
#include <memory>
#include <chrono>
#include <unordered_set>
#include <random>
#include <set>



#define PRESENT_THREADS 4
#define TEMP_THREADS 8
#define GUESTS 500000

std::mutex mutex;

std::unique_ptr<std::unordered_set<int>> generateUnorderedSet(int size)
{
  std::unique_ptr<std::vector<int>> vect = std::make_unique<std::vector<int>>();
  
   for (int i = 0; i < size; i++)
        vect->push_back(i);
  
   unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::shuffle(vect->begin(), vect->end(), std::default_random_engine(seed));

   return std::make_unique<std::unordered_set<int>>(vect->begin(), vect->end());

}

int generateNumber(int min, int max)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

    return dist(rng);
}

void tasks(LinkedList* list, std::unordered_set<int>* giftBag, std::unordered_set<int>* cards)
{
    while (cards->size() < GUESTS)
    {
        int task = generateNumber(1, 3);

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
                int randomGuest = generateNumber(0, GUESTS - 1);
                // Silences the unused warning when compiling when VERBOSE isn't defined
                bool found = list->contains(randomGuest);

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

void presents()
{
    std::unique_ptr<LinkedList> list = std::make_unique<LinkedList>();
    std::unique_ptr<std::unordered_set<int>> cards = std::make_unique<std::unordered_set<int>>();
    std::thread threads[PRESENT_THREADS] = {};

    std::cout << "Generating " << GUESTS << " gifts..." << std::endl;

    std::unique_ptr<std::unordered_set<int>> giftBag = generateUnorderedSet(GUESTS);

    for (int i = 0; i < PRESENT_THREADS; i++) 
    {
        threads[i] = std::thread(tasks, list.get(), giftBag.get(), cards.get());
    }

    std::cout << "Running " << PRESENT_THREADS << " threads..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}

bool sensorsCheck(int caller, std::vector<bool>& sensors)
{
    for (int i = 0; i < static_cast<int>(sensors.size()); i++)
    {
        if (!sensors[i] && caller != i)
            return false;
    }
    return true;
}

void printLargestDifference(std::vector<int>& sensorReadings)
{
    int step = 10;
    int startInterval = 0;
    int maxDifference = INT_MIN;

    // Because the sensor readings are all stored in one contiguous array,
    // we need to loop through the array in chunks to find the largest
    // difference for that sensor
    for (int threadIndex = 0; threadIndex < TEMP_THREADS; threadIndex++)
    {
        int offset = threadIndex * 60;

        for (int i = offset; i < 60 - step + 1; i++)
        {
            int max = *std::max_element(sensorReadings.begin() + i, sensorReadings.begin() + i + step);
            int min = *std::min_element(sensorReadings.begin() + i, sensorReadings.begin() + i + step);
            int diff = max - min;

            if (diff > maxDifference)
            {
                maxDifference = diff;
                startInterval = i;
            }
        }
    }

    std::cout << "Largest temperature difference: " << maxDifference << "F"
              << " starting at minute " << startInterval
              << " and ending at minute " << (startInterval + 10) << std::endl;
}

void printHighestTemperatures(std::vector<int>& sensorReadings)
{
    std::set<int> temperatures{};

    for (auto it = sensorReadings.rbegin(); it != sensorReadings.rend(); it++)
    {
        if (temperatures.find(*it) == temperatures.end())
            temperatures.insert(*it);

        if (temperatures.size() == 5)
            break;
    }

    std::cout << "Highest temperatures: ";

    for (int temperature : temperatures)
        std::cout << temperature << "F ";

    std::cout << std::endl;
}

void printLowestTemperatures(std::vector<int>& sensorReadings) {
    std::set<int> temperatures{};

    for (auto it = sensorReadings.begin(); it != sensorReadings.end(); it++) {
        if (temperatures.find(*it) == temperatures.end()) {
            temperatures.insert(*it);
        }

        if (temperatures.size() == 5) {
            break;
        }
    }

    std::cout << "Lowest temperatures: ";

    for (int temperature : temperatures) {
        std::cout << temperature << "F ";
    }

    std::cout << std::endl;
}

void generateReport(int hour, std::vector<int>& sensorReadings)
{
    std::cout << "[Hour " << hour + 1 << " report]" << std::endl;

    printLargestDifference(sensorReadings);

    std::sort(sensorReadings.begin(), sensorReadings.end());

    printHighestTemperatures(sensorReadings);
    printLowestTemperatures(sensorReadings);

    std::cout << std::endl
              << std::endl;
}

void measureTemperature(int threadId, std::vector<int>& sensorReadings, std::vector<bool>& sensorsReady)
{
    for (int hour = 0; hour < 72; hour++)
    {
        for (int minute = 0; minute < 60; minute++)
        {
            sensorsReady[threadId] = false;
            sensorReadings[minute + (threadId * 60)] = generateNumber(-100, 70);
            sensorsReady[threadId] = true;

            // Make sure we wait for all sensors to take a reading before we continue
            // with another temperature reading
            while (!sensorsCheck(threadId, sensorsReady))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        if (threadId == 0)
        {
            mutex.lock();
            generateReport(hour, sensorReadings);
            mutex.unlock();
        }
    }
}

int temperatures()
{
    // Keeps track of every reading by the threads. Each thread can only access
    // 60 readings. For example, thread 1 writes to 0 - 59, thread 2 writes to
    // 60 - 119, and so on
    std::vector<int> sensorReadings(TEMP_THREADS * 60);
    std::vector<bool> sensorsReady(TEMP_THREADS);
    std::thread threads[TEMP_THREADS] = {};

    for (int i = 0; i < TEMP_THREADS; i++)
        threads[i] = std::thread(measureTemperature, i, std::ref(sensorReadings), std::ref(sensorsReady));

    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads)
        thread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}

int main()
{
//   std::cout << "Testing Presents";
//   presents();  
  std::cout << "Testing Temperature";
  temperatures();


}
