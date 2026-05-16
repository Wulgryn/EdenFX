#include "system/linux/linux_application_finder.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

namespace Eden::System
{
    namespace fs = std::filesystem;

    namespace
    {
        static std::string ToLower(std::string value)
        {
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
            {
                return static_cast<char>(std::tolower(c));
            });

            return value;
        }

        static std::string Trim(const std::string& value)
        {
            std::size_t start = 0;
            while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
            {
                start++;
            }

            std::size_t end = value.size();
            while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
            {
                end--;
            }

            return value.substr(start, end - start);
        }

        static std::string NormalizeForCompare(const std::string& value)
        {
            std::string result;
            result.reserve(value.size());

            for (char c : ToLower(Trim(value)))
            {
                if (std::isalnum(static_cast<unsigned char>(c)))
                {
                    result.push_back(c);
                }
            }

            return result;
        }

        static bool Contains(const std::string& value, const std::string& part)
        {
            return value.find(part) != std::string::npos;
        }

        static bool SafeExists(const fs::path& path)
        {
            std::error_code error;
            return fs::exists(path, error);
        }

        static bool SafeIsDirectory(const fs::path& path)
        {
            std::error_code error;
            return fs::is_directory(path, error);
        }

        static bool SafeIsRegularFile(const fs::path& path)
        {
            std::error_code error;
            return fs::is_regular_file(path, error);
        }

        static std::string PathKey(const fs::path& path)
        {
            std::error_code error;
            fs::path absolute = fs::absolute(path, error);
            if (error)
            {
                absolute = path;
            }

            return ToLower(absolute.lexically_normal().generic_string());
        }

        static void AddPathOnce(Collections::List<Eden::IO::Path>& result, std::set<std::string>& added, const fs::path& path)
        {
            if (path.empty() || !SafeExists(path))
            {
                return;
            }

            std::string key = PathKey(path);
            if (added.insert(key).second)
            {
                result.add(Eden::IO::Path(path));
            }
        }

        static std::vector<std::string> SplitPathLikeList(const std::string& value, char separator)
        {
            std::vector<std::string> result;
            std::string current;

            for (char c : value)
            {
                if (c == separator)
                {
                    if (!current.empty())
                    {
                        result.push_back(current);
                    }

                    current.clear();
                }
                else
                {
                    current.push_back(c);
                }
            }

            if (!current.empty())
            {
                result.push_back(current);
            }

            return result;
        }

        static bool HasExecutablePermission(const fs::path& path)
        {
            std::error_code error;
            fs::file_status status = fs::status(path, error);
            if (error)
            {
                return false;
            }

            fs::perms permissions = status.permissions();
            return (permissions & fs::perms::owner_exec) != fs::perms::none ||
                   (permissions & fs::perms::group_exec) != fs::perms::none ||
                   (permissions & fs::perms::others_exec) != fs::perms::none;
        }

        static bool IsLikelyExecutableFile(const fs::path& path)
        {
            if (!SafeIsRegularFile(path))
            {
                return false;
            }

            if (HasExecutablePermission(path))
            {
                return true;
            }

            std::string extension = ToLower(path.extension().string());
            return extension == ".appimage" || extension == ".sh" || extension == ".run";
        }

        static std::vector<fs::path> DirectExecutableNames(const Eden::Text::String& applicationName)
        {
            std::vector<fs::path> names;
            std::string query = Trim(applicationName.to_std_string());

            if (query.empty())
            {
                return names;
            }

            names.push_back(fs::path(query));

            fs::path queryPath(query);
            if (!queryPath.has_extension())
            {
                names.push_back(fs::path(query + ".AppImage"));
                names.push_back(fs::path(query + ".sh"));
                names.push_back(fs::path(query + ".run"));
            }

            return names;
        }

        static bool NameMatches(const Eden::Text::String& applicationName, const fs::path& path, const std::string& displayName, const ApplicationSearchOptions& options)
        {
            std::string rawQuery = Trim(applicationName.to_std_string());
            std::string query = options.caseInsensitive ? ToLower(rawQuery) : rawQuery;
            std::string normalizedQuery = NormalizeForCompare(rawQuery);

            if (query.empty() || normalizedQuery.empty())
            {
                return false;
            }

            std::vector<std::string> candidates;
            candidates.push_back(path.filename().string());
            candidates.push_back(path.stem().string());
            candidates.push_back(path.parent_path().filename().string());

            if (!displayName.empty())
            {
                candidates.push_back(displayName);
            }

            for (std::string candidate : candidates)
            {
                std::string compared = options.caseInsensitive ? ToLower(candidate) : candidate;
                std::string normalizedCandidate = NormalizeForCompare(candidate);

                if (compared == query || normalizedCandidate == normalizedQuery)
                {
                    return true;
                }

                if (options.allowContainsMatch && normalizedCandidate.size() >= normalizedQuery.size())
                {
                    if (Contains(normalizedCandidate, normalizedQuery))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        static void AddApplicationInfoOnce(
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& applicationName,
            const fs::path& path,
            const Eden::Text::String& source,
            bool isDirectory,
            const Eden::Text::String& displayName = "")
        {
            if (path.empty() || !SafeExists(path))
            {
                return;
            }

            std::string key = PathKey(path);
            if (!added.insert(key).second)
            {
                return;
            }

            Eden::Text::String name = displayName.empty()
                ? Eden::Text::String(path.filename().string())
                : displayName;

            result.add(ApplicationInfo(true, applicationName, Eden::IO::Path(path), source, isDirectory, name));
        }

        static void TryDirectNamesInDirectory(
            const fs::path& directory,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& source)
        {
            if (!SafeIsDirectory(directory))
            {
                return;
            }

            for (const fs::path& name : DirectExecutableNames(applicationName))
            {
                fs::path file = directory / name;
                if (options.includeFiles && IsLikelyExecutableFile(file))
                {
                    AddApplicationInfoOnce(result, added, applicationName, file, source, false);
                }
            }
        }

        static void SearchDirectoryOneLevel(
            const fs::path& directory,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& source)
        {
            if (!SafeIsDirectory(directory))
            {
                return;
            }

            TryDirectNamesInDirectory(directory, applicationName, options, result, added, source);

            std::error_code error;
            fs::directory_iterator iterator(directory, fs::directory_options::skip_permission_denied, error);
            fs::directory_iterator end;

            while (!error && iterator != end)
            {
                const fs::directory_entry& entry = *iterator;
                fs::path path = entry.path();

                std::error_code typeError;
                bool isDir = entry.is_directory(typeError);
                bool isFile = entry.is_regular_file(typeError);

                if (isFile && options.includeFiles && IsLikelyExecutableFile(path) && NameMatches(applicationName, path, "", options))
                {
                    AddApplicationInfoOnce(result, added, applicationName, path, source, false);
                }
                else if (isDir && options.includeDirectories && NameMatches(applicationName, path, "", options))
                {
                    AddApplicationInfoOnce(result, added, applicationName, path, source, true);
                }

                iterator.increment(error);
                if (error)
                {
                    error.clear();
                }
            }
        }

        static bool ShouldSkipDirectory(const fs::path& path)
        {
            std::string full = ToLower(path.lexically_normal().generic_string());
            std::string name = ToLower(path.filename().string());

            return full == "/proc" || full == "/sys" || full == "/dev" ||
                   full == "/run" || full == "/tmp" || full == "/var/cache" ||
                   name == ".cache" || name == "lost+found";
        }

        static void SearchDirectoryRecursive(
            const fs::path& root,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& source)
        {
            if (!SafeIsDirectory(root))
            {
                return;
            }

            SearchDirectoryOneLevel(root, applicationName, options, result, added, source);

            if (!options.deepDriveScan)
            {
                return;
            }

            std::error_code error;
            fs::recursive_directory_iterator iterator(root, fs::directory_options::skip_permission_denied, error);
            fs::recursive_directory_iterator end;

            while (!error && iterator != end)
            {
                const fs::directory_entry& entry = *iterator;
                fs::path path = entry.path();

                std::error_code typeError;
                bool isDir = entry.is_directory(typeError);
                bool isFile = entry.is_regular_file(typeError);

                if (isDir)
                {
                    if (ShouldSkipDirectory(path) || (options.maxDriveScanDepth >= 0 && iterator.depth() >= options.maxDriveScanDepth))
                    {
                        iterator.disable_recursion_pending();
                    }

                    if (options.includeDirectories && NameMatches(applicationName, path, "", options))
                    {
                        AddApplicationInfoOnce(result, added, applicationName, path, source, true);
                    }
                }
                else if (isFile && options.includeFiles && IsLikelyExecutableFile(path) && NameMatches(applicationName, path, "", options))
                {
                    AddApplicationInfoOnce(result, added, applicationName, path, source, false);
                }

                iterator.increment(error);
                if (error)
                {
                    error.clear();
                }
            }
        }

        static std::string ReadDesktopEntryValue(const fs::path& desktopFile, const std::string& key)
        {
            std::ifstream file(desktopFile);
            if (!file.is_open())
            {
                return std::string();
            }

            std::string line;
            bool inDesktopEntry = false;

            while (std::getline(file, line))
            {
                line = Trim(line);
                if (line.empty() || line[0] == '#')
                {
                    continue;
                }

                if (line.size() >= 2 && line.front() == '[' && line.back() == ']')
                {
                    inDesktopEntry = line == "[Desktop Entry]";
                    continue;
                }

                if (!inDesktopEntry)
                {
                    continue;
                }

                std::string prefix = key + "=";
                if (line.rfind(prefix, 0) == 0)
                {
                    return line.substr(prefix.size());
                }
            }

            return std::string();
        }

        static std::string FirstExecToken(const std::string& exec)
        {
            std::string text = Trim(exec);
            if (text.empty())
            {
                return std::string();
            }

            std::string token;
            bool inQuote = false;

            for (std::size_t i = 0; i < text.size(); i++)
            {
                char c = text[i];

                if (c == '"')
                {
                    inQuote = !inQuote;
                    continue;
                }

                if (!inQuote && std::isspace(static_cast<unsigned char>(c)))
                {
                    break;
                }

                if (!inQuote && c == '%')
                {
                    break;
                }

                token.push_back(c);
            }

            return token;
        }

        static fs::path ResolveExecutableFromPath(const std::string& executableName)
        {
            if (executableName.empty())
            {
                return fs::path();
            }

            fs::path path(executableName);
            if (path.has_parent_path() && SafeExists(path))
            {
                return path;
            }

            const char* pathValue = std::getenv("PATH");
            if (!pathValue)
            {
                return fs::path();
            }

            for (const std::string& directory : SplitPathLikeList(pathValue, ':'))
            {
                fs::path candidate = fs::path(directory) / executableName;
                if (SafeExists(candidate))
                {
                    return candidate;
                }
            }

            return fs::path();
        }

        static void SearchDesktopFiles(
            const fs::path& root,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added)
        {
            if (!SafeIsDirectory(root))
            {
                return;
            }

            std::error_code error;
            fs::recursive_directory_iterator iterator(root, fs::directory_options::skip_permission_denied, error);
            fs::recursive_directory_iterator end;

            while (!error && iterator != end)
            {
                fs::path path = iterator->path();

                if (iterator->is_regular_file(error) && ToLower(path.extension().string()) == ".desktop")
                {
                    std::string displayName = ReadDesktopEntryValue(path, "Name");
                    std::string exec = ReadDesktopEntryValue(path, "Exec");

                    if (NameMatches(applicationName, path, displayName, options))
                    {
                        fs::path executable = ResolveExecutableFromPath(FirstExecToken(exec));
                        fs::path resultPath = executable.empty() ? path : executable;

                        AddApplicationInfoOnce(
                            result,
                            added,
                            applicationName,
                            resultPath,
                            "desktop_entry",
                            SafeIsDirectory(resultPath),
                            Eden::Text::String(displayName.empty() ? path.stem().string() : displayName));
                    }
                }

                iterator.increment(error);
                if (error)
                {
                    error.clear();
                }
            }
        }
    }

    Collections::List<ApplicationInfo> LinuxApplicationFinder::find_all(
        const Eden::Text::String& applicationName,
        const Collections::List<Eden::IO::Path>& extraSearchPaths,
        const ApplicationSearchOptions& options) const
    {
        Collections::List<ApplicationInfo> result;
        std::set<std::string> added;

        if (applicationName.trim().empty())
        {
            return result;
        }

        fs::path queryPath(applicationName.to_std_string());
        if ((queryPath.has_parent_path() || queryPath.is_absolute()) && SafeExists(queryPath))
        {
            AddApplicationInfoOnce(result, added, applicationName, queryPath, "direct_path", SafeIsDirectory(queryPath));
        }

        Collections::List<Eden::IO::Path> roots = get_search_roots(extraSearchPaths, options);

        for (const Eden::IO::Path& root : roots)
        {
            SearchDirectoryOneLevel(static_cast<fs::path>(root), applicationName, options, result, added, "search_root");
        }

        if (options.scanSystemLocations)
        {
            SearchDesktopFiles("/usr/share/applications", applicationName, options, result, added);
            SearchDesktopFiles("/usr/local/share/applications", applicationName, options, result, added);

            const char* home = std::getenv("HOME");
            if (home)
            {
                SearchDesktopFiles(fs::path(home) / ".local/share/applications", applicationName, options, result, added);
            }
        }

        if (options.scanAllDrives && options.deepDriveScan)
        {
            SearchDirectoryRecursive("/", applicationName, options, result, added, "drive_scan");

            // Ezek külön is bekerülnek, ha a / keresés valamiért nem tudta bejárni őket.
            SearchDirectoryRecursive("/mnt", applicationName, options, result, added, "drive_scan");
            SearchDirectoryRecursive("/media", applicationName, options, result, added, "drive_scan");
            SearchDirectoryRecursive("/run/media", applicationName, options, result, added, "drive_scan");
        }

        return result;
    }

    Collections::List<Eden::IO::Path> LinuxApplicationFinder::get_search_roots(
        const Collections::List<Eden::IO::Path>& extraSearchPaths,
        const ApplicationSearchOptions& options) const
    {
        Collections::List<Eden::IO::Path> result;
        std::set<std::string> added;

        for (const Eden::IO::Path& path : extraSearchPaths)
        {
            AddPathOnce(result, added, static_cast<fs::path>(path));
        }

        if (options.scanPath)
        {
            const char* pathValue = std::getenv("PATH");
            if (pathValue)
            {
                for (const std::string& item : SplitPathLikeList(pathValue, ':'))
                {
                    AddPathOnce(result, added, fs::path(item));
                }
            }
        }

        if (options.scanSystemLocations)
        {
            AddPathOnce(result, added, "/usr/bin");
            AddPathOnce(result, added, "/usr/local/bin");
            AddPathOnce(result, added, "/bin");
            AddPathOnce(result, added, "/sbin");
            AddPathOnce(result, added, "/usr/sbin");
            AddPathOnce(result, added, "/opt");
            AddPathOnce(result, added, "/snap/bin");
            AddPathOnce(result, added, "/var/lib/flatpak/exports/bin");

            const char* home = std::getenv("HOME");
            if (home)
            {
                AddPathOnce(result, added, fs::path(home) / ".local/bin");
                AddPathOnce(result, added, fs::path(home) / ".local/share/flatpak/exports/bin");
                AddPathOnce(result, added, fs::path(home) / "Applications");
            }
        }

        if (options.scanAllDrives)
        {
            AddPathOnce(result, added, "/");
            AddPathOnce(result, added, "/mnt");
            AddPathOnce(result, added, "/media");
            AddPathOnce(result, added, "/run/media");
        }

        return result;
    }
}
