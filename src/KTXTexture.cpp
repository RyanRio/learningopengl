#include "KTXTexture.h"

using namespace std;

KTXTexture::KTXTexture(string file) {
    KTX_error_code result;
    result = ktxTexture_CreateFromNamedFile(file.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS,
                                            &m_texture);
}