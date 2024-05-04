#pragma once

#include <utils/genericid.h>
#include <utils/numerictypes.h>

#include <string>

namespace puma
{
    class LogCategory;
    constexpr u32 kInvalidLogCategory = kMaxU32;
}

DECLARE_GENERIC_ID( puma::LogCategory, puma::u32, puma::kInvalidLogCategory);

namespace puma
{
    enum class LogType
    {
        Info = 0,
        Warning,
        Error
    };

    struct CategoryInfo
    {
        LogCategory id;
        const char* name;
    };

}