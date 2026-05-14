#ifndef EDEN_IO_DIRECTORY_HPP
#define EDEN_IO_DIRECTORY_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

#include <filesystem>

namespace Eden::IO
{
    namespace fs = std::filesystem;
    
    namespace Managed
    {
        class DirectoryData : public virtual OBJECT
        {
            public:
            Eden::IO::Path m_Path;
        };
    }
    
    class Directory : public MANAGEDOBJECT<Managed::DirectoryData>
    {
        public:
        Directory();
        Directory(const char* path);
        Directory(const std::string& path);
        Directory(const std::wstring& path);
        Directory(const Eden::Text::String& path);
        Directory(const Eden::IO::Path& path);
        Directory(const fs::path& path);
        
        // =====================================================
        // Nem statikus DirectoryInfo-szerű rész
        // =====================================================
        
        const Eden::IO::Path& path() const;
        Eden::IO::Path& path();
        
        Eden::Text::String name() const;
        Eden::IO::Path parent() const;
        Eden::IO::Path full_name() const;
        
        bool exists() const;
        bool is_empty() const;
        
        void create() const;
        void Delete(bool recursive = false) const;
        void move_to(const Eden::IO::Path& target);
        void copy_to(const Eden::IO::Path& target, bool overwrite = false) const;
        
        Collections::List<Eden::IO::Path> get_files(bool recursive = false, const Eden::Text::String& searchPattern = "*") const;
        Collections::List<Eden::IO::Path> get_directories(bool recursive = false, const Eden::Text::String& searchPattern = "*") const;
        Collections::List<Eden::IO::Path> get_entries(bool recursive = false, const Eden::Text::String& searchPattern = "*") const;
        
        Collections::List<Eden::IO::Path> enumerate_files(bool recursive = false, const Eden::Text::String& searchPattern = "*") const;
        Collections::List<Eden::IO::Path> enumerate_directories(bool recursive = false, const Eden::Text::String& searchPattern = "*") const;
        Collections::List<Eden::IO::Path> enumerate_entries(bool recursive = false, const Eden::Text::String& searchPattern = "*") const;
        
        fs::file_time_type last_write_time() const;
        void set_last_write_time(fs::file_time_type time) const;
        
        // =====================================================
        // Statikus C# Directory-szerű rész
        // =====================================================
        
        static bool Exists(const Eden::IO::Path& path);
        static bool IsEmpty(const Eden::IO::Path& path);
        
        static Directory Create(const Eden::IO::Path& path);
        static Directory CreateDirectory(const Eden::IO::Path& path);
        
        static void Delete(const Eden::IO::Path& path, bool recursive = false);
        static void Move(const Eden::IO::Path& source, const Eden::IO::Path& target);
        static void Copy(const Eden::IO::Path& source, const Eden::IO::Path& target, bool overwrite = false);
        
        static Collections::List<Eden::IO::Path> GetFiles(const Eden::IO::Path& path, bool recursive = false, const Eden::Text::String& searchPattern = "*");
        static Collections::List<Eden::IO::Path> GetDirectories(const Eden::IO::Path& path, bool recursive = false, const Eden::Text::String& searchPattern = "*");
        static Collections::List<Eden::IO::Path> GetEntries(const Eden::IO::Path& path, bool recursive = false, const Eden::Text::String& searchPattern = "*");
        
        static Collections::List<Eden::IO::Path> EnumerateFiles(const Eden::IO::Path& path, bool recursive = false, const Eden::Text::String& searchPattern = "*");
        static Collections::List<Eden::IO::Path> EnumerateDirectories(const Eden::IO::Path& path, bool recursive = false, const Eden::Text::String& searchPattern = "*");
        static Collections::List<Eden::IO::Path> EnumerateEntries(const Eden::IO::Path& path, bool recursive = false, const Eden::Text::String& searchPattern = "*");
        
        static Eden::IO::Path GetCurrentDirectory();
        static void SetCurrentDirectory(const Eden::IO::Path& path);
        
        static Eden::IO::Path GetParent(const Eden::IO::Path& path);
        
        static fs::file_time_type GetLastWriteTime(const Eden::IO::Path& path);
        static void SetLastWriteTime(const Eden::IO::Path& path, fs::file_time_type time);
        
        private:
        static bool WildcardMatch(const Eden::Text::String& pattern, const Eden::Text::String& value);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::IO;
#endif

#endif // EDEN_IO_DIRECTORY_HPP
