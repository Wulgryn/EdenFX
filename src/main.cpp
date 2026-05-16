#define AUTO_USINGS
#include "io/directory.hpp"
#include "collections/list.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{
    Directory d("data");
    Directory a = d;

    List<Directory> list;
    list.add(d);
    return 0;
}
