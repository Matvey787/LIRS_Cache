#include "lirs.h"
#include "cacheRendering.h"

static void run();

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



    // Генерация данных для сравнения с идеальным кэшем
    // generateData();
}

static void run()
{
    size_t capacity = 0;
    size_t numOfKeys = 0;
    size_t hits = 0;
    int key = 0;


    // std::cout << "Enter [cache capacity] [number of keys] <keys>" << std::endl;
    std::cin >> capacity >> numOfKeys;

    LIRSCache<int> cache(capacity);

    for (size_t i = 1; i <= numOfKeys; ++i)
    {
        std::cin >> key;

        if (cache.get(key) != nullptr)
        {
            hits++;
        }
        else
        {
            cache.put(key, key);
        }   
    }

    // std::cout << "Hits: " << hits << std::endl;
    std::cout << hits << std::endl;
}