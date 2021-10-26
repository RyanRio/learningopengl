#include "KTXTexture.h"

using namespace std;

KTXTexture::KTXTexture(string file) {
    KTX_error_code result;
    result = ktxTexture_CreateFromNamedFile(
        file.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, (ktxTexture **)&m_texture);

    if (ktxTexture2_NeedsTranscoding(m_texture)) {
        result = ktxTexture2_TranscodeBasis(m_texture, KTX_TTF_BC7_RGBA, 0);
    }
}