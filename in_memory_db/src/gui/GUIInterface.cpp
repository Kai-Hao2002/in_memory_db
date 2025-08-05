//src/gui/GUIInterface.cpp
#include "gui/GUIInterface.hpp"
#include "parser/Parser.hpp"
#include "util/Formatter.hpp"
#include "db/Database.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>



namespace gui {


GUIInterface::GUIInterface()
    : window(sf::VideoMode(800, 700), "In-Memory DB GUI")
{
    if (!font.loadFromFile("fonts/Roboto-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    const float margin = 10.f;
    const float inputHeight = 40.f;
    const float buttonHeight = 30.f;
    const float buttonY = margin; // first row of buttons
    const float buttonY2 = buttonY + buttonHeight + margin; // Second row of buttons
    const float resultY = buttonY + buttonHeight + margin * 2; // Output is below the button
    const float inputY = 600 - inputHeight - margin; // The input box is at the bottom

    // Query result output area text
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(margin, resultY);  // Place below the button

    // input box
    inputBox.setSize(sf::Vector2f(600, inputHeight*3));
    inputBox.setFillColor(sf::Color(220, 220, 220));
    inputBox.setPosition(margin, inputY);

    // Enter text
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(margin + 5, inputY + 5);

    inputHintText.setFont(font);
    inputHintText.setCharacterSize(18);
    inputHintText.setFillColor(sf::Color(150, 150, 150)); // 灰色
    inputHintText.setString("Please click on the input box to start typing");
    inputHintText.setPosition(inputBox.getPosition().x + 5, inputBox.getPosition().y + 5);


    // Execute button (lower right corner)
    executeButton.setSize(sf::Vector2f(100, inputHeight));
    executeButton.setFillColor(sf::Color(100, 150, 250));
    executeButton.setPosition(620, inputY);

    executeButtonText.setFont(font);
    executeButtonText.setCharacterSize(18);
    executeButtonText.setFillColor(sf::Color::White);
    executeButtonText.setString("Execute");
    executeButtonText.setPosition(635, inputY + 5);

    // Output CSV button
    csvButton.setSize(sf::Vector2f(120, buttonHeight));
    csvButton.setFillColor(sf::Color(180, 180, 180));
    csvButton.setPosition(margin, buttonY);

    csvButtonText.setFont(font);
    csvButtonText.setCharacterSize(14);
    csvButtonText.setFillColor(sf::Color::Black);
    csvButtonText.setString("Output CSV");
    csvButtonText.setPosition(margin + 10, buttonY + 5);

    // Output ASCII button
    asciiButton.setSize(sf::Vector2f(120, buttonHeight));
    asciiButton.setFillColor(sf::Color(180, 180, 180));
    asciiButton.setPosition(margin + 130, buttonY);

    asciiButtonText.setFont(font);
    asciiButtonText.setCharacterSize(14);
    asciiButtonText.setFillColor(sf::Color::Black);
    asciiButtonText.setString("Output ASCII");
    asciiButtonText.setPosition(margin + 140, buttonY + 5);

    // Save button
    saveButton.setSize(sf::Vector2f(120, buttonHeight));
    saveButton.setFillColor(sf::Color(180, 180, 180));
    saveButton.setPosition(margin + 260, buttonY);

    saveButtonText.setFont(font);
    saveButtonText.setCharacterSize(14);
    saveButtonText.setFillColor(sf::Color::Black);
    saveButtonText.setString("Save DB");
    saveButtonText.setPosition(margin + 270, buttonY + 5);

    // Load button
    loadButton.setSize(sf::Vector2f(120, buttonHeight));
    loadButton.setFillColor(sf::Color(180, 180, 180));
    loadButton.setPosition(margin + 390, buttonY);

    loadButtonText.setFont(font);
    loadButtonText.setCharacterSize(14);
    loadButtonText.setFillColor(sf::Color::Black);
    loadButtonText.setString("Load DB");
    loadButtonText.setPosition(margin + 400, buttonY + 5);

    // SELECT button
    selectButton.setSize({100, 30});
    selectButton.setFillColor(sf::Color(200, 200, 200));
    selectButton.setPosition(margin + 3 * 110, buttonY2);
    selectButtonText.setFont(font);
    selectButtonText.setString("SELECT");
    selectButtonText.setCharacterSize(14);
    selectButtonText.setFillColor(sf::Color::Black);
    selectButtonText.setPosition(margin + 3 * 110 + 10, buttonY2 + 5);

    // INSERT button
    insertButton.setSize({100, 30});
    insertButton.setFillColor(sf::Color(200, 200, 200));
    insertButton.setPosition(margin + 1 * 110, buttonY2);
    insertButtonText.setFont(font);
    insertButtonText.setString("INSERT");
    insertButtonText.setCharacterSize(14);
    insertButtonText.setFillColor(sf::Color::Black);
    insertButtonText.setPosition(margin + 1 * 110 + 10, buttonY2 + 5);

    // DELETE button
    deleteButton.setSize({100, 30});
    deleteButton.setFillColor(sf::Color(200, 200, 200));
    deleteButton.setPosition(margin + 4 * 110, buttonY2);
    deleteButtonText.setFont(font);
    deleteButtonText.setString("DELETE");
    deleteButtonText.setCharacterSize(14);
    deleteButtonText.setFillColor(sf::Color::Black);
    deleteButtonText.setPosition(margin + 4 * 110 + 10, buttonY2 + 5);

    // UPDATE button
    updateButton.setSize({100, 30});
    updateButton.setFillColor(sf::Color(200, 200, 200));
    updateButton.setPosition(margin + 2 * 110, buttonY2);
    updateButtonText.setFont(font);
    updateButtonText.setString("UPDATE");
    updateButtonText.setCharacterSize(14);
    updateButtonText.setFillColor(sf::Color::Black);
    updateButtonText.setPosition(margin + 2 * 110 + 10, buttonY2 + 5);

    // CREATE button
    createButton.setSize({100, 30});
    createButton.setFillColor(sf::Color(200, 200, 200));
    createButton.setPosition(margin + 0 * 110, buttonY2);
    createButtonText.setFont(font);
    createButtonText.setString("CREATE");
    createButtonText.setCharacterSize(14);
    createButtonText.setFillColor(sf::Color::Black);
    createButtonText.setPosition(margin + 0 * 110 + 10, buttonY2 + 5);

    // JOIN button
    joinButton.setSize({100, 30});
    joinButton.setFillColor(sf::Color(200, 200, 200));
    joinButton.setPosition(margin + 5 * 110, buttonY2);
    joinButtonText.setFont(font);
    joinButtonText.setString("JOIN");
    joinButtonText.setCharacterSize(14);
    joinButtonText.setFillColor(sf::Color::Black);
    joinButtonText.setPosition(margin + 5 * 110 + 10, buttonY2 + 5);



    
    //cursor
    cursor.setSize(sf::Vector2f(2.f, 18.f));
    cursor.setFillColor(sf::Color::Black);
    cursorTimer.restart();

}


void GUIInterface::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            handle_input(event);  // Pass each event in for processing
        }

        draw();
    }
}


void GUIInterface::handle_input(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            
            if (inputBox.getGlobalBounds().contains(mousePos)) {
        isInputActive = true;
        showCursor = true;
        cursorTimer.restart();

        float padding = 5.f;
        sf::Vector2f inputBoxPos = inputBox.getPosition();
        float relativeX = mousePos.x - inputBoxPos.x - padding;
        float relativeY = mousePos.y - inputBoxPos.y - padding;

        std::vector<std::string> lines;
        std::istringstream iss(currentInput);
        std::string line;
        while (std::getline(iss, line)) {
            lines.push_back(line);
        }

        int lineHeight = inputText.getCharacterSize() + 4;
        int clickedLine = static_cast<int>(relativeY / lineHeight);

        if (lines.empty()) {
            clickedLine = 0;
            lines.push_back("");
        } else {
            if (clickedLine >= static_cast<int>(lines.size())) clickedLine = lines.size() - 1;
            if (clickedLine < 0) clickedLine = 0;
        }

        const std::string& clickedText = lines[clickedLine];
        sf::Text tempText("", font, 18);
        float accumulatedWidth = 0.f;
        std::size_t charIndex = 0;
        for (; charIndex < clickedText.size(); ++charIndex) {
            tempText.setString(clickedText.substr(0, charIndex + 1));
            float width = tempText.getLocalBounds().width;
            if (width > relativeX) break;
        }

        cursorPosition = 0;
        for (int i = 0; i < clickedLine; ++i) {
            cursorPosition += lines[i].size() + 1;
        }
        cursorPosition += charIndex;

        
        inputText.setString(currentInput);
        showCursor = true;
        cursorTimer.restart();

        } else if (executeButton.getGlobalBounds().contains(mousePos)) {
            execute_sql(currentInput);
            currentInput.clear();
            cursorPosition = 0;
            inputText.setString("");
            isInputActive = false;
            showCursor = false;
        } else if (csvButton.getGlobalBounds().contains(mousePos)) {
            output_csv = true;
            result_output = "Switched to CSV Output";
            isInputActive = false;
            showCursor = false;
        } else if (asciiButton.getGlobalBounds().contains(mousePos)) {
            output_csv = false;
            result_output = "Switched to ASCII Output";
            isInputActive = false;
            showCursor = false;
        } else if (saveButton.getGlobalBounds().contains(mousePos)) {
            std::string filename = trim(currentInput);
            if (filename.empty()) {
                result_output = "Please enter a filename in the input box.";
            } else {
                try {
                    database.save_to_file(filename);
                    result_output = "Database saved to '" + filename + "' successfully.";
                    currentInput.clear();
                    cursorPosition = 0;
                    inputText.setString("");
                } catch (const std::exception& e) {
                    result_output = std::string("Failed to save: ") + e.what();
                }
            }
            isInputActive = false;
            showCursor = false;
        } else if (loadButton.getGlobalBounds().contains(mousePos)) {
            std::string filename = trim(currentInput);
            if (filename.empty()) {
                result_output = "Please enter a filename in the input box.";
            } else {
                try {
                    database.load_from_file(filename);
                    result_output = "Database loaded from '" + filename + "' successfully.";
                    currentInput.clear();
                    cursorPosition = 0;
                    inputText.setString("");
                } catch (const std::exception& e) {
                    result_output = std::string("Failed to load: ") + e.what();
                }
            }
            isInputActive = false;
            showCursor = false;
        } else if (selectButton.getGlobalBounds().contains(mousePos)) {
            // currentInput = "SELECT * FROM table_name WHERE condition;";
            currentInput = "SELECT * FROM students WHERE name='Charlie';";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (insertButton.getGlobalBounds().contains(mousePos)) {
            //currentInput = "INSERT INTO table_name (column1, column2) VALUES (value1, value2);";
            currentInput = "INSERT INTO students (id, name) VALUES (1, 'Bob');";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (deleteButton.getGlobalBounds().contains(mousePos)) {
            //currentInput = "DELETE FROM table_name WHERE condition;";
            currentInput = "DELETE FROM students WHERE id=1;";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (updateButton.getGlobalBounds().contains(mousePos)) {
            //currentInput = "UPDATE table_name SET column1 = value1 WHERE condition;";
            currentInput = "UPDATE students SET name='Charlie' WHERE id=1;";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (createButton.getGlobalBounds().contains(mousePos)) {
            //currentInput = "CREATE TABLE table_name (column1 TYPE, column2 TYPE);";
            currentInput = "CREATE TABLE students (id INT, name STRING);";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (joinButton.getGlobalBounds().contains(mousePos)) {
            //currentInput = "SELECT t1.col1, t2.col2 FROM table1 t1 INNER JOIN table2 t2 ON t1.col = t2.col;";
            currentInput = "SELECT students.name, scores.score FROM students INNER JOIN scores ON students.id = scores.s_id;";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else {
            isInputActive = false;
            showCursor = false;
        }
    }

    // Text input processing, including line breaks
    if (event.type == sf::Event::TextEntered && isInputActive) {
        if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            currentInput.insert(cursorPosition, 1, '\n');
            cursorPosition++;
        } else if (event.text.unicode == 8) {
            // Backspace does not need to be processed here
        } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
            currentInput.insert(cursorPosition, 1, static_cast<char>(event.text.unicode));
            cursorPosition++;
        }
        inputText.setString(currentInput);
        showCursor = true;
        cursorTimer.restart();
    }

    // Cursor control and delete key
    if (event.type == sf::Event::KeyPressed && isInputActive) {
        if (event.key.code == sf::Keyboard::Left && cursorPosition > 0) {
            cursorPosition--;
        } else if (event.key.code == sf::Keyboard::Right && cursorPosition < currentInput.length()) {
            cursorPosition++;
        } else if (event.key.code == sf::Keyboard::BackSpace && cursorPosition > 0) {
            currentInput.erase(cursorPosition - 1, 1);
            cursorPosition--;
        } else if (event.key.code == sf::Keyboard::Delete && cursorPosition < currentInput.length()) {
            currentInput.erase(cursorPosition, 1);
        } else if (event.key.code == sf::Keyboard::Up) {
    if (currentInput.empty() || cursorPosition == 0) return; // Don't use break, use return instead or just do nothing

    size_t lineStart = currentInput.rfind('\n', cursorPosition - 1);
    size_t prevLineStart = (lineStart == std::string::npos) ? std::string::npos : currentInput.rfind('\n', lineStart - 1);

    size_t col = cursorPosition - (lineStart == std::string::npos ? 0 : lineStart + 1);

    if (prevLineStart == std::string::npos) {
        cursorPosition = 0 + std::min(col, lineStart == std::string::npos ? currentInput.size() : lineStart);
    } else {
            size_t prevLineLength = lineStart - prevLineStart - 1;
            cursorPosition = prevLineStart + 1 + std::min(col, prevLineLength);
        }
    } else if (event.key.code == sf::Keyboard::Down) {
        if (currentInput.empty() || cursorPosition == currentInput.size()) return; 

        size_t lineStart = currentInput.rfind('\n', cursorPosition == 0 ? 0 : cursorPosition - 1);
        size_t nextLineStart = currentInput.find('\n', lineStart == std::string::npos ? 0 : lineStart + 1);

        size_t col = cursorPosition - (lineStart == std::string::npos ? 0 : lineStart + 1);

        if (nextLineStart == std::string::npos) {
            cursorPosition = currentInput.size();
        } else {
            size_t nextNextLineStart = currentInput.find('\n', nextLineStart + 1);
            size_t nextLineLength = (nextNextLineStart == std::string::npos ? currentInput.size() : nextNextLineStart) - (nextLineStart + 1);
            cursorPosition = nextLineStart + 1 + std::min(col, nextLineLength);
        }
    }


        inputText.setString(currentInput);
        showCursor = true;
        cursorTimer.restart();
    }

}






void GUIInterface::draw() {
    window.clear(sf::Color::White);

    // Button
    window.draw(csvButton);
    window.draw(csvButtonText);

    window.draw(asciiButton);
    window.draw(asciiButtonText);

    window.draw(saveButton);
    window.draw(saveButtonText);

    window.draw(loadButton);
    window.draw(loadButtonText);

    // Second row Template SQL buttons
    window.draw(createButton);
    window.draw(createButtonText);   

    window.draw(insertButton);
    window.draw(insertButtonText);

    window.draw(updateButton);
    window.draw(updateButtonText);

    window.draw(selectButton);
    window.draw(selectButtonText);


    window.draw(deleteButton);
    window.draw(deleteButtonText);

    window.draw(joinButton);
    window.draw(joinButtonText);

    // Display execution results (multi-line processing)
    std::string displayText = result_output.empty() ? "Welcome to In-Memory DB GUI" : result_output;

    const float maxWidth = 780.f; // Maximum width
    const float x = 10.f;
    const float y = 100.f; // Start displaying from a point below the button
    const unsigned int charSize = 18;
    float currentY = y;

    std::istringstream stream(displayText);
    std::string displayLine; // Rename to avoid duplication with the following line
    while (std::getline(stream, displayLine)) {
        std::string word, wrappedLine;
        std::istringstream lineStream(displayLine);
        sf::Text tempText("", font, charSize);
        while (lineStream >> word) {
            std::string testLine = wrappedLine + (wrappedLine.empty() ? "" : " ") + word;
            tempText.setString(testLine);
            if (tempText.getLocalBounds().width > maxWidth) {
                sf::Text drawText(wrappedLine, font, charSize);
                drawText.setFillColor(sf::Color::Black);
                drawText.setPosition(x, currentY);
                window.draw(drawText);
                currentY += charSize + 4;
                wrappedLine = word;
            } else {
                wrappedLine = testLine;
            }
        }
        if (!wrappedLine.empty()) {
            sf::Text drawText(wrappedLine, font, charSize);
            drawText.setFillColor(sf::Color::Black);
            drawText.setPosition(x, currentY);
            window.draw(drawText);
            currentY += charSize + 4;
        }
    }

    // --- Input box drawing ---
    window.draw(inputBox);

    // Preset text display position
    const float padding = 5.f; // Input box padding
    float inputBoxLeft = inputBox.getPosition().x + padding;
    float inputBoxWidth = inputBox.getSize().x - 2 * padding;
    float inputTextY = inputBox.getPosition().y + (inputBox.getSize().y - 18) / 2.f; // Vertically centered, font size 18

    // Set the inputText font and size, and first set it to the left side of the input box
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(inputBoxLeft, inputTextY);

    // Set input text
    inputText.setString(currentInput);

    // Split into multiple lines first to avoid repeating the line above
    std::vector<std::string> lines;
    std::istringstream iss(currentInput);
    std::string line; 
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    if (lines.empty()) {
        lines.push_back("");
    }

    // Ensure cursorPosition is within the currentInput range
    if (cursorPosition > (int)currentInput.size()) {
        cursorPosition = (int)currentInput.size();
    }
    if (cursorPosition < 0) {
        cursorPosition = 0;
    }

    // Find the row and position of the cursor within the row
    int cursorLine = 0;
    int pos = 0;
    for (; cursorLine < (int)lines.size(); ++cursorLine) {
        if (cursorPosition <= pos + (int)lines[cursorLine].size())
            break;
        pos += lines[cursorLine].size() + 1; // +1 is the line break symbol
    }
    if (cursorLine >= (int)lines.size()) {
        cursorLine = (int)lines.size() - 1;
    }
    if (cursorLine < 0) {
        cursorLine = 0;
    }

    int cursorCol = cursorPosition - pos;
    if (cursorCol > (int)lines[cursorLine].size()) {
        cursorCol = (int)lines[cursorLine].size();
    }
    if (cursorCol < 0) {
        cursorCol = 0;
    }

    // Use lines[cursorLine] to safely get the string
    sf::Text temp(lines[cursorLine].substr(0, cursorCol), font, 18);


    temp.setPosition(inputText.getPosition());
    float cursorX = temp.getPosition().x + temp.getLocalBounds().width;

    // Cursor Y axis: calculate height based on number of rows
    int lineHeight = inputText.getCharacterSize() + 4;
    float cursorY = inputText.getPosition().y + cursorLine * lineHeight;

    // Also calculate the overall text offset to keep the cursor visible
    float cursorRelativeX = cursorX - inputBoxLeft;
    float startX = inputBoxLeft;

    if (cursorRelativeX > inputBoxWidth) {
        startX = inputBoxLeft - (cursorRelativeX - inputBoxWidth);
    }
    if (startX > inputBoxLeft) {
        startX = inputBoxLeft;
    }

    // The entire text is offset to the left
    inputText.setPosition(startX, inputTextY);

    // Update the cursor position
    cursor.setPosition(startX + temp.getLocalBounds().left + temp.getLocalBounds().width, cursorY);

    // Display prompt text or input text
    if (currentInput.empty() && !isInputActive) {
        window.draw(inputHintText);
    } else {
        window.draw(inputText);
    }

    // The cursor flashes (only when the input box is activated)
    if (isInputActive) {
        if (cursorTimer.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorTimer.restart();
        }
        if (showCursor) {
            window.draw(cursor);
        }
    }

    // --- execute button ---
    window.draw(executeButton);
    window.draw(executeButtonText);

    window.display();
}




void GUIInterface::execute_sql(const std::string& sql) {
    try {
        Parser parser(sql);
        auto stmt = parser.parse();
        stmt->execute(database);

        if (stmt->has_results()) {
            auto headers = stmt->get_columns();
            auto rows = stmt->get_results();
            std::ostringstream oss;
            if (output_csv) {
                db::Formatter::print_csv(headers, rows, oss);
            } else {
                db::Formatter::print_ascii_table(headers, rows, oss);
            }
            result_output = oss.str();
        } else {
            result_output = stmt->name() + " executed successfully.";
        }
    } catch (const std::exception& ex) {
        result_output = std::string("Error: ") + ex.what();
    }
}

std::string GUIInterface::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos || last == std::string::npos) return "";
    return str.substr(first, last - first + 1);
}

}
