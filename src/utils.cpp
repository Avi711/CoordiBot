//
// Created by kehat on 5/16/23.
//
#include "../include/graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <filesystem>


std::map<int, Vertex *> *generateMap() {
    auto *data = new std::map<int, Vertex *>;
    std::filesystem::path relativePath = "bitmaps/vertices.csv";
    std::ifstream file(std::filesystem::current_path() / relativePath);
    if (!file.is_open()) {
        std::cout << "Failed to open the CSV file." << std::endl;
        return data;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        // Read the cells of the current line
        std::vector<std::string> cells;
        while (std::getline(ss, cell, ',')) {
            cells.push_back(cell);
        }

        int id = std::stod(cells[0]);
        double x = std::stod(cells[1]);
        double y = std::stod(cells[2]);

        Vertex *vertex = new Vertex(id, x, y);
        for (int i = 4; i < cells.size(); i++) {
            if (cells[i] != "") {
                vertex->addNeighbor(stoi(cells[i]));
            }
        }
        (*data)[id] = vertex;
    }
    file.close();
    return data;
}





