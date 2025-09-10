#ifndef LIRS_H
#define LIRS_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <optional>

const double LIR_PERCENTAGE = 0.7;

enum class pageKey_t
{
    LIR,
    HIR,
    NOTYPE
};

template <typename valType>
class LIRSCache {
private:
    size_t c_; // capacity of the cache
    size_t lirss_; // lirs size
    size_t hirss_; // hirs size
    std::list<int> lirs_;
    std::list<int> hirs_;
    std::unordered_map<int, std::pair<valType, pageKey_t>> cache_;

public:
    LIRSCache(size_t capacity) : c_(capacity)
    {
        lirss_ = static_cast<size_t>(c_ * LIR_PERCENTAGE);
        hirss_ = c_ - lirss_;
    }

    valType* get(int key);
    void put(int key, const valType& value);

    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LIRSCache<T>& cache);
};

template<typename valType>
valType* LIRSCache<valType>::get(int key)
{
    if (cache_.find(key) == cache_.end()) return nullptr;

    auto& page = cache_.at(key);

    if (page.second == pageKey_t::LIR) // LIRS case
    {
        lirs_.remove(key);
        lirs_.push_front(key);
    }
    else // HIRS case
    {
        hirs_.remove(key);
        lirs_.push_front(key);
        page.second = pageKey_t::LIR;

        if (lirs_.size() > lirss_) // If lirs is crowded
        {
            auto& evicted = lirs_.back();
            lirs_.pop_back();
            cache_.at(evicted).second = pageKey_t::HIR;
            hirs_.push_front(evicted);
        }
    }
    return &page.first;
}

template<typename valType>
void LIRSCache<valType>::put(int key, const valType& value)
{
    if (cache_.find(key) != cache_.end()) // Can update existing
    {
        cache_.at(key).first = value;
        get(key);
        return;
    }

    pageKey_t pageType = pageKey_t::NOTYPE;

    if (lirs_.size() < lirss_)
    {
        lirs_.push_front(key);
        pageType = pageKey_t::LIR;
    }
    else
    {
        if (hirs_.size() >= hirss_)
        {
            size_t evicted = hirs_.back();
            hirs_.pop_back();
            cache_.erase(evicted);
        }
        hirs_.push_front(key);
        pageType = pageKey_t::HIR;
    }

    // Add new element as HIR
    cache_[key] = {value, pageType};
}

template <typename valType>
std::ostream& operator<<(std::ostream& os, const LIRSCache<valType>& cache)
{
    os << "LIRS Cache State     Capacity: " << cache.c_ << std::endl;
    os << "                     LIRS: [ ";
    for (const auto& key : cache.lirs_)
    {
        os << key << " ";
    }
    os << "]" << std::endl;

    os << "                     HIRS: [ ";
    for (const auto& key : cache.hirs_)
    {
        os << key << " ";
    }
    os << "]" << std::endl;


    return os;
}

#endif // LIRS_H
