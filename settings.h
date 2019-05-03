#pragma once

#include "setting.h"

namespace D {
struct Settings {
  Setting< int >   IntSetting{&Holder, "int_setting", 0};
  Setting< bool >  BoolSetting{&Holder, "bool_setting", true};
  Setting< float > FloatSetting{&Holder, "float_setting", 3.f};
};
}

extern D::Settings g_cfg;
