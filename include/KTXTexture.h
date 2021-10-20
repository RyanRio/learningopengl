#include <ktx.h>
#include <string>

class KTXTexture {
  public:
    KTXTexture(std::string file);
    ktxTexture *m_texture = nullptr;
};