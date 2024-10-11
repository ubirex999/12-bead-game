#include <SFML/Graphics.hpp>
#include <iostream>
#include<string.h>
#include<fstream>
using namespace sf;
using namespace std;
// function to save game 
bool saveGameState(const int arr[][5], int p_red, int p_green,int bari) {
    ofstream outputFile("game_state.txt");

    if (outputFile.is_open()) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                outputFile << arr[i][j] << " ";
            }
            outputFile << endl;
        }
        cout << "Saving p_red: " << p_red << ", p_green: " << p_green << endl;
        outputFile << p_red << " " << p_green<<" "<<bari;
        
        outputFile.close();
        return true; // Saving is successful
    }
    else {
        cout << "Unable to open file for saving." << endl;
        return false; // Saving is failed
    }
}
// Function to load the game state from a file
bool loadGameState(int arr[][5], int& p_red, int& p_green,int &bari) {
    ifstream inputFile("game_state.txt");

    if (inputFile.is_open()) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                inputFile >> arr[i][j];
            }
            
        }
        
        inputFile >> p_red >> p_green>>bari;
        cout << "loding p_red: " << p_red << ", p_green: " << p_green << endl;

        inputFile.close();
        return true; // Loading successful
    }
    else {
        cout << "Unable to open file for loading. Starting a new game." << endl;
        return false; // Loading failed
    }
}

int main() {
    int arr[5][5] = { {1,1,1,1,1},{1,1,1,1,1},{1,1,0,2,2},{2,2,2,2,2},{2,2,2,2,2} };
    int p_red=12;
    int p_green=12;
    int bari = 0; // to control turns

    bool isGameLoaded = loadGameState(arr, p_red, p_green, bari);
    if (isGameLoaded) {
        cout << "Game loaded successfully!" << endl;
        // tells us if the game is loaded or not
    }
    else {
        cout << "Could not load the game. Starting a new game." << endl;
        // Initialize arr with default values here if needed
    }
    int defaultArr[5][5] = { {1,1,1,1,1},{1,1,1,1,1},{1,1,0,2,2},{2,2,2,2,2},{2,2,2,2,2} };
    // Copy the default values to arr only if it hasn't been loaded from the file
    if (p_red == 0 && p_green == 0) {
        memcpy(arr, defaultArr, sizeof(arr));
    }
    int boardSize = 5;
    int boxSize = 100;
    

    Font message;
    if (!message.loadFromFile("TEXT FONT/Fontfile.otf"))      // Bringing the formaat of the text to be displayed
    {
        cout << " Error loaded while bringing the file ";
    }
    // Ye sara user instructions ka  
    Text ins, ins2, ins3;
    Text New;
    New.setFont(message);
    ins.setFont(message);
    ins2.setFont(message);
    ins3.setFont(message);
    ins.setCharacterSize(30);
    ins2.setCharacterSize(30);
    ins3.setCharacterSize(30);
    ins.setFillColor(Color::Black);
    ins.setPosition(Vector2f(550.f, 30.f));
    ins2.setPosition(Vector2f(550.f, 60.f));
    ins2.setFillColor(Color::Black);
    ins3.setPosition(Vector2f(550.f, 90.f));
    ins3.setFillColor(Color::Black);
    // SCORE KA TEXT _______________________________________
    Text scorered, scoregreen;
    scorered.setFont(message);
    scoregreen.setFont(message);
    scorered.setCharacterSize(30);
    scoregreen.setCharacterSize(30);
    scorered.setPosition(Vector2f(550.f, 250.0f));
    scoregreen.setPosition(Vector2f(550.f, 280.0f));
    scorered.setFillColor(Color::Red);
    scoregreen.setFillColor(Color::Green);

    // Winner ko Display krwane ka CODE _--------------------------------------------------------------
    Text winner;
    winner.setFont(message);
    winner.setCharacterSize(60);
    winner.setPosition(Vector2f(300.f, 250.f));
    winner.setFillColor(Color::Blue);




    RenderWindow window(VideoMode(1000, 500), "SFML Board");
    window.setFramerateLimit(60);
    CircleShape beads[5][5]; /// beads ko produce 
    Vector2f selectpos; // variable to check place 
    Vector2f placepos;  // variable to jump place
    Vector2i leftclickpos;
    Vector2f lsr[5], ler[5];
    Vector2f lsc[5], lec[5];
    Vector2f diamondPoints[4];
    scorered.setString("Red beds");
    bool leftclick = false;  // check if left click is done 
    Color leftclickcolor; // To store the original color 

     

    //---------------------------------------------------------------DOTS KA STRUCTURE---------------------------------------------------------
    int x = 1, y = 0;
    for (int i = 0; i < 5; ++i) {
        x = 0;
        for (int j = 0; j < 5; ++j) {
            beads[i][j].setRadius(30);
            if (arr[i][j] == 1) {
                beads[i][j].setFillColor(Color::Red);
            }
            else if (arr[i][j] == 2) {
                beads[i][j].setFillColor(Color::Green);
            }
            else {
                beads[i][j].setFillColor(Color::Transparent);
            }
            beads[i][j].setPosition(Vector2f(x * 100, y * 100));
            x++;
        }
        y++;
    }

    // LINES KA STRUCTURE_______________________________________

    for (int i = 0; i < 5; ++i) {
        lsr[i] = beads[i][0].getPosition() + Vector2f(30, 30);
        ler[i] = beads[i][5 - 1].getPosition() + Vector2f(30, 30);
    }
    for (int i = 0; i < boardSize; ++i) {
        lsc[i] = beads[0][i].getPosition() + Vector2f(30, 30);
        lec[i] = beads[5 - 1][i].getPosition() + Vector2f(30, 30);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                bool isGameSaved = saveGameState(arr, p_red, p_green,bari);

                if (isGameSaved) {
                    cout << "Game saved successfully!" << endl;
                }
                else {
                    cout << "Could not save the game." << endl;
                }

                window.close();
            }
            // to start new game 
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::N) {
                // Reset the game state variables
                p_red = 12;
                p_green = 12;
                bari = 0;

                // Reset the board to default values
                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 5; ++j) {
                        arr[i][j] = defaultArr[i][j];
                    }
                }
                // Display a message or perform any other actions as needed
                cout << "New game started!" << endl;
            }

            // Left click
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                int column = event.mouseButton.x / boxSize;
                int row = event.mouseButton.y / boxSize;
                Vector2i help = Mouse::getPosition(window);

                // Check clicked box has a circle
                if (column >= 0 && column < boardSize && row >= 0 && row < boardSize &&
                    beads[row][column].getFillColor() != Color::Transparent && !leftclick) {
                    if (bari % 2 == 0) {
                        if (beads[row][column].getFillColor() == Color::Red) {
                            break;

                        }
                    }
                    else {
                        if (beads[row][column].getFillColor() == Color::Green) {
                            break;

                        }
                    }
                    leftclickpos = Vector2i(row, column);
                    leftclickcolor = beads[row][column].getFillColor();
                    beads[row][column].setFillColor(Color::Blue);
                    leftclick = true;
                    selectpos = beads[row][column].getPosition();
                }
            }

            // Deselect space bar
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Tab && leftclick) {
                beads[leftclickpos.x][leftclickpos.y].setFillColor(leftclickcolor);
                leftclick = false;
            }

            // Right click
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) {
                int column = event.mouseButton.x / boxSize;
                int row = event.mouseButton.y / boxSize;
                
                // Move check 
                if (column >= 0 && column < boardSize && row >= 0 && row < boardSize && leftclick && beads[row][column].getFillColor() == Color::Transparent)
                {
                    Vector2f placepos = beads[row][column].getPosition();

                    // Stop diagonal movements
                    if ((row + column) % 2 != 0 && (selectpos.x != placepos.x && selectpos.y != placepos.y)) {

                        break;

                    }

                    // Check horizontal movements
                    if (placepos.x == selectpos.x && selectpos.y != placepos.y)
                    {
                        if (placepos.y - selectpos.y == 100 || placepos.y - selectpos.y == -100) {
                            // Single horizontal movement
                            if (beads[row][column].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor()) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                bari++;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if (placepos.y - selectpos.y == 200) {
                            // Double horizontal movement
                            if (beads[row - 1][column].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() &&
                                beads[row - 1][column].getFillColor() != leftclickcolor && beads[row - 1][column].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row - 1][column].setFillColor(Color::Transparent);
                                arr[row - 1][column] = 0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move Vertical";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if (placepos.y - selectpos.y == -200) {
                            // Double horizontal movement
                            if (beads[row + 1][column].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() &&
                                beads[row + 1][column].getFillColor() != leftclickcolor && beads[row + 1][column].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row + 1][column].setFillColor(Color::Transparent);
                                arr[row + 1][column]=0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move Vertical";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }

                    }


                    // Horizontal movements 

                    if (placepos.x != selectpos.x && selectpos.y == placepos.y)
                    {
                        if (placepos.x - selectpos.x == 100 || placepos.x - selectpos.x == -100) {
                            // Single horizontal movement
                            if (beads[row][column].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor()) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                bari++;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if (placepos.x - selectpos.x == 200) {
                            // Double horizontal movement
                            if (beads[row][column - 1].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() &&
                                beads[row][column - 1].getFillColor() != leftclickcolor && beads[row][column - 1].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row][column - 1].setFillColor(Color::Transparent);
                                arr[row][column - 1] = 0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if (placepos.x - selectpos.x == -200) {
                            // Double horizontal movement
                            if (beads[row][column + 1].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() && beads[row][column + 1].getFillColor() != leftclickcolor && beads[row][column + 1].getFillColor() != Color::Transparent)
                            {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row][column + 1].setFillColor(Color::Transparent);
                                arr[row][column + 1] = 0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }

                    }
                    if (placepos.x != selectpos.x && selectpos.y != placepos.y)
                    {
                        if ((placepos.y - selectpos.y == 100 || placepos.y - selectpos.y == -100) && (placepos.x - selectpos.x == 100 || placepos.x - selectpos.x == -100)) {
                            // Single horizontal movement
                            if (beads[row][column].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor()) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                bari++;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if ((placepos.y - selectpos.y == 200) && (placepos.x - selectpos.x == 200)) {
                            // Double horizontal movement
                            if (beads[row - 1][column - 1].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() &&
                                beads[row - 1][column - 1].getFillColor() != leftclickcolor && beads[row - 1][column - 1].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row - 1][column - 1].setFillColor(Color::Transparent);
                                arr[row - 1][column - 1]=0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if ((placepos.y - selectpos.y == -200) && (placepos.x - selectpos.x == -200))
                        {
                            // Double horizontal movement
                            if (beads[row + 1][column + 1].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() &&
                                beads[row + 1][column + 1].getFillColor() != leftclickcolor && beads[row + 1][column + 1].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row + 1][column + 1].setFillColor(Color::Transparent);
                                arr[row + 1][column + 1]=0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if ((placepos.y - selectpos.y == 200) && (placepos.x - selectpos.x == -200))
                        {
                            // Double horizontal movement
                            if (beads[row - 1][column + 1].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() && beads[row - 1][column + 1].getFillColor() != leftclickcolor && beads[row - 1][column + 1].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row - 1][column + 1].setFillColor(Color::Transparent);
                                arr[row - 1][column + 1]=0;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                leftclick = false;
                                cout << "Move horizontal";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                bari++;
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                        else if ((placepos.y - selectpos.y == -200) && (placepos.x - selectpos.x == 200))
                        {
                            // Double horizontal movement
                            if (beads[row + 1][column - 1].getFillColor() != beads[leftclickpos.x][leftclickpos.y].getFillColor() && beads[row + 1][column - 1].getFillColor() != leftclickcolor && beads[row + 1][column - 1].getFillColor() != Color::Transparent) {
                                beads[row][column].setFillColor(leftclickcolor);
                                beads[row + 1][column - 1].setFillColor(Color::Transparent);
                                arr[row + 1] [column - 1]=0 ;
                                beads[leftclickpos.x][leftclickpos.y].setFillColor(Color::Transparent);
                                //cout << leftclickpos.x;
                                leftclick = false;
                                cout << "Move horizontal";
                                if (bari % 2 == 0) { p_red--; }
                                else { p_green--; }
                                arr[leftclickpos.x][leftclickpos.y] = 0;
                                bari++;
                            }

                            arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                            arr[leftclickpos.x][leftclickpos.y] = 0;
                        }
                    }

                   // arr[row][column] = (leftclickcolor == Color::Red) ? 1 : 2; // This checks if the color of the bead is red it assigns the value 1 at the position of the clicked bead(row and column).otherwise green
                   // arr[leftclickpos.x][leftclickpos.y] = 0; // This sets the position where the bead was at to 0. It means the previous position becomes empty.
                }   // this is very crucial as it keeps the board updateed
            }
        }
        New.setString("n for new game");
        New.setPosition(Vector2f(550.f, 120));
        New.setFillColor(Color::Black);
        New.setCharacterSize(30);
        ins.setString("Left Click: Select");
        ins2.setString("Right Click : Place");
        ins3.setString("Tab Key : De-select");
        scorered.setString("Red turn");
        scoregreen.setString("Green turn");


        window.clear(Color::White);
        if ((bari - 1) % 2 == 0) window.draw(scorered);
        else window.draw(scoregreen);
        window.draw(ins);
        window.draw(ins2);
        window.draw(ins3);
        window.draw(New);


        if (p_red == 0) {
            winner.setString("GREEN WINS!!!!!!");
            window.clear();
            window.draw(winner);
            window.display();
            break;
        }
        if (p_green == 0) {
            winner.setString("Red WINS!!!!!!");
            window.clear();
            window.draw(winner);
            window.display();
            break;
        }
        for (int i = 0; i < boardSize; ++i) {
            Vertex line[] = {
                Vertex(lsr[i], Color::Black),
                Vertex(ler[i], Color::Black)
            };
            window.draw(line, 2, Lines);
        }
        for (int i = 0; i < boardSize; ++i) {
            Vertex lineCols[] = {
                Vertex(lsc[i], Color::Black),
                Vertex(lec[i], Color::Black)
            };
            window.draw(lineCols, 2, Lines);
        }
        Vertex diagonalLine[] = {
        Vertex(beads[0][0].getPosition() + Vector2f(30, 30), Color::Black),
        Vertex(beads[boardSize - 1][boardSize - 1].getPosition() + Vector2f(30, 30), Color::Black)
        };
        window.draw(diagonalLine, 2, Lines);

        Vertex diagonalLine2[] = {
        Vertex(beads[0][boardSize - 1].getPosition() + Vector2f(30, 30), Color::Black),
        Vertex(beads[boardSize - 1][0].getPosition() + Vector2f(30, 30), Color::Black)
        };
        window.draw(diagonalLine2, 2, Lines);

        Vertex smallDiagonalLine[] = {
        Vertex(beads[0][2].getPosition() + Vector2f(30, 30), Color::Black),
        Vertex(beads[2][0].getPosition() + Vector2f(30, 30), Color::Black)
        };
        window.draw(smallDiagonalLine, 2, Lines);

        Vertex smallDiagonalLine1[] = {
        Vertex(beads[2][0].getPosition() + Vector2f(30, 30), Color::Black),
        Vertex(beads[4][2].getPosition() + Vector2f(30, 30), Color::Black)
        };
        window.draw(smallDiagonalLine1, 2, Lines);


        Vertex smallDiagonalLine2[] = {
        Vertex(beads[4][2].getPosition() + Vector2f(30, 30), Color::Black),
        Vertex(beads[2][4].getPosition() + Vector2f(30, 30), Color::Black)
        };
        window.draw(smallDiagonalLine2, 2, Lines);

        Vertex smallDiagonalLine3[] = {
       Vertex(beads[2][4].getPosition() + Vector2f(30, 30), Color::Black),
       Vertex(beads[0][2].getPosition() + Vector2f(30, 30), Color::Black)
        };
        window.draw(smallDiagonalLine3, 2, Lines);



        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                window.draw(beads[i][j]);
            }
        }


        window.display();
    }
    return 0;
}



