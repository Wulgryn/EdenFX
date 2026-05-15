#include "io/file_stream.hpp"

#include <filesystem>
#include <sstream>
#include <stdexcept>

namespace Eden::IO
{
    namespace fs = std::filesystem;

    FileStream::FileStream() = default;

    FileStream::FileStream(const Eden::IO::Path& path, FileMode mode, FileAccess access)
    {
        open(path, mode, access);
    }

    FileStream::~FileStream()
    {
        if (getRefCount() <= 1)
        {
            close();
        }
    }

    void FileStream::open(const Eden::IO::Path& path, FileMode mode, FileAccess access)
    {
        close();

        fs::path nativePath = static_cast<fs::path>(path);

        if (mode == FileMode::CreateNew && fs::exists(nativePath))
        {
            throw std::runtime_error("File already exists: " + path.to_std_string());
        }

        if (mode == FileMode::Open && !fs::exists(nativePath))
        {
            throw std::runtime_error("File does not exist: " + path.to_std_string());
        }

        if (mode == FileMode::OpenOrCreate && !fs::exists(nativePath))
        {
            std::ofstream creator(nativePath, std::ios::binary);
            creator.close();
        }

        std::ios::openmode openMode = ToOpenMode(mode, access);

        this->data()->m_Path = path;
        this->data()->m_Mode = mode;
        this->data()->m_Access = access;
        this->data()->m_Stream.open(nativePath, openMode);
        this->data()->m_IsOpen = this->data()->m_Stream.is_open();

        if (!this->data()->m_IsOpen)
        {
            throw std::runtime_error("Could not open file: " + path.to_std_string());
        }
    }

    void FileStream::close()
    {
        if (this->data()->m_Stream.is_open())
        {
            this->data()->m_Stream.close();
        }

        this->data()->m_IsOpen = false;
    }

    void FileStream::flush()
    {
        if (is_open())
        {
            this->data()->m_Stream.flush();
        }
    }

    bool FileStream::is_open() const
    {
        return this->data()->m_IsOpen && this->data()->m_Stream.is_open();
    }

    bool FileStream::can_read() const
    {
        return this->data()->m_Access == FileAccess::Read || this->data()->m_Access == FileAccess::ReadWrite;
    }

    bool FileStream::can_write() const
    {
        return this->data()->m_Access == FileAccess::Write || this->data()->m_Access == FileAccess::ReadWrite;
    }

    const Eden::IO::Path& FileStream::path() const
    {
        return this->data()->m_Path;
    }

    FileMode FileStream::mode() const
    {
        return this->data()->m_Mode;
    }

    FileAccess FileStream::access() const
    {
        return this->data()->m_Access;
    }

    long long FileStream::length() const
    {
        fs::path nativePath = static_cast<fs::path>(this->data()->m_Path);

        if (!fs::exists(nativePath))
        {
            return 0;
        }

        return static_cast<long long>(fs::file_size(nativePath));
    }

    long long FileStream::position()
    {
        if (!is_open())
        {
            return -1;
        }

        std::streampos readPosition = this->data()->m_Stream.tellg();

        if (readPosition != std::streampos(-1))
        {
            return static_cast<long long>(readPosition);
        }

        std::streampos writePosition = this->data()->m_Stream.tellp();

        if (writePosition != std::streampos(-1))
        {
            return static_cast<long long>(writePosition);
        }

        return -1;
    }

    void FileStream::seek(long long offset, FileSeekOrigin origin)
    {
        if (!is_open())
        {
            return;
        }

        std::ios::seekdir direction = std::ios::beg;

        if (origin == FileSeekOrigin::Current)
        {
            direction = std::ios::cur;
        }
        else if (origin == FileSeekOrigin::End)
        {
            direction = std::ios::end;
        }

        this->data()->m_Stream.clear();

        if (can_read())
        {
            this->data()->m_Stream.seekg(offset, direction);
        }

        if (can_write())
        {
            this->data()->m_Stream.seekp(offset, direction);
        }
    }

    int FileStream::read()
    {
        if (!is_open() || !can_read())
        {
            return -1;
        }

        return this->data()->m_Stream.get();
    }

    Collections::List<FileStream::Byte> FileStream::read(int count)
    {
        Collections::List<Byte> result;

        if (!is_open() || !can_read() || count <= 0)
        {
            return result;
        }

        for (int i = 0; i < count; i++)
        {
            int value = read();

            if (value == EOF)
            {
                break;
            }

            result.add(static_cast<Byte>(value));
        }

        return result;
    }

    Collections::List<FileStream::Byte> FileStream::read_all_bytes()
    {
        Collections::List<Byte> result;

        if (!is_open() || !can_read())
        {
            return result;
        }

        this->data()->m_Stream.clear();
        this->data()->m_Stream.seekg(0, std::ios::beg);

        char current = 0;

        while (this->data()->m_Stream.get(current))
        {
            result.add(static_cast<Byte>(current));
        }

        return result;
    }

    Eden::Text::String FileStream::read_all_text()
    {
        if (!is_open() || !can_read())
        {
            return Eden::Text::String();
        }

        this->data()->m_Stream.clear();
        this->data()->m_Stream.seekg(0, std::ios::beg);

        std::ostringstream builder;
        builder << this->data()->m_Stream.rdbuf();
        return Eden::Text::String(builder.str());
    }

    void FileStream::write(Byte value)
    {
        if (!is_open() || !can_write())
        {
            return;
        }

        this->data()->m_Stream.put(static_cast<char>(value));
    }

    void FileStream::write(const Collections::List<Byte>& bytes)
    {
        if (!is_open() || !can_write())
        {
            return;
        }

        for (int i = 0; i < bytes.count(); i++)
        {
            write(bytes[i]);
        }
    }

    void FileStream::write(const Eden::Text::String& text)
    {
        if (!is_open() || !can_write())
        {
            return;
        }

        this->data()->m_Stream << text.std();
    }

    void FileStream::write_line(const Eden::Text::String& text)
    {
        write(text);
        write(Eden::Text::String("\n"));
    }

    FileStream::NativeStream& FileStream::native()
    {
        return this->data()->m_Stream;
    }

    const FileStream::NativeStream& FileStream::native() const
    {
        return this->data()->m_Stream;
    }

    FileStream FileStream::Open(const Eden::IO::Path& path, FileMode mode, FileAccess access)
    {
        return FileStream(path, mode, access);
    }

    FileStream FileStream::OpenRead(const Eden::IO::Path& path)
    {
        return FileStream(path, FileMode::Open, FileAccess::Read);
    }

    FileStream FileStream::OpenWrite(const Eden::IO::Path& path, bool append)
    {
        return FileStream(path, append ? FileMode::Append : FileMode::Create, FileAccess::Write);
    }

    std::ios::openmode FileStream::ToOpenMode(FileMode mode, FileAccess access)
    {
        std::ios::openmode openMode = std::ios::binary;

        if (access == FileAccess::Read)
        {
            openMode |= std::ios::in;
        }
        else if (access == FileAccess::Write)
        {
            openMode |= std::ios::out;
        }
        else
        {
            openMode |= std::ios::in | std::ios::out;
        }

        if (mode == FileMode::Create || mode == FileMode::CreateNew || mode == FileMode::Truncate)
        {
            openMode |= std::ios::trunc;

            if (access == FileAccess::Read)
            {
                openMode |= std::ios::out;
            }
        }
        else if (mode == FileMode::Append)
        {
            openMode &= ~std::ios::trunc;
            openMode |= std::ios::app | std::ios::out;
        }

        return openMode;
    }
}
