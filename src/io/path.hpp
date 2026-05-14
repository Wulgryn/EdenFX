#ifndef EDEN_IO_PATH_HPP
#define EDEN_IO_PATH_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "text/string.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace Eden::IO
{
    namespace fs = std::filesystem;

    namespace Managed
    {
        class PathData : public virtual OBJECT
        {
        public:
            fs::path m_Path;
        };
    }

    class Path : public MANAGEDOBJECT<Managed::PathData>
    {
    public:
        using NativePath = fs::path;

    public:
        // =====================================================
        // Konstruktorok
        // =====================================================

        Path();
        Path(const char* path);
        Path(const std::string& path);
        Path(const std::wstring& path);
        Path(const Eden::Text::String& path);
        Path(const fs::path& path);

        // =====================================================
        // Alap elérés
        // =====================================================

        Eden::Text::String to_string() const;
        std::wstring to_wstring() const;
        std::string to_std_string() const;

        operator fs::path() const;
        explicit operator Eden::Text::String() const;

        bool empty() const;
        void clear();

        // =====================================================
        // Nem statikus C#-szerű metódusok
        // =====================================================

        Path directory_name() const;
        Path root() const;

        bool has_extension() const;
        bool is_rooted() const;
        bool is_absolute() const;

        Path full_path() const;
        Path normalized() const;
        Path relative_to(const Path& basePath) const;

        Path with_extension(const Path& extension) const;
        Path with_file_name(const Path& fileName) const;
        Path combine_with(const Path& other) const;

        void append(const Path& part);
        void replace_extension(const Path& extension);
        void replace_file_name(const Path& fileName);
        void make_preferred();

        // =====================================================
        // Operátorok
        // =====================================================

        Path operator/(const Path& other) const;
        Path& operator/=(const Path& other);

        bool operator==(const Path& other) const;
        bool operator!=(const Path& other) const;
        bool operator<(const Path& other) const;

        // =====================================================
        // String formátumok
        // =====================================================

        Eden::Text::String to_generic_string() const;
        Eden::Text::String to_preferred_string() const;

        // =====================================================
        // Has... metódusok
        // =====================================================

        bool has_root_name() const;
        bool has_root_directory() const;
        bool has_root_path() const;
        bool has_relative_path() const;
        bool has_parent_path() const;
        bool has_filename() const;
        bool has_filename_without_extension() const;

        // =====================================================
        // Nem statikus C++ std::filesystem-szerű aliasok
        // =====================================================

        Path filename() const;
        Path filename_without_extension() const;
        Path extension() const;
        Path parent_path() const;
        Path root_path() const;
        Path root_name() const;
        Path root_directory() const;
        Path relative_path() const;

        Path lexically_normal() const;
        Path lexically_relative(const Path& base) const;
        Path lexically_proximate(const Path& base) const;

        bool is_relative() const;

        // =====================================================
        // Statikus C# Path-szerű metódusok
        // =====================================================

        template <typename... TParts>
        static Path Combine(const TParts&... parts)
        {
            fs::path result;
            ((result /= ToNativePath(parts)), ...);
            return Path(result);
        }

        template <typename... TParts>
        static Path Join(const TParts&... parts)
        {
            fs::path result;
            ((result /= ToNativePath(parts)), ...);
            return Path(result);
        }

        static Eden::Text::String GetFileName(const Path& path);
        static Eden::Text::String GetFileNameWithoutExtension(const Path& path);
        static Eden::Text::String GetExtension(const Path& path);

        static Path GetDirectoryName(const Path& path);
        static Path GetPathRoot(const Path& path);

        static bool HasExtension(const Path& path);
        static bool IsPathRooted(const Path& path);
        static bool IsPathFullyQualified(const Path& path);

        static Path ChangeExtension(const Path& path, const Path& extension);

        static Path GetFullPath(const Path& path);
        static Path GetRelativePath(const Path& relativeTo, const Path& path);

        static Path GetTempPath();
        static Path GetRandomFileName();
        static Path GetTempFileName();

        // =====================================================
        // Statikus C++ std::filesystem-szerű metódusok
        // =====================================================

        static Path Absolute(const Path& path);
        static Path WeaklyCanonical(const Path& path);
        static Path Canonical(const Path& path);

        static Path LexicallyNormal(const Path& path);
        static Path LexicallyRelative(const Path& path, const Path& base);
        static Path LexicallyProximate(const Path& path, const Path& base);

        static fs::path::value_type DirectorySeparatorChar();
        static fs::path::value_type AltDirectorySeparatorChar();
        static fs::path::value_type PathSeparator();

        static bool EndsInDirectorySeparator(const Path& path);
        static Path TrimEndingDirectorySeparator(const Path& path);

        static Collections::List<char> GetInvalidFileNameChars();
        static Collections::List<char> GetInvalidPathChars();

    private:
        template <typename T>
        static fs::path ToNativePath(const T& value)
        {
            return fs::path(value);
        }

        static fs::path ToNativePath(const Path& value)
        {
            return value.data()->m_Path;
        }

        static fs::path ToNativePath(const Eden::Text::String& value)
        {
            return fs::path(value.native());
        }
    };
}

#endif // EDEN_IO_PATH_HPP
