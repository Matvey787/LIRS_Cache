#ifndef CACHE_RENDERING_H
#define CACHE_RENDERING_H

#include <string>
#include <vector>

namespace Rendering {

    void generateData(const std::string& cacheDataFile = "py/data.txt", 
                      const std::string& OPTDataFile = "");
}


#endif // CACHE_RENDERING_H