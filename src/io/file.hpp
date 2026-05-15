#ifndef EDEN_IO_FILE_HPP
#define EDEN_IO_FILE_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/file_stream.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

#include <filesystem>

namespace Eden::IO
{
    namespace fs = std::filesystem;

    namespace Managed
    {
        class FileData : public virtual OBJECT
        {
        public:
            Eden::IO::Path m_Path;
        };
    }

    class File : public MANAGEDOBJECT<File, Managed::FileData>
    {
    public:
        using Byte = unsigned char;

        File();
        File(const char* path);
        File(const std::string& path);
        File(const std::wstring& path);
        File(const Eden::Text::String& path);
        File(const Eden::IO::Path& path);
        File(const fs::path& path);

        const Eden::IO::Path& path() const;
        Eden::IO::Path& path();

        Eden::Text::String name() const;
        Eden::Text::String extension() const;
        Eden::IO::Path directory() const;
        Eden::IO::Path full_name() const;

        bool exists() const;
        long long length() const;

        FileStream create() const;
        FileStream open(FileMode mode = FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite) const;
        FileStream open_read() const;
        FileStream open_write(bool append = false) const;

        Eden::Text::String read_all_text() const;
        Collections::List<Eden::Text::String> read_all_lines() const;
        Collections::List<Byte> read_all_bytes() const;

        void write_all_text(const Eden::Text::String& text) const;
        void append_all_text(const Eden::Text::String& text) const;
        void write_all_lines(const Collections::List<Eden::Text::String>& lines) const;
        void append_all_lines(const Collections::List<Eden::Text::String>& lines) const;
        void write_all_bytes(const Collections::List<Byte>& bytes) const;

        void Delete() const;
        void copy_to(const Eden::IO::Path& destination, bool overwrite = false) const;
        void move_to(const Eden::IO::Path& destination, bool overwrite = false);

        fs::file_time_type last_write_time() const;
        void set_last_write_time(fs::file_time_type time) const;

        static bool Exists(const Eden::IO::Path& path);
        static long long Length(const Eden::IO::Path& path);

        static FileStream Create(const Eden::IO::Path& path);
        static FileStream Open(const Eden::IO::Path& path, FileMode mode = FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite);
        static FileStream OpenRead(const Eden::IO::Path& path);
        static FileStream OpenWrite(const Eden::IO::Path& path, bool append = false);

        static Eden::Text::String ReadAllText(const Eden::IO::Path& path);
        static Collections::List<Eden::Text::String> ReadAllLines(const Eden::IO::Path& path);
        static Collections::List<Byte> ReadAllBytes(const Eden::IO::Path& path);

        static void WriteAllText(const Eden::IO::Path& path, const Eden::Text::String& text);
        static void AppendAllText(const Eden::IO::Path& path, const Eden::Text::String& text);
        static void WriteAllLines(const Eden::IO::Path& path, const Collections::List<Eden::Text::String>& lines);
        static void AppendAllLines(const Eden::IO::Path& path, const Collections::List<Eden::Text::String>& lines);
        static void WriteAllBytes(const Eden::IO::Path& path, const Collections::List<Byte>& bytes);

        static void Delete(const Eden::IO::Path& path);
        static void Copy(const Eden::IO::Path& source, const Eden::IO::Path& destination, bool overwrite = false);
        static void Move(const Eden::IO::Path& source, const Eden::IO::Path& destination, bool overwrite = false);
        static void Replace(const Eden::IO::Path& sourceFileName, const Eden::IO::Path& destinationFileName, const Eden::IO::Path& destinationBackupFileName);

        static fs::file_time_type GetLastWriteTime(const Eden::IO::Path& path);
        static void SetLastWriteTime(const Eden::IO::Path& path, fs::file_time_type time);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::IO;
#endif

#endif // EDEN_IO_FILE_HPP
