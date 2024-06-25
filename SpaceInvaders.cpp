#include <iostream>
#include "myconsole.h"
#include "mygraphics.h"
#include <string>
#include <fstream>



using namespace std;


bool checkWinner(int gameArr[][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (gameArr[i][j] == 2) {
                return false;
            }
        }
    }
    return true;
}
//loads matrix values from txt file to the game array;
void loadGame(int gameArr[][10]) {
    ifstream layout("layout.txt");

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            layout >> gameArr[i][j];
        }
    }

    layout.close();
}

void loadGameCoordinates(int cord[2]) {
    ifstream coordinate("coordinate.txt");
    for (int i = 0; i < 2; i++) {
        coordinate >> cord[i];
    }
    coordinate.close();
}

int getLives() {
    int lives = 3;
    ifstream updatedLives("lives.txt");
    updatedLives >> lives;
    updatedLives.close();
    return lives;
}

int getScore() {
    int score = 3;
    ifstream updatedScore("score.txt");
    updatedScore >> score;
    updatedScore.close();
    return score;
}

void selectionSort(int a[], int n) {
    int i, j, min, temp;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (a[j] < a[min])
                min = j;
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

//Store top 5 high scores
void saveHighScores(int score) {
    ifstream highscores("scores.txt");
    int scores[6];
    for (int i = 0; i < 5; i++) {
        highscores >> scores[i];
    }
    highscores.close();
    //append the current score to the scores array
    scores[5] = score;
    int n = sizeof(scores) / sizeof(scores[0]);
    // sort the array in ascending order
    selectionSort(scores, n);

    ofstream updatedScore("scores.txt");
    // save only top 5 scores from array since the array is already sorted so last index is removed
    for (int i = 5; i >= 1; i--) {
        updatedScore << scores[i] << endl;
    }
    updatedScore.close();
}

void getHighScores(int scores[5]) {
    ifstream highscores("scores.txt");
    for (int i = 0; i < 5; i++) {
        highscores >> scores[i];
    }
    highscores.close();
}

void saveLives(int lives) {
    ofstream updatedLives("lives.txt");
    updatedLives << lives;
    updatedLives.close();
}

void saveScore(int score) {
    ofstream updatedScore("score.txt");
    updatedScore << score;
    updatedScore.close();
}

void saveGame(int gameArr[][10], int cord[2], int score, int lives) {
    ofstream RFile("layout.txt");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if ((gameArr[i][j] == 0) || (gameArr[i][j] == 1) || (gameArr[i][j] == 2)) {
                RFile << gameArr[i][j] << " ";
            }
        }
        RFile << endl;
    }
    RFile.close();

    ofstream coordinate("coordinate.txt");
    for (int i = 0; i < 2; i++) {
        coordinate << cord[i] << endl;
    }
    coordinate.close();

    saveHighScores(score);

    saveLives(lives);

    saveScore(score);
}

int main()
{
    // Game Layout
    // 2 = rectangles
    // 1 = ball
    // 0 = empty space
    int game[10][10] = {
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    };
    cout << "Start New Game: Press any key(except 2)" << endl;
    cout << "Load Previous Game Press 2" << endl;
    string state;
    cin >> state;
    // loads previous saved game
    if (state == "2") {
        loadGame(game);
    }

    //game dimensions
    int win_x, win_y;

    // ball x and y poistions
    int ball_x, ball_y;

    bool flag = true;

    win_x = 450;
    win_y = 450;
    int lives = 3;
    int scoreCount = 0;

    SetWindowSize(win_x, win_y);
    int cord[2];
    if (state == "2") {
        //for finding coordinates of ball x and y axis
        loadGameCoordinates(cord);
        lives = getLives();
        scoreCount = getScore();
        // set ball x and y cords
        ball_x = cord[0];
        ball_y = cord[1];
    }
    else {
        ball_x = win_x / 2;
        ball_y = win_y - 50;

        cord[0] = ball_x;
        cord[1] = ball_y;
    }



    int alien_start_pos_x = 0;
    int alien_end_pos_x = 50;

    int alien_start_pos_y = 0;

    //for moving aliens down
    int alien_move_downward_y = alien_start_pos_y;
    int alien_move_downward_x = alien_end_pos_x;

    bool stop = false;

    bool rightKey = false;
    bool leftKey = false;
    bool downkey = false;

    bool shift = false;
    bool endLoop = false;
    int loopCount = 0;

    char scoress[] = "Score: ";
    bool collision = false;
    char gameover[] = "Game Over";

    char score[] = { '0','0', 0 };
    string allien_killed = "00";
    char scores[] = "score";
    char livesMessage[] = "Lives: ";

    char live[] = { '0', 0 };
    string allien_lives = "0";

    char highScoreMessage[] = "Top Scores: ";
    char highscores[] = { '0', '0', 0 };
    string high_score = "00";

    while (flag)
    {
        //GAME LOOP
        //MOVE BALL WITH LEFT AND RIGHT ARROW KEYS.
        int a = CheckKeyPressed();

        if (a == LEFTKEY && ball_x <= 0) {
            ball_x -= 0;
        }
        else if (a == RIGHTKEY && ball_x >= win_x) {
            ball_x -= 0;
        }
        else if (a == DOWNKEY && (ball_y + 35) >= (win_y - 35)) {
            ball_x -= 0;
        }
        else if (a == LEFTKEY) {
            ball_x -= 50;
            leftKey = true;
        }
        else if (a == RIGHTKEY) {
            rightKey = true;
            ball_x += 50;
        }
        else if (a == UPKEY && !collision)
            ball_y -= 35;
        else if (a == DOWNKEY) {
            downkey = true;
            ball_y += 35;
        }
        // fire when hit space key
        // ACII for space key is 32
        else if (a == 32) {
            myLine(ball_x + 15, ball_y, ball_x + 15, ball_y - 15, RGB(255, 255, 0));
            stop = false;
            shift = true;
        }
        // when s key is pressed game is to be saved and ended.
        // ASCII key for s is 83
        else if (a == 83) {
            cord[0] = ball_x;
            cord[1] = ball_y;
            saveGame(game, cord, scoreCount, lives);
            ClearScreen();
            flag = false;
            break;
        }
        // when p is presses game is paused and game state is saved
        else if (a == 80) {
            cord[0] = ball_x;
            cord[1] = ball_y;
            saveGame(game, cord, scoreCount, lives);
            system("PAUSE");
            loadGame(game);
        }
        // Poistion of the block in the game array which will be removed when a fire hits it.
        int elementToRemoveX = 0;
        int elementToRemoveY = 0;
        bool fire = false;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (a == LEFTKEY) {
                    // swap poistion to left in the array
                    if (game[i][j] == 1 && leftKey && (j - 1) >= 0) {
                        game[i][j] = 0;
                        game[i][j - 1] = 1;
                        leftKey = false;
                    }
                }
                else if (a == RIGHTKEY) {
                    // swap poistion to right in the array
                    if (game[i][j] == 1 && rightKey && (j + 1) <= 9) {
                        game[i][j] = 0;
                        game[i][j + 1] = 1;
                        rightKey = false;
                    }
                }
                else if (a == UPKEY) {
                    // swap ball poistion with row and if a rectange already exits detect collision
                    if (game[i][j] == 1) {
                        if (game[i - 1][j] == 2) {
                            collision = true;
                        }
                        else if (game[i - 1][j] != 2) {
                            game[i - 1][j] = 1;
                            game[i][j] = 0;
                        }

                        if (collision) {
                            game[9][j] = 1;
                            game[i][j] = 0;
                        }
                    }
                }
                else if (a == DOWNKEY) {
                    // swap poistion to down
                    if (game[i][j] == 1 && downkey && i < 9) {
                        game[i + 1][j] = 1;
                        game[i][j] = 0;
                        downkey = false;
                    }
                }
                else if (a == 32) {
                    if (game[i][j] == 1) {
                        // check row and col that needs to be removed
                        for (int k = 0; k < 10; k++) {
                            if (game[k][j] == 2) {
                                elementToRemoveX = k;
                                fire = true;
                                elementToRemoveY = j;
                            }
                        }
                    }
                }
            }

        }

        //CLEAR PREVIOUS FRAME

        ClearScreen();

        myLine(0, win_y / 1, win_x, win_y / 1, RGB(255, 255, 0));

        int alien_start_pos_x = 0;
        int alien_end_pos_x = 50;

        alien_start_pos_y = alien_move_downward_y;
        //after every 100 loops move the aliens downwad
        if (loopCount == 100 && !endLoop) {
            alien_move_downward_y += 50;
            loopCount = 0;
        }
        loopCount++;
        int alien_end_pos_y = 50;
        int prevRow = 0;
        int nextLineX = ball_x + 15;
        int nextLineY = ball_y - 15;
        bool ChangeVal = false;
        //draw game elements
        if (!collision) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    // draw rectangles
                    if (game[i][j] == 2) {
                        // if rectangle has reached bottom line set flag
                        if (alien_start_pos_y >= win_y) {
                            endLoop = true;
                        }

                        myRect(alien_start_pos_x, alien_start_pos_y, alien_end_pos_x, alien_end_pos_y, RGB(0, 0, 0), RGB(255, 0, 0));//1 st box
                        ChangeVal = true;
                    }
                    else if (game[i][j] == 1) {
                        myEllipse(ball_x, ball_y, ball_x + 40, ball_y + 40, RGB(204, 255, 153), RGB(204, 255, 153));
                    }
                    if (a == 32) {
                        //draw bullet and update its coordinates
                        myLine(nextLineX, nextLineY, nextLineX, nextLineY - 15, RGB(255, 255, 0));
                        nextLineY = nextLineY - 15;
                        // remove the rectangle that is fired and update score count
                        if (elementToRemoveX >= 0 && elementToRemoveY >= 0 && fire && !stop) {
                            game[elementToRemoveX][elementToRemoveY] = 0;
                            scoreCount += 1;
                            stop = true;
                        }
                    }
                    // detect collision
                    if (alien_start_pos_y >= win_y) {
                        endLoop = true;

                        collision = true;
                    }

                    if (ChangeVal) {
                        alien_start_pos_x = alien_end_pos_x;
                        //alien_start_pos_y = alien_end_pos_y;
                        alien_end_pos_x += 50;
                    }
                }

                if (ChangeVal) {
                    alien_start_pos_y += 50;
                    alien_start_pos_x = 0;
                    alien_end_pos_y += 50;
                    alien_end_pos_x = 50;

                    ChangeVal = false;
                }
            }
        }
        if (collision) {
            ball_y = win_y - 50;
        }


        //detect collision and if lives are remaining pass the loop
        if (collision && lives == 0) {
            myDrawText(100, 250, 250, gameover, RGB(255, 255, 255), RGB(255, 0, 0));
        }
        else if (collision) {
            lives--;
            collision = false;
            endLoop = false;
            continue;
        }
        //convert int to string as myDrawTextWithFont takes char array.
        string str = to_string(scoreCount);
        allien_killed[0] = str[0];
        allien_killed[1] = str[1];

        score[0] = allien_killed[0];
        score[1] = allien_killed[1];
        // Score Heading
        myDrawTextWithFont(550, 0, 0, scoress, RGB(0, 255, 0), RGB(20, 20, 20));
        char scores[] = "00";
        //score Value
        myDrawTextWithFont(650, 0, 0, score, RGB(0, 255, 0), RGB(20, 20, 20));

        string live_str = to_string(lives);
        allien_lives[0] = live_str[0];

        live[0] = allien_lives[0];
        //Live Heading
        myDrawTextWithFont(550, 25, 0, livesMessage, RGB(0, 255, 0), RGB(20, 20, 20));
        //Live value
        myDrawTextWithFont(650, 25, 0, live, RGB(0, 255, 0), RGB(20, 20, 20));

        int highScoresArr[5];
        getHighScores(highScoresArr);

        myDrawTextWithFont(550, 50, 0, highScoreMessage, RGB(0, 255, 0), RGB(20, 20, 20));
        int StartX = 650;
        for (int i = 0; i < 5; i++) {
            string hscore_str = to_string(highScoresArr[i]);

            high_score[0] = hscore_str[0];
            high_score[1] = hscore_str[1];

            highscores[0] = high_score[0];
            highscores[1] = high_score[1];
            StartX += 50;
            myDrawTextWithFont(StartX, 50, 0, highscores, RGB(0, 255, 0), RGB(20, 20, 20));
        }


        if (checkWinner(game)) {
            ClearScreen();
            char winner[] = "WINNER";
            myDrawTextWithFont(550, 250, 20, winner, RGB(0, 255, 0), RGB(20, 20, 20));
        }
    }
}