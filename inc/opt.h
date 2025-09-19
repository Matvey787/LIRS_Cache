#ifndef OPT_H
#define OPT_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <limits>

class OPTCache
{
public:
    OPTCache(size_t capacity, const std::vector<int>& requests)
        : c_(capacity), requests_(requests)
    {
        keysFutureOccurrs_.reserve(requests.size());

        for (size_t i = 0; i < requests.size(); i++)
            keysFutureOccurrs_[requests[i]].push_back(i);
    }

    size_t get_hits()
    {
        size_t hits = 0;

        for (size_t i = 0; i < requests_.size(); i++)
        {
            int currKey = requests_[i];

            bool isKeyInCache = cache_.count(currKey);

            if (isKeyInCache)
                ++hits;

            update_keysFutureOccurrs(currKey);

            if (!isKeyInCache && getNextUseIndex(currKey) != std::numeric_limits<size_t>::max())
                put(currKey);
        }

        return hits;
    }

private:
    size_t c_;
    std::unordered_set<int> cache_;
    std::vector<int> requests_;
    std::unordered_map<int, std::deque<size_t>> keysFutureOccurrs_;
    
    void put(int key)
    {
        if (cache_.size() < c_)
        {
            cache_.insert(key);
        }
        else
        {
            int victim = findVictim();
            cache_.erase(victim);
            cache_.insert(key);
        }
    }

    int findVictim()
    {
        int victim = -1;
        size_t farthestUse = 0;

        for (int k : cache_)
        {
            size_t nextUse = getNextUseIndex(k);

            if (nextUse == std::numeric_limits<size_t>::max())
                return k;

            if (nextUse > farthestUse)
            {
                farthestUse = nextUse;
                victim = k;
            }
        }

        return victim;
    }

    size_t getNextUseIndex(int key)
    {
        auto it = keysFutureOccurrs_.find(key);
        if (it == keysFutureOccurrs_.end() || it->second.empty())
            return std::numeric_limits<size_t>::max();
        return it->second.front();
    }

    void update_keysFutureOccurrs(int key)
    {
        auto it = keysFutureOccurrs_.find(key);
        if (it != keysFutureOccurrs_.end() && !it->second.empty())
            it->second.pop_front();
    }
};

#endif // OPT_H
