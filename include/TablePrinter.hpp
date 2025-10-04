#ifndef TABLE_PRINTER_HPP
#define TABLE_PRINTER_HPP

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class TablePrinter {
  public:
    static void print(const std::vector<std::vector<std::string>> &table,
                      int colWidth = 12) {
        if (table.empty())
            return;

        printBorder(table[0].size(), colWidth);

        for (size_t i = 0; i < table.size(); i++) {
            std::cout << "|";
            for (auto &cell : table[i]) {
                std::cout << std::setw(colWidth) << std::left << cell << "|";
            }
            std::cout << "\n";

            printBorder(table[i].size(), colWidth);
        }
    }

  private:
    static void printBorder(size_t cols, int colWidth) {
        std::cout << "+";
        for (size_t i = 0; i < cols; i++) {
            std::cout << std::string(colWidth, '-') << "+";
        }
        std::cout << "\n";
    }
};

#endif // TABLE_PRINTER_HPP
