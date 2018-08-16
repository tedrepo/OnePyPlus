#include "../Environment.h"
#include "CleanerBase.h"

namespace sys {

void CleanerBase::run(){};

template <typename cleaner_name>
void CleanerBase::save_to_env(const cleaner_name *self_ptr) {
    env->cleaners["s"] = std::make_shared<cleaner_name>(*self_ptr); //TODO:写名字
}
} // namespace sys
