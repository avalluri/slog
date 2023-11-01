/* Copyright (C) 2023 Amarnath Valluri - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: , or visit:
 * https://opensource.org/license/MIT/
 */
#ifndef __SLOG_UTILS_H_
#define __SLOG_UTILS_H_

#include <string>

namespace slog {
namespace utils {
// MAYBE_UNUSED could be used to avoid warnings
// from compilers for unused function arguments
// and local variables.
// NOTE: Avoid using in release code.
#define MAYBE_UNUSED(arg) (void)(arg)

// directory_separaotr defines folder separation 
// character string
#if defined(WIN32) || defined(_WIN32)
#define directory_separator "\\"
#else
#define directory_separator "/"
#endif


// file_exists Checks if the given path is a regular file
// It does not follow symlinks.
bool file_exists(const std::string& path);

// directory_exists Checks if the given path is a directory.
// do not follow symlinks.
bool directory_exists(const std::string &path);

// is_symlink checks if the given location is a symbolic link
bool is_symlink(const std::string &path);

// dirname returns the parent directory of the given
// path:
// ex(on Linux):-
//   dirname("/a/b/c/") => "/a/b/c"
//   dirname("/a/b/c") => "/a/b"
//   dirname("abc") => ""
std::string dirname(const std::string &path);

bool create_directory(const std::string& dir);

// create_directory_path creates the given directory and all
// its parent directories in the path.
bool create_directory_path(const std::string& dir);

// ensure_directory_path make sure that the given directory path
// exists, if not it creates. Returns false in case the directory
// is path empty or it fails to validate/create.
//
// It raises an exception if the directory path holds a symlink
bool ensure_directory_path(const std::string& dir);

} // namespace utils
} // namespace slog

#endif // __SLOG_UTILS_H_
