#include "lirs.h"
#include "cacheRendering.h"

#include <vector>
#include <numeric>
#include <fstream>
#include <random>

const double CACHE_SIZE_COEFF = 0.3; // cache size determined by this fraction of the number of keys

static void generateKeys(std::vector<int>& keys, size_t numOfKeys = 50, size_t keyDensity = 4);
static size_t getCacheMisses(const std::vector<int>& keys);


void generateData(const std::string& dataFile)
{
    std::ofstream fileWriteTo(dataFile);
    fileWriteTo << "misses numOfKeys keyDensity\n";

    if (!fileWriteTo)
    {
        std::cerr << "Error opening/creating data file." << std::endl;
        return;
    }

    std::cout << "Generating data..." << std::endl;

    srand (time(NULL));

    for (size_t numOfKeys = 10; numOfKeys <= 1000; numOfKeys += 10)
    {
        for (size_t keyDensity = 5; keyDensity < 100; keyDensity += 5)
        {
            std::vector<size_t> misses;
            for (int i = 0; i < 10; i++)
            {
                std::vector<int> keys;
                generateKeys(keys, numOfKeys, keyDensity);
                misses.push_back(getCacheMisses(keys));
            }
            size_t missesAverage = std::accumulate(misses.begin(), misses.end(), 0) / misses.size();
            fileWriteTo << missesAverage << " " << numOfKeys << " " << keyDensity << "\n";
        }
    }
    

    fileWriteTo.close();

    std::cout << "Data generation complete. Results saved to " << dataFile << std::endl;
}

static void generateKeys(std::vector<int>& keys, size_t numOfKeys, size_t keyDensity)
{
    int maxKey = static_cast<int>(numOfKeys * keyDensity / 100);
    for (int i = 0; i < numOfKeys; i++)
    {
        int key = static_cast<int>((double)rand() * maxKey / RAND_MAX);
        keys.push_back(key);
    }
}

static size_t getCacheMisses(const std::vector<int>& keys)
{
    LIRSCache<int> cache(keys.size() * CACHE_SIZE_COEFF);
    size_t misses = 0;

    for (const auto& key : keys)
    {
        if (cache.get(key) == nullptr)
        {
            misses++;
            cache.put(key, key);
        }
    }
    return misses;
}
