#include "io/path.hpp"

#include <fstream>
#include <random>

namespace Eden::IO
{
    // =====================================================
    // Konstruktorok
    // =====================================================

    Path::Path() = default;

    Path::Path(const char* path)
    {
        if (path != nullptr)
        {
            this->data()->m_Path = fs::path(path);
        }
    }

    Path::Path(const std::string& path)
    {
        this->data()->m_Path = fs::path(path);
    }

    Path::Path(const std::wstring& path)
    {
        this->data()->m_Path = fs::path(path);
    }

    Path::Path(const Eden::Text::String& path)
    {
        this->data()->m_Path = fs::path(path.std());
    }

    Path::Path(const fs::path& path)
    {
        this->data()->m_Path = path;
    }

    // =====================================================
    // Alap elérés
    // =====================================================

    Eden::Text::String Path::to_string() const
    {
        return Eden::Text::String(this->data()->m_Path.string());
    }

    std::wstring Path::to_wstring() const
    {
        return this->data()->m_Path.wstring();
    }

    std::string Path::to_std_string() const
    {
        return this->data()->m_Path.string();
    }

    Path::operator fs::path() const
    {
        return this->data()->m_Path;
    }

    Path::operator Eden::Text::String() const
    {
        return to_string();
    }

    bool Path::empty() const
    {
        return this->data()->m_Path.empty();
    }

    void Path::clear()
    {
        this->data()->m_Path.clear();
    }

    // =====================================================
    // Nem statikus C#-szerű metódusok
    // =====================================================

    Path Path::directory_name() const
    {
        return Path(this->data()->m_Path.parent_path());
    }

    Path Path::root() const
    {
        return Path(this->data()->m_Path.root_path());
    }

    bool Path::has_extension() const
    {
        return this->data()->m_Path.has_extension();
    }

    bool Path::is_rooted() const
    {
        return this->data()->m_Path.has_root_path();
    }

    bool Path::is_absolute() const
    {
        return this->data()->m_Path.is_absolute();
    }

    Path Path::full_path() const
    {
        return Path(fs::absolute(this->data()->m_Path).lexically_normal());
    }

    Path Path::normalized() const
    {
        return Path(this->data()->m_Path.lexically_normal());
    }

    Path Path::relative_to(const Path& basePath) const
    {
        fs::path absoluteBase = fs::absolute(basePath.data()->m_Path).lexically_normal();
        fs::path absoluteThis = fs::absolute(this->data()->m_Path).lexically_normal();

        return Path(absoluteThis.lexically_relative(absoluteBase));
    }

    Path Path::with_extension(const Path& extension) const
    {
        fs::path result = this->data()->m_Path;
        result.replace_extension(extension.data()->m_Path);
        return Path(result);
    }

    Path Path::with_file_name(const Path& fileName) const
    {
        fs::path result = this->data()->m_Path;
        result.replace_filename(fileName.data()->m_Path);
        return Path(result);
    }

    Path Path::combine_with(const Path& other) const
    {
        return Path(this->data()->m_Path / other.data()->m_Path);
    }

    void Path::append(const Path& part)
    {
        this->data()->m_Path /= part.data()->m_Path;
    }

    void Path::replace_extension(const Path& extension)
    {
        this->data()->m_Path.replace_extension(extension.data()->m_Path);
    }

    void Path::replace_file_name(const Path& fileName)
    {
        this->data()->m_Path.replace_filename(fileName.data()->m_Path);
    }

    void Path::make_preferred()
    {
        this->data()->m_Path.make_preferred();
    }

    // =====================================================
    // Operátorok
    // =====================================================

    Path Path::operator/(const Path& other) const
    {
        return Path(this->data()->m_Path / other.data()->m_Path);
    }

    Path& Path::operator/=(const Path& other)
    {
        this->data()->m_Path /= other.data()->m_Path;
        return *this;
    }

    bool Path::operator==(const Path& other) const
    {
        return this->data()->m_Path == other.data()->m_Path;
    }

    bool Path::operator!=(const Path& other) const
    {
        return !(*this == other);
    }

    bool Path::operator<(const Path& other) const
    {
        return this->data()->m_Path < other.data()->m_Path;
    }

    // =====================================================
    // String formátumok
    // =====================================================

    Eden::Text::String Path::to_generic_string() const
    {
        return Eden::Text::String(this->data()->m_Path.generic_string());
    }

    Eden::Text::String Path::to_preferred_string() const
    {
        fs::path copy = this->data()->m_Path;
        copy.make_preferred();
        return Eden::Text::String(copy.string());
    }

    // =====================================================
    // Has... metódusok
    // =====================================================

    bool Path::has_root_name() const
    {
        return this->data()->m_Path.has_root_name();
    }

    bool Path::has_root_directory() const
    {
        return this->data()->m_Path.has_root_directory();
    }

    bool Path::has_root_path() const
    {
        return this->data()->m_Path.has_root_path();
    }

    bool Path::has_relative_path() const
    {
        return this->data()->m_Path.has_relative_path();
    }

    bool Path::has_parent_path() const
    {
        return this->data()->m_Path.has_parent_path();
    }

    bool Path::has_filename() const
    {
        return this->data()->m_Path.has_filename();
    }

    bool Path::has_filename_without_extension() const
    {
        return this->data()->m_Path.has_stem();
    }

    // =====================================================
    // Nem statikus C++ std::filesystem-szerű aliasok
    // =====================================================

    Path Path::filename() const
    {
        return Path(this->data()->m_Path.filename());
    }

    Path Path::filename_without_extension() const
    {
        return Path(this->data()->m_Path.stem());
    }

    Path Path::extension() const
    {
        return Path(this->data()->m_Path.extension());
    }

    Path Path::parent_path() const
    {
        return Path(this->data()->m_Path.parent_path());
    }

    Path Path::root_path() const
    {
        return Path(this->data()->m_Path.root_path());
    }

    Path Path::root_name() const
    {
        return Path(this->data()->m_Path.root_name());
    }

    Path Path::root_directory() const
    {
        return Path(this->data()->m_Path.root_directory());
    }

    Path Path::relative_path() const
    {
        return Path(this->data()->m_Path.relative_path());
    }

    Path Path::lexically_normal() const
    {
        return Path(this->data()->m_Path.lexically_normal());
    }

    Path Path::lexically_relative(const Path& base) const
    {
        return Path(this->data()->m_Path.lexically_relative(base.data()->m_Path));
    }

    Path Path::lexically_proximate(const Path& base) const
    {
        return Path(this->data()->m_Path.lexically_proximate(base.data()->m_Path));
    }


    bool Path::is_relative() const
    {
        return this->data()->m_Path.is_relative();
    }

    // =====================================================
    // Statikus C# Path-szerű metódusok
    // =====================================================

    Eden::Text::String Path::GetFileName(const Path& path)
    {
        return Eden::Text::String(path.data()->m_Path.filename().string());
    }

    Eden::Text::String Path::GetFileNameWithoutExtension(const Path& path)
    {
        return Eden::Text::String(path.data()->m_Path.stem().string());
    }

    Eden::Text::String Path::GetExtension(const Path& path)
    {
        return Eden::Text::String(path.data()->m_Path.extension().string());
    }

    Path Path::GetDirectoryName(const Path& path)
    {
        return Path(path.data()->m_Path.parent_path());
    }

    Path Path::GetPathRoot(const Path& path)
    {
        return Path(path.data()->m_Path.root_path());
    }

    bool Path::HasExtension(const Path& path)
    {
        return path.data()->m_Path.has_extension();
    }

    bool Path::IsPathRooted(const Path& path)
    {
        return path.data()->m_Path.has_root_path();
    }

    bool Path::IsPathFullyQualified(const Path& path)
    {
        return path.data()->m_Path.is_absolute();
    }

    Path Path::ChangeExtension(const Path& path, const Path& extension)
    {
        fs::path result = path.data()->m_Path;
        result.replace_extension(extension.data()->m_Path);
        return Path(result);
    }

    Path Path::GetFullPath(const Path& path)
    {
        return Path(fs::absolute(path.data()->m_Path).lexically_normal());
    }

    Path Path::GetRelativePath(const Path& relativeTo, const Path& path)
    {
        fs::path absoluteBase = fs::absolute(relativeTo.data()->m_Path).lexically_normal();
        fs::path absolutePath = fs::absolute(path.data()->m_Path).lexically_normal();

        return Path(absolutePath.lexically_relative(absoluteBase));
    }

    Path Path::GetTempPath()
    {
        return Path(fs::temp_directory_path());
    }

    Path Path::GetRandomFileName()
    {
        static constexpr char characters[] = "0123456789abcdefghijklmnopqrstuvwxyz";

        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<int> distribution(0, 35);

        std::string value;
        value.reserve(12);

        for (int i = 0; i < 8; i++)
        {
            value.push_back(characters[distribution(generator)]);
        }

        value.push_back('.');

        for (int i = 0; i < 3; i++)
        {
            value.push_back(characters[distribution(generator)]);
        }

        return Path(value);
    }

    Path Path::GetTempFileName()
    {
        Path result = GetTempPath() / GetRandomFileName();

        while (fs::exists(result.data()->m_Path))
        {
            result = GetTempPath() / GetRandomFileName();
        }

        std::ofstream file(result.data()->m_Path);
        file.close();

        return result;
    }

    // =====================================================
    // Statikus C++ std::filesystem-szerű metódusok
    // =====================================================

    Path Path::Absolute(const Path& path)
    {
        return Path(fs::absolute(path.data()->m_Path));
    }

    Path Path::WeaklyCanonical(const Path& path)
    {
        return Path(fs::weakly_canonical(path.data()->m_Path));
    }

    Path Path::Canonical(const Path& path)
    {
        return Path(fs::canonical(path.data()->m_Path));
    }

    Path Path::LexicallyNormal(const Path& path)
    {
        return Path(path.data()->m_Path.lexically_normal());
    }

    Path Path::LexicallyRelative(const Path& path, const Path& base)
    {
        return Path(path.data()->m_Path.lexically_relative(base.data()->m_Path));
    }

    Path Path::LexicallyProximate(const Path& path, const Path& base)
    {
        return Path(path.data()->m_Path.lexically_proximate(base.data()->m_Path));
    }

    fs::path::value_type Path::DirectorySeparatorChar()
    {
        return fs::path::preferred_separator;
    }

    fs::path::value_type Path::AltDirectorySeparatorChar()
    {
#ifdef _WIN32
        return static_cast<fs::path::value_type>('/');
#else
        return static_cast<fs::path::value_type>('/');
#endif
    }

    fs::path::value_type Path::PathSeparator()
    {
#ifdef _WIN32
        return static_cast<fs::path::value_type>(';');
#else
        return static_cast<fs::path::value_type>(':');
#endif
    }

    bool Path::EndsInDirectorySeparator(const Path& path)
    {
        auto text = path.data()->m_Path.native();

        if (text.empty())
        {
            return false;
        }

        auto last = text[text.size() - 1];

        return last == static_cast<fs::path::value_type>('/') ||
               last == static_cast<fs::path::value_type>('\\');
    }

    Path Path::TrimEndingDirectorySeparator(const Path& path)
    {
        auto text = path.data()->m_Path.native();

        if (text.empty())
        {
            return Path();
        }

        std::size_t minSize = path.data()->m_Path.root_path().native().size();

        if (minSize == 0)
        {
            minSize = 1;
        }

        while (text.size() > minSize)
        {
            auto last = text[text.size() - 1];

            if (last != static_cast<fs::path::value_type>('/') &&
                last != static_cast<fs::path::value_type>('\\'))
            {
                break;
            }

            text.pop_back();
        }

        return Path(fs::path(text));
    }

    Collections::List<char> Path::GetInvalidFileNameChars()
    {
        Collections::List<char> result;

#ifdef _WIN32
        const char* chars = "<>:\"/\\|?*";

        for (const char* c = chars; *c != '\0'; ++c)
        {
            result.add(*c);
        }

        for (int i = 0; i < 32; i++)
        {
            result.add(static_cast<char>(i));
        }
#else
        result.Add('/');
        result.Add('\0');
#endif

        return result;
    }

    Collections::List<char> Path::GetInvalidPathChars()
    {
        Collections::List<char> result;

#ifdef _WIN32
        const char* chars = "<>\"|?*";

        for (const char* c = chars; *c != '\0'; ++c)
        {
            result.add(*c);
        }

        for (int i = 0; i < 32; i++)
        {
            result.add(static_cast<char>(i));
        }
#else
        result.Add('\0');
#endif

        return result;
    }
}
