#ifndef LIRS_H
#define LIRS_H



#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <algorithm>

namespace LIRS {

namespace detail {
    const double LIR_PERCENTAGE = 0.7;

    enum class pageKey_t
    {
        LIR,
        HIR,
        NRHIR
    };
}


template <typename valType, typename keyType = int>
class LIRSCache {

    struct PageData_t
    {
        valType value_;
        detail::pageKey_t type_;
        typename std::list<keyType>::iterator listIter_;
    };

    size_t c_; // capacity of the cache
    size_t lirss_; // lirs size
    size_t hirss_; // hirs size
    size_t nrhirss_; // максимальное количество non-resident HIRS ("призраки")
    std::list<keyType> lirs_;
    std::list<keyType> hirs_;
    std::list<keyType> nrhirs_;
    std::unordered_map<keyType, PageData_t> cache_;

public:
    LIRSCache(size_t capacity) : c_(capacity)
    {
        if (capacity == 0)
        {
            throw std::invalid_argument("Cache capacity must be greater than 0");
        }
        lirss_ = static_cast<size_t>(c_ * detail::LIR_PERCENTAGE);
        hirss_ = c_ - lirss_;
        nrhirss_ = c_;
    }

    valType* get(keyType key);
    void put(keyType key, const valType& value);

    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const LIRSCache<T>& cache);
};

template<typename valType, typename keyType>
valType* LIRSCache<valType, keyType>::get(keyType key)
{
    auto pageIter = cache_.find(key);
    if (pageIter == cache_.end()) return nullptr;

    if (pageIter->second.type_ == detail::pageKey_t::LIR) // LIRS case
    {
        lirs_.erase(pageIter->second.listIter_);
        lirs_.push_front(key);
        pageIter->second.listIter_ = lirs_.begin();
    }
    else // HIRS case
    {
        
        if (pageIter->second.listIter_ != hirs_.end()) {
            hirs_.erase(pageIter->second.listIter_);
        }
        
        lirs_.push_front(key);
        pageIter->second.type_ = detail::pageKey_t::LIR;
        pageIter->second.listIter_ = lirs_.begin();

        if (lirs_.size() > lirss_) // If lirs is crowded
        {
            const keyType evicted = lirs_.back();
            lirs_.pop_back();
            
            cache_.at(evicted).type_ = detail::pageKey_t::HIR;
            hirs_.push_front(evicted);
            cache_.at(evicted).listIter_ = hirs_.begin();  
        }
    }
    return &pageIter->second.value_;
}

template<typename valType, typename keyType>
void LIRSCache<valType, keyType>::put(keyType key, const valType& value)
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
        cache_[key] = {value, detail::pageKey_t::LIR, lirs_.begin()};  
        
        if (lirs_.size() > lirss_) // If lirs is crowded
        {
            const keyType evicted = lirs_.back();
            lirs_.pop_back();
            
            cache_.at(evicted).type_ = detail::pageKey_t::HIR;
            hirs_.push_front(evicted);
            cache_.at(evicted).listIter_ = hirs_.begin();  
        }

        
        if (hirs_.size() >= hirss_)
        {
            const keyType evicted = hirs_.back();  
            cache_.erase(evicted);
            hirs_.pop_back();

            nrhirs_.push_front(evicted);  
            if (nrhirs_.size() > nrhirss_) nrhirs_.pop_back();
        }
    }
    else if (lirs_.size() < lirss_)
    {
        lirs_.push_front(key);
        cache_[key] = {value, detail::pageKey_t::LIR, lirs_.begin()};
    }
    else
    {
        if (hirs_.size() >= hirss_)
        {
            const keyType evicted = hirs_.back();  
            cache_.erase(evicted);
            hirs_.pop_back();

            nrhirs_.push_front(evicted);  
            if (nrhirs_.size() > nrhirss_) nrhirs_.pop_back();
        }
        hirs_.push_front(key);
        cache_[key] = {value, detail::pageKey_t::HIR, hirs_.begin()};
    }
}

template <typename valType, typename keyType>
std::ostream& operator<<(std::ostream& os, const LIRSCache<valType, keyType>& cache)
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

} // namespace LIRS

#endif // LIRS_H
