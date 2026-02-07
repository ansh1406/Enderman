#pragma once

#include "enderman/body.hpp"
#include "enderman/types.hpp"
#include "enderman/response.hpp"
#include "enderman/request.hpp"

#include "enderman/standard_bodies/binary_body.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <cctype>
#include <unordered_map>

namespace
{
    const std::unordered_map<std::string, std::string> MIME_TYPES = {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".css", "text/css"},
        {".txt", "text/plain"},
        {".csv", "text/csv"},
        {".xml", "application/xml"},

        {".js", "application/javascript"},
        {".mjs", "application/javascript"},
        {".json", "application/json"},
        {".map", "application/json"},

        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".gif", "image/gif"},
        {".bmp", "image/bmp"},
        {".webp", "image/webp"},
        {".svg", "image/svg+xml"},
        {".ico", "image/x-icon"},
        {".tiff", "image/tiff"},
        {".tif", "image/tiff"},

        {".mp3", "audio/mpeg"},
        {".wav", "audio/wav"},
        {".ogg", "audio/ogg"},
        {".aac", "audio/aac"},
        {".flac", "audio/flac"},

        {".mp4", "video/mp4"},
        {".webm", "video/webm"},
        {".ogv", "video/ogg"},
        {".avi", "video/x-msvideo"},
        {".mov", "video/quicktime"},
        {".mkv", "video/x-matroska"},

        {".woff", "font/woff"},
        {".woff2", "font/woff2"},
        {".ttf", "font/ttf"},
        {".otf", "font/otf"},

        {".pdf", "application/pdf"},
        {".zip", "application/zip"},
        {".gz", "application/gzip"},
        {".tar", "application/x-tar"},
        {".rar", "application/vnd.rar"},
        {".7z", "application/x-7z-compressed"},

        {".wasm", "application/wasm"}};
}

namespace enderman
{
    MiddlewareFunction serve_static(const std::filesystem::path &base_path)
    {

        auto get_mime_type = [](const std::filesystem::path &file_path)
        {
            auto ext = file_path.extension().string();
            for (auto &c : ext)
                c = std::tolower(c);
            auto it = MIME_TYPES.find(ext);
            if (it != MIME_TYPES.end())
            {
                return it->second;
            }
            return "application/octet-stream";
        };

        return [get_mime_type, base_path](Request &req, Response &res, const Next &next)
        {
            try
            {
                std::filesystem::path file_path = base_path / req.relative_path();

                if (std::filesystem::exists(file_path))
                {
                    if (std::filesystem::is_regular_file(file_path))
                    {
                        std::ifstream file(file_path, std::ios::binary);
                        if (file)
                        {
                            std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                            std::shared_ptr<BinaryBody> body = std::make_shared<BinaryBody>();
                            body->move_data(std::move(data));
                            body->content_type = get_mime_type(file_path);
                            res.set_body(body).send();
                        }
                        else
                        {
                            next(std::make_exception_ptr(std::runtime_error("Failed to open file: " + file_path.string())));
                        }
                    }
                    else if (std::filesystem::is_directory(file_path))
                    {
                        std::filesystem::path index_file = file_path / "index.html";
                        if (std::filesystem::exists(index_file) && std::filesystem::is_regular_file(index_file))
                        {
                            std::ifstream file(index_file, std::ios::binary);
                            if (file)
                            {
                                std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                                std::shared_ptr<BinaryBody> body = std::make_shared<BinaryBody>();
                                body->move_data(std::move(data));
                                body->content_type = get_mime_type(index_file);
                                res.set_body(body).send();
                            }
                            else
                            {
                                next(std::make_exception_ptr(std::runtime_error("Failed to open file: " + index_file.string())));
                            }
                        }
                        else
                        {
                            next(nullptr);
                        }
                    }
                    else
                    {
                        next(nullptr);
                    }
                }
                else
                {
                    next(nullptr);
                }
            }
            catch (const std::exception &e)
            {
                next(std::make_exception_ptr(e));
            }
        };
    }
}