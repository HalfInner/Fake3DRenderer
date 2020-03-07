//
// Created by kajbr on 06.03.2020.
//

#ifndef FAKE3DRENDERER_TEXTUREMANAGER_HH
#define FAKE3DRENDERER_TEXTUREMANAGER_HH
#define STB_IMAGE_IMPLEMENTATION

#include "Utils.hh"
#include "stb_image.hh"

class OpenGlStbImageReaderWrapper {
  public:
    explicit OpenGlStbImageReaderWrapper(const std::string &&path) {
        data_ = stbi_load(path.c_str(), &width_, &height_, &nrChannels_, 0);
        if (data_) {
            glGenTextures(1, &textureId_);
            glBindTexture(GL_TEXTURE_2D, textureId_);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "LOADING::TEX::ERR cannot load texture from: " << path << "\n";
        }
    }

    ~OpenGlStbImageReaderWrapper() {
        if (data_) {
            stbi_image_free(data_);
        }
    }

    unsigned operator()() {
        return textureId_;
    }

    OpenGlStbImageReaderWrapper(OpenGlStbImageReaderWrapper &&) = default;
    OpenGlStbImageReaderWrapper(OpenGlStbImageReaderWrapper const &) = delete;
    OpenGlStbImageReaderWrapper &operator=(OpenGlStbImageReaderWrapper &&) = default;
    OpenGlStbImageReaderWrapper &operator=(OpenGlStbImageReaderWrapper const &) = delete;

  private:
    int width_{}, height_{}, nrChannels_{};
    unsigned int textureId_{0};
    unsigned char *data_ = nullptr;
};

using namespace Utils;
class TextureManager {
  public:
    void initialize() {
        textureIds_[Utils::TypeObject::PoolBall] = textureWrappers_.emplace_back("PoolBall8.jpg")();
    }

    void bindTexture(Utils::TypeObject type) {
        glBindTexture(GL_TEXTURE_2D, textureIds_[type]);
    }

  private:
    std::vector<OpenGlStbImageReaderWrapper> textureWrappers_;
    std::unordered_map<Utils::TypeObject, unsigned> textureIds_{{Utils::TypeObject::NotSpecified, 0}};
};

#undef STB_IMAGE_IMPLEMENTATION
#endif //FAKE3DRENDERER_TEXTUREMANAGER_HH
