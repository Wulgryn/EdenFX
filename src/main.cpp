#include "collections/list.hpp"
#include "io/directory.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

#include <iostream>
using namespace std;

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{
    using Eden::Collections::List;
    using Eden::IO::Directory;
    using Eden::IO::Path;
    using Eden::Text::String;

    List<int> numbers;
    numbers.add(10);

    String name = "latest.txt";
    Path path = Path("data") / "logs" / name;

    std::cout << path.to_generic_string().to_std_string() << "\n";
    std::cout << path.filename().to_std_string() << "\n";

    Directory current = Directory::GetCurrentDirectory();
    cout << current.path().to_std_string() << "\n";
    auto files = current.get_files(true, "*.cpp");

    std::cout << files.count() << "\n";

    return 0;
}
