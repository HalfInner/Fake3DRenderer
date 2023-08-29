//
// Created by kajbr on 06.03.2020.
//

#ifndef FAKE3DRENDERER_TEXTUREMANAGER_HH
#define FAKE3DRENDERER_TEXTUREMANAGER_HH

#include <string>
#include <unordered_map>
#include <vector>

#include "Utils.hh"

class OpenGlStbImageReaderWrapper {
  public:
    explicit OpenGlStbImageReaderWrapper(const std::string &&path);

    ~OpenGlStbImageReaderWrapper();

    unsigned operator()();

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
  public:
    void initialize();

    void bindTexture(Utils::TypeObject type);

  private:
    std::vector<OpenGlStbImageReaderWrapper> textureWrappers_;
    std::unordered_map<Utils::TypeObject, unsigned> textureIds_{{Utils::TypeObject::NotSpecified, 0}};
};

#endif //FAKE3DRENDERER_TEXTUREMANAGER_HH
