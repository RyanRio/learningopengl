#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <ktx.h>
#include <string>
#include <vector>

// loads texture data from a file, other specializations can make use of more complex loading techniques if needed
template <typename T>
class TextureLoader {
public:
    static T* load(std::string file);
};


#endif // !TEXTURE_LOADER_H