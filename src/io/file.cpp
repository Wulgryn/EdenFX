#include "io/file.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Eden::IO
{
    File::File() = default;

    File::File(const char* path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    File::File(const std::string& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    File::File(const std::wstring& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    File::File(const Eden::Text::String& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    File::File(const Eden::IO::Path& path)
    {
        this->data()->m_Path = path;
    }

    File::File(const fs::path& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    const Eden::IO::Path& File::path() const
    {
        return this->data()->m_Path;
    }

    Eden::IO::Path& File::path()
    {
        return this->data()->m_Path;
    }

    Eden::Text::String File::name() const
    {
        return this->data()->m_Path.filename().to_string();
    }

    Eden::Text::String File::extension() const
    {
        return this->data()->m_Path.extension().to_string();
    }

    Eden::IO::Path File::directory() const
    {
        return this->data()->m_Path.parent_path();
    }

    Eden::IO::Path File::full_name() const
    {
        return this->data()->m_Path.full_path();
    }

    bool File::exists() const
    {
        return File::Exists(this->data()->m_Path);
    }

    long long File::length() const
    {
        return File::Length(this->data()->m_Path);
    }

    FileStream File::create() const
    {
        return File::Create(this->data()->m_Path);
    }

    FileStream File::open(FileMode mode, FileAccess access) const
    {
        return File::Open(this->data()->m_Path, mode, access);
    }

    FileStream File::open_read() const
    {
        return File::OpenRead(this->data()->m_Path);
    }

    FileStream File::open_write(bool append) const
    {
        return File::OpenWrite(this->data()->m_Path, append);
    }

    Eden::Text::String File::read_all_text() const
    {
        return File::ReadAllText(this->data()->m_Path);
    }

    Collections::List<Eden::Text::String> File::read_all_lines() const
    {
        return File::ReadAllLines(this->data()->m_Path);
    }

    Collections::List<File::Byte> File::read_all_bytes() const
    {
        return File::ReadAllBytes(this->data()->m_Path);
    }

    void File::write_all_text(const Eden::Text::String& text) const
    {
        File::WriteAllText(this->data()->m_Path, text);
    }

    void File::append_all_text(const Eden::Text::String& text) const
    {
        File::AppendAllText(this->data()->m_Path, text);
    }

    void File::write_all_lines(const Collections::List<Eden::Text::String>& lines) const
    {
        File::WriteAllLines(this->data()->m_Path, lines);
    }

    void File::append_all_lines(const Collections::List<Eden::Text::String>& lines) const
    {
        File::AppendAllLines(this->data()->m_Path, lines);
    }

    void File::write_all_bytes(const Collections::List<Byte>& bytes) const
    {
        File::WriteAllBytes(this->data()->m_Path, bytes);
    }

    void File::Delete() const
    {
        File::Delete(this->data()->m_Path);
    }

    void File::copy_to(const Eden::IO::Path& destination, bool overwrite) const
    {
        File::Copy(this->data()->m_Path, destination, overwrite);
    }

    void File::move_to(const Eden::IO::Path& destination, bool overwrite)
    {
        File::Move(this->data()->m_Path, destination, overwrite);
        this->data()->m_Path = destination;
    }

    fs::file_time_type File::last_write_time() const
    {
        return File::GetLastWriteTime(this->data()->m_Path);
    }

    void File::set_last_write_time(fs::file_time_type time) const
    {
        File::SetLastWriteTime(this->data()->m_Path, time);
    }

    bool File::Exists(const Eden::IO::Path& path)
    {
        fs::path nativePath = static_cast<fs::path>(path);
        return fs::exists(nativePath) && fs::is_regular_file(nativePath);
    }

    long long File::Length(const Eden::IO::Path& path)
    {
        if (!Exists(path))
        {
            return 0;
        }

        return static_cast<long long>(fs::file_size(static_cast<fs::path>(path)));
    }

    FileStream File::Create(const Eden::IO::Path& path)
    {
        return FileStream(path, FileMode::Create, FileAccess::ReadWrite);
    }

    FileStream File::Open(const Eden::IO::Path& path, FileMode mode, FileAccess access)
    {
        return FileStream(path, mode, access);
    }

    FileStream File::OpenRead(const Eden::IO::Path& path)
    {
        return FileStream(path, FileMode::Open, FileAccess::Read);
    }

    FileStream File::OpenWrite(const Eden::IO::Path& path, bool append)
    {
        return FileStream(path, append ? FileMode::Append : FileMode::Create, FileAccess::Write);
    }

    Eden::Text::String File::ReadAllText(const Eden::IO::Path& path)
    {
        std::ifstream file(static_cast<fs::path>(path), std::ios::binary);

        if (!file.is_open())
        {
            return Eden::Text::String();
        }

        std::ostringstream builder;
        builder << file.rdbuf();
        return Eden::Text::String(builder.str());
    }

    Collections::List<Eden::Text::String> File::ReadAllLines(const Eden::IO::Path& path)
    {
        Collections::List<Eden::Text::String> result;
        std::ifstream file(static_cast<fs::path>(path));

        if (!file.is_open())
        {
            return result;
        }

        std::string line;

        while (std::getline(file, line))
        {
            result.add(Eden::Text::String(line));
        }

        return result;
    }

    Collections::List<File::Byte> File::ReadAllBytes(const Eden::IO::Path& path)
    {
        Collections::List<Byte> result;
        std::ifstream file(static_cast<fs::path>(path), std::ios::binary);

        if (!file.is_open())
        {
            return result;
        }

        char current = 0;

        while (file.get(current))
        {
            result.add(static_cast<Byte>(current));
        }

        return result;
    }

    void File::WriteAllText(const Eden::IO::Path& path, const Eden::Text::String& text)
    {
        std::ofstream file(static_cast<fs::path>(path), std::ios::binary | std::ios::trunc);
        file << text.std();
    }

    void File::AppendAllText(const Eden::IO::Path& path, const Eden::Text::String& text)
    {
        std::ofstream file(static_cast<fs::path>(path), std::ios::binary | std::ios::app);
        file << text.std();
    }

    void File::WriteAllLines(const Eden::IO::Path& path, const Collections::List<Eden::Text::String>& lines)
    {
        std::ofstream file(static_cast<fs::path>(path), std::ios::binary | std::ios::trunc);

        for (int i = 0; i < lines.count(); i++)
        {
            file << lines[i].std();

            if (i + 1 < lines.count())
            {
                file << '\n';
            }
        }
    }

    void File::AppendAllLines(const Eden::IO::Path& path, const Collections::List<Eden::Text::String>& lines)
    {
        std::ofstream file(static_cast<fs::path>(path), std::ios::binary | std::ios::app);

        for (int i = 0; i < lines.count(); i++)
        {
            file << lines[i].std() << '\n';
        }
    }

    void File::WriteAllBytes(const Eden::IO::Path& path, const Collections::List<Byte>& bytes)
    {
        std::ofstream file(static_cast<fs::path>(path), std::ios::binary | std::ios::trunc);

        for (int i = 0; i < bytes.count(); i++)
        {
            file.put(static_cast<char>(bytes[i]));
        }
    }

    void File::Delete(const Eden::IO::Path& path)
    {
        if (fs::exists(static_cast<fs::path>(path)))
        {
            fs::remove(static_cast<fs::path>(path));
        }
    }

    void File::Copy(const Eden::IO::Path& source, const Eden::IO::Path& destination, bool overwrite)
    {
        fs::copy_options options = overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::none;
        fs::copy_file(static_cast<fs::path>(source), static_cast<fs::path>(destination), options);
    }

    void File::Move(const Eden::IO::Path& source, const Eden::IO::Path& destination, bool overwrite)
    {
        if (overwrite && fs::exists(static_cast<fs::path>(destination)))
        {
            fs::remove(static_cast<fs::path>(destination));
        }

        fs::rename(static_cast<fs::path>(source), static_cast<fs::path>(destination));
    }

    void File::Replace(const Eden::IO::Path& sourceFileName, const Eden::IO::Path& destinationFileName, const Eden::IO::Path& destinationBackupFileName)
    {
        if (fs::exists(static_cast<fs::path>(destinationFileName)))
        {
            fs::copy_file(static_cast<fs::path>(destinationFileName), static_cast<fs::path>(destinationBackupFileName), fs::copy_options::overwrite_existing);
        }

        File::Move(sourceFileName, destinationFileName, true);
    }

    fs::file_time_type File::GetLastWriteTime(const Eden::IO::Path& path)
    {
        return fs::last_write_time(static_cast<fs::path>(path));
    }

    void File::SetLastWriteTime(const Eden::IO::Path& path, fs::file_time_type time)
    {
        fs::last_write_time(static_cast<fs::path>(path), time);
    }
}
