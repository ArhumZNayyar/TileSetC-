#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "resource.h"
#include <filesystem>
#include <vector>
#include <string>

class Tileset {
    void CreateTileset(const std::string& folderPath, int tileWidth, int tileHeight) {
        std::vector<cv::Mat> images;

        if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath)) {
            std::cerr << "Directory does not exist or is not accessible: " << folderPath << std::endl;
            return;
        }

        try {
            // Iterate through the directory and load images.
            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.path().extension() == ".png") {
                    cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_COLOR);
                    if (!img.empty()) {
                        images.push_back(img);
                    }
                    else {
                        std::cerr << "Failed to load image: " << entry.path() << std::endl;
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return;
        }

        // Calculate the dimensions of the Tileset.
        int tilesPerRow = 12;
        int tilesPerCol = 12;
        int tilesetWidth = tilesPerRow * tileWidth;
        int tilesetHeight = tilesPerCol * tileHeight;

        cv::Mat tileset(tilesetHeight, tilesetWidth, CV_8UC3, cv::Scalar(0, 0, 0));

        // Place each image into the Tileset.
        for (size_t i = 0; i < images.size(); ++i) {
            int row = i / tilesPerRow;
            int col = i % tilesPerRow;
            cv::Rect roi(col * tileWidth, row * tileHeight, tileWidth, tileHeight);
            cv::Mat targetROI = tileset(roi);
            cv::resize(images[i], targetROI, targetROI.size());
        }
        cv::imwrite("tileset.png", tileset);
    }
};


