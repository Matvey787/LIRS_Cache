# [5.10.2025] Cleanup overall code structure

1. **Problem [lirs.h:85]:** желательно пометить evicted как const, так как он не модифицируется, аналогично и в других местах: `int evict   ed = lirs_.back();`  
**Decision:** Поправил  

2. **Problem [lirs.h:29]:** можем ли мы сделать тип ключа также шаблонным, пример есть [тут](https://github.com/tilir/cpp-graduate/blob/master/01-basics/cache.hpp#L20)  
**Decision:** Поправил  

3. **Problem [lirs.h:179]:** git сохранит всю нужную историю и позволит откатить файл к нужной ревизии, поэтому ненужный код лучше не оставлять закомментированным. Стоит пройтись по всему репозиторию  
**Decision:** Чистим файлы

4. **Problem []:** Стоит использовать namespace для кэшей  
**Decision:** Поправил

5. **Problem []:** Можно изучить возможность увеличить производительность кэша, если ускорить поиск вытесняемого элемента (возможно, сортировать элементы в кэше по NextUse)
**Decision:** --- will come later)

# [27.09.2025] Bug fixes, algorithm cleanup, and overall code structure  

1. **Problem [lirs.h:74]:**  evicted является ссылкой на последний элемент lirs_, после pop_back ссылка провисает  
**Decision:** Переносим `lirs_.pop_back();` в конец логического блока

2. **Problem [lirs.h]:**  кэш никак не обрабатывает ситуацию, если он создан с capacity = 0 (Пример теста: 0 1 42)  
   **Decision:** Добавляем проверку на capacity в конструктор кэша, при необходимости выбрасываем исключение `std::invalid_argument("Cache capacity must be greater than 0");`, оборачиваем в `try-catch`.

3. **Problem [lirs.h]:**  алгоритм сейчас немного отличается от LIRS: не хватает resident и non-resident HIR  
   **Decision:** Я ввел non-resident HIRS в алгоритм, добавив соответствующую логику в метод put.

4. **Problem [main.cpp:2]:** желательно include, необходимый только при ifdef OPT_M, также поместить под аналогичный ifdef
**Decision:** Добавляем `#include "opt.h"` под `#ifdef OPT_M`

5. **Problem [main.cpp:3, 33]:** стоит добавить дополнительный вариант сборки и соответствующий define, дабы не оставлять закомментированный код  
**Decision:**

```cpp
#if defined(OPT_M)
    #include "opt.h"
#elif defined(CACHE_RENDER_M)
    #include "cacheRendering.h"
#else
    #include "lirs.h"
#endif

...

#ifdef CACHE_RENDER_M
    generateData("py/cacheData.txt", "py/OPTCacheData.txt");
#else
    // Запуска собственного теста (как в задании)
    run();
#endif
```

6. **Problem [main.cpp:45, 51]:** не хватает проверки реультата std::cin

**Decision:**

```cpp
    if (numOfKeys == 0)
    {
        // 0 хитов
        return;
    }

    ...
    // Проверка на количество ключей

    if(keys.size() != numOfKeys)
        throw std::runtime_error("Invalid number of keys");
```

7. **Problem [CMakeLists.txt:19]:** файл cacheRendering.cpp не используется в результирующих файлах, его нет смысла компилировать ctest  
**Decision:** cacheRendering.cpp вынесен в отдельный target `add_executable(rendering ${COMMON_SOURCE_FILES} src/cacheRendering.cpp)`

8. **Problem [CMakeLists.txt:19]:** ctest - при запуске тестов, некоторые (test 3, 4, 5) падают для идеального кэша. Хотелось бы разделения тестов для LIRS и идеального кэша.  
**Decision:** В функцию `create_tests_for_target` добавлен параметр для указания папки с тестами для конкретного таргета.  

9. **Suggestion [CMakeLists.txt:19]:** Дополнительно: можно добавить модульные тесты для проверки поведения кэша в краевых случаях.  
**Decision:** --- will come later)

10. **Problem [cacheRendering.cpp:85]:** Сейчас функция generateKeys будет гарантированно генерировать вектор состоящий из 0, если подать низкое число элементов и низкую плотность (произведение значений менее 100)  

**Decision:**

```cpp
    size_t maxKeyWanted = (double)keys.capacity() * keyDensity / 100;
    size_t maxKey = std::max(MINIMAL_KEY, maxKeyWanted);
```

11. **Problem [cacheRendering.cpp:90]:** Сейчас используется rand из C, желательно перейти на генератор из `<random>`. Также, в файле есть проблемы со смешиванием использования int/size_t - стоит привести к единому виду.  
**Decision:** Перешел на генератор `<random>`

12. **Suggestion:** можно подумать над ускорением метода generateKeys, стоит присмотреться к сложности метода vector::push_back  
**Decision:** Память для ключей выделяем тепрь перед `generateKeys`, соответственно избавляемся от лишнего аргумента,
уходим от частой реаллокации вектора за счет `reserve`.
