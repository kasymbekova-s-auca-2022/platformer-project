#ifndef RLE_LOADER_H
#define RLE_LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

struct RLELevel {
    size_t rows;
    size_t cols;
    std::vector<char> data;
};

inline std::vector<RLELevel> load_rle_levels(const std::string& path) {
    std::vector<RLELevel> levels;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open RLE file: " << path << std::endl;
        return levels;
    }

    std::vector<std::string> level_lines;
    std::string line;

    auto process_level_lines = [&levels](const std::vector<std::string>& level_lines) {
        std::vector<std::string> rows;
        for (const std::string& l : level_lines) {
            std::stringstream ss(l);
            std::string segment;
            while (std::getline(ss, segment, '|')) {
                rows.push_back(segment);
            }
        }

        size_t cols = 0;
        std::vector<char> data;
        for (const auto& rle_row : rows) {
            std::vector<char> row;
            for (size_t i = 0; i < rle_row.size();) {
                size_t count = 0;
                while (i < rle_row.size() && std::isdigit(rle_row[i])) {
                    count = count * 10 + (rle_row[i] - '0');
                    ++i;
                }
                if (i < rle_row.size()) {
                    char ch = rle_row[i++];
                    if (count == 0) count = 1;  // <- фикс: если не указано число, по умолчанию 1
                    row.insert(row.end(), count, ch);
                }
            }

            if (row.size() > cols) cols = row.size();
            data.insert(data.end(), row.begin(), row.end());
        }

        if (!data.empty()) {
            levels.push_back({rows.size(), cols, data});
        }
    };

    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty() || line[0] == ';') {
            if (!level_lines.empty()) {
                process_level_lines(level_lines);
                level_lines.clear();
            }
        } else {
            level_lines.push_back(line);
        }
    }

    if (!level_lines.empty()) {
        process_level_lines(level_lines);
    }

    return levels;
}

#endif
