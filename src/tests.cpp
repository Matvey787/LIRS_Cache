#include "lirs.h"
#include "tests.h"

#define TEST_START_M(testName) \
    std::cout << "---------- " << __FUNCTION__ << ": " << testName << " ----------" << std::endl;
#define TEST_END_M \
    std::cout << "---------- " << __FUNCTION__ << " end printing ----------\n" << std::endl;

void test1()
{
    TEST_START_M("Basic Operations")

    LIRSCache<std::string> cache(5);

    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C");
    cache.put(4, "D");
    cache.put(5, "E");
    std::cout << "\nAfter putting elements (key 1, key 2, key 3, key 4, key 5):" << std::endl;
    std::cout << cache;

    std::cout << "Get 1: " << cache.get(1) << std::endl;
    std::cout << "Get 2: " << cache.get(2) << std::endl;
    std::cout << cache;

    
    cache.put(6, "F");
    std::cout << "\nAfter putting element (key 6):" << std::endl;
    std::cout << cache;


    std::cout << "Get 3: " << cache.get(3) << std::endl;
    std::cout << "Get 4: " << cache.get(4) << std::endl;
    std::cout << "Get 5: " << cache.get(5) << std::endl;
    std::cout << "Get 6: " << cache.get(6) << std::endl;
    std::cout << cache;

    TEST_END_M
}

void test2()
{
    TEST_START_M("HIR to LIR Conversion");

    LIRSCache<std::string> cache(5);
    
    cache.put(1, "A");
    cache.put(2, "B");
    std::cout << "Initial state:" << std::endl;
    std::cout << cache;
    
    std::cout << "Get 1 (HIR->LIR): " << cache.get(1) << std::endl;
    std::cout << cache;
    
    cache.put(3, "C");
    std::cout << "After adding third element:" << std::endl;
    std::cout << cache;
    
    std::cout << "Get 2 (HIR->LIR): " << cache.get(2) << std::endl;
    std::cout << cache;

    TEST_END_M
}

void test3()
{
    TEST_START_M("LIR Demotion")

    LIRSCache<std::string> cache(4);
    
    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C");
    cache.put(4, "D");
    
    cache.get(1);
    cache.get(2);
    cache.get(3);
    cache.get(4);
    
    std::cout << "All elements converted to LIR:" << std::endl;
    std::cout << cache;
    
    cache.put(5, "E");
    std::cout << "After adding 5th element (should demote one LIR):" << std::endl;
    std::cout << cache;

    TEST_END_M
}

void test4()
{
    TEST_START_M("Frequency vs Recency")

    LIRSCache<std::string> cache(3);
    
    cache.put(1, "A"); // Frequent but not recent
    cache.put(2, "B");
    cache.get(1);
    cache.get(2);
    
    cache.put(3, "C"); // Recent but not frequent
    cache.put(4, "D");
    
    std::cout << "After mixed pattern:" << std::endl;
    std::cout << cache;
    
    std::cout << "Access pattern: 1(freq), 2(freq), 3(rec), 4(rec)" << std::endl;

    TEST_END_M
}
