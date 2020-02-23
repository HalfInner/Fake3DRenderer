//
// Created by kajbr on 22.02.2020.
//

#ifndef FAKE3DRENDERER_UTILS_HH
#define FAKE3DRENDERER_UTILS_HH



struct NotCreatable {
    NotCreatable() = delete;
    ~NotCreatable() = delete;

    NotCreatable(const NotCreatable &) = delete;
    NotCreatable(NotCreatable &&) = delete;

    NotCreatable &operator=(const NotCreatable &) = delete;
    NotCreatable &operator=(NotCreatable &&) = delete;
};

#endif //FAKE3DRENDERER_UTILS_HH
