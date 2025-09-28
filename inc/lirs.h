#ifndef LIRS_H
#define LIRS_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <algorithm>

const double LIR_PERCENTAGE = 0.7;

enum class pageKey_t
{
    LIR,
    HIR,
    NRHIR
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
    size_t nrhirss_; // максимальное количество non-resident HIRS ("призраки")
    std::list<int> lirs_;
    std::list<int> hirs_;
    std::list<int> nrhirs_;
    std::unordered_map<int, PageData_t<valType>> cache_;

public:
    LIRSCache(size_t capacity) : c_(capacity)
    {
        if (capacity == 0)
        {
            throw std::invalid_argument("Cache capacity must be greater than 0");
        }
        lirss_ = static_cast<size_t>(c_ * LIR_PERCENTAGE);
        hirss_ = c_ - lirss_;
        nrhirss_ = c_;
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
            cache_.at(evicted).type_ = pageKey_t::HIR;
            hirs_.push_front(evicted);
            cache_.at(evicted).listIter_ = hirs_.begin();
            lirs_.pop_back();
        }
    }
    return &pageIter->second.value_;
}

template<typename valType>
void LIRSCache<valType>::put(int key, const valType& value)
{
    auto page = cache_.find(key);
    if (page != cache_.end())
    {
        get(key);
        return;
    }

    auto nrhirs_keyIter = std::find(nrhirs_.begin(), nrhirs_.end(), key);
    if (nrhirs_keyIter != nrhirs_.end()) // NRHIR -> LIR (restore ghost page)
    {
        nrhirs_.erase(nrhirs_keyIter);
        lirs_.push_front(key);
        if (lirs_.size() > lirss_) // If lirs is crowded
        {
            auto& evicted = lirs_.back();
            cache_.at(evicted).type_ = pageKey_t::HIR;
            hirs_.push_front(evicted);
            lirs_.pop_back();
        }

        if (hirs_.size() >= hirss_)
        {
            size_t evicted = hirs_.back();
            cache_.erase(evicted);
            hirs_.pop_back();

            nrhirs_.push_front(key);
            if (nrhirs_.size() > nrhirss_) nrhirs_.pop_back();

        }
        cache_[key] = {value, pageKey_t::LIR, lirs_.begin()};
    }
    else if (lirs_.size() < lirss_)
    {
        lirs_.push_front(key);
        cache_[key] = {value, pageKey_t::LIR, lirs_.begin()};
    }
    else
    {
        if (hirs_.size() >= hirss_)
        {
            size_t evicted = hirs_.back();
            cache_.erase(evicted);
            hirs_.pop_back();

            nrhirs_.push_front(key);

            if (nrhirs_.size() > nrhirss_) nrhirs_.pop_back();
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

// #ifndef LIRS_H
// #define LIRS_H

// #include <iostream>
// #include <list>
// #include <unordered_map>
// #include <optional>

// const double LIR_PERCENTAGE = 0.7;

// enum class pageKey_t
// {
//     LIR,
//     HIR,
//     NOTYPE
// };


// template <typename valType>
// struct PageData_t
// {
//     valType value_;
//     pageKey_t type_;
//     typename std::list<int>::iterator listIter_;
// };

// template <typename valType>
// class LIRSCache {
// private:
//     size_t c_; // capacity of the cache
//     size_t lirss_; // lirs size
//     size_t hirss_; // hirs size
//     std::list<int> lirs_;
//     std::list<int> hirs_;
//     std::unordered_map<int, PageData_t<valType>> cache_;

// public:
//     LIRSCache(size_t capacity) : c_(capacity)
//     {
//         if (capacity == 0)
//         {
//             throw std::invalid_argument("Cache capacity must be greater than 0");
//         }
//         lirss_ = static_cast<size_t>(c_ * LIR_PERCENTAGE);
//         hirss_ = c_ - lirss_;
//     }

//     valType* get(int key);
//     void put(int key, const valType& value);

//     template <typename T>
//     friend std::ostream& operator<<(std::ostream& os, const LIRSCache<T>& cache);
// };

// template<typename valType>
// valType* LIRSCache<valType>::get(int key)
// {
//     auto pageIter = cache_.find(key);

//     if (pageIter == cache_.end()) return nullptr;


//     if (pageIter->second.type_ == pageKey_t::LIR) // LIRS case
//     {
//         lirs_.erase(pageIter->second.listIter_);
//         lirs_.push_front(key);
//         pageIter->second.listIter_ = lirs_.begin();
//     }
//     else // HIRS case
//     {
//         hirs_.erase(pageIter->second.listIter_);
//         lirs_.push_front(key);
//         pageIter->second.type_ = pageKey_t::LIR;
//         pageIter->second.listIter_ = lirs_.begin();

//         if (lirs_.size() > lirss_) // If lirs is crowded
//         {
//             auto& evicted = lirs_.back();
//             cache_.at(evicted).type_ = pageKey_t::HIR;
//             hirs_.push_front(evicted);
//             cache_.at(evicted).listIter_ = hirs_.begin();
//             lirs_.pop_back();
//         }
        
//     }
//     return &pageIter->second.value_;
// }

// template<typename valType>
// void LIRSCache<valType>::put(int key, const valType& value)
// {
//     auto page = cache_.find(key);
//     if (page != cache_.end()) // Can update existing
//     {
//         page->second.value_ = value;
//         get(key);
//         return;
//     }

//     if (lirs_.size() < lirss_)
//     {
//         lirs_.push_front(key);
//         cache_[key] = {value, pageKey_t::LIR, lirs_.begin()};
//     }
//     else
//     {
//         if (hirs_.size() >= hirss_)
//         {
//             size_t evicted = hirs_.back();
//             hirs_.pop_back();
//             cache_.erase(evicted);
//         }
//         hirs_.push_front(key);
//         cache_[key] = {value, pageKey_t::HIR, hirs_.begin()};
//     }
// }

// template <typename valType>
// std::ostream& operator<<(std::ostream& os, const LIRSCache<valType>& cache)
// {
//     os << "LIRS Cache State     Capacity: " << cache.c_ << std::endl;
//     os << "                     LIRS: [ ";
//     for (const auto& key : cache.lirs_)
//     {
//         os << key << " ";
//     }
//     os << "]" << std::endl;

//     os << "                     HIRS: [ ";
//     for (const auto& key : cache.hirs_)
//     {
//         os << key << " ";
//     }
//     os << "]" << std::endl;


//     return os;
// }

// #endif // LIRS_H
