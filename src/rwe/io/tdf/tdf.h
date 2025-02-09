#pragma once

#include <rwe/io/tdf/TdfBlock.h>
#include <rwe/rwe_string.h>

namespace rwe
{
    TdfBlock parseTdf(ConstUtf8Iterator& begin, ConstUtf8Iterator& end);

    TdfBlock parseTdfFromString(const std::string& input);

    std::vector<TdfBlock> parseListTdfFromString(const std::string& input);

    TdfBlock parseTdfFromBytes(const std::vector<char>& bytes);

    std::vector<TdfBlock> parseListTdfFromBytes(const std::vector<char>& bytes);
}
