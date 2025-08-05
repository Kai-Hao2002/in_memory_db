//include/gui/GUIInterface.hpp

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "db/Database.hpp"

namespace gui {

class GUIInterface {
public:
    GUIInterface();
    void run();

private:

    std::string trim(const std::string& s);
    sf::RenderWindow window;
    db::Database database;
    bool output_csv = false;

    std::string sql_input;
    std::string result_output;

    // GUI component
    sf::Font font;
    sf::Text text;
    sf::Text inputText;
    sf::Text inputHintText;
    sf::RectangleShape inputBox;
    sf::RectangleShape executeButton;
    sf::Text executeButtonText;

    // Buttons
    sf::RectangleShape csvButton;
    sf::Text csvButtonText;

    sf::RectangleShape asciiButton;
    sf::Text asciiButtonText;

    sf::RectangleShape saveButton;
    sf::Text saveButtonText;

    sf::RectangleShape loadButton;
    sf::Text loadButtonText;

    std::string filename = "db.txt";  // default filename

    sf::RectangleShape selectButton, insertButton, deleteButton, updateButton, createButton, joinButton;
    sf::Text selectButtonText, insertButtonText, deleteButtonText, updateButtonText, createButtonText, joinButtonText;



    // 輸入狀態
    std::string currentInput;
    bool isInputActive = false;

    void handle_input(const sf::Event& event); 
    void execute_sql(const std::string& sql);
    void draw();

    //游標
    std::size_t cursorPosition = 0;      // Cursor position
    sf::RectangleShape cursor;           // Cursor graph
    sf::Clock cursorTimer;               // Controlling Flashing
    bool showCursor = true;



};

}
