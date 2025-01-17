#pragma once
#include "common.hpp"
#include "Mesh.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
namespace fs = std::filesystem;
class ObjMesh : protected Mesh {
	ObjMesh(fs::path objFilePath){
		position = readObjFile(objFilePath);
	}
	//頂点座標のみ読み込む 
	
	std::vector<glm::vec3> readObjFile(fs::path path) {
		std::ifstream file(path);
		if (file.fail()) {
			std::cerr << "Failed to open file" << std::endl;
			return;
		}
		std::string line;
		std::vector<glm::vec3> ret;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string type;
			iss >> type;
			if (type == "v") {
				glm::float32 x, y, z;
				iss >> x >> y >> z;
				ret.push_back({ x,y,z });
			}
		}
		return ret;
	}
	
};