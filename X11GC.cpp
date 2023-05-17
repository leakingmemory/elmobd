//
// Created by sigsegv on 5/17/23.
//

#include "X11GC.h"
#include "X11GCImpl.h"

std::shared_ptr<X11GC> X11GC::Create(const std::shared_ptr<X11Window> &window) {
    return std::make_shared<X11GCImpl>(window);
}

