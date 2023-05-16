//
// Created by kehat on 5/16/23.
//
#include "../include/graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>


std::map<int, Vertex> getMap() {
    std::map<int, Vertex> data;

    std::ifstream file("/home/kehat/Desktop/CoordiBot/bitmaps/vertices.csv");
    if (!file.is_open()) {
        std::cout << "Failed to open the CSV file." << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line); // Read and discard the header line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        // Read the cells of the current line
        std::vector<std::string> cells;
        while (std::getline(ss, cell, ',')) {
            cells.push_back(cell);
        }

            double id = std::stod(cells[0]);
            double x = std::stod(cells[1]);
            double y = std::stod(cells[2]);

            Vertex vertex(x,y);
            // Assign additional fields as needed

            data[id] = vertex;

    }

    file.close();

    return data;
}






