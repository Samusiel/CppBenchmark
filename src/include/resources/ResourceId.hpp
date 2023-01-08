#pragma once

#include <filesystem>
#include <string>
#include <variant>

namespace Resources {

using WebPath = std::string;
using FileSystemPath = std::filesystem::path;

using ResourceId = std::variant<WebPath, FileSystemPath>;

} // namspace Resources