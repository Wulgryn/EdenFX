#include "io/directory.hpp"

namespace Eden::IO
{
    Directory::Directory() = default;

    Directory::Directory(const char* path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    Directory::Directory(const std::string& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    Directory::Directory(const std::wstring& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    Directory::Directory(const Eden::Text::String& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    Directory::Directory(const Eden::IO::Path& path)
    {
        this->data()->m_Path = path;
    }

    Directory::Directory(const fs::path& path)
    {
        this->data()->m_Path = Eden::IO::Path(path);
    }

    // =====================================================
    // Nem statikus DirectoryInfo-szerű rész
    // =====================================================

    const Eden::IO::Path& Directory::path() const
    {
        return this->data()->m_Path;
    }

    Eden::IO::Path& Directory::path()
    {
        return this->data()->m_Path;
    }

    Eden::Text::String Directory::name() const
    {
        return this->data()->m_Path.filename().to_string();
    }

    Eden::IO::Path Directory::parent() const
    {
        return this->data()->m_Path.parent_path();
    }

    Eden::IO::Path Directory::full_name() const
    {
        return this->data()->m_Path;
    }

    bool Directory::exists() const
    {
        return Directory::Exists(this->data()->m_Path);
    }

    bool Directory::is_empty() const
    {
        return Directory::IsEmpty(this->data()->m_Path);
    }

    void Directory::create() const
    {
        Directory::Create(this->data()->m_Path);
    }

    void Directory::Delete(bool recursive) const
    {
        Directory::Delete(this->data()->m_Path, recursive);
    }

    void Directory::move_to(const Eden::IO::Path& target)
    {
        Directory::Move(this->data()->m_Path, target);
        this->data()->m_Path = target;
    }

    void Directory::copy_to(const Eden::IO::Path& target, bool overwrite) const
    {
        Directory::Copy(this->data()->m_Path, target, overwrite);
    }

    Collections::List<Eden::IO::Path> Directory::get_files(bool recursive, const Eden::Text::String& searchPattern) const
    {
        return Directory::GetFiles(this->data()->m_Path, recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::get_directories(bool recursive, const Eden::Text::String& searchPattern) const
    {
        return Directory::GetDirectories(this->data()->m_Path, recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::get_entries(bool recursive, const Eden::Text::String& searchPattern) const
    {
        return Directory::GetEntries(this->data()->m_Path, recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::enumerate_files(bool recursive, const Eden::Text::String& searchPattern) const
    {
        return get_files(recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::enumerate_directories(bool recursive, const Eden::Text::String& searchPattern) const
    {
        return get_directories(recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::enumerate_entries(bool recursive, const Eden::Text::String& searchPattern) const
    {
        return get_entries(recursive, searchPattern);
    }

    fs::file_time_type Directory::last_write_time() const
    {
        return Directory::GetLastWriteTime(this->data()->m_Path);
    }

    void Directory::set_last_write_time(fs::file_time_type time) const
    {
        Directory::SetLastWriteTime(this->data()->m_Path, time);
    }

    // =====================================================
    // Statikus C# Directory-szerű rész
    // =====================================================

    bool Directory::Exists(const Eden::IO::Path& path)
    {
        return fs::exists(path) && fs::is_directory(path);
    }

    bool Directory::IsEmpty(const Eden::IO::Path& path)
    {
        if (!Exists(path))
        {
            return false;
        }

        return fs::is_empty(path);
    }

    Directory Directory::Create(const Eden::IO::Path& path)
    {
        fs::create_directories(path);
        return Directory(path);
    }

    Directory Directory::CreateDirectory(const Eden::IO::Path& path)
    {
        return Create(path);
    }

    void Directory::Delete(const Eden::IO::Path& path, bool recursive)
    {
        if (!Exists(path))
        {
            return;
        }

        if (recursive)
        {
            fs::remove_all(path);
        }
        else
        {
            fs::remove(path);
        }
    }

    void Directory::Move(const Eden::IO::Path& source, const Eden::IO::Path& target)
    {
        fs::rename(source, target);
    }

    void Directory::Copy(const Eden::IO::Path& source, const Eden::IO::Path& target, bool overwrite)
    {
        fs::copy_options options = fs::copy_options::recursive;

        if (overwrite)
        {
            options |= fs::copy_options::overwrite_existing;
        }
        else
        {
            options |= fs::copy_options::skip_existing;
        }

        fs::copy(source, target, options);
    }

    Collections::List<Eden::IO::Path> Directory::GetFiles(const Eden::IO::Path& path, bool recursive, const Eden::Text::String& searchPattern)
    {
        Collections::List<Eden::IO::Path> result;

        if (!Exists(path))
        {
            return result;
        }

        fs::directory_options options = fs::directory_options::skip_permission_denied;

        if (recursive)
        {
            for (const auto& entry : fs::recursive_directory_iterator(path, options))
            {
                if (entry.is_regular_file() && WildcardMatch(searchPattern, Eden::Text::String(entry.path().filename().string())))
                {
                    result.add(Eden::IO::Path(entry.path()));
                }
            }
        }
        else
        {
            for (const auto& entry : fs::directory_iterator(path, options))
            {
                if (entry.is_regular_file() && WildcardMatch(searchPattern, Eden::Text::String(entry.path().filename().string())))
                {
                    result.add(Eden::IO::Path(entry.path()));
                }
            }
        }

        return result;
    }

    Collections::List<Eden::IO::Path> Directory::GetDirectories(const Eden::IO::Path& path, bool recursive, const Eden::Text::String& searchPattern)
    {
        Collections::List<Eden::IO::Path> result;

        if (!Exists(path))
        {
            return result;
        }

        fs::directory_options options = fs::directory_options::skip_permission_denied;

        if (recursive)
        {
            for (const auto& entry : fs::recursive_directory_iterator(path, options))
            {
                if (entry.is_directory() && WildcardMatch(searchPattern, Eden::Text::String(entry.path().filename().string())))
                {
                    result.add(Eden::IO::Path(entry.path()));
                }
            }
        }
        else
        {
            for (const auto& entry : fs::directory_iterator(path, options))
            {
                if (entry.is_directory() && WildcardMatch(searchPattern, Eden::Text::String(entry.path().filename().string())))
                {
                    result.add(Eden::IO::Path(entry.path()));
                }
            }
        }

        return result;
    }

    Collections::List<Eden::IO::Path> Directory::GetEntries(const Eden::IO::Path& path, bool recursive, const Eden::Text::String& searchPattern)
    {
        Collections::List<Eden::IO::Path> result;

        if (!Exists(path))
        {
            return result;
        }

        fs::directory_options options = fs::directory_options::skip_permission_denied;

        if (recursive)
        {
            for (const auto& entry : fs::recursive_directory_iterator(path, options))
            {
                if (WildcardMatch(searchPattern, Eden::Text::String(entry.path().filename().string())))
                {
                    result.add(Eden::IO::Path(entry.path()));
                }
            }
        }
        else
        {
            for (const auto& entry : fs::directory_iterator(path, options))
            {
                if (WildcardMatch(searchPattern, Eden::Text::String(entry.path().filename().string())))
                {
                    result.add(Eden::IO::Path(entry.path()));
                }
            }
        }

        return result;
    }

    Collections::List<Eden::IO::Path> Directory::EnumerateFiles(const Eden::IO::Path& path, bool recursive, const Eden::Text::String& searchPattern)
    {
        return GetFiles(path, recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::EnumerateDirectories(const Eden::IO::Path& path, bool recursive, const Eden::Text::String& searchPattern)
    {
        return GetDirectories(path, recursive, searchPattern);
    }

    Collections::List<Eden::IO::Path> Directory::EnumerateEntries(const Eden::IO::Path& path, bool recursive, const Eden::Text::String& searchPattern)
    {
        return GetEntries(path, recursive, searchPattern);
    }

    Eden::IO::Path Directory::GetCurrentDirectory()
    {
        return Eden::IO::Path(fs::current_path());
    }

    void Directory::SetCurrentDirectory(const Eden::IO::Path& path)
    {
        fs::current_path(path);
    }

    Eden::IO::Path Directory::GetParent(const Eden::IO::Path& path)
    {
        return path.directory_name();
    }

    fs::file_time_type Directory::GetLastWriteTime(const Eden::IO::Path& path)
    {
        return fs::last_write_time(path);
    }

    void Directory::SetLastWriteTime(const Eden::IO::Path& path, fs::file_time_type time)
    {
        fs::last_write_time(path, time);
    }

    bool Directory::WildcardMatch(const Eden::Text::String& pattern, const Eden::Text::String& value)
    {
        std::string p = pattern.empty() ? std::string("*") : pattern.to_string();
        const std::string& v = value.to_string();

        std::size_t pIndex = 0;
        std::size_t vIndex = 0;
        std::size_t starIndex = std::string::npos;
        std::size_t matchIndex = 0;

        while (vIndex < v.size())
        {
            if (pIndex < p.size() && (p[pIndex] == '?' || p[pIndex] == v[vIndex]))
            {
                pIndex++;
                vIndex++;
            }
            else if (pIndex < p.size() && p[pIndex] == '*')
            {
                starIndex = pIndex;
                matchIndex = vIndex;
                pIndex++;
            }
            else if (starIndex != std::string::npos)
            {
                pIndex = starIndex + 1;
                matchIndex++;
                vIndex = matchIndex;
            }
            else
            {
                return false;
            }
        }

        while (pIndex < p.size() && p[pIndex] == '*')
        {
            pIndex++;
        }

        return pIndex == p.size();
    }
}
