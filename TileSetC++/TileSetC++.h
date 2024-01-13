#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "resource.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

class Tileset {
public:
    void CreateTileset(const std::string& folderPath, const std::string& fileName, int tileWidth, int tileHeight, bool transparent) {
        std::vector<cv::Mat> images;
        if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath)) {
            std::cerr << "Directory does not exist or is not accessible: " << folderPath << std::endl;
            return;
        }

        try {
            // Iterate through the directory and load images with alpha channel.
            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.path().extension() == ".png") {
                    cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED);
                    if (!img.empty()) {
                        if (img.channels() < 4 || transparent) {
                            cv::cvtColor(img, img, cv::COLOR_BGR2BGRA);
                        }
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
        cv::Mat tileset(tilesetHeight, tilesetWidth, CV_8UC4, cv::Scalar(0, 0, 0, 0));

        std::cout << "Transparent(0/1): " << transparent << std::endl;

        // Place each image into the Tileset.
        std::cout << "Image matrix size: " << images.size() << std::endl;
        std::cout << "Building image..." << std::endl;
        for (size_t i = 0; i < images.size(); ++i) {
            int row = i / tilesPerRow;
            int col = i % tilesPerRow;
            cv::Rect roi(col * tileWidth, row * tileHeight, tileWidth, tileHeight);
            cv::Mat targetROI = tileset(roi);

            // Resize and copy images.
            cv::Mat resized;
            cv::resize(images[i], resized, targetROI.size());
            if (transparent || images[i].channels() == 4) {
                // Use alpha channel for copying.
                resized.copyTo(targetROI, resized);
            }
            else {
                // Copy without alpha channel.
                cv::cvtColor(resized, resized, cv::COLOR_BGRA2BGR);
                resized.copyTo(targetROI);
            }
        }

        cv::imwrite(fileName + ".png", tileset);
        std::cout << "Done!" << std::endl;
        std::cout << "Output: " << fileName + ".png" << std::endl;
    }
};


