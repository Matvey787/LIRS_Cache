#ifndef OPT_H
#define OPT_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <unordered_set>
#include <limits>

class OPTCache
{
public:

    OPTCache (const size_t capasity, const std::vector<int>& requests) : 
        c_(capasity),
        requests_(requests) 
    {
        cache_.reserve(c_);
        // looking into the future
        for(size_t i = 0; i < requests.size(); i++)
        {
            int currKey = requests[i];
            keysFutureOccurrs_[currKey].push_back(i);
        }
    }

    size_t get_hits()
    {
        size_t hits = 0;

        for (size_t i = 0; i < requests_.size(); i++)
        {
            int currKey = requests_[i];

            if (get(currKey))
                ++hits;
            else
                put(currKey, i);

            
            update_keysFutureOccurrs(currKey);
        }

        return hits;
    }
    

private:
    size_t c_;
    std::unordered_set<int> cache_;
    std::vector<int> requests_;
    std::unordered_map<int, std::vector<size_t>> keysFutureOccurrs_;



    bool get(int key)
    {
        if (cache_.find(key) != cache_.end())
            return true;
        else
            return false;
    }

    void put(int key, size_t keyIndex)
    {
        if (cache_.size() < c_)
        {
            cache_.insert(key);
        }
        else if (keysFutureOccurrs_.find(key)->second.size() > 1)
        {
            int victim = findVictim(keyIndex);
            cache_.erase(victim);
            cache_.insert(key);
        }
    }

    int findVictim(size_t keyIndex) {
        int victim = -1;
        size_t farthestUse = 0;

        for (int key : cache_)
        {
            size_t nextUse = getNextUseIndex(key);
            
            // this key will never be there again
            if (nextUse == std::numeric_limits<size_t>::max())
            {
                return key;
            }
            
            // we use the furthest key
            if (nextUse > farthestUse)
            {
                farthestUse = nextUse;
                victim = key;
            }
        }

        return victim;
    }

    size_t getNextUseIndex(int key)
    {
        auto it = keysFutureOccurrs_.find(key);

        if (it == keysFutureOccurrs_.end() || it->second.empty())
        {
            return std::numeric_limits<size_t>::max();
        }
        return it->second[0];
    }

    void update_keysFutureOccurrs(int key)
    {
        auto it = keysFutureOccurrs_.find(key);
        if (it != keysFutureOccurrs_.end() && !it->second.empty())
        {
            auto oldKeyUsage = it->second.begin();
            it->second.erase(oldKeyUsage);
        }
    }
};

#endif // OPT_H