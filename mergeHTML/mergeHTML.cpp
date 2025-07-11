// mergeHTML.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

// Helper function: Read the contents of a file into a string
std::string readFile(const fs::path& path) {
    std::ifstream file(path, std::ios::in);
    if (!file) return "";

    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    return content;
}

// Extract <head>...</head> from the HTML string
std::string extractHead(const std::string& html) {
    auto startTag = html.find("<head");
    if (startTag == std::string::npos) return "";

    auto start = html.find(">", startTag);
    if (start == std::string::npos) return "";

    auto end = html.find("</head>", start);
    if (end == std::string::npos) return "";

    return "<head" + html.substr(startTag + 5, end - startTag - 5 + 7); // includes </head>
}

// Extract content inside <body>...</body> from the HTML string
std::string extractBodyContent(const std::string& html) {
    auto startTag = html.find("<body");
    if (startTag == std::string::npos) return "";

    auto start = html.find(">", startTag);
    if (start == std::string::npos) return "";

    auto end = html.find("</body>", start);
    if (end == std::string::npos) return "";

    return html.substr(start + 1, end - start - 1); // only content between <body> and </body>
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Drag and drop HTML files onto this program to merge them.\n";
        return 0;
    }

    std::vector<fs::path> files;
    for (int i = 1; i < argc; ++i) {
        fs::path p = argv[i];
        if (fs::exists(p) && p.extension() == ".html")
            files.push_back(p);
    }

    if (files.empty()) {
        std::cout << "No valid HTML files found.\n";
        return 1;
    }

    // Sort files alphabetically by filename
    std::sort(files.begin(), files.end());

    std::string combinedBody;
    std::string headContent;

    for (size_t i = 0; i < files.size(); ++i) {
        std::string html = readFile(files[i]);
        if (html.empty()) {
            std::cerr << "Error reading file: " << files[i] << "\n";
            continue;
        }

        if (i == 0) {
            headContent = extractHead(html); // Use head from the first file only
        }

        std::string bodyPart = extractBodyContent(html);
        combinedBody += bodyPart + "\n";
    }

    // Construct final HTML document
    std::string result = "<!DOCTYPE html>\n<html>\n";
    result += headContent.empty() ? "<head></head>\n" : headContent + "\n";
    result += "<body>\n" + combinedBody + "</body>\n</html>\n";

    std::ofstream out("merged_output.html");
    if (!out) {
        std::cerr << "Error writing output file.\n";
        return 1;
    }

    out << result;
    out.close();

    std::cout << "Done! 'merged_output.html' has been created.\n";
    return 0;
}


// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
