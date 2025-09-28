#include "lirs.h"
#include "opt.h"
#include "cacheRendering.h"

#include <vector>
#include <numeric>
#include <fstream>
#include <random>
#include <algorithm>

const double CACHE_SIZE_COEFF = 0.3; // cache size determined by this fraction of the number of keys
const size_t MINIMAL_KEY = 5;

static void generateKeys(std::vector<int>& keys, size_t keyDensity = 4);
static size_t getCacheHits(const std::vector<int>& keys);



void generateData(const std::string& cacheDataFile, const std::string& OPTDataFile)
{
    bool useOPT = false;

    if (!OPTDataFile.empty()) { useOPT = true; }

    std::ofstream cacheFile(cacheDataFile);
    std::ofstream optCacheFile(OPTDataFile);

    if (!cacheFile || (useOPT && !optCacheFile))
    {
        std::cerr << "Error opening/creating data file." << std::endl;
        return;
    }

    cacheFile << "hits numOfKeys keyDensity\n";

    if (useOPT) optCacheFile << "hits numOfKeys keyDensity\n";

    std::cout << "Generating data..." << std::endl;

    srand(static_cast<unsigned int>(time(nullptr)));

    size_t maxNumberOfKeys = 1000;
    size_t maxKeyDensity = 500;

    for (size_t numOfKeys = 50; numOfKeys < maxNumberOfKeys; numOfKeys += 20)
    {
        for (size_t keyDensity = 5; keyDensity < maxKeyDensity; keyDensity += 20)
        {
            std::vector<size_t> LIRSHits;
            std::vector<size_t> OPTHits;
            for (int i = 0; i < 10; i++)
            {
                std::vector<int> keys;
                keys.reserve(numOfKeys);
                generateKeys(keys, keyDensity);
                
                LIRSHits.push_back(getCacheHits(keys));
                
                if (useOPT)
                {
                    size_t optCacheCapasity = keys.size() * CACHE_SIZE_COEFF;
                    OPTCache opt(optCacheCapasity, keys);
                    size_t currOPTHits = opt.get_hits();
                    OPTHits.push_back(currOPTHits);
                }
                
            }
            
            size_t LIRSHitsSumm = std::accumulate(LIRSHits.begin(), LIRSHits.end(), 0);
            size_t LIRSHitsAverage = LIRSHitsSumm / LIRSHits.size();
            cacheFile << LIRSHitsAverage << " " << numOfKeys << " " << keyDensity << "\n";
            
            if (useOPT)
            {
                size_t OPTHitsSumm = std::accumulate(OPTHits.begin(), OPTHits.end(), 0);
                size_t OPTHitsAverage = OPTHitsSumm / OPTHits.size();
                optCacheFile << OPTHitsAverage << " " << numOfKeys << " " << keyDensity << "\n";
            }
        }
    }
    

    cacheFile.close();

    if (useOPT) optCacheFile.close();

    std::cout << "Data generation complete. Results of cache saved to " << cacheDataFile << std::endl;
}

static void generateKeys(std::vector<int>& keys, size_t keyDensity)
{
    int maxKey = static_cast<int>(keys.capacity() * keyDensity / 100);
    for (int i = 0; i < keys.capacity(); i++)
    {
        int key = static_cast<int>((double)rand() * maxKey / RAND_MAX);
        keys.push_back(key);
    }
}

// static void generateKeys(std::vector<int>& keys, size_t keyDensity)
// {
//     size_t maxKeyWanted = (double)keys.capacity() * keyDensity / 100;
//     size_t maxKey = std::max(MINIMAL_KEY, maxKeyWanted);

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<int> dist(0, maxKey);

//     for (size_t i = 0; i < keys.capacity(); i++)
//     {
//         int key = dist(gen);
//         keys[i] = key;
//     }
// }

static size_t getCacheHits(const std::vector<int>& keys)
{
    LIRSCache<int> cache(keys.size() * CACHE_SIZE_COEFF);
    size_t hits = 0;

    // std::cout << "here58" << keys.size()<< std::endl;
    for (const auto key : keys)
    {
        // std::cout << "here888 " << key << std::endl;
        if (cache.get(key) != nullptr){
            // std::cout << "here70" << std::endl;
            hits++;
        }
        else
        {
            // std::cout << "here71" << std::endl;
            cache.put(key, key);
            // std::cout << "here72" << std::endl;
        }
    }
    // std::cout << "here59" << std::endl;
    return hits;
}
