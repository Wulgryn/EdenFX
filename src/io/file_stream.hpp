#ifndef EDEN_IO_FILE_STREAM_HPP
#define EDEN_IO_FILE_STREAM_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/path.hpp"
#include "text/string.hpp"

#include <cstddef>
#include <fstream>
#include <ios>

namespace Eden::IO
{
    enum class FileMode
    {
        CreateNew,
        Create,
        Open,
        OpenOrCreate,
        Truncate,
        Append
    };

    enum class FileAccess
    {
        Read,
        Write,
        ReadWrite
    };

    enum class FileSeekOrigin
    {
        Begin,
        Current,
        End
    };

    namespace Managed
    {
        class FileStreamData : public virtual OBJECT
        {
        public:
            Eden::IO::Path m_Path;
            std::fstream m_Stream;
            FileMode m_Mode = FileMode::Open;
            FileAccess m_Access = FileAccess::Read;
            bool m_IsOpen = false;
        };
    }

    class FileStream : public MANAGEDOBJECT<FileStream, Managed::FileStreamData>
    {
    public:
        using NativeStream = std::fstream;
        using Byte = unsigned char;

        FileStream();
        FileStream(const Eden::IO::Path& path, FileMode mode = FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite);

        ~FileStream();

        void open(const Eden::IO::Path& path, FileMode mode = FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite);
        void close();
        void flush();

        bool is_open() const;
        bool can_read() const;
        bool can_write() const;

        const Eden::IO::Path& path() const;
        FileMode mode() const;
        FileAccess access() const;

        long long length() const;
        long long position();
        void seek(long long offset, FileSeekOrigin origin = FileSeekOrigin::Begin);

        int read();
        Collections::List<Byte> read(int count);
        Collections::List<Byte> read_all_bytes();
        Eden::Text::String read_all_text();

        void write(Byte value);
        void write(const Collections::List<Byte>& bytes);
        void write(const Eden::Text::String& text);
        void write_line(const Eden::Text::String& text);

        NativeStream& native();
        const NativeStream& native() const;

        static FileStream Open(const Eden::IO::Path& path, FileMode mode = FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite);
        static FileStream OpenRead(const Eden::IO::Path& path);
        static FileStream OpenWrite(const Eden::IO::Path& path, bool append = false);

    private:
        static std::ios::openmode ToOpenMode(FileMode mode, FileAccess access);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::IO;
#endif

#endif // EDEN_IO_FILE_STREAM_HPP
