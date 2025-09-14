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
struct PageData_t
{
    valType value_;
    pageKey_t type_;
    typename std::list<int>::iterator listIter_;
};

template <typename valType>
class LIRSCache {
private:
    size_t c_; // capacity of the cache
    size_t lirss_; // lirs size
    size_t hirss_; // hirs size
    std::list<int> lirs_;
    std::list<int> hirs_;
    std::unordered_map<int, PageData_t<valType>> cache_;

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
    auto pageIter = cache_.find(key);

    if (pageIter == cache_.end()) return nullptr;


    if (pageIter->second.type_ == pageKey_t::LIR) // LIRS case
    {
        lirs_.erase(pageIter->second.listIter_);
        lirs_.push_front(key);
        pageIter->second.listIter_ = lirs_.begin();
    }
    else // HIRS case
    {
        hirs_.erase(pageIter->second.listIter_);
        lirs_.push_front(key);
        pageIter->second.type_ = pageKey_t::LIR;
        pageIter->second.listIter_ = lirs_.begin();

        if (lirs_.size() > lirss_) // If lirs is crowded
        {
            auto& evicted = lirs_.back();
            lirs_.pop_back();
            cache_.at(evicted).type_ = pageKey_t::HIR;
            hirs_.push_front(evicted);
        }
    }
    return &pageIter->second.value_;
}

template<typename valType>
void LIRSCache<valType>::put(int key, const valType& value)
{
    auto page = cache_.find(key);
    if (page != cache_.end()) // Can update existing
    {
        page->second.value_ = value;
        get(key);
        return;
    }

    if (lirs_.size() < lirss_)
    {
        lirs_.push_front(key);
        cache_[key] = {value, pageKey_t::LIR, lirs_.begin()};
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
        cache_[key] = {value, pageKey_t::HIR, hirs_.begin()};
    }
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
