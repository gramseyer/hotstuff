#include "cleanup.h"

#include <cerrno>
#include <system_error>

namespace utils {

void
threrror(const char *msg)
{
  throw std::system_error(errno, std::system_category(), msg);
}

void
threrror(const std::string msg)
{
  throw std::system_error(errno, std::system_category(), std::move(msg));
}

} /* utils */
