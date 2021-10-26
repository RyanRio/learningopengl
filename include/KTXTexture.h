#include <ktx.h>
#include <string>

class KTXTexture {
  public:
    KTXTexture(std::string file);
    ktxTexture2 *m_texture = nullptr;
};