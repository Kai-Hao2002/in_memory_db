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
    const float buttonY = margin; // 按鈕在最上方
    const float buttonY2 = buttonY + buttonHeight + margin; // 第二排按鈕
    const float resultY = buttonY + buttonHeight + margin * 2; // 輸出在按鈕下方
    const float inputY = 600 - inputHeight - margin; // 輸入框在最底部

    // 查詢結果輸出區域文字
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(margin, resultY);  // 放在按鈕下方

    // 輸入框
    inputBox.setSize(sf::Vector2f(600, inputHeight*3));
    inputBox.setFillColor(sf::Color(220, 220, 220));
    inputBox.setPosition(margin, inputY);

    // 輸入文字
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(margin + 5, inputY + 5);

    inputHintText.setFont(font);
    inputHintText.setCharacterSize(18);
    inputHintText.setFillColor(sf::Color(150, 150, 150)); // 灰色
    inputHintText.setString("Please click on the input box to start typing");
    inputHintText.setPosition(inputBox.getPosition().x + 5, inputBox.getPosition().y + 5);


    // Execute 按鈕（右下角）
    executeButton.setSize(sf::Vector2f(100, inputHeight));
    executeButton.setFillColor(sf::Color(100, 150, 250));
    executeButton.setPosition(620, inputY);

    executeButtonText.setFont(font);
    executeButtonText.setCharacterSize(18);
    executeButtonText.setFillColor(sf::Color::White);
    executeButtonText.setString("Execute");
    executeButtonText.setPosition(635, inputY + 5);

    // Output CSV 按鈕
    csvButton.setSize(sf::Vector2f(120, buttonHeight));
    csvButton.setFillColor(sf::Color(180, 180, 180));
    csvButton.setPosition(margin, buttonY);

    csvButtonText.setFont(font);
    csvButtonText.setCharacterSize(14);
    csvButtonText.setFillColor(sf::Color::Black);
    csvButtonText.setString("Output CSV");
    csvButtonText.setPosition(margin + 10, buttonY + 5);

    // Output ASCII 按鈕
    asciiButton.setSize(sf::Vector2f(120, buttonHeight));
    asciiButton.setFillColor(sf::Color(180, 180, 180));
    asciiButton.setPosition(margin + 130, buttonY);

    asciiButtonText.setFont(font);
    asciiButtonText.setCharacterSize(14);
    asciiButtonText.setFillColor(sf::Color::Black);
    asciiButtonText.setString("Output ASCII");
    asciiButtonText.setPosition(margin + 140, buttonY + 5);

    // Save 按鈕
    saveButton.setSize(sf::Vector2f(120, buttonHeight));
    saveButton.setFillColor(sf::Color(180, 180, 180));
    saveButton.setPosition(margin + 260, buttonY);

    saveButtonText.setFont(font);
    saveButtonText.setCharacterSize(14);
    saveButtonText.setFillColor(sf::Color::Black);
    saveButtonText.setString("Save DB");
    saveButtonText.setPosition(margin + 270, buttonY + 5);

    // Load 按鈕
    loadButton.setSize(sf::Vector2f(120, buttonHeight));
    loadButton.setFillColor(sf::Color(180, 180, 180));
    loadButton.setPosition(margin + 390, buttonY);

    loadButtonText.setFont(font);
    loadButtonText.setCharacterSize(14);
    loadButtonText.setFillColor(sf::Color::Black);
    loadButtonText.setString("Load DB");
    loadButtonText.setPosition(margin + 400, buttonY + 5);

    // SELECT 按鈕
    selectButton.setSize({100, 30});
    selectButton.setFillColor(sf::Color(200, 200, 200));
    selectButton.setPosition(margin + 0 * 110, buttonY2);
    selectButtonText.setFont(font);
    selectButtonText.setString("SELECT");
    selectButtonText.setCharacterSize(14);
    selectButtonText.setFillColor(sf::Color::Black);
    selectButtonText.setPosition(margin + 0 * 110 + 10, buttonY2 + 5);

    // INSERT 按鈕
    insertButton.setSize({100, 30});
    insertButton.setFillColor(sf::Color(200, 200, 200));
    insertButton.setPosition(margin + 1 * 110, buttonY2);
    insertButtonText.setFont(font);
    insertButtonText.setString("INSERT");
    insertButtonText.setCharacterSize(14);
    insertButtonText.setFillColor(sf::Color::Black);
    insertButtonText.setPosition(margin + 1 * 110 + 10, buttonY2 + 5);

    // DELETE 按鈕
    deleteButton.setSize({100, 30});
    deleteButton.setFillColor(sf::Color(200, 200, 200));
    deleteButton.setPosition(margin + 2 * 110, buttonY2);
    deleteButtonText.setFont(font);
    deleteButtonText.setString("DELETE");
    deleteButtonText.setCharacterSize(14);
    deleteButtonText.setFillColor(sf::Color::Black);
    deleteButtonText.setPosition(margin + 2 * 110 + 10, buttonY2 + 5);

    // UPDATE 按鈕
    updateButton.setSize({100, 30});
    updateButton.setFillColor(sf::Color(200, 200, 200));
    updateButton.setPosition(margin + 3 * 110, buttonY2);
    updateButtonText.setFont(font);
    updateButtonText.setString("UPDATE");
    updateButtonText.setCharacterSize(14);
    updateButtonText.setFillColor(sf::Color::Black);
    updateButtonText.setPosition(margin + 3 * 110 + 10, buttonY2 + 5);

    // CREATE 按鈕
    createButton.setSize({100, 30});
    createButton.setFillColor(sf::Color(200, 200, 200));
    createButton.setPosition(margin + 4 * 110, buttonY2);
    createButtonText.setFont(font);
    createButtonText.setString("CREATE");
    createButtonText.setCharacterSize(14);
    createButtonText.setFillColor(sf::Color::Black);
    createButtonText.setPosition(margin + 4 * 110 + 10, buttonY2 + 5);

    // JOIN 按鈕
    joinButton.setSize({100, 30});
    joinButton.setFillColor(sf::Color(200, 200, 200));
    joinButton.setPosition(margin + 5 * 110, buttonY2);
    joinButtonText.setFont(font);
    joinButtonText.setString("JOIN");
    joinButtonText.setCharacterSize(14);
    joinButtonText.setFillColor(sf::Color::Black);
    joinButtonText.setPosition(margin + 5 * 110 + 10, buttonY2 + 5);



    
    //游標
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

            handle_input(event);  // 將每一筆事件傳進去處理
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

        // 這三行是關鍵！更新文字與游標視覺位置
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
            currentInput = "SELECT * FROM table_name WHERE condition;";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (insertButton.getGlobalBounds().contains(mousePos)) {
            currentInput = "INSERT INTO table_name (column1, column2) VALUES (value1, value2);";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (deleteButton.getGlobalBounds().contains(mousePos)) {
            currentInput = "DELETE FROM table_name WHERE condition;";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (updateButton.getGlobalBounds().contains(mousePos)) {
            currentInput = "UPDATE table_name SET column1 = value1 WHERE condition;";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (createButton.getGlobalBounds().contains(mousePos)) {
            currentInput = "CREATE TABLE table_name (column1 TYPE, column2 TYPE);";
            cursorPosition = currentInput.size();
            inputText.setString(currentInput);
            isInputActive = true;
            showCursor = true;
            cursorTimer.restart();
        } else if (joinButton.getGlobalBounds().contains(mousePos)) {
            currentInput = "SELECT t1.col1, t2.col2 FROM table1 t1 INNER JOIN table2 t2 ON t1.col = t2.col;";
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

    // 文字輸入處理，包含換行
    if (event.type == sf::Event::TextEntered && isInputActive) {
        if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            currentInput.insert(cursorPosition, 1, '\n');
            cursorPosition++;
        } else if (event.text.unicode == 8) {
            // Backspace不用在這裡處理
        } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
            currentInput.insert(cursorPosition, 1, static_cast<char>(event.text.unicode));
            cursorPosition++;
        }
        inputText.setString(currentInput);
        showCursor = true;
        cursorTimer.restart();
    }

    // 游標控制和刪除鍵
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
    if (currentInput.empty() || cursorPosition == 0) return; // 不使用 break，改用 return 或直接不做事

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
        if (currentInput.empty() || cursorPosition == currentInput.size()) return; // 同理改成 return

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

    // 按鈕
    window.draw(csvButton);
    window.draw(csvButtonText);

    window.draw(asciiButton);
    window.draw(asciiButtonText);

    window.draw(saveButton);
    window.draw(saveButtonText);

    window.draw(loadButton);
    window.draw(loadButtonText);

    // 第二排 Template SQL 按鈕
    window.draw(selectButton);
    window.draw(selectButtonText);

    window.draw(insertButton);
    window.draw(insertButtonText);

    window.draw(deleteButton);
    window.draw(deleteButtonText);

    window.draw(updateButton);
    window.draw(updateButtonText);

    window.draw(createButton);
    window.draw(createButtonText);

    window.draw(joinButton);
    window.draw(joinButtonText);

    // 顯示執行結果（多行處理）
    std::string displayText = result_output.empty() ? "Welcome to In-Memory DB GUI" : result_output;

    const float maxWidth = 780.f; // 最大寬度
    const float x = 10.f;
    const float y = 100.f; // 從按鈕下方一點開始顯示
    const unsigned int charSize = 18;
    float currentY = y;

    std::istringstream stream(displayText);
    std::string displayLine; // 改名，避免和後面line重複
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

    // --- 輸入框繪製 ---
    window.draw(inputBox);

    // 預設文字顯示位置
    const float padding = 5.f; // 輸入框內邊距
    float inputBoxLeft = inputBox.getPosition().x + padding;
    float inputBoxWidth = inputBox.getSize().x - 2 * padding;
    float inputTextY = inputBox.getPosition().y + (inputBox.getSize().y - 18) / 2.f; // 垂直置中，字體大小18

    // 設定 inputText 字型與字號，並先設到輸入框左側位置
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(inputBoxLeft, inputTextY);

    // 設置輸入文字
    inputText.setString(currentInput);

    // 先拆成多行，避免跟上面重複使用 line
    std::vector<std::string> lines;
    std::istringstream iss(currentInput);
    std::string line; 
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    if (lines.empty()) {
        lines.push_back("");
    }

    // 確保 cursorPosition 在 currentInput 範圍內
    if (cursorPosition > (int)currentInput.size()) {
        cursorPosition = (int)currentInput.size();
    }
    if (cursorPosition < 0) {
        cursorPosition = 0;
    }

    // 找游標在哪一行跟行內位置
    int cursorLine = 0;
    int pos = 0;
    for (; cursorLine < (int)lines.size(); ++cursorLine) {
        if (cursorPosition <= pos + (int)lines[cursorLine].size())
            break;
        pos += lines[cursorLine].size() + 1; // +1 是換行符號
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

    // 用 lines[cursorLine] 安全取得字串
    sf::Text temp(lines[cursorLine].substr(0, cursorCol), font, 18);


    temp.setPosition(inputText.getPosition());
    float cursorX = temp.getPosition().x + temp.getLocalBounds().width;

    // 游標 Y 軸：依據行數算高度
    int lineHeight = inputText.getCharacterSize() + 4;
    float cursorY = inputText.getPosition().y + cursorLine * lineHeight;

    // 同樣計算文字整體偏移以保持游標可見
    float cursorRelativeX = cursorX - inputBoxLeft;
    float startX = inputBoxLeft;

    if (cursorRelativeX > inputBoxWidth) {
        startX = inputBoxLeft - (cursorRelativeX - inputBoxWidth);
    }
    if (startX > inputBoxLeft) {
        startX = inputBoxLeft;
    }

    // 文字整體往左偏移
    inputText.setPosition(startX, inputTextY);

    // 更新游標位置
    cursor.setPosition(startX + temp.getLocalBounds().left + temp.getLocalBounds().width, cursorY);

    // 顯示提示文字或輸入文字
    if (currentInput.empty() && !isInputActive) {
        window.draw(inputHintText);
    } else {
        window.draw(inputText);
    }

    // 游標閃爍顯示（只有輸入框被激活時）
    if (isInputActive) {
        if (cursorTimer.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorTimer.restart();
        }
        if (showCursor) {
            window.draw(cursor);
        }
    }

    // --- 執行按鈕 ---
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
