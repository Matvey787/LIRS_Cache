#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

#if defined(OPT_M)
#include "opt.h"
#elif defined(CACHE_RENDER_M)
#include "cacheRendering.h"
#else
#include "lirs.h"
#endif


#ifndef CACHE_RENDER_M
static void run();
static size_t checkCache(const std::vector<int>& keys, size_t capacity);
#endif

int main()
{
    try
    {
        // Немного о графике, который находится в imgs
        // Мы меняем Number of Keys, чтобы посмотреть, как кэш реагирует на то, если пользователь 
        // открывает большое число страниц.
        // Помимо этого кэш также реагирует на плотность ключей, то есть, как часто
        // пользователь обращается к страницам разной тематики.
        // key density = (количество уникальных ключей / общее количество запросов) * 100%

        // Генерация данных для сравнения с идеальным кэшем
        #ifdef CACHE_RENDER_M
            Rendering::generateData("py/cacheData.txt", "py/OPTCacheData.txt");
        #else
            // Запуска собственного теста (как в задании)
            run();
        #endif
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}

#ifndef CACHE_RENDER_M
static void run()
{
    size_t capacity = 0;
    size_t numOfKeys = 0;
    int key = 0;
    std::vector<int> keys;


    // std::cout << "Enter [cache capacity] [number of keys] <keys>" << std::endl;
    std::cin >> capacity >> numOfKeys;

    if (numOfKeys == 0)
    {
        // 0 хитов
        return;
    }

    for (size_t i = 1; i <= numOfKeys; ++i)
    {
        std::cin >> key;
        keys.push_back(key);
    }

    if(keys.size() != numOfKeys)
        throw std::runtime_error("Invalid number of keys");
    

    // std::cout << "Hits: " << hits << std::endl;
    std::cout << checkCache(keys, capacity) << std::endl;
}

template<typename keyType>
keyType slow_get_page(keyType key)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return key;
}

static size_t checkCache(const std::vector<int>& keys, size_t capacity)
{
    size_t hits = 0;

    #ifdef OPT_M
        OPT::OPTCache cache(capacity, keys);
        hits = cache.get_hits();
    #else
        LIRS::LIRSCache<int, int> cache(capacity);
        for (int key : keys)
        {
            if (cache.get(key) != nullptr)
            {
                hits++;
            }
            else
            {
                cache.put(key, slow_get_page(key));
            }
        }
    #endif

    return hits;
}
#endif
