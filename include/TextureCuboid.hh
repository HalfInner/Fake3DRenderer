//
// Created by kajbr on 08.03.2020.
//

#ifndef FAKE3DRENDERER_TEXTURECUBOID_HH
#define FAKE3DRENDERER_TEXTURECUBOID_HH

namespace Graphic {

class TextureCuboid : public Renderable {
  public:
    explicit TextureCuboid(glm::vec3 position = {}, glm::vec3 sideLengths = {1.f, 1.f, 1.f},
            glm::vec3 color = {1.0f, 0.5f, 0.31f}) :
    position_(position), sideLengths_(sideLengths), color_(color) {}

    void initialize(std::shared_ptr<Graphic::Buffer> buffer) override {
        buffer_ = std::move(buffer);

        auto sidePosX = sideLengths_.x / 2;
        auto sidePosY = sideLengths_.y / 2;
        auto sidePosZ = sideLengths_.z / 2;

        auto letUpCorner = glm::vec3{0, 1, 0.f};
        auto letBottomCorner = glm::vec3{0, 0, 0.f};
        auto rightUpCorner = glm::vec3{1, 1, 0.f};
        auto rightBottomCorner = glm::vec3{1, 0, 0.f};

        auto front_t_1 = glm::vec3{-sidePosX, sidePosY, sidePosZ};
        auto front_t_t1 = letUpCorner;
        auto front_t_2 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto front_t_t2 = letBottomCorner;
        auto front_t_3 = glm::vec3{sidePosX, sidePosY, sidePosZ};
        auto front_t_t3 = rightUpCorner;
        auto front_t_4 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto front_t_t4 = rightBottomCorner;
        auto front_norm_ = glm::vec3{0, 0, 1};

        auto rear_t_1 = glm::vec3{-sidePosX, sidePosY, -sidePosZ};
        auto rear_t_t1 = letUpCorner;
        auto rear_t_2 = glm::vec3{-sidePosX, -sidePosY, -sidePosZ};
        auto rear_t_t2 = letBottomCorner;
        auto rear_t_3 = glm::vec3{sidePosX, sidePosY, -sidePosZ};
        auto rear_t_t3 = rightUpCorner;
        auto rear_t_4 = glm::vec3{sidePosX, -sidePosY, -sidePosZ};
        auto rear_t_t4 = rightBottomCorner;
        auto rear_norm_ = glm::vec3{0, 0, -1};

        auto left_t_1 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto left_t_t1 = letUpCorner;
        auto left_t_2 = glm::vec3{-sidePosX, -sidePosY, -sidePosZ};
        auto left_t_t2 = letBottomCorner;
        auto left_t_3 = glm::vec3{-sidePosX, sidePosY, sidePosZ};
        auto left_t_t3 = rightUpCorner;
        auto left_t_4 = glm::vec3{-sidePosX, sidePosY, -sidePosZ};
        auto left_t_t4 = rightBottomCorner;
        auto left_norm_ = glm::vec3{-1, 0, 0};

        auto right_t_1 = glm::vec3{sidePosX, -sidePosY, sidePosZ};
        auto right_t_t1 = letUpCorner;
        auto right_t_2 = glm::vec3{sidePosX, -sidePosY, -sidePosZ};
        auto right_t_t2 = letBottomCorner;
        auto right_t_3 = glm::vec3{sidePosX, sidePosY, sidePosZ};
        auto right_t_t3 = rightUpCorner;
        auto right_t_4 = glm::vec3{sidePosX, sidePosY, -sidePosZ};
        auto right_t_t4 = rightBottomCorner;
        auto right_norm_ = glm::vec3{1, 0, 0};

        auto top_t_1 = glm::vec3{-sidePosX, sidePosY, sidePosZ};
        auto top_t_t1 = letUpCorner;
        auto top_t_2 = glm::vec3{sidePosX, sidePosY, sidePosZ};
        auto top_t_t2 = letBottomCorner;
        auto top_t_3 = glm::vec3{-sidePosX, sidePosY, -sidePosZ};
        auto top_t_t3 = rightUpCorner;
        auto top_t_4 = glm::vec3{sidePosX, sidePosY, -sidePosZ};
        auto top_t_t4 = rightBottomCorner;
        auto top_norm_ = glm::vec3{0, 1, 0};

        auto bottom_t_1 = glm::vec3{-sidePosX, -sidePosY, sidePosZ};
        auto bottom_t_t1 = letUpCorner;
        auto bottom_t_2 = glm::vec3{sidePosX, -sidePosY, sidePosZ};
        auto bottom_t_t2 = letBottomCorner;
        auto bottom_t_3 = glm::vec3{-sidePosX, -sidePosY, -sidePosZ};
        auto bottom_t_t3 = rightUpCorner;
        auto bottom_t_4 = glm::vec3{sidePosX, -sidePosY, -sidePosZ};
        auto bottom_t_t4 = rightBottomCorner;
        auto bottom_norm_ = glm::vec3{0, -1, 0};

        buffer_->initialize({
                                    front_t_1, front_norm_, front_t_t1,
                                    front_t_2, front_norm_, front_t_t2,
                                    front_t_3, front_norm_, front_t_t3,
                                    front_t_4, front_norm_, front_t_t4,
                                    rear_t_1, rear_norm_, rear_t_t1,
                                    rear_t_2, rear_norm_, rear_t_t2,
                                    rear_t_3, rear_norm_, rear_t_t3,
                                    rear_t_4, rear_norm_, rear_t_t4,
                                    left_t_1, left_norm_, left_t_t1,
                                    left_t_2, left_norm_, left_t_t2,
                                    left_t_3, left_norm_, left_t_t3,
                                    left_t_4, left_norm_, left_t_t4,
                                    right_t_1, right_norm_, right_t_t1,
                                    right_t_2, right_norm_, right_t_t2,
                                    right_t_3, right_norm_, right_t_t3,
                                    right_t_4, right_norm_, right_t_t4,
                                    top_t_1, top_norm_, top_t_t1,
                                    top_t_2, top_norm_, top_t_t2,
                                    top_t_3, top_norm_, top_t_t3,
                                    top_t_4, top_norm_, top_t_t4,
                                    bottom_t_1, bottom_norm_, bottom_t_t1,
                                    bottom_t_2, bottom_norm_, bottom_t_t2,
                                    bottom_t_3, bottom_norm_, bottom_t_t3,
                                    bottom_t_4, bottom_norm_, bottom_t_t4,
                            }, {
                                    0, 1, 2, 1, 2, 3,
                                    4, 5, 6, 6, 7, 5,
                                    8, 10, 9, 10, 11, 9,
                                    12, 13, 14, 13, 14, 15,
                                    16, 17, 18, 17, 18, 19,
                                    20, 21, 22, 21, 22, 23}, BufferType::NormalWithTexture);
    }

    RendererInfo beginDraw(float elapsedTime) override {
        RendererInfo ri{};
        ri.elements = 6 * 2 * 3;
        ri.position = position_;
        ri.color = color_;
        ri.typeObject = TypeObject::PoolBall;
        buffer_->bind();
        return ri;
    }

    void endDraw() override {
        buffer_->unbind();
    }

  private:
    std::shared_ptr<Buffer> buffer_{nullptr};
    glm::vec3 position_;
    glm::vec3 sideLengths_;
    glm::vec3 color_;
};

} // namespace Graphic
#endif //FAKE3DRENDERER_TEXTURECUBOID_HH
