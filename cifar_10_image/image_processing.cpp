#include <iostream>
#include <fstream>
#include <vector>

const int IMAGE_SIZE = 32 * 32 * 3;  // 3072 octets par image
const int LABEL_SIZE = 1;
const int RECORD_SIZE = LABEL_SIZE + IMAGE_SIZE;
const int NUM_IMAGES = 10000;

struct CIFARImage {
    unsigned char label;
    std::vector<unsigned char> pixels; // Stocke les pixels de l'image

    CIFARImage() : pixels(IMAGE_SIZE) {} // Initialisation
};

// Fonction pour charger un fichier CIFAR-10
std::vector<CIFARImage> load_cifar10_file(const std::string& filename) {
    std::vector<CIFARImage> images(NUM_IMAGES);
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << filename << std::endl;
        return {};
    }

    for (int i = 0; i < NUM_IMAGES; ++i) {
        file.read(reinterpret_cast<char*>(&images[i].label), LABEL_SIZE);
        file.read(reinterpret_cast<char*>(images[i].pixels.data()), IMAGE_SIZE);
    }

    file.close();
    return images;
}

// int main() {
//     std::string filename = "data_batch_1.bin"; // Remplacez par le chemin réel
//     std::vector<CIFARImage> images = load_cifar10_file(filename);

//     if (!images.empty()) {
//         std::cout << "Chargé " << images.size() << " images depuis " << filename << std::endl;
//         std::cout << "Label de la première image : " << static_cast<int>(images[0].label) << std::endl;
//     }

//     return 0;
// }
