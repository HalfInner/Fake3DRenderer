//
// Created by kajbr on 06.03.2020.
//

#ifndef FAKE3DRENDERER_TEXTUREMANAGER_HH
#define FAKE3DRENDERER_TEXTUREMANAGER_HH
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.hh"

class OpenGlStbImageReaderWrapper {
  public:
    explicit OpenGlStbImageReaderWrapper(const std::string &&path) {
        data_ = stbi_load(path.c_str(), &width_, &height_, &nrChannels_, 0);
        if (data_) {
            glGenTextures(1, &textureId_);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);
            glGenerateMipmap(GL_TEXTURE_2D);
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

class TextureManager {
    enum class Texture {
        PoolBall, Grass, None
    };

  public:
    void initialize() {
        textureIds_[Texture::PoolBall] = textureWrappers_.emplace_back("..//textureId_//PoolBall.jpg")();
    }

    ~TextureManager() = default;
  private:
    std::vector<OpenGlStbImageReaderWrapper> textureWrappers_;
    std::unordered_map<Texture, unsigned> textureIds_{{Texture::None, 0}};
};

#undef STB_IMAGE_IMPLEMENTATION
#endif //FAKE3DRENDERER_TEXTUREMANAGER_HH
