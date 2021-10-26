#ifndef KTX_TEXTURE
#define KTX_TEXTURE

#include <ktx.h>
#include <string>
#include <vector>

class KTXTexture {
  public:
    KTXTexture(std::string file);
    ktxTexture2 *m_texture = nullptr;
};

#endif // !KTX_TEXTURE