#include "TextureLoader.h"

using namespace std;

ktxTexture2* TextureLoader<ktxTexture2>::load(std::string file){
    ktxTexture2* m_texture;
    KTX_error_code result;
    result = ktxTexture_CreateFromNamedFile(
        file.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, (ktxTexture**)&m_texture);

    if (ktxTexture2_NeedsTranscoding(m_texture)) {
        result = ktxTexture2_TranscodeBasis(m_texture, KTX_TTF_BC7_RGBA, 0);
    }
    return m_texture;
};

ktxTexture1* TextureLoader<ktxTexture1>::load(std::string file) {
    ktxTexture1* m_texture;
    KTX_error_code result;
    result = ktxTexture_CreateFromNamedFile(
        file.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, (ktxTexture**)&m_texture);

    return m_texture;
}