#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include<cmath>
#include<bits/stdc++.h>

using namespace std;


#define charSize 60.0f
#define charInitialX 0.0f
#define charInitialY 100.0f
#define enemyInitialX -500.0f
#define enemyInitialY 130.0f
#define initialSpeed 7.0f
#define enemyInitialSpeed 6.0f
#define initialVelocity  21.0f
#define gravity 1.0f
#define respawnDelay 0
#define windowInitialX 0.0f
#define windowInitialY 0.0f
#define windowFinalX 1280.0f
#define windowFinalY 720.0f
#define coordinateX 1280
#define coordinateY 720
#define PI 3.14159265358979323846



vector<float> ft;
int score = 0;
int lives = 3;
float deadPosition=0.0;
int obstacleClearCount =3;

bool isStage1 = true;
bool isStage2 = false;
bool isStage3 = false;
bool allStage = false;
bool winner = false;
bool playMainBgSound = true;
bool jerryFootStepSound = false;




//---------------****function prototyping****---------------------//
void renderBitmapString(float x, float y, float z, void *font, char *string);
void exit1(int value);


// random floating number generator
float random_float(float min, float max) {
    // Use a static random number generator to ensure it's initialized only once
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}
void set_ft_values(int n, float min_val, float max_val){
    for(int i=0; i<n; i++){
        ft.push_back(random_float(min_val,max_val));
    }
}

// Function to Use SPACE BAR

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 13: // Enter
        isStage1 = true;
        glutPostRedisplay(); // Mark the window for a redraw
        break;

    case 27: // 'Esc' key
        exit(0);
        break;
    }
}

// drawCircle function is to create cloud
void drawCircle(float radius, float cx, float cy, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}
void draw_cloud(float x, float y) {
    // Draw a cloud using multiple circles
    glColor3f(1.0f, 1.0f, 1.0f); // White color

    drawCircle(40.0f, x+100.0f, y, 50);
    drawCircle(50.0f, x+150.0f, y, 50);
    drawCircle(47.0f, x+190.0f, y, 50);
    drawCircle(40.0f, x+240.0f, y, 50);
    drawCircle(45.0f, x+280.0f, y, 50);
    drawCircle(37.0f, x+320.0f, y-10.0f, 50);
}
void draw_cloud1(float x, float y) {
    // Draw a cloud using multiple circles
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    drawCircle(35.0f, x+100.0f, y, 50);
    drawCircle(47.0f, x+135.0f, y+5, 50);
    drawCircle(65.0f, x+190.0f, y+10, 50);
    drawCircle(40.0f, x+250.0f, y, 50);
}


void drawText(const string& text, int x, int y, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2i(x, y);

    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}


    //drawText("tom & Jerry Run", 560, 500, GLUT_BITMAP_TIMES_ROMAN_24, 0.0f, 0.0f, 1.0f);




//............................................Character Start.........................................

class Character {
private:
    float x;
    float y;
    float speed;
    float size;


public:
    float jumpVelocity;
    float currentJumpVelocity;
    bool isJumping;
    bool moveLeftFlag;
    bool moveRightFlag;
    bool isDead;


    Character(float initialX = charInitialX, float initialY = charInitialY)
        : x(initialX), y(initialY), speed(initialSpeed), jumpVelocity(initialVelocity), size(charSize), isJumping(false), currentJumpVelocity(0.0f),
          isDead(false), moveLeftFlag(false), moveRightFlag(false) {}

    //Setters
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }
    void setJumpVelocity (float velocity) { jumpVelocity = velocity; }
    void setCurrentJumpVelocity (float velocity) { currentJumpVelocity = velocity; }
    void setCharacterDead(bool dead) { isDead = dead; }
    void setJumping(bool jump) { isJumping = jump; }
    void setSpeed(float speed) {this->speed = speed;}

    //Getters
    float getJumpVelocity () { return jumpVelocity; }
    float getCurrentJumpVelocity () { return currentJumpVelocity; }
    float getX() { return x; }
    float getY() { return y; }
    bool isCharacterDead() const { return isDead; }
    bool getJumping() { return isJumping; }
    int getRespawnDelay() const { return respawnDelay; }
    float getSize() const { return size; }
    bool getMoveLeftFlag() { return moveLeftFlag; }
    bool getMoveRightFlag() { return moveRightFlag; }
    float getSpeed() { return speed; }
    float getWidth() { return size; }
    float getHeight() { return  size; }


    void draw() {

    float scaleX =100.0, scaleY =100.0;

    glBegin(GL_POLYGON);//face
    glColor3f(0.833,0.37,0.6);
    glVertex2f(x+0.45f*scaleX, y+0.57f*scaleY);
    glVertex2f(x+0.52f*scaleX, y+0.59f*scaleY);
    glVertex2f(x+0.52f*scaleX, y+0.74f*scaleY);
    glVertex2f(x+0.45f*scaleX, y+0.8f*scaleY);
    glVertex2f(x+0.3f*scaleX, y+0.8f*scaleY);
    glVertex2f(x+0.25f*scaleX, y+0.78f*scaleY);
    glVertex2f(x+0.2f*scaleX, y+0.69f*scaleY);
    glVertex2f(x+0.21f*scaleX, y+0.52f*scaleY);
    glVertex2f(x+0.4f*scaleX, y+0.45f*scaleY);
    glVertex2f(x+0.4f*scaleX, y+0.5f*scaleY);
    glVertex2f(x+0.45f*scaleX, y+0.57f*scaleY);
    glEnd();

    glBegin(GL_POLYGON);//0.5 face
    glColor3f(0.763,0.844,0.48);
    glVertex2f(x+0.4f*scaleX, y+0.45f*scaleY);
    glVertex2f(x+0.4f*scaleX, y+0.5f*scaleY);
    glVertex2f(x+0.45f*scaleX, y+0.57f*scaleY);
    glVertex2f(x+0.52f*scaleX, y+0.59f*scaleY);
    glVertex2f(x+0.57f*scaleX, y+0.59f*scaleY);
    glVertex2f(x+0.57f*scaleX, y+0.54f*scaleY);
    glVertex2f(x+0.52f*scaleX, y+0.5f*scaleY);
    glVertex2f(x+0.499f*scaleX, y+0.45f*scaleY);
    glVertex2f(x+0.4f*scaleX, y+0.45f*scaleY);
    glEnd();

    glBegin(GL_POLYGON);//nose
    glColor3f(0,0,0);
    glVertex2f(x+0.50*scaleX,y+0.58*scaleY);
    glVertex2f(x+0.52*scaleX,y+0.58*scaleY);
    glVertex2f(x+0.52*scaleX,y+0.56*scaleY);
    glVertex2f(x+0.50*scaleX,y+0.56*scaleY);
    glVertex2f(x+0.51*scaleX,y+0.58*scaleY);

    glEnd();
    glBegin(GL_POLYGON);//eyes
    glColor3f(1,1,1);
    glVertex2f(x+0.52*scaleX,y+0.59*scaleY);
    glVertex2f(x+0.52*scaleX,y+0.73*scaleY);
    glVertex2f(x+0.49*scaleX,y+0.73*scaleY);
    glVertex2f(x+0.49*scaleX,y+0.59*scaleY);
   // glVertex2f(x+0.5*scaleX,y+0.59*scaleY);
   // glVertex2f(x+0.5*scaleX,y+0.73*scaleY);
    glVertex2f(x+0.52*scaleX,y+0.59*scaleY);
    glEnd();
    glBegin(GL_POLYGON);//pupil
    glColor3f(0,0,0);

    glVertex2f(x+0.49*scaleX,y+0.59*scaleY);
    glVertex2f(x+0.51*scaleX,y+0.59*scaleY);
    glVertex2f(x+0.51*scaleX,y+0.65*scaleY);
    glVertex2f(x+0.49*scaleX,y+0.65*scaleY);
    glVertex2f(x+0.49*scaleX,y+0.59*scaleY);

    glEnd();
    glBegin(GL_POLYGON);//eyes
    glColor3f(1,1,1);
    glVertex2f(x+0.469*scaleX,y+0.59*scaleY);
    glVertex2f(x+0.469*scaleX,y+0.73*scaleY);
    glVertex2f(x+0.43*scaleX,y+0.73*scaleY);
    glVertex2f(x+0.43*scaleX,y+0.59*scaleY);
    glVertex2f(x+0.469*scaleX,y+0.59*scaleY);
    glEnd();
    glBegin(GL_POLYGON);//pupil
    glColor3f(0, 0, 0);
    glVertex2f(x+0.43*scaleX, y+0.59*scaleY);
    glVertex2f(x+0.46*scaleX, y+0.59*scaleY);
    glVertex2f(x+0.46*scaleX, y+0.65*scaleY);
    glVertex2f(x+0.43*scaleX, y+0.65*scaleY);
    glVertex2f(x+0.43*scaleX, y+0.59*scaleY);
    glEnd();

    glBegin(GL_POLYGON);//mouth
    glColor3f(1, 0, 0);
    glVertex2f(x+0.45*scaleX, y+0.49*scaleY);
    glVertex2f(x+0.45*scaleX, y+0.51*scaleY);
    glVertex2f(x+0.51*scaleX, y+0.51*scaleY);
    glVertex2f(x+0.49*scaleX, y+0.49*scaleY);
    glVertex2f(x+0.45*scaleX, y+0.49*scaleY);
    glEnd();

    glBegin(GL_POLYGON);//face
    glColor3f(0.833,0.37,0.6);
    glVertex2f(x+0.35*scaleX, y+0.8*scaleY);
    glVertex2f(x+0.42*scaleX, y+0.8*scaleY);
    glVertex2f(x+0.42*scaleX, y+0.9*scaleY);
    glVertex2f(x+0.35*scaleX, y+0.8*scaleY);

    glEnd();
    glBegin(GL_TRIANGLE_FAN);//ear
    glColor3f(0.833,0.37,0.6);
    glVertex2f(x+0.25*scaleX, y+0.78*scaleY);
    glVertex2f(x+0.19*scaleX, y+0.85*scaleY);
    glVertex2f(x+0.1*scaleX, y+0.85*scaleY);
    glVertex2f(x+0.03*scaleX,y+0.8*scaleY);
    glVertex2f(x+0.11*scaleX, y+0.8*scaleY);
    glVertex2f(x+0.2*scaleX, y+0.69*scaleY);
    glVertex2f(x+0.25*scaleX, y+0.78*scaleY);
    glVertex2f(x+0.19*scaleX, y+0.85*scaleY);
    glVertex2f(x+0.1*scaleX, y+0.85*scaleY);
    glVertex2f(x+0.25*scaleX, y+0.78*scaleY);

    glEnd();
    glBegin(GL_POLYGON);//ear
    glColor3f(0.763,0.844,0.48);
    glVertex2f(x+0.21*scaleX, y+0.69*scaleY);
    glVertex2f(x+0.11*scaleX, y+0.8*scaleY);
    glVertex2f(x+0.03*scaleX, y+0.8*scaleY);
    glVertex2f(x+0*scaleX,y+0.72*scaleY);
   glVertex2f(x+0*scaleX, y+0.65*scaleY);
    glVertex2f(x+0.03*scaleX, y+0.559*scaleY);
    glVertex2f(x+0.11*scaleX, y+0.5*scaleY);
    glVertex2f(x+0.21*scaleX, y+0.52*scaleY);
    glVertex2f(x+0.21*scaleX, y+0.69*scaleY);

    glEnd();

    glBegin(GL_POLYGON);//body
    glColor3f(0.833,0.37,0.6);
    glVertex2f(x+0.29*scaleX, y+0.492*scaleY);
    glVertex2f(x+0.23*scaleX, y+0.45*scaleY);
    glVertex2f(x+0.2*scaleX, y+0.4*scaleY);
    glVertex2f(x+0.19*scaleX,y+0.32*scaleY);
    glVertex2f(x+0.21*scaleX, y+0.25*scaleY);
   //glVertex2f(x+0.21*scaleX, y+0.17*scaleY);
    glVertex2f(x+0.41*scaleX, y+0.17*scaleY);
    glVertex2f(x+0.45*scaleX, y+0.24*scaleY);
    glVertex2f(x+0.45*scaleX, y+0.4*scaleY);
    glVertex2f(x+0.42*scaleX, y+0.441*scaleY);
    glEnd();


    glBegin(GL_POLYGON);//body
    glColor3f(0.763,0.844,0.48);
    glVertex2f(x+0.41*scaleX, y+0.17*scaleY);
    glVertex2f(x+0.45*scaleX, y+0.24*scaleY);
    glVertex2f(x+0.45*scaleX, y+0.4*scaleY);
    glVertex2f(x+0.42*scaleX, y+0.42*scaleY);
    glVertex2f(x+0.39*scaleX, y+0.4*scaleY);
    glVertex2f(x+0.39*scaleX,y+0.24*scaleY);
    glVertex2f(x+0.41*scaleX, y+0.17*scaleY);
    glEnd();

    glBegin(GL_POLYGON);//leg
    glColor3f(0.833,0.37,0.6);
    glVertex2f(x+0.27*scaleX, y+0.1*scaleY);
    glVertex2f(x+0.22*scaleX, y+0.05*scaleY);
    glVertex2f(x+0.236*scaleX, y+0.01*scaleY);
    glVertex2f(x+0.46*scaleX, y+0.01*scaleY);
    glVertex2f(x+0.476*scaleX,y+0.05*scaleY);
    glVertex2f(x+0.41*scaleX, y+0.1*scaleY);
    glVertex2f(x+0.41*scaleX, y+0.17*scaleY);
    glVertex2f(x+0.21*scaleX, y+0.25*scaleY);
    glVertex2f(x+0.25*scaleX, y+0.17*scaleY);
    glEnd();

    glLineWidth(0.001);
    glBegin(GL_LINES);//leg
    glColor3f(0,0,0);
    glVertex2f(x+0.35*scaleX, y+0.01*scaleY);
    glVertex2f(x+0.35*scaleX, y+0.17*scaleY);
    glEnd();


    glBegin(GL_LINE_STRIP);//hand
    glColor3f(0, 0, 0);
    glVertex2f(x+0.29*scaleX, y+0.49*scaleY);
    glVertex2f(x+0.23*scaleX, y+0.45*scaleY);
    glVertex2f(x+0.2*scaleX, y+0.4*scaleY);
    glVertex2f(x+0.19*scaleX,y+0.32*scaleY);
   glVertex2f(x+0.21*scaleX, y+0.25*scaleY);
    glVertex2f(x+0.23*scaleX, y+0.23*scaleY);

    glVertex2f(x+0.25*scaleX, y+0.25*scaleY);

    glVertex2f(x+0.26*scaleX, y+0.32*scaleY);
    glEnd();


    //glLineWidth(5*scaleY);

    glBegin(GL_LINE_STRIP);//body
    glColor3f(0.833,0.37,0.6);
    glVertex2f(x+0.21*scaleX, y+0.26*scaleY);
    glVertex2f(x+0.14*scaleX, y+0.25*scaleY);
    glVertex2f(x+0.11*scaleX, y+0.22*scaleY);
    glVertex2f(x+0.06*scaleX,y+0.23*scaleY);
   glVertex2f(x+0.02*scaleX, y+0.25*scaleY);
    glVertex2f(x+0.03*scaleX, y+0.3*scaleY);

    glEnd();
    glLineWidth(1);

    glBegin(GL_LINE_STRIP);//m
    glColor3f(0, 0, 0);
    glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
    glVertex2f(x+0.6*scaleX, y+0.62*scaleY);
    glEnd();
    glBegin(GL_LINE_STRIP);//m

    glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
    glVertex2f(x+0.4*scaleX,y+0.57*scaleY);

    glEnd();
    glBegin(GL_LINE_STRIP);//m

    glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
    glVertex2f(x+0.4*scaleX,y+0.54*scaleY);

    glEnd();
    glBegin(GL_LINE_STRIP);//m
    glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
    glVertex2f(x+0.6*scaleX, y+0.59*scaleY);
    glEnd();
    }

    void moveLeft() {
        cout<<"jerry x: "<<x<<endl;
        cout<<"jerry y: "<<y<<endl;
        x -= speed;
        if (x < windowInitialX) {
            x = windowInitialX;
        }
    }

    void moveRight() {
        cout<<"jerry x: "<<x<<endl;
        cout<<"jerry y: "<<y<<endl;
        x += speed;
        if(isStage1){
            if (x > 7300) {
                x = 0.0;
                isStage1= false;
                winner= true;
                glutPostRedisplay();
            }
        }
    }

    void jump() {
        if (!isJumping) {
            isJumping = true;
            currentJumpVelocity = jumpVelocity;
        }
    }

    // Function to handle arrow key events
    void specialKeyPressed(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                moveLeftFlag = true;
                moveRightFlag = false;
                break;
            case GLUT_KEY_RIGHT:
                moveRightFlag = true;
                moveLeftFlag = false;
                break;
            case GLUT_KEY_UP:
                if (!isJumping) {
                    isJumping = true;
                    currentJumpVelocity = jumpVelocity;
                }
                break;
            default:
                break;
        }
    }

    // Function to handle arrow key release events
    void specialKeyReleased(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                moveLeftFlag = false;
                break;
            case GLUT_KEY_RIGHT:
                moveRightFlag = false;
                break;
            default:
                break;
        }
    }

};
Character character;

//.........................END Character........................


//----------------------****Enemy****--------------------------------
class Enemy {
private:
    float x;
    float y;
    float speed;
    float size;


public:
    float jumpVelocity;
    float currentJumpVelocity;
    bool isJumping;
    bool moveLeftFlag;
    bool moveRightFlag;
    bool isDead;


    Enemy(float initialX = enemyInitialX, float initialY = enemyInitialY)
        : x(initialX), y(initialY), speed(enemyInitialSpeed), jumpVelocity(initialVelocity), size(charSize), isJumping(false), currentJumpVelocity(0.0f),
          isDead(false), moveLeftFlag(false), moveRightFlag(false) {}

    //Setters
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }
    void setJumpVelocity (float velocity) { jumpVelocity = velocity; }
    void setCurrentJumpVelocity (float velocity) { currentJumpVelocity = velocity; }
    void setCharacterDead(bool dead) { isDead = dead; }
    void setJumping(bool jump) { isJumping = jump; }
    void setSpeed(float speed) {this->speed = speed;}

    //Getters
    float getJumpVelocity () { return jumpVelocity; }
    float getCurrentJumpVelocity () { return currentJumpVelocity; }
    float getX() { return x; }
    float getY() { return y; }
    bool isCharacterDead() const { return isDead; }
    bool getJumping() { return isJumping; }
    int getRespawnDelay() const { return respawnDelay; }
    float getSize() const { return size; }
    bool getMoveLeftFlag() { return moveLeftFlag; }
    bool getMoveRightFlag() { return moveRightFlag; }
    float getSpeed() { return speed; }
    float getWidth() { return size; }
    float getHeight() { return  size; }


    void draw() {
        //glTranslatef(0.0,30.0,0.0);
        float scaleX =150.0f, scaleY =150.0f;
        glBegin(GL_POLYGON); // face
        glColor3f(0.2, 0.7, 0.9);
        glVertex2f(x+0.45f*scaleX, y+0.57f*scaleY);
        glVertex2f(x+0.52f*scaleX, y+0.59f*scaleY);
        glVertex2f(x+0.52f*scaleX, y+0.74f*scaleY);
        glVertex2f(x+0.45f*scaleX, y+0.8f*scaleY);
        glVertex2f(x+0.3f*scaleX, y+0.8f*scaleY);
        glVertex2f(x+0.25f*scaleX, y+0.79f*scaleY);
        glVertex2f(x+0.2f*scaleX, y+0.69f*scaleY);
        glVertex2f(x+0.21f*scaleX, y+0.52f*scaleY);
        glVertex2f(x+0.4f*scaleX, y+0.45f*scaleY);
        glVertex2f(x+0.4f*scaleX, y+0.5f*scaleY);
        glVertex2f(x+0.45f*scaleX, y+0.57f*scaleY);
        glEnd();

        glBegin(GL_POLYGON); // 0.5 face
        glColor3f(1, 1, 1);
        glVertex2f(x+0.4f*scaleX, y+0.45f*scaleY);
        glVertex2f(x+0.4f*scaleX, y+0.5f*scaleY);
        glVertex2f(x+0.45f*scaleX, y+0.57f*scaleY);
        glVertex2f(x+0.52f*scaleX, y+0.59f*scaleY);
        glVertex2f(x+0.57f*scaleX, y+0.59f*scaleY);
        glVertex2f(x+0.57f*scaleX, y+0.54f*scaleY);
        glVertex2f(x+0.52f*scaleX, y+0.5f*scaleY);
        glVertex2f(x+0.499f*scaleX, y+0.45f*scaleY);
        glVertex2f(x+0.4f*scaleX, y+0.45f*scaleY);
        glEnd();
        glBegin(GL_POLYGON); // nose
        glColor3f(0*scaleX, y+0*scaleX, y+0*scaleY);
        glVertex2f(x+0.50*scaleX, y+0.58*scaleY);
        glVertex2f(x+0.52*scaleX, y+0.58*scaleY);
        glVertex2f(x+0.52*scaleX, y+0.56*scaleY);
        glVertex2f(x+0.50*scaleX, y+0.56*scaleY);
        glVertex2f(x+0.51*scaleX, y+0.58*scaleY);

        glEnd();
        glBegin(GL_POLYGON); // eyes
        glColor3f(1, 1, 1);
        glVertex2f(x+0.52*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.52*scaleX, y+0.73*scaleY);
        glVertex2f(x+0.49*scaleX, y+0.73*scaleY);
        glVertex2f(x+0.49*scaleX, y+0.59*scaleY);
        // glVertex2f(x+0.5,0.59*scaleY);
        // glVertex2f(x+0.5,0.73*scaleY);
        glVertex2f(x+0.52*scaleX, y+0.59*scaleY);
        glEnd();
        glBegin(GL_POLYGON); // pupil
        glColor3f(0, 0, 0);

        glVertex2f(x+0.49*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.52*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.52*scaleX, y+0.65*scaleY);
        glVertex2f(x+0.49*scaleX, y+0.65*scaleY);
        glVertex2f(x+0.49*scaleX, y+0.59*scaleY);
        glEnd();

        glBegin(GL_POLYGON); // eyes
        glColor3f(1, 1, 1);
        glVertex2f(x+0.469*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.469*scaleX, y+0.73*scaleY);
        glVertex2f(x+0.43*scaleX, y+0.73*scaleY);
        glVertex2f(x+0.43*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.469*scaleX, y+0.59*scaleY);
        glEnd();

        glBegin(GL_POLYGON); // pupil
        glColor3f(0, 0, 0);
        glVertex2f(x+0.43*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.47*scaleX, y+0.59*scaleY);
        glVertex2f(x+0.47*scaleX, y+0.65*scaleY);
        glVertex2f(x+0.43*scaleX, y+0.65*scaleY);
        glVertex2f(x+0.43*scaleX, y+0.59*scaleY);
        glEnd();
        glBegin(GL_POLYGON); // mouth
        glColor3f(1, 0, 0);
        glVertex2f(x+0.45*scaleX, y+0.49*scaleY);
        glVertex2f(x+0.45*scaleX, y+0.51*scaleY);
        glVertex2f(x+0.51*scaleX, y+0.51*scaleY);
        glVertex2f(x+0.49*scaleX, y+0.49*scaleY);
        glVertex2f(x+0.45*scaleX, y+0.49*scaleY);
        glEnd();
        glBegin(GL_LINE_STRIP); // ear
        glColor3f(0, 0, 0);
        glVertex2f(x+0.37*scaleX, y+0.8*scaleY);
        glVertex2f(x+0.25*scaleX, y+0.87*scaleY);
        glVertex2f(x+0.15*scaleX, y+0.87*scaleY);
        glVertex2f(x+0.24*scaleX, y+0.78*scaleY);

        // glVertex2f(x+0.23*scaleX, y+0.78*scaleY);
        glEnd();
        glBegin(GL_POLYGON); // ear
        glColor3f(0.2, 0.7, 0.9);
        glVertex2f(x+0.37*scaleX, y+0.8*scaleY);
        glVertex2f(x+0.25*scaleX, y+0.87*scaleY);
        glVertex2f(x+0.15*scaleX, y+0.87*scaleY);
        glVertex2f(x+0.24*scaleX, y+0.78*scaleY);
        // glVertex2f(x+0.39*scaleX, y+0.8*scaleY);

        glEnd();
        glBegin(GL_TRIANGLE_FAN); // ear
        glColor3f(0.2, 0.7, 0.9);
        glVertex2f(x+0.25*scaleX, y+0.78*scaleY);
        glVertex2f(x+0.19*scaleX, y+0.82*scaleY);
        glVertex2f(x+-0.01*scaleX, y+0.8*scaleY);
        glVertex2f(x+0.3*scaleX, y+0.7*scaleY);
        glVertex2f(x+0.23*scaleX, y+0.7*scaleY);

        glEnd();
        glBegin(GL_TRIANGLE_FAN); // ear
        glColor3f(0.763, 0.844, 0.48);
        glVertex2f(x+-0.009*scaleX, y+0.8*scaleY);
        glVertex2f(x+0.21*scaleX, y+0.6*scaleY);
        glVertex2f(x+0.22*scaleX, y+0.72*scaleY);
        glVertex2f(x+0.19*scaleX, y+0.78*scaleY);

        glEnd();

        glLineWidth(1);

        glBegin(GL_LINE_STRIP); // m
        glColor3f(0, 0,0);
        glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
        glVertex2f(x+0.6*scaleX, y+0.62*scaleY);
        glEnd();
        glBegin(GL_LINE_STRIP); // m

        glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
        glVertex2f(x+0.4*scaleX, y+0.57*scaleY);

        glEnd();
        glBegin(GL_LINE_STRIP); // m

        glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
        glVertex2f(x+0.4*scaleX, y+0.54*scaleY);

        glEnd();
        glBegin(GL_LINE_STRIP); // m
        glVertex2f(x+0.51*scaleX, y+0.57*scaleY);
        glVertex2f(x+0.6*scaleX, y+0.59*scaleY);
        glEnd();
        glBegin(GL_POLYGON); // tail

        glColor3f(0.2, 0.7, 0.9);

        glVertex2f(x+-0.15*scaleX, y+0.03*scaleY);
        glVertex2f(x+-0.05*scaleX, y+0.1*scaleY);
        glVertex2f(x+-0.24*scaleX, y+0.5*scaleY);

        glVertex2f(x+-0.34*scaleX, y+0.45*scaleY);

        glEnd();
        glBegin(GL_TRIANGLE_FAN); // 0.5 tail
        glColor3f(1, 1, 1);
        glVertex2f(x+-0.35*scaleX, y+0.53*scaleY);

        glVertex2f(x+-0.34*scaleX, y+0.45*scaleY);
        glVertex2f(x+-0.24*scaleX, y+0.5*scaleY);
        glVertex2f(x+-0.3*scaleX, y+0.57*scaleY);
        glVertex2f(x+-0.4*scaleX, y+0.6*scaleY);
        glEnd();
        glBegin(GL_TRIANGLE_FAN); // body
        glColor3f(0.2, 0.7, 0.9);

        glVertex2f(x+0.21*scaleX, y+0.52*scaleY);
        glVertex2f(x+-0.38*scaleX, y+0.01*scaleY);
        glVertex2f(x+-0.38*scaleX, y-0.2*scaleY);
        glVertex2f(x+-0.2*scaleX, y-0.2*scaleY);

        glVertex2f(x+-0.2*scaleX, y-0.1*scaleY);
        glVertex2f(x+0.1*scaleX, y-0.1*scaleY);
        glVertex2f(x+0.1*scaleX, y-0.2*scaleY);
        glVertex2f(x+0.3*scaleX, y-0.2*scaleY);

        glVertex2f(x+0.3*scaleX, y+0.01*scaleY);
        glVertex2f(x+0.3*scaleX, y+0.2*scaleY);
        glVertex2f(x+0.4*scaleX, y+0.5*scaleY);

        glEnd();

        glBegin(GL_TRIANGLE_FAN); // 0.5 leg
        glColor3f(1, 1, 1);

        glVertex2f(x+-0.38*scaleX, y-0.2*scaleY);
        glVertex2f(x+-0.43*scaleX, y-0.25*scaleY);
        glVertex2f(x+-0.38*scaleX, y-0.3*scaleY);
        glVertex2f(x+-0.2*scaleX, y-0.3*scaleY);
        glVertex2f(x+-0.2*scaleX, y-0.2*scaleY);
        glEnd();
        glBegin(GL_TRIANGLE_FAN); // 0.5 leg
        glColor3f(1, 1, 1);

        glVertex2f(x+0.1*scaleX, y-0.2*scaleY);
        glVertex2f(x+0.1*scaleX, y-0.3*scaleY);
        glVertex2f(x+0.3*scaleX, y-0.3*scaleY);
        glVertex2f(x+0.33*scaleX, y-0.25*scaleY);
        glVertex2f(x+0.3*scaleX, y-0.2*scaleY);
        glEnd();
        glBegin(GL_TRIANGLE_FAN); // hand
        glColor3f(0.2, 0.7, 0.9);

        glVertex2f(x+0.3*scaleX, y+0.37*scaleY);
        glVertex2f(x+0.45*scaleX, y+0.19*scaleY);
        glVertex2f(x+0.45*scaleX, y+0.29*scaleY);

        glVertex2f(x+0.3*scaleX, y+0.50*scaleY);

        glEnd();
        glBegin(GL_TRIANGLE_FAN); // 0.5hand
        glColor3f(1, 1, 1);

        glVertex2f(x+0.45*scaleX, y+0.19*scaleY);
        glVertex2f(x+0.45*scaleX, y+0.29*scaleY);
        glVertex2f(x+0.50*scaleX, y+0.22*scaleY);
        glVertex2f(x+0.50*scaleX, y+0.10*scaleY);

        glEnd();
        glLineWidth(7);

        glBegin(GL_LINE_STRIP); // f
        glColor3f(1, 1, 1);
        glVertex2f(x+0.46*scaleX, y+0.20*scaleY);
        glVertex2f(x+0.49*scaleX, y+0.26*scaleY);

        glEnd();
        glLineWidth(1*scaleY);

        glEnd();
        glLineWidth(1);

        glBegin(GL_LINE_STRIP); // f
        glColor3f(0, 0, 0);
        glVertex2f(x+0.48*scaleX, y+0.17*scaleY);
        glVertex2f(x+0.50*scaleX, y+0.16*scaleY);


        glEnd();

        glBegin(GL_POLYGON); // 0.5 body

        glColor3f(0.763, 0.844, 0.48);

        glVertex2f(x+0.35*scaleX, y+0.4*scaleY);
        glVertex2f(x+0.25*scaleX, y+0.4*scaleY);
        glVertex2f(x+-0.1*scaleX, y+0.1*scaleY);

        glVertex2f(x+-0.145*scaleX, y-0.1*scaleY);
        glVertex2f(x+0.1*scaleX, y-0.10*scaleY);
        glVertex2f(x+0.28*scaleX, y+0.2*scaleY);

        glEnd();
        glBegin(GL_TRIANGLE_FAN); // hand
        glColor3f(0.2, 0.7, 0.9);

        glVertex2f(x+0.07*scaleX, y+0.4*scaleY);
        glVertex2f(x+0.37*scaleX, y+0.15*scaleY);
        glVertex2f(x+0.37*scaleX, y+0.27*scaleY);
        glVertex2f(x+0.2*scaleX, y+0.4*scaleY);

        glEnd();
        glBegin(GL_TRIANGLE_FAN); // 0.5hand
        glColor3f(1, 1, 1);
        glVertex2f(x+0.37*scaleX, y+0.15*scaleY);
        glVertex2f(x+0.37*scaleX, y+0.27*scaleY);
        glVertex2f(x+0.43*scaleX, y+0.19*scaleY);
        glVertex2f(x+0.43*scaleX, y+0.05*scaleY);

        glEnd();
    }

    bool ceckJerryCollision(Character character){
        float charLeft = character.getX();
        float charRight = character.getX() + character.getWidth();
        float charTop = character.getY() + character.getHeight();
        float charBottom = character.getY();

        float enemyLeft = getX() - getWidth();
        float enemyRight = getX() + getWidth();
        float enemyTop = getY() + getHeight();
        float enemyBottom = getY() - getHeight();

        // Check if the bounding boxes intersect
        if (enemyRight < charLeft || charLeft > enemyRight || charTop < enemyBottom || charBottom > enemyTop) {
            // Bounding boxes do not intersect, so no collision
            return false;
        }
        else {
            return true;
        }
    }

    void moveLeft() {
        cout<<"tom x: "<<x<<endl;
        cout<<"tom y: "<<y<<endl;
        x -= speed;
        if (x < windowInitialX) {
            x = windowInitialX;
        }
    }

    void moveLefttoFront() {
        cout<<"tom x: "<<x<<endl;
        cout<<"tom y: "<<y<<endl;
        x =x-x+enemyInitialX;
        if (x < windowInitialX) {
            x = windowInitialX;
        }
    }

    void moveRight() {
        cout<<"tom x: "<<x<<endl;
        cout<<"tom y: "<<y<<endl;
        x += speed;
        if(isStage1){
            if (x > 7300) {
                x = 0.0;
                isStage1= false;
                winner=true;
                glutPostRedisplay();
            }
        }
    }

    void jump() {
        if (!isJumping) {
            isJumping = true;
            currentJumpVelocity = jumpVelocity;
        }
    }

};

//---------------------------------****Enemy end*****--------------------

//............................Coin Start.........................
class Coin {
private:
    float x;      // X-coordinate of the coin's center
    float y;      // Y-coordinate of the coin's center
    float radius; // Radius of the coin (considering it's a circle)
    float red;    // Red component of the coin's color
    float green;  // Green component of the coin's color
    float blue;   // Blue component of the coin's color

public:
    // Constructor to initialize the coin
    Coin(float x, float y, float radius, float red, float green, float blue)
    : x(x), y(y), radius(radius), red(red), green(green), blue(blue) {}

    bool isActive = true;
    bool isCollected = false;
    bool collectedCoinFlag;
    bool isCollidingWithCoin = false; // Add a flag to track if character is colliding with a coin
    bool isCollidingWithEnemy = false;
    // Getters for the coin properties
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }
    float getRed() const { return red; }
    float getGreen() const { return green; }
    float getBlue() const { return blue; }

    // Setters for the coin properties
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setRadius(float newRadius) { radius = newRadius; }
    void setRed(float newRed) { red = newRed; }
    void setGreen(float newGreen) { green = newGreen; }
    void setBlue(float newBlue) { blue = newBlue; }


    void draw() const{

        if(isActive && !isCollected) {

            //outer circle
            GLfloat a= x;
            GLfloat b= y;
            GLfloat radius = 20.0f;
            int triangleAmount = 100; //# of lines used to draw circle

            GLfloat twicePi = 2.0f * PI;

            glColor3ub(255,215,0);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(a, b); // center of circle
            for(int i = 0; i <= triangleAmount; i++)
            {
                glVertex2f( a + (radius * cos(i *  twicePi / triangleAmount)),
                            b + (radius * sin(i * twicePi / triangleAmount)) );
            }
            glEnd();

        glPopMatrix();
        }
    }


    // Function to check collision with the coin
        bool checkCollision(Character character) const {
            float charLeft = character.getX();
                float charRight = character.getX() + character.getWidth();
                float charTop = character.getY() + character.getHeight();
                float charBottom = character.getY();

                float coinLeft = getX() - getRadius();
                float coinRight = getX() + getRadius();
                float coinTop = getY() + getRadius();
                float coinBottom = getY() - getRadius();

                // Check if the bounding boxes intersect
                if (charRight < coinLeft || charLeft > coinRight || charTop < coinBottom || charBottom > coinTop) {
                    // Bounding boxes do not intersect, so no collision
                    return false;
                } else {
                    // Bounding boxes intersect, so collision
                    return true;
                }
            }

    // Function to collect the coin and update the score
    void collectCoin() {
        if (isActive) {
            isActive = false; // Set isActive to false for the collected coin
            score++;
            collectedCoinFlag = true;
            isCollidingWithCoin = true; // Set the flag to true to prevent further collisions
            isCollected = true; // Set the flag to indicate the coin is collected
        }
    }

    // Function to check collision between character and coins
    void checkCoinCollision(Character character) {
        // Reset the flag before checking collisions with coins
            isCollidingWithCoin = false;
            if (checkCollision(character)) {
                collectCoin();
            }
    }
};

//.....................................Coin End.....................................


//...................................Obstacle Start.................................

class Obstacle {
private:
    float obsX;
    float obsY;
    float obsWidth;
    float obsHeight;
    float obsArm;
    float colorR;
    float colorG;
    float colorB;
    int d;


public:
    //for rectengale
    Obstacle(float xPos, float yPos, float width, float height, float red, float green, float blue, int d)
        : obsX(xPos), obsY(yPos), obsWidth(width), obsHeight(height), colorR(red), colorG(green), colorB(blue), d(d) {}

    void draw() const{
        // stone with moss
        if(d ==1){
            glColor3f(colorR, colorG, colorB);
            glBegin(GL_POLYGON);
            glVertex2f(obsX, obsY+25);
            glVertex2f(obsX+10.0, obsY);
            glVertex2f(obsX + obsWidth-20.0, obsY);
            glVertex2f(obsX + obsWidth, obsY+10.0);
            glVertex2f(obsX + obsWidth-10.0, obsY+15.0);
            glVertex2f(obsX + obsWidth-20.0, obsY+20.0);
            glVertex2f(obsX + obsWidth/2.0, obsY+obsHeight-10.0);
            glVertex2f(obsX + obsWidth/2.0-10, obsY+obsHeight);
            glVertex2f(obsX + (obsWidth/2.0)-20.0, obsY+obsHeight-10.0);
            glVertex2f(obsX + (obsWidth/2.0)-25.0, obsY+obsHeight-25.0);
            glVertex2f(obsX + 15.0, obsY+obsHeight-60.0);
            glVertex2f(obsX, obsY+25);
            glEnd();

            glColor3f(0.59f, 0.863f, 0.039f);
            glBegin(GL_POLYGON);
            glVertex2f(obsX + obsWidth-20.0, obsY+20.0);
            glVertex2f(obsX + obsWidth/2.0, obsY+obsHeight-10.0);
            glVertex2f(obsX + obsWidth/2.0-10, obsY+obsHeight);
            glVertex2f(obsX + (obsWidth/2.0)-20.0, obsY+obsHeight-10.0);
            glVertex2f(obsX + (obsWidth/2.0)-25.0, obsY+obsHeight-25.0);
            glEnd();
        }

        //small rock
        else if(d==2){
            glColor3f(colorR, colorG, colorB);
            glBegin(GL_POLYGON);
            glVertex2f(obsX, obsY+25);
            glVertex2f(obsX+15.0, obsY);
            glVertex2f(obsX + obsWidth-20.0, obsY);
            glVertex2f(obsX + obsWidth-5.0, obsY+10.0);
            glVertex2f(obsX + obsWidth-10.0, obsY+15.0);
            glVertex2f(obsX + obsWidth-20.0, obsY+20.0);
            glVertex2f(obsX + obsWidth/2.0, obsY+obsHeight-10.0);
            glVertex2f(obsX + obsWidth/2.0-10, obsY+obsHeight);
            glVertex2f(obsX + (obsWidth/2.0)-20.0, obsY+obsHeight-10.0);
            glVertex2f(obsX + (obsWidth/2.0)-25.0, obsY+obsHeight-30.0);
            glVertex2f(obsX + 15.0, obsY+obsHeight-60.0);
            glVertex2f(obsX, obsY+25);
            glEnd();
        }
         //rectangle
        else if(d ==3){
            glColor3f(colorR, colorG, colorB);
            glBegin(GL_POLYGON);
            glVertex2f(obsX, obsY);
            glVertex2f(obsX + obsWidth, obsY);
            glVertex2f(obsX + obsWidth, obsY + obsHeight);
            glVertex2f(obsX, obsY + obsHeight);
            glEnd();
        }
        //fire hydrant
        else if(d ==4){
            glColor3f(0.0468, 0.480, 0.0240);
            glBegin(GL_POLYGON);
            glVertex2f(obsX, obsY);
            glVertex2f(obsX + 10, obsY+obsHeight-20);
            glVertex2f(obsX + 15, obsY+obsHeight-10);
            glVertex2f(obsX + 20, obsY+obsHeight);
            glVertex2f(obsX + obsWidth-20, obsY+obsHeight);
            glVertex2f(obsX + obsWidth-15, obsY+obsHeight-10);
            glVertex2f(obsX + obsWidth-10, obsY+obsHeight-20);
            glVertex2f(obsX + obsWidth, obsY);
            glVertex2f(obsX, obsY);
            glEnd();

            glEnd();
        }

        // tree wood part
        else if(d ==5){
            glColor3f(0.490, 0.245, 0.00);
            glBegin(GL_POLYGON);
            glVertex2f(obsX, obsY+obsHeight);
            glVertex2f(obsX + 10, obsY+obsHeight+10);
            glVertex2f(obsX + obsWidth/2-20, obsY+obsHeight-2);
            glVertex2f(obsX + obsWidth/2, obsY+obsHeight-5);
            glVertex2f(obsX + obsWidth/2+20, obsY+obsHeight-2);
            glVertex2f(obsX + obsWidth-10, obsY+obsHeight-10);
            glVertex2f(obsX + obsWidth, obsY+obsHeight);
            glVertex2f(obsX + obsWidth-20, obsY+obsHeight-20);
            glVertex2f(obsX + obsWidth-20, obsY+obsHeight-30);
            glVertex2f(obsX + obsWidth+5, obsY-10);
            glVertex2f(obsX + obsWidth-10, obsY);
            glVertex2f(obsX + obsWidth/2+20, obsY+2);
            glVertex2f(obsX + obsWidth/2, obsY+5);
            glVertex2f(obsX + obsWidth/2-20, obsY+2);
            glVertex2f(obsX , obsY);
            glVertex2f(obsX, obsY+obsHeight);

            glEnd();

        }

    }

    // Add methods to set and get color components
        void setColor(float red, float green, float blue) {
            colorR = red;
            colorG = green;
            colorB = blue;
        }

        bool checkCollision(Character character) const {
        float charLeft = character.getX();
        float charRight = character.getX() + character.getWidth();
        float charTop = character.getY() + character.getHeight();
        float charBottom = character.getY();

        float obstacleLeft = getX();
        float obstacleRight = getX() + getWidth();
        float obstacleTop = getY() + getHeight();
        float obstacleBottom = getY();

        if (charRight < obstacleLeft || charLeft > obstacleRight || charTop < obstacleBottom || charBottom > obstacleTop) {
            return false;
        } else {
            return true;
        }

    }

    float getX() const { return obsX; }
    float getY() const { return obsY; }
    float getWidth() const { return obsWidth; }
    float getHeight() const { return obsHeight; }
};

class River {
private:
    float rivX;
    float rivY;
    float rivWidth;
    float rivHeight;
    float colorR;
    float colorG;
    float colorB;

public:
    River(float xPos, float yPos, float width, float height, float red, float green, float blue)
        : rivX(xPos), rivY(yPos), rivWidth(width), rivHeight(height), colorR(red), colorG(green), colorB(blue) {}

    void draw() const {
        glColor3f(colorR, colorG, colorB);

        glBegin(GL_QUADS);
        glVertex2f(rivX, rivY);
        glVertex2f(rivX + rivWidth, rivY);
        glVertex2f(rivX + rivWidth, rivY + rivHeight);
        glVertex2f(rivX, rivY + rivHeight);
        glEnd();
    }

        void setColor(float red, float green, float blue) {
            colorR = red;
            colorG = green;
            colorB = blue;
        }

    // Function to check if there is a collision between the character and the river
    bool checkRiverCollision(Character character) const{
        // Check if the character's position is within the river area
        if (character.getY() <= (rivY + rivHeight) && character.getX()  >= rivX && character.getX() <= (rivX + rivWidth-character.getSize())){
            return true;
        }
        else {

            return false; // No collision
        }
    }

};

//............................................River End....................................

//.......................................Stage Component Start........................

void drawHill(float x, float hillHeight, float hillWidth) {
    glPushMatrix();
    glTranslatef(x, 101.0f, 0.0f);

    int numPoints = 100;
    float step = 2.0f * hillWidth / numPoints;

    // Draw the black border around the hill
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numPoints; ++i) {
        float t = (float) (i / numPoints);
        float angle = t * 3.14159f;
        float yOffset = hillHeight * sin(angle);

        // Use black color for the border
        glColor3f(0.0f, 0.0f, 0.0f);

        // Outer border vertices
        glVertex2f(i * step, -yOffset - 3.0f);
        glVertex2f(i * step, yOffset + 3.0f);
    }
    glEnd();

    // Draw the inner hill
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numPoints; ++i) {
        float t = (float) (i / numPoints);
        float angle = t * 3.14159f;
        float yOffset = hillHeight * sin(angle);

        // Use green color for the hill
        glColor3f(0.5f, 0.7f, 0.3f);

        // Inner hill vertices (adjusted for the border)
        glVertex2f(i * step, -yOffset);
        glVertex2f(i * step, yOffset);
    }
    glEnd();

    glPopMatrix();
}


void drawCloud(float x, float y) {
    int numSegments = 20; // Number of segments for the cloud circle
    float cloudRadius = 30.0f; // Radius of the cloud circle

    glColor3ub(255, 255, 255); // White color for clouds

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of the cloud
    for (int j = 0; j <= numSegments; ++j) {
        float angle = j * (360.0f / numSegments);
        float cloudX = x + cloudRadius * cos(angle * PI / 180.0);
        float cloudY = y + cloudRadius * sin(angle * PI / 180.0);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();

    // Draw more circles to create the cloud shape
    // Add more circles and adjust their positions and radii to customize the cloud shape
    numSegments = 20;
    cloudRadius = 50.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 60, y); // Center of the second circle
    for (int j = 0; j <= numSegments; ++j) {
        float angle = j * (360.0f / numSegments);
        float cloudX = x + 60 + cloudRadius * cos(angle * PI / 180.0);
        float cloudY = y + cloudRadius * sin(angle * PI / 180.0);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();

    numSegments = 20;
    cloudRadius = 40.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 120, y); // Center of the third circle
    for (int j = 0; j <= numSegments; ++j) {
        float angle = j * (360.0f / numSegments);
        float cloudX = x + 120 + cloudRadius * cos(angle * PI / 180.0);
        float cloudY = y + cloudRadius * sin(angle * PI / 180.0);
        glVertex2f(cloudX, cloudY);
    }
    glEnd();
}


// Function to draw the ground (bricks and lines)
void drawGround(float maxXPosition) {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, 0.0f, 0.0f); // Adjust the translation if needed

    // Draw the bricks (assuming each brick has a width of 50 units)
    glColor3f(0.545f, 0.271f, 0.075f); // Brown color for bricks
    for (float x = -700.0f; x <= maxXPosition; x += 50.0f) {
        glBegin(GL_QUADS);
        glVertex2f(x, 0.0f);
        glVertex2f(x + 50.0f, 0.0f);
        glVertex2f(x + 50.0f, 90.0f);
        glVertex2f(x, 90.0f);
        glEnd();
    }
    // upper border
    glColor3f(0.59f, 0.863f, 0.039f);
    glBegin(GL_QUADS);
    glVertex2f(-700.0f, 90.0f);
    glVertex2f(8000.0f, 90.0f);
    glVertex2f(8000.0f, 100.0f);
    glVertex2f(-700.0f, 100.0f);
    glEnd();


    glColor3f(0.5f, 0.5f, 0.5f); // Gray color
    float scaleX =0.4;
    float scaleY =0.4;
    int i=0;



    for (float x = -700.0f; x <= maxXPosition; x += 250.0f) {
        set_ft_values(35,0.0f,60.0f);
        //glRotatef(80.5, 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(x+100 * scaleX, ft[i]+100 * scaleY);
        glVertex2f(x+120 * scaleX, ft[i]+120 * scaleY);
        glVertex2f(x+140 * scaleX, ft[i]+100 * scaleY);
        glVertex2f(x+160 * scaleX, ft[i]+120 * scaleY);
        glVertex2f(x+180 * scaleX, ft[i]+100 * scaleY);
        glVertex2f(x+160 * scaleX, ft[i]+80 * scaleY);
        glVertex2f(x+140 * scaleX, ft[i]+60 * scaleY);
        glVertex2f(x+120 * scaleX, ft[i]+80 * scaleY);
        glEnd();
        i++;
    }


    glPopMatrix(); // Restore the previous matrix state
}

void drawGround2(float maxXPosition) {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, 0.0f, 0.0f); // Adjust the translation if needed

    // Draw the bricks (assuming each brick has a width of 50 units)
    glColor3f(0.115,0.16,0.20); // Brown color for bricks
    for (float x = 0.0f; x <= maxXPosition; x += 50.0f) {
        glBegin(GL_QUADS);
        glVertex2f(x, 0.0f);
        glVertex2f(x + 50.0f, 0.0f);
        glVertex2f(x + 50.0f, 100.0f);
        glVertex2f(x, 100.0f);
        glEnd();
    }

    // Draw the lines between bricks along x-axis
    glLineWidth(3.0f); // Increase the line width to make it bolder
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for lines
    glBegin(GL_LINES);
    for (float x = 0.0f; x <= maxXPosition; x += 50.0f) {
        glVertex2f(x, 0.0f);
        glVertex2f(x, 100.0f);
    }
    glEnd();

    // Draw the lines between bricks along y-axis
    glBegin(GL_LINES);
    for (float y = 0.0f; y <= 100.0f; y += 50.0f) {
        glVertex2f(0.0f, y);
        glVertex2f(maxXPosition, y);
    }
    glEnd();

    glPopMatrix(); // Restore the previous matrix state
}

void drawBorder(float x1, float y1, float x2, float y2, float borderWidth) {

    // Draw the black border around the obstacle
    glLineWidth(borderWidth); // Set the desired line width for the border
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the border
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawBrickLines(float minX, float minY, float maxX, float maxY, float brickWidth, float brickHeight, float lineWidth) {
    glPushMatrix(); // Save the current matrix state
    glTranslatef(0.0f, 0.0f, 0.0f); // Adjust the translation if needed

    // Draw the lines between bricks along x-axis
    glLineWidth(lineWidth); // Set the desired line width
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for lines
    glBegin(GL_LINES);
    for (float x = minX; x <= maxX; x += brickWidth) {
        glVertex2f(x, minY);
        glVertex2f(x, maxY);
    }
    glEnd();

    // Draw the lines between bricks along y-axis
    glBegin(GL_LINES);
    for (float y = minY; y <= maxY; y += brickHeight) {
        glVertex2f(minX, y);
        glVertex2f(maxX, y);
    }
    glEnd();

    glPopMatrix(); // Restore the previous matrix state
}

void drawJerryHouseGate(float x, float y, float width, float height, float r, float g, float b) {
    // Set the color for the gate
    glColor3f(r, g, b);

    // Draw the gate using rectangles and a filled arch
    glBegin(GL_QUADS);
    // Draw the main gate structure
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the filled arch above the gate
    float centerX = x + width / 2.0f;
    float centerY = y + height;
    float radius = width / 2.0f;

    int numSegments = 100;
    float angleStep = 3.14159f / numSegments;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Center vertex of the fan

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x1 = centerX + radius * cos(angle);
        float y1 = centerY + radius * sin(angle);
        glVertex2f(x1, y1);
    }

    glEnd();
}

//..........................................Stage Component End......................................

// ......................................Creating Necessary Objects................................


    Enemy tom;

    vector<Obstacle> obstacles;
    vector<River> rivers;


    // Stage 01 Coins
    Coin coin1(215.0f,130.0f, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin2(285, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin3(950, 320, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin4(1395, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin5(1600, 510, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin6(1880, 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin7(2650, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin8(3280, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin9(3785, 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin10(4372, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin11(4950, 130, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin12(5000, 300, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin13(5050, 130, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin14(5325, 462, 20.0f, 1.0f, 0.843f, 0.0f);
    Coin coin15(5770, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin16(6095, 300, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin
    Coin coin17(6605, 130, 20.0f, 1.0f, 0.843f, 0.0f); // Yellow coin


    //..................................Object Creation End............................

    //....................... Gathering every piece together to complete the game................

    void cameraMovement(){

    // Calculate the camera's position to center the stage around the character
        float cameraX = -(character.getX() - windowFinalX / 2);
        float cameraY = 0.0f; // Keep a fixed Y position for the camera
        float cameraZ = 0.0f; // Set a suitable Z position for the camera (you can adjust this based on your scene)

        // Apply the translation transformation
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(cameraX, cameraY, cameraZ);
}


// Function to check collision with all the obstacles and the river
bool checkAllObstacleCollisions(Character character) {
    for (const auto& obstacle : obstacles) {
        if (obstacle.checkCollision(character)) {
            return true;
        }
    }
    return false; // No collision
}


bool checkAllRiverCollisions(Character character) {
    for (const auto& river : rivers){
        if (river.checkRiverCollision(character)){
            return true;
        }
    }
    return false; // No Collision
}


void respawnCharacter(int value) {
    lives--;



    deadPosition = character.getX();
    character.setCharacterDead(false);
    character.setX(charInitialX);
    character.setY(charInitialY);
    character.setJumping(false);
    character.setCurrentJumpVelocity(0.0f);
    glutPostRedisplay(); // Mark the window for a redraw
}
void respawnEnemy(int value) {
    deadPosition = tom.getX();
    tom.setCharacterDead(false);
    tom.setX(enemyInitialX);
    tom.setY(enemyInitialY);
    tom.setJumping(false);
    tom.setCurrentJumpVelocity(0.0f);
    glutPostRedisplay(); // Mark the window for a redraw
}


//Function to show Scoreboard
void drawScoreboard(int characterX) {
    int scoreboardX = characterX - windowFinalX / 2;
    int scoreboardY = 650; // Adjust this as needed

    // Draw the scoreboard at the calculated position
    drawText("Score: " + to_string(score), scoreboardX, scoreboardY, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
}


//Function to show Lives
void drawLives(int characterX)
{
    int livesX = characterX - windowFinalX / 2;
    int livesY = 690; // Adjust this as needed


    drawText("Lives: " + to_string(lives), livesX, livesY, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
}


//Update function
void update(int value) {
    // Store the current character position for collision checking
    float prevCharacterX = character.getX();
    float prevCharacterY = character.getY();


    if(isStage1){ // Stage 01 Coins
        if(tom.ceckJerryCollision(character)){
            deadPosition =character.getX();
            //character.isDead =true;
            lives =0;
        }
        coin1.checkCoinCollision(character);
        coin2.checkCoinCollision(character);
        coin3.checkCoinCollision(character);
        coin4.checkCoinCollision(character);
        coin5.checkCoinCollision(character);
        coin6.checkCoinCollision(character);
        coin7.checkCoinCollision(character);
        coin8.checkCoinCollision(character);
        coin9.checkCoinCollision(character);
        coin10.checkCoinCollision(character);
        coin11.checkCoinCollision(character);
        coin12.checkCoinCollision(character);
        coin13.checkCoinCollision(character);
        coin14.checkCoinCollision(character);
        coin15.checkCoinCollision(character);
        coin16.checkCoinCollision(character);
        coin17.checkCoinCollision(character);
    }


    // Move the character based on the current direction
    if (character.getMoveLeftFlag()) {
        character.moveLeft();
    }
    if (character.getMoveRightFlag()) {
        character.moveRight();
    }
    if(character.getX()>tom.getX()){
        tom.moveRight();
    }
    if(character.getX()<tom.getX()){
        tom.moveLeft();
    }
    if(character.getX()==tom.getX()){
        character.setCharacterDead(true);
    }

    // Check for collision with obstacles and handle accordingly
    bool isObstacleCollision = checkAllObstacleCollisions(character);
    if (isObstacleCollision) {
        // Restore the previous character position
        character.setX(prevCharacterX);
    }

    // Check for collision with the river, and handle accordingly
    bool isRiverColliding = checkAllRiverCollisions(character);
    if (isRiverColliding && !character.isCharacterDead()) {
        character.setY(character.getY() - gravity);
        glutTimerFunc(100, respawnEnemy, 0);
    }

    // Apply gravity if jumping
    if (character.getJumping()) {
        // Update the vertical position with projectile motion
        character.setY(character.getY() + character.getCurrentJumpVelocity());

        // Check if the character reaches the ground or an obstacle
        if (checkAllObstacleCollisions(character)) {
            // If on top of an obstacle, adjust the vertical position to stay on top
            character.setY(prevCharacterY);
            character.setJumping(false);
            character.setCurrentJumpVelocity(0.0f); // Reset jump velocity
        } else {
            character.setCurrentJumpVelocity(character.getCurrentJumpVelocity() - gravity);

            if (checkAllRiverCollisions(character) && !character.isCharacterDead()){
                character.setY(character.getY()-gravity);
                character.setCharacterDead(true);
                glutTimerFunc(respawnDelay, respawnCharacter, 0);
            }
            else if (character.getY() <= charInitialY){
                character.setY(charInitialY);
                character.setJumping(false);
                character.setCurrentJumpVelocity(0.0f); // Reset jump velocity
            }
        }
    } else {
            // If not jumping, apply continuous gravity to make the character fall
            // Adjust the gravity value to achieve the desired falling speed
            character.setY(character.getY()- gravity * 5);

        // Check for collision with obstacles or River below the character
        bool isOnObstacle = checkAllObstacleCollisions(character);
        bool isOnRiver = checkAllRiverCollisions(character);

        if (isOnObstacle) {
            // If on top of an obstacle, adjust the vertical position to stay on top
            character.setY(prevCharacterY);
        }

        // Check if on top of river
        if (isOnRiver) {
            // Character has died, reset its position to the starting position

            character.setY(character.getY() - 0.5f);
            character.isDead = true;
            glutTimerFunc(respawnDelay, respawnCharacter, 0);
            glutTimerFunc(respawnDelay, respawnEnemy, 0);
            character.isJumping = false;
            character.currentJumpVelocity = 0.0f;

        }
        // Check if the character reaches the ground level
        if ((!isOnRiver) && character.getY() <= charInitialY) {
            character.setY(charInitialY); // Snap to the ground level
        }
    }

    glutPostRedisplay(); // Mark the window for a redraw
    glutTimerFunc(16, update, 0); // Call moveCharacter function after 16 milliseconds
}


// Function to add a new obstacle
void addObstacle(float xPos, float yPos, float width, float height, float red, float green, float blue, int d) {
    obstacles.emplace_back(xPos, yPos, width, height, red, green, blue,d);
}


// Function to add a new river
void addRiver(float xPos, float yPos, float width, float height, float red, float green, float blue) {
    rivers.emplace_back(xPos, yPos, width, height, red, green, blue);
}



//Function to draw obstacles
void drawAllObstacleObjects(){
    // Draw the obstacles
    for (const auto& obstacle : obstacles) {
        obstacle.draw();
    }
}

void drawAllRiverObjects(){
    // Draw the rivers
    for (const auto& river : rivers) {
        river.draw();
    }
}
void draw_tree(float obsX, float obsY, float scale)
{
    glColor3f(0.0162, 0.540, 0.217);
    glBegin(GL_TRIANGLES);
    glVertex2f(obsX,obsY+20*scale);
    glVertex2f(obsX+30*scale,obsY+50*scale);
    glVertex2f(obsX+60*scale,obsY+20*scale);
    glEnd();

    glColor3f(0.0162, 0.540, 0.217);
    glBegin(GL_TRIANGLES);
    glVertex2f(obsX+5*scale,obsY+40*scale);
    glVertex2f(obsX+30*scale,obsY+70*scale);
    glVertex2f(obsX+55*scale,obsY+40*scale);
    glEnd();

    glColor3f(0.0162, 0.540, 0.217);
    glBegin(GL_TRIANGLES);
    glVertex2f(obsX+15*scale,obsY+60*scale);
    glVertex2f(obsX+30*scale,obsY+90*scale);
    glVertex2f(obsX+45*scale,obsY+60*scale);
    glEnd();

    glColor3f(0.510, 0.247, 0.00);
    glBegin(GL_POLYGON);
    glVertex2f(obsX+25*scale,obsY+20*scale);
    glVertex2f(obsX+35*scale,obsY+20*scale);
    glVertex2f(obsX+35*scale,obsY);
    glVertex2f(obsX+25*scale,obsY);
    glEnd();
    //glFlush();


}

//---------------------------- STAGE 01 --------------------------------------------------------


void drawStage01(){

    draw_cloud1(-600,600);
    draw_cloud(-300,450);
    drawCloud(200,600);
    draw_cloud1(700,600);
    draw_cloud1(1100,450);
    draw_cloud(1600,450);
    drawCloud(2200,600);
    draw_cloud1(2500,500);
    draw_cloud1(2900,400);
    draw_cloud1(3200,650);
    draw_cloud(3400,450);
    draw_cloud1(3900,600);
    draw_cloud1(4300,550);
    draw_cloud1(4500,400);
    drawCloud(5000,400);
    draw_cloud(5500,650);
    draw_cloud1(6000,450);
    draw_cloud1(6400,600);
    drawCloud(6800,450);
    drawCloud(7400,500);
    draw_cloud(8000,400);



    //sound();
    isStage1 = true;
    draw_tree(0.0,100.0,4.0);
    draw_tree(500.0,100.0,6.0);
    draw_tree(1300,100,3.0);
    draw_tree(1690,100,5.0);
    draw_tree(2700,100,6.0);
    draw_tree(3300,100,7.0);
    draw_tree(4300,100,5.0);
    draw_tree(5100,100,4.0);
    draw_tree(5800,100,3.0);
    draw_tree(6800,100,7.0);
    draw_tree(7600,100,4.0);

    drawAllObstacleObjects();


    addObstacle(400, 101, 80, 100, 0.440, 0.378, 0.378,2);
    addObstacle(911, 101, 150, 147, 0.440, 0.378, 0.378,1);
    addObstacle(1210, 300, 200, 50, 0.440, 0.378, 0.378,5);
    addObstacle(1511, 101, 100, 147, 0.440, 0.378, 0.378,3);
    addObstacle(1510, 400, 200, 50, 0.440, 0.378, 0.378,5);

    addObstacle(1828, 101, 150, 147, 0.440, 0.378, 0.378,1);
    addObstacle(2200, 101, 200, 50, 0.440, 0.378, 0.378,5);
    addObstacle(2380, 101, 100, 80, 0.440, 0.378, 0.378,4);

    addObstacle(3511, 101, 100, 80, 0.440, 0.378, 0.378,3);
    addObstacle(3728, 101, 150, 147, 0.440, 0.378, 0.378,1);
    addObstacle(4000, 101, 80, 100, 0.440, 0.378, 0.378,2);
    addObstacle(4500, 101, 200, 50, 0.440, 0.378, 0.378,5);
    addObstacle(4700, 300, 200, 50, 0.440, 0.378, 0.378,5);

    addObstacle(5000, 300, 200, 50, 0.440, 0.378, 0.378,5);
    addObstacle(5300, 101, 150, 147, 0.440, 0.378, 0.378,1);
    addObstacle(6050, 101, 150, 147, 0.440, 0.378, 0.378,1);
    addObstacle(6300, 101, 200, 50, 0.440, 0.378, 0.378,5);
    addObstacle(6500, 300, 200, 50, 0.440, 0.378, 0.378,5);
    addObstacle(6750, 101, 80, 100, 0.440, 0.378, 0.378,2);
    addObstacle(7000, 101, 150, 147, 0.440, 0.378, 0.378,1);





    addRiver(3000,0, 100, 102, 0.55f, 0.53f, 0.94f);
    addRiver(4100 ,0, 100, 102, 0.55f, 0.53f, 0.94f);
    addRiver(5500, 0, 100, 102, 0.55f, 0.53f, 0.94f);


    drawJerryHouseGate(7350, 101, 100, 60, 0, 0, 0);

    drawGround(8000.0f);

    drawAllRiverObjects();     //MUST BE CALLED AFTER CREATING GROUND If  you have rivers call this function

    coin1.draw();
    coin2.draw();
    coin3.draw();
    coin4.draw();
    coin5.draw();
    coin6.draw();
    coin7.draw();
    coin8.draw();
    coin9.draw();
    coin10.draw();
    coin11.draw();
    coin12.draw();
    coin13.draw();
    coin14.draw();
    coin15.draw();
    coin16.draw();
    coin17.draw();


    character.draw();
    tom.draw();

    drawScoreboard(character.getX());
    drawLives(character.getX());
}



void renderBitmapString(float x, float y, float z, void *font, char *string)
{
    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}


//Exit Function
void exit1(int value)
{
    exit(0);
    glutTimerFunc(10,exit1,0);
}

// Function to handle the display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if(lives >0){

        if(isStage1){
            cameraMovement();
            drawStage01();
        }

        else if(winner){
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black
            glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)

            glColor3f(1.0,0.0,0.0);
            renderBitmapString(7100, 400.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Congratulation! You Won");
            drawText("Score: " + to_string(score), 7100, 360.0f, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
            glFlush(); // Render now

            glutTimerFunc(9000,exit1,0);
        }
    }
    else{
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)

        glColor3f(1.0,0.0,0.2);
        renderBitmapString(deadPosition, 400.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
        drawText("Score: " + to_string(score), deadPosition, 360.0f, GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
        glFlush(); // Render now

        glutTimerFunc(9000,exit1,0);
    }
    glutSwapBuffers();
}


//Play Sound In The Background
void sound()    {
    if(playMainBgSound)
        PlaySound("C:\\Users\\saiya\\Downloads\\level 2\\level 2\\bin\\Release\\a.wav", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);
}


//Function to handle key presses
void specialKeyPressed (int key, int x, int y){
    character.specialKeyPressed(key, x, y);
}


//Function to handle key releases
void specialKeyReleased (int key, int x, int y){
    character.specialKeyReleased(key, x, y);
}


// Function to set up the OpenGL environment
void init() {
    glClearColor(0.529, 0.808, 0.980, 1.0f); // Set the background color to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, coordinateX, 0, coordinateY); // Set the coordinate system
}


// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

//........................Drawing Complete.....................

//.............................Driver code or main function Start.........................

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowFinalX, windowFinalY);
    glutCreateWindow("Tom & Jerry Run");
    init();
    sound();
    //glutKeyboardFunc(keyboard); //Set Menu Keyboard Function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0); // Start the character movement timer
    glutSpecialFunc(specialKeyPressed); // Set the special key callback function
    glutSpecialUpFunc(specialKeyReleased); // Set the special key release callback function
    glutMainLoop();
    return 0;
}

//.......................................Main Function End...............................
