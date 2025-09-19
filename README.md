# LIRS Cache

## Algorithm
`LIRS` (Low Inter-Reference Recency Set) — множество с низкой референсной давностью, то есть элементы, которые часто используются.  
`HIRS` (High Inter-Reference Recency Set) — множество с высокой референсной давностью, то есть элементы, которые реже используются в кэше.  
<table>
    <tr>
        <td><img src="imgs/get.png" width="90%" height="auto"></td>
        <td><img src="imgs/put.png" width="90%" height="auto"></td>
    </tr>
</table>

## Performance

Сравнение с "идеальным" (но в реальности неосуществимым) OPT (Optimal Page Replacemen) кэшем.

![graph](imgs/LIRS_Cache_Performance.png)

## Оптимизация LIRS хеша

Заходим в `Intel VTune Profiler`  
![vtune start](imgs/vtuneStart.png)

Изначально на большой тест [test11.dat](tests/test11.dat) имеем:
![without opt](imgs/withoutOpt.png)

Было принято избавиться от std::list::remove, будем дополнительно хранить информацию о итераторе данной
 страницы, то есть к lirs или hirs принадлежит данная страница и место, где она там хранится.  

Меняем

```cpp
std::unordered_map<int, std::pair<valType, pageKey_t>> cache_;
```

на такую конструкцию:

```cpp
std::unordered_map<int, PageData<valType>> cache_;
```

PageData_t:

```cpp
template <typename valType>
struct PageData_t
{
    valType value_;
    pageKey_t type_;
    typename std::list<int>::iterator listIter_;
};
```

Имеем:
![with opt](imgs/withOpt.png)

## Оптимизация OPT хеша

На большой тест изначально имеем: 
![alt text](imgs/opt_perf1.png)

При удалении первого элемента (erase(begin())) из vector приходится сдвигать все остальные элементы,
сдвиг памти - долго.
Меняем

```cpp
std::unordered_map<int, std::vector<size_t>> keysFutureOccurrs_;
```

На

```cpp
std::unordered_map<int, std::deque<size_t>> keysFutureOccurrs_;
```

Берем `std::deque` для быстрого доступа к следующему вызову по данному ключу, быстрое удаление в конце (благодаря двусторонней очереди)

Имеем

![alt text](imgs/opt_perf2.png)