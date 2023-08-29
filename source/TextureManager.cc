//
// Created by kajbr on 06.03.2020.
//
#define STB_IMAGE_IMPLEMENTATION
#include "TextureManager.hh"

#include <glad/glad.h>
#include <iostream>
#include <string>

#include "stb_image.hh"

void TextureManager::initialize() {
    textureIds_[Utils::TypeObject::PoolBall] = textureWrappers_.emplace_back("PoolBall8.jpg")();
}

void TextureManager::bindTexture(Utils::TypeObject type) {
    glBindTexture(GL_TEXTURE_2D, textureIds_[type]);
}

OpenGlStbImageReaderWrapper::OpenGlStbImageReaderWrapper(const std::string &&path) {
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

OpenGlStbImageReaderWrapper::~OpenGlStbImageReaderWrapper() {
    if (data_) {
        stbi_image_free(data_);
    }
}

unsigned OpenGlStbImageReaderWrapper::operator()() {
    return textureId_;
}

#undef STB_IMAGE_IMPLEMENTATION