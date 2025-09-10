#include "lirs.h"
#include "cacheRendering.h"
#include "tests.h"


static void run();

int main()
{
    // visual testing
    // test1(); // Basic Operations
    // test2(); // HIR to LIR Conversion
    // test3(); // LIR Demotion
    // test4(); // Frequency vs Recency
    // std::cout << "All tests were printed." << std::endl;



    // Запуска собственного теста (как в задании) 
    // run();



    // Немного о графике, который находится в imgs
    // Мы меняем Number of Keys, чтобы посмотреть, как кэш реагирует на то, если пользователь 
    // открывает большое число страниц.
    // Помимо этого кэш также реагирует на плотность ключей, то есть, как часто
    // пользователь к страницам разной тематики.
    // key density = (количество уникальных ключей / общее количество запросов) * 100%



    // Генерация данных для сравнения с идеальным кэшем
    generateData();
}

static void run()
{
    size_t capacity = 0;
    size_t numOfKeys = 0;
    size_t misses = 0;
    int key = 0;


    // std::cout << "Enter [cache capacity] [number of keys] <keys>" << std::endl;
    std::cin >> capacity >> numOfKeys;

    LIRSCache<int> cache(capacity);

    for (size_t i = 1; i <= numOfKeys; ++i)
    {
        std::cin >> key;

        if (cache.get(key) == nullptr)
        {
            misses++;
            cache.put(key, key);
        }
        std::cout << cache << std::endl;
    }

    // std::cout << "Misses: " << misses << std::endl;
}