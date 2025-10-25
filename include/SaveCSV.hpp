#ifndef CSV_WRITER_HPP
#define CSV_WRITER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class CSVWriter {
  public:
    explicit CSVWriter(const std::string &filename = "file.csv")
        : m_filename(filename) {}

    void save(const std::vector<std::string> &header,
              const std::vector<std::vector<double>> &data) const {
        std::ofstream file(m_filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << m_filename << "\n";
            return;
        }

        for (size_t i = 0; i < header.size(); ++i) {
            file << header[i];
            if (i < header.size() - 1)
                file << ",";
        }
        file << "\n";

        for (const auto &row : data) {
            for (size_t j = 0; j < row.size(); ++j) {
                file << row[j];
                if (j < row.size() - 1)
                    file << ",";
            }
            file << "\n";
        }

        file.close();
        std::cout << "✅ CSV saved to " << m_filename << "\n";
    }

  private:
    std::string m_filename;
};

#endif
