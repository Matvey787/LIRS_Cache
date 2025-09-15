#include "lirs.h"
#include "opt.h"
#include "cacheRendering.h"

static void run();
static size_t checkCache(const std::vector<int>& keys, size_t capacity);

int main()
{
    // Запуска собственного теста (как в задании) 
    run();



    // Немного о графике, который находится в imgs
    // Мы меняем Number of Keys, чтобы посмотреть, как кэш реагирует на то, если пользователь 
    // открывает большое число страниц.
    // Помимо этого кэш также реагирует на плотность ключей, то есть, как часто
    // пользователь к страницам разной тематики.
    // key density = (количество уникальных ключей / общее количество запросов) * 100%


    // -------------------------- OPT cache testing --------------------------

    // std::vector<int> test = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // OPTCache opt(2, test);

    // std::cout << "Hits: " << opt.get_hits() << std::endl;

    // -----------------------------------------------------------------------

    // Генерация данных для сравнения с идеальным кэшем
    // generateData("py/cacheData.txt", "py/OPTCacheData.txt");
}

static void run()
{
    size_t capacity = 0;
    size_t numOfKeys = 0;
    int key = 0;
    std::vector<int> keys;


    // std::cout << "Enter [cache capacity] [number of keys] <keys>" << std::endl;
    std::cin >> capacity >> numOfKeys;

    LIRSCache<int> cache(capacity);

    for (size_t i = 1; i <= numOfKeys; ++i)
    {
        std::cin >> key;
        keys.push_back(key);
    }

    // std::cout << "Hits: " << hits << std::endl;
    std::cout << checkCache(keys, capacity) << std::endl;
}

static size_t checkCache(const std::vector<int>& keys, size_t capacity)
{
    size_t hits = 0;

#ifdef OPT_M
    OPTCache cache(capacity, keys);
    hits = cache.get_hits();
#else
    LIRSCache<int> cache(capacity);
    for (int key : keys)
    {
        if (cache.get(key) != nullptr)
            hits++;
        else
            cache.put(key, key);
    }
#endif

    return hits;
}