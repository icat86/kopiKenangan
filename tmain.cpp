#include <GL/glut.h>
#include <math.h>
#include <iostream>
using namespace std;

#define FPS 120
#define TO_RADIANS 3.14/180.0

const double PI = 3.141592653589793;

const int width = 1280;
const int height = 720;

float blockLength = 200.0f;
float blockWidth = 200.0f;
float blockHeight = 200.0f;

int i;
float sudut;
double x_geser, y_geser, z_geser;

//posisi kamera
float pitch = 0.0, yaw = 0.0;                   //bawah-atas, kiri-kanan
float camX = 0, camZ = 800, terbang = -360;    //camX = 0, camZ = 1000, terbang = -360;

void display();
void reshape(int w, int h);
void timer(int);
void passive_motion(int, int);
void camera();void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);

struct Motion {
    bool Forward, Backward, Left, Right, Naik, Turun;
};
Motion motion = { false,false,false,false,false,false };

void init() {
    glClearColor(0.7, 0.99, 0.99, 0.0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glutWarpPointer(width / 2, height / 2);
}

void kotak(double panjang, double lebar, double tinggi) {
    glScalef(panjang, lebar, tinggi);
    glutSolidCube(1);
}

void cylinder(float cx, float cy, float cz, float radius, float height, int num_segments) {
    float halfHeight = height / 2.0f;

    // Draw the sides of the cylinder
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        glVertex3f(x + cx, cy - halfHeight, z + cz);
        glVertex3f(x + cx, cy + halfHeight, z + cz);
    }
    glEnd();

    // Draw the top circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy + halfHeight, cz);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);
        glVertex3f(x + cx, cy + halfHeight, z + cz);
    }
    glEnd();

    // Draw the bottom circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy - halfHeight, cz);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);
        glVertex3f(x + cx, cy - halfHeight, z + cz);
    }
    glEnd();
}

void vcylinder(float cx, float cy, float cz, float radius, float height, int num_segments) {
    float halfHeight = height / 2.0f;

    // Draw the sides of the cylinder
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        glVertex3f(x + cx, y + cy, cz - halfHeight);
        glVertex3f(x + cx, y + cy, cz + halfHeight);
    }
    glEnd();

    // Draw the top circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, cz + halfHeight);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex3f(x + cx, y + cy, cz + halfHeight);
    }
    glEnd();

    // Draw the bottom circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, cz - halfHeight);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex3f(x + cx, y + cy, cz - halfHeight);
    }
    glEnd();
}

void kalimat(float x, float y, float z, void* font, const char* string, float scale, float thickness) {
    const char* c;
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);  // text size
    glLineWidth(thickness);         // Set the text thickness
    for (c = string; *c != '\0'; c++) {
        glutStrokeCharacter(font, *c);
    }
    glPopMatrix();
}

void luv(float radius, float height) {
    int num_segments = 100;
    float halfHeight = height / 2.0f;

    // Draw the sides of the heart
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * 16 * pow(sinf(theta), 3);
        float y = radius * (13 * cosf(theta) - 5 * cosf(2 * theta) - 2 * cosf(3 * theta) - cosf(4 * theta));

        glVertex3f(x, y, -halfHeight);
        glVertex3f(x, y, halfHeight);
    }
    glEnd();

    // Draw the top surface
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, halfHeight);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * 16 * pow(sinf(theta), 3);
        float y = radius * (13 * cosf(theta) - 5 * cosf(2 * theta) - 2 * cosf(3 * theta) - cosf(4 * theta));
        glVertex3f(x, y, halfHeight);
    }
    glEnd();

    // Draw the bottom surface
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, -halfHeight);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * PI * float(i) / float(num_segments);
        float x = radius * 16 * pow(sinf(theta), 3);
        float y = radius * (13 * cosf(theta) - 5 * cosf(2 * theta) - 2 * cosf(3 * theta) - cosf(4 * theta));
        glVertex3f(x, y, -halfHeight);
    }
    glEnd();
}

void ground() {
    glBegin(GL_QUADS);
    glColor3f(0.4, 0.4, 0.4);
    glVertex3f(-1000.0, -500, -2500.0);         //kiri belakang
    glVertex3f(1000.0, -500, -2500.0);          //kanan belakang
    glVertex3f(1000.0, -500, 1500.0);           //kanan depan
    glVertex3f(-1000.0, -500, 1500.0);          //kiri depan
    glEnd();
}

void tangga_masuk() {
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslatef(0, -100 + 5 * i, 110 - 10 * i);
        kotak(400, 5, 10);
        glPopMatrix();
    }
}

void tangga_lnt2() {
    int num_steps = 8;
    float initial_height = 188;
    float height_decrement = 25;
    float initial_depth = -1000;
    float depth_increment = 30;
    float x_position = 145;
    float step_width = 110;
    float step_height = 25;
    float step_depth = 30;

    for (int i = 0; i < num_steps; i++) {
        float height = initial_height - i * height_decrement;
        float depth = initial_depth - i * depth_increment;
        glColor3f(i % 2 == 0 ? 0.0f : 1.0f, 0.0f, 0.0f);

        glPushMatrix();
        glTranslatef(x_position, height, depth);
        kotak(step_width, step_height, step_depth);
        glPopMatrix();
    }

    //glColor3f(0.0, 0.0, 0.0);
    //kotak belok kanan
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(145, -12, -1280);
    kotak(110, 25, 110);                //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(80, -37, -1280);
    kotak(25, 25, 110);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(55, -62, -1280);
    kotak(25, 25, 110);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(30, -87, -1280);
    kotak(25, 25, 110);
    glPopMatrix();


}

void tangga_lt2(){      //useless
    //tangga ke lt2
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(145, 188, -1000);          //tinggi tiap tinggi -25
    kotak(110, 25, 30);                     //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(145, 163, -1030);
    kotak(110, 25, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(145, 138, -1060);
    kotak(110, 25, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(145, 113, -1090);
    kotak(110, 25, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(145, 88, -1120);
    kotak(110, 25, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(145, 63, -1150);
    kotak(110, 25, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(145, 38, -1180);
    kotak(110, 25, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(145, 13, -1210);
    kotak(110, 25, 30);
    glPopMatrix();

    //kotak belok kanan
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(145, -12, -1280);
    kotak(110, 25, 110);
    glPopMatrix();
}

void meja1(){
    float radius1 = 30.0f;              // lebar
    float height1 = 3.0f;               // tinggi
    float radius2 = 1.0f;
    float height2 = 62.0f;

    //atas meja
    glColor3f(0.44, 0.26, 0.15);
    cylinder(-160.0f, -20.0f, -600.0f, radius1, height1, 50);

    //kaki meja
    glColor3f(0.0, 0.0, 0.0);
    cylinder(-160.0f, -52.0f, -600.0f, radius2, height2, 50);

    //bawah meja
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -600);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -600);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

}

void meja_lt2(){

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 301, 900);
    meja1();
    glPopMatrix();
    glEnd();

}

void meja(){                //manual hadeh
    float radius1 = 30.0f;              // lebar
    float height1 = 3.0f;               // tinggi
    float radius2 = 1.0f;
    float height2 = 62.0f;

    //sisi kiri
    glColor3f(0.44, 0.26, 0.15);
    //cylinder(-160.0f, -20.0f, -600.0f, radius1, height1, 50);
    cylinder(-160.0f, -20.0f, -900.0f, radius1, height1, 50);
    cylinder(-160.0f, -20.0f, -1200.0f, radius1, height1, 50);
    cylinder(-160.0f, -20.0f, -1500.0f, radius1, height1, 50);
    //cylinder(-90.0f, -20.0f, -1500.0f, radius1, height1, 50);
    cylinder(-160.0f, -20.0f, -1800.0f, radius1, height1, 50);
    cylinder(-90.0f, -20.0f, -1800.0f, radius1, height1, 50);
    cylinder(-160.0f, -20.0f, -2100.0f, radius1, height1, 50);
    cylinder(-90.0f, -20.0f, -2100.0f, radius1, height1, 50);

    //sisi kanan
    cylinder(170.0f, -20.0f, -1800.0f, radius1, height1, 50);
    cylinder(100.0f, -20.0f, -1800.0f, radius1, height1, 50);
    cylinder(170.0f, -20.0f, -2100.0f, radius1, height1, 50);
    cylinder(100.0f, -20.0f, -2100.0f, radius1, height1, 50);
    cylinder(170.0f, -20.0f, -2400.0f, radius1, height1, 50);
    cylinder(100.0f, -20.0f, -2400.0f, radius1, height1, 50);


    //kaki meja
    glColor3f(0.0, 0.0, 0.0);
    //cylinder(-160.0f, -52.0f, -600.0f, radius2, height2, 50);
    cylinder(-160.0f, -52.0f, -900.0f, radius2, height2, 50);
    cylinder(-160.0f, -52.0f, -1200.0f, radius2, height2, 50);
    cylinder(-160.0f, -52.0f, -1500.0f, radius2, height2, 50);
    //cylinder(-90.0f, -52.0f, -1500.0f, radius2, height2, 50);
    cylinder(-160.0f, -52.0f, -1800.0f, radius2, height2, 50);
    cylinder(-90.0f, -52.0f, -1800.0f, radius2, height2, 50);
    cylinder(-160.0f, -52.0f, -2100.0f, radius2, height2, 50);
    cylinder(-90.0f, -52.0f, -2100.0f, radius2, height2, 50);

    cylinder(170.0f, -52.0f, -1800.0f, radius2, height2, 50);
    cylinder(100.0f, -52.0f, -1800.0f, radius2, height2, 50);
    cylinder(170.0f, -52.0f, -2100.0f, radius2, height2, 50);
    cylinder(100.0f, -52.0f, -2100.0f, radius2, height2, 50);
    cylinder(170.0f, -52.0f, -2400.0f, radius2, height2, 50);
    cylinder(100.0f, -52.0f, -2400.0f, radius2, height2, 50);


    //baris pertama
    /*
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -600);
    kotak(25, 2, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -600);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();
    */


    //baris kedua
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -900);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -900);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    //baris ketiga
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -1200);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -1200);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    //baris keempat - kiri
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -1500);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -1500);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    /*
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-90, -83, -1500);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-90, -83, -1500);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();
    */

    //baris kelima - kiri
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -1800);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -1800);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-90, -83, -1800);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-90, -83, -1800);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    //baris keenam - kiri
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -2100);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-160, -83, -2100);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-90, -83, -2100);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-90, -83, -2100);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();



    //baris kelima - kanan
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(170.0, -83.0, -1800.0);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(170.0, -83.0, -1800.0);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100.0, -83.0, -1800.0);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100.0, -83.0, -1800.0);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    //baris keenam - kanan
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(170.0, -83.0, -2100.0);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(170.0, -83.0, -2100.0);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100.0, -83.0, -2100.0);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100.0, -83.0, -2100.0);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    //baris ketujuh - kanan
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(170.0, -83.0, -2400.0);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(170.0, -83.0, -2400.0);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100.0, -83.0, -2400.0);
    kotak(25, 2, 2);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100.0, -83.0, -2400.0);
    kotak(2, 2, 25);
    glPopMatrix();
    glEnd();
}

void kursi(){
    float radius1 = 0.5f;               // lebar
    float height1 = 34.0f;              // tinggi

    //tempat duduk
    glBegin(GL_QUADS);
    glColor3f(0.17, 0.11, 0.1);
    glVertex3f(-184.0, -50.0, -550.0);
    glVertex3f(-136.0, -50.0, -550.0);
    glVertex3f(-140.0, -50.0, -510.0);
    glVertex3f(-180.0, -50.0, -510.0);
    glEnd();

    //tempat sandar
    glBegin(GL_QUADS);
    glColor3f(0.17, 0.11, 0.1);
    glVertex3f(-180.0, -50.0, -510.0);
    glVertex3f(-140.0, -50.0, -510.0);
    glVertex3f(-140.0, -15.0, -500.0);
    glVertex3f(-180.0, -15.0, -500.0);
    glEnd();

    //kaki kursi
    glColor3f(0.0, 0.0, 0.0);
    cylinder(-183.0f, -67.0f, -549.0f, radius1, height1, 50);
    cylinder(-137.0f, -67.0f, -549.0f, radius1, height1, 50);
    cylinder(-179.0f, -67.0f, -510.0f, radius1, height1, 50);
    cylinder(-141.0f, -67.0f, -510.0f, radius1, height1, 50);
}

void kursi1(){
    float radius1 = 0.5f;               // lebar
    float height1 = 34.0f;              // tinggi

    //tempat duduk
    glBegin(GL_QUADS);
    glColor3f(0.17, 0.11, 0.1);
    glVertex3f(-184.0, -50.0, -510.0);
    glVertex3f(-136.0, -50.0, -510.0);
    glVertex3f(-140.0, -50.0, -550.0);
    glVertex3f(-180.0, -50.0, -550.0);
    glEnd();

    //tempat sandar
    glBegin(GL_QUADS);
    glColor3f(0.17, 0.11, 0.1);
    glVertex3f(-180.0, -50.0, -550.0);
    glVertex3f(-140.0, -50.0, -550.0);
    glVertex3f(-140.0, -15.0, -560.0);
    glVertex3f(-180.0, -15.0, -560.0);
    glEnd();

    //kaki kursi
    glColor3f(0.0, 0.0, 0.0);
    cylinder(-183.0f, -67.0f, -511.0f, radius1, height1, 50);
    cylinder(-137.0f, -67.0f, -511.0f, radius1, height1, 50);
    cylinder(-179.0f, -67.0f, -550.0f, radius1, height1, 50);
    cylinder(-141.0f, -67.0f, -550.0f, radius1, height1, 50);

}

void interior(){
    //meja kasir
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, -60, -301);
    kotak(55, 100, 500);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //dapur (bagian depan)
    glPushMatrix();
    glColor3f(0.64, 0.57, 0.49);
    glTranslatef(85, 50, -578);
    kotak(225, 300, 50);
    glPopMatrix();
    glEnd();

    //dapur (bagian tengah)
    glPushMatrix();
    glColor3f(0.64, 0.57, 0.49);
    glTranslatef(32.5, 50, -964);
    kotak(120, 300, 722);
    glPopMatrix();
    glEnd();

    //pintu masuk dapur
    glBegin(GL_QUADS);
    glColor3f(0.07, 0.06, 0.08);
    glVertex3f(80.0, -100.0, -551.0);
    glVertex3f(180.0, -100.0, -551.0);
    glVertex3f(180.0, 100.0, -551.0);
    glVertex3f(80.0, 100.0, -551.0);
    glEnd();

    //pinggir pintu masuk dapur
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(130, 100, -550);
    kotak(105, 5, 5);                   //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(80, 0, -550);
    kotak(5, 200, 5);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(180, 0, -550);
    kotak(5, 200, 5);
    glPopMatrix();
    glEnd();

    //kaca di pintu dapur
    float radius1 = 25.0f;              // besar-kecil
    float height1 = 1.0f;               // tipis-tebal
    glColor3f(0.58, 0.72, 0.73);
    vcylinder(130.0f, 35.0f, -550.0f, radius1, height1, 50);

    //pintu keluar dapur
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(-28.0, -100.0, -1150.0);
    glVertex3f(-28.0, -100.0, -1280.0);
    glVertex3f(-28.0, 100.0, -1280.0);
    glVertex3f(-28.0, 100.0, -1150.0);
    glEnd();


    //toilet lt1
    glPushMatrix();
    glColor3f(0.64, 0.57, 0.49);
    glTranslatef(110, 50, -1535);
    kotak(180, 300, 200);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();


    //pintu toilet ltl
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(19.5, -100.0, -1500.0);
    glVertex3f(19.5, -100.0, -1610.0);
    glVertex3f(19.5, 80.0, -1610.0);
    glVertex3f(19.5, 80.0, -1500.0);
    glEnd();

    //pintu belakang
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(-200.0, -100.0, -2499.0);
    glVertex3f(-100.0, -100.0, -2499.0);
    glVertex3f(-100.0, 100.0, -2499.0);
    glVertex3f(-200.0, 100.0, -2499.0);
    glEnd();


    //toilet lt2
    glPushMatrix();
    glColor3f(0.64, 0.57, 0.49);
    glTranslatef(100, 381, -2100);
    kotak(200, 360, 200);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //unknown lt2
    glPushMatrix();
    glColor3f(0.64, 0.57, 0.49);
    glTranslatef(-100, 381, -2200);
    kotak(200, 360, 200);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();


    //pagar tangga lt2
    //bagian bawah
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100, 199, -1268);
    kotak(3, 3, 330);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(150, 199, -1435);
    kotak(102, 3, 3);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pegangan atas
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(85, 290, -1270);
    kotak(3, 3, 360);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(140, 290, -1450);
    kotak(110, 3, 3);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //garis2 tangga
    int startR = 210;
    int endT = 290;
    int pluss = 10;

    for (int x = startR; x <=endT; x += pluss){
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(85, x, -1270);
        kotak(1, 1, 360);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
        glPopMatrix();
        glEnd();
    }

    for (int x = startR; x <=endT; x += pluss){
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(140, x, -1450);
        kotak(110, 1, 1);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
        glPopMatrix();
        glEnd();
    }

    //gagang tangga
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(85, 240, -1091);
    kotak(3, 100, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(85, 240, -1291);
    kotak(3, 100, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(85, 240, -1451);
    kotak(3, 100, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(195, 240, -1451);
    kotak(3, 103, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();






    //meja lt1
    glPushMatrix();
    glTranslatef(0, 0, 0);
    meja();
    glPopMatrix();
    glEnd();


    //meja lt2
    //baris pertama
    glPushMatrix();
    glTranslatef(-10, -15, -320);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -320);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(160, -15, -320);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(270, -15, -320);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, -15, -320);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris kedua
    glPushMatrix();
    glTranslatef(-10, -15, -620);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -620);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(160, -15, -620);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(270, -15, -620);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, -15, -620);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris ketiga
    glPushMatrix();
    glTranslatef(-10, -15, -920);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -920);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(160, -15, -920);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(270, -15, -920);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, -15, -920);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris keempat
    glPushMatrix();
    glTranslatef(-10, -15, -1220);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -1220);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris kelima
    glPushMatrix();
    glTranslatef(-10, -15, -1300);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -1300);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris keenam
    glPushMatrix();
    glTranslatef(-10, -15, -1600);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -1600);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris ketujuh - kiri
    glPushMatrix();
    glTranslatef(-10, -15, -1900);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -1900);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris ketujuh - kanan
    glPushMatrix();
    glTranslatef(270, -15, -1900);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, -15, -1900);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris kedelapan - kiri
    glPushMatrix();
    glTranslatef(-10, -15, -2200);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(50, -15, -2200);
    meja_lt2();
    glPopMatrix();
    glEnd();

    //baris kedelapan - kanan
    glPushMatrix();
    glTranslatef(270, -15, -2200);
    meja_lt2();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, -15, -2200);
    meja_lt2();
    glPopMatrix();
    glEnd();



    ///kursi lt1

    //baris pertama
    /*
    glPushMatrix();
    glTranslatef(0, 0, 0);
    kursi();
    glPopMatrix();
    glEnd();


    glPushMatrix();
    glTranslatef(0, 0, -150);
    kursi1();
    glPopMatrix();
    glEnd();
    */


    //baris kedua
    glPushMatrix();
    glTranslatef(0, 0, -300);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0, -450);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris ketiga
    glPushMatrix();
    glTranslatef(0, 0, -600);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0, -750);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris keempat
    glPushMatrix();
    glTranslatef(0, 0, -900);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0, -1050);
    kursi1();
    glPopMatrix();
    glEnd();


    //baris kelima      kiri
    glPushMatrix();
    glTranslatef(0, 0, -1200);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0, -1350);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(70, 0, -1200);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(70, 0, -1350);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris kelima      kanan
    glPushMatrix();
    glTranslatef(260, 0, -1200);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(260, 0, -1350);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, 0, -1200);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, 0, -1350);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris keenam      kiri
    glPushMatrix();
    glTranslatef(0, 0, -1500);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0, -1650);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(70, 0, -1500);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(70, 0, -1650);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris keenam      kanan
    glPushMatrix();
    glTranslatef(260, 0, -1500);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(260, 0, -1650);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, 0, -1500);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, 0, -1650);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris ketujuh      kanan
    glPushMatrix();
    glTranslatef(260, 0, -1800);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(330, 0, -1800);
    kursi();
    glPopMatrix();
    glEnd();


    ///kuris lt2
    //baris pertama - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, 580);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(-12.5, 285, 450);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, 580);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, 450);
    kursi1();
    glPopMatrix();
    glEnd();

    //tengah
    glPushMatrix();
    glTranslatef(157.5, 285, 580);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(157.5, 285, 450);
    kursi1();
    glPopMatrix();
    glEnd();

    //kanan
    glPushMatrix();
    glTranslatef(267.5, 285, 580);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(267.5, 285, 450);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, 580);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, 450);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris kedua - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, 280);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(-12.5, 285, 150);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, 280);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, 150);
    kursi1();
    glPopMatrix();
    glEnd();

    //tengah
    glPushMatrix();
    glTranslatef(157.5, 285, 280);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(157.5, 285, 150);
    kursi1();
    glPopMatrix();
    glEnd();

    //kanan
    glPushMatrix();
    glTranslatef(267.5, 285, 280);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(267.5, 285, 150);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, 280);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, 150);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris ketiga - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, -40);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(-12.5, 285, -150);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -40);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -150);
    kursi1();
    glPopMatrix();
    glEnd();

    //tengah
    glPushMatrix();
    glTranslatef(157.5, 285, -40);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(157.5, 285, -150);
    kursi1();
    glPopMatrix();
    glEnd();

    //kanan
    glPushMatrix();
    glTranslatef(267.5, 285, -40);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(267.5, 285, -150);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, -40);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, -150);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris keempat - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, -340);
    kursi();
    glPopMatrix();
    glEnd();


    glPushMatrix();
    glTranslatef(47.5, 285, -340);
    kursi();
    glPopMatrix();
    glEnd();

    //baris kelima - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, -540);
    kursi1();
    glPopMatrix();
    glEnd();


    glPushMatrix();
    glTranslatef(47.5, 285, -540);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris keenam - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, -720);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(-12.5, 285, -820);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -720);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -820);
    kursi1();
    glPopMatrix();
    glEnd();

    //baris ketujuh - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, -1020);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(-12.5, 285, -1120);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -1020);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -1120);
    kursi1();
    glPopMatrix();
    glEnd();

    //kanan
    glPushMatrix();
    glTranslatef(267.5, 285, -1020);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(267.5, 285, -1120);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, -1020);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, -1120);
    kursi1();
    glPopMatrix();
    glEnd();


    //baris delapan - kiri
    glPushMatrix();
    glTranslatef(-12.5, 285, -1320);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(-12.5, 285, -1420);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -1320);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(47.5, 285, -1420);
    kursi1();
    glPopMatrix();
    glEnd();

    //kanan
    glPushMatrix();
    glTranslatef(267.5, 285, -1320);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(267.5, 285, -1420);
    kursi1();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, -1320);
    kursi();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(322.5, 285, -1420);
    kursi1();
    glPopMatrix();
    glEnd();

}

void bangunan_utama() {

    glPushMatrix();
    glTranslatef(0, 0, 0);
    interior();
    glPopMatrix();
    glEnd();

    //pagar buat jendela putih
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 310, 100);
    kotak(360, 2, 4);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 350, 100);
    kotak(360, 2, 4);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 370, 100);
    kotak(360, 2, 4);
    glPopMatrix();
    glEnd();

    int startX = -160;
    int endX = 160;
    int step = 20;  // +20 sampe 160

    for (int x = startX; x <= endX; x += step) {
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glTranslatef(x, 330, 100);
        kotak(2, 40, 4);
        glPopMatrix();
    }

    // Back face
    glBegin(GL_QUADS);
    glColor3f(0.16, 0.18, 0.18);
    glVertex3f(-200.0, -100.0, -2500.0);
    glVertex3f(200.0, -100.0, -2500.0);
    glVertex3f(200.0, 1000.0, -2500.0);
    glVertex3f(-200.0, 1000.0, -2500.0);
    glEnd();

    // Left face
    glBegin(GL_QUADS);
    glVertex3f(-200.0, -100.0, -2500.0);
    glVertex3f(-200.0, -100.0, 100.0);
    glVertex3f(-200.0, 1000.0, 100.0);
    glVertex3f(-200.0, 1000.0, -2500.0);
    glEnd();

    // Right face
    glBegin(GL_QUADS);
    glVertex3f(200.0, -100.0, -2500.0);
    glVertex3f(200.0, -100.0, 100.0);
    glVertex3f(200.0, 1000.0, 100.0);
    glVertex3f(200.0, 1000.0, -2500.0);
    glEnd();

    // Top face
    glBegin(GL_QUADS);
    glVertex3f(-200.0, 1000.0, -2500.0);
    glVertex3f(200.0, 1000.0, -2500.0);
    glVertex3f(200.0, 1000.0, 100.0);
    glVertex3f(-200.0, 1000.0, 100.0);
    glEnd();

    // Bottom face      //1nd floor
    glBegin(GL_QUADS);
    glColor3f(0.63, 0.6, 0.55);
    glVertex3f(-200.0, -83.0, -2500.0);
    glVertex3f(200.0, -83.0, -2500.0);
    glVertex3f(200.0, -83.0, -50.0);
    glVertex3f(-200.0, -83.0, -50.0);
    glEnd();


    //tangga ke lantai2
    glPushMatrix();
    glTranslatef(0, 0, -100);
    tangga_lnt2();
    glPopMatrix();
    glEnd();


    //depan
    //bagian atas     //1nd floor
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-200.0, 200.0, -1100.0);
    glVertex3f(200.0, 200.0, -1100.0);
    glVertex3f(200.0, 200.0, 100.0);
    glVertex3f(-200.0, 200.0, 100.0);
    glEnd();

    // Bottom face      //2nd floor
    glBegin(GL_QUADS);
    glColor3f(0.63, 0.6, 0.55);
    glVertex3f(-200.0, 201.0, -1100.0);
    glVertex3f(200.0, 201.0, -1100.0);
    glVertex3f(200.0, 201.0, 100.0);
    glVertex3f(-200.0, 201.0, 100.0);
    glEnd();


    //tengah kiri
    //bagian atas     //1nd floor
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-200.0, 200.0, -1450.0);
    glVertex3f(100.0, 200.0, -1450.0);
    glVertex3f(100.0, 200.0, -110.0);
    glVertex3f(-200.0, 200.0, -110.0);
    glEnd();

    // Bottom face      //2nd floor
    glBegin(GL_QUADS);
    glColor3f(0.63, 0.6, 0.55);
    glVertex3f(-200.0, 201.0, -1450.0);
    glVertex3f(100.0, 201.0, -1450.0);
    glVertex3f(100.0, 201.0, -1100.0);
    glVertex3f(-200.0, 201.0, -1100.0);
    glEnd();


    //belakang
    //bagian atas     //1nd floor
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-200.0, 200.0, -2500.0);
    glVertex3f(200.0, 200.0, -2500.0);
    glVertex3f(200.0, 200.0, -1435.0);
    glVertex3f(-200.0, 200.0, -1435.0);
    glEnd();

    // Bottom face      //2nd floor
    glBegin(GL_QUADS);
    glColor3f(0.63, 0.6, 0.55);
    glVertex3f(-200.0, 201.0, -2500.0);
    glVertex3f(200.0, 201.0, -2500.0);
    glVertex3f(200.0, 201.0, -1435.0);
    glVertex3f(-200.0, 201.0, -1435.0);
    glEnd();




    //bagian atas      //2nd floor
    glBegin(GL_QUADS);
    glColor3f(0.8, 0.8, 0.8);
    glVertex3f(-200.0, 550.0, -2500.0);
    glVertex3f(200.0, 550.0, -2500.0);
    glVertex3f(200.0, 550.0, 100.0);
    glVertex3f(-200.0, 550.0, 100.0);
    glEnd();

    glColor3f(0.16, 0.18, 0.18);
    // Front face   //bawah jendela
    glBegin(GL_QUADS);
    glVertex3f(-200.0, 200.0, 100.0);
    glVertex3f(200.0, 200.0, 100.0);
    glVertex3f(200.0, 300.0, 100.0);
    glVertex3f(-200.0, 300.0, 100.0);
    glEnd();

    // Front face   //atas jendela
    glBegin(GL_QUADS);
    glVertex3f(-200.0, 500.0, 100.0);
    glVertex3f(200.0, 500.0, 100.0);
    glVertex3f(200.0, 550.0, 100.0);
    glVertex3f(-200.0, 550.0, 100.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(0.04, 0.15, 0.26, 0.9);
    glVertex3f(-200.0, 550.0, -800.0);
    glVertex3f(200.0, 550.0, -800.0);
    glVertex3f(200.0, 790.0, -800.0);
    glVertex3f(-200.0, 790.0, -800.0);
    glEnd();

    //kaca atas biru
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(0.04, 0.15, 0.26, 0.9);
    glVertex3f(-180.0, 550.0, 100.0);
    glVertex3f(180.0, 550.0, 100.0);
    glVertex3f(180.0, 790.0, 100.0);
    glVertex3f(-180.0, 790.0, 100.0);
    glEnd();
    glDisable(GL_BLEND);



    //pinggir kiri kaca
    glBegin(GL_QUADS);
    glColor3f(0.16, 0.18, 0.18);
    glVertex3f(-200.0, 550.0, 100.0);
    glVertex3f(-180.0, 550.0, 100.0);
    glVertex3f(-180.0, 800.0, 100.0);
    glVertex3f(-200.0, 800.0, 100.0);
    glEnd();

    //pinggir kanan kaca
    glBegin(GL_QUADS);
    glColor3f(0.16, 0.18, 0.18);
    glVertex3f(200.0, 550.0, 100.0);
    glVertex3f(180.0, 550.0, 100.0);
    glVertex3f(180.0, 800.0, 100.0);
    glVertex3f(200.0, 800.0, 100.0);
    glEnd();


    //pembatas bawah jendela
    glPushMatrix();
    glColor3f(0.16, 0.18, 0.18);
    glTranslatef(0, 300, 100);
    kotak(360, 5, 5);                   //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas tengah
    glPushMatrix();
    glColor3f(0.16, 0.18, 0.18);
    glTranslatef(0, 400, 100);
    kotak(20, 200, 5);                   //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pinggir kiri jendela
    glBegin(GL_QUADS);
    glColor3f(0.16, 0.18, 0.18);
    glVertex3f(-200.0, 300.0, 100.0);
    glVertex3f(-180.0, 300.0, 100.0);
    glVertex3f(-180.0, 500.0, 100.0);
    glVertex3f(-200.0, 500.0, 100.0);
    glEnd();

    //pinggir kanan jendela
    glBegin(GL_QUADS);
    glColor3f(0.16, 0.18, 0.18);
    glVertex3f(200.0, 300.0, 100.0);
    glVertex3f(180.0, 300.0, 100.0);
    glVertex3f(180.0, 500.0, 100.0);
    glVertex3f(200.0, 500.0, 100.0);
    glEnd();

    //pembatas smokin area di atas
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, 0.2);
    glVertex3f(-200.0, 201.0, -950.0);
    glVertex3f(200.0, 201.0, -950.0);
    glVertex3f(200.0, 1000.0, -950.0);
    glVertex3f(-200.0, 1000.0, -950.0);
    glEnd();
    glDisable(GL_BLEND);

    //pembatas hitam bawah
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-60, 251, -950);
    kotak(280, 100, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas garis hitam bawah
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 202, -950);
    kotak(400, 3, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas garis hitam atas
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 450, -950);
    kotak(400, 5, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas garis hitam atap
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 547, -950);
    kotak(400, 5, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas hitam kiri
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-198, 375, -950);
    kotak(5, 350, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas hitam tengah-kiri
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-60, 375, -950);
    kotak(5, 350, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //pembatas hitam kanan
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(198, 375, -950);
    kotak(5, 350, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();


    //pembatas hitam tengah-kanan (buat pintu)
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(80, 375, -950);
    kotak(5, 350, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //gagang pintu lt2
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100, 300, -950);
    kotak(5, 50, 2);                 //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //bagian pinggir love
    glBegin(GL_QUADS);
    glColor3f(0.16, 0.18, 0.18);
    glVertex3f(-200.0, 790.0, 100.0);
    glVertex3f(200.0, 790.0, 100.0);
    glVertex3f(200.0, 1000.0, 100.0);
    glVertex3f(-200.0, 1000.0, 100.0);
    glEnd();

    //bagian love atas
    glPushMatrix();
    glTranslatef(0, 960, 94);
    glColor3f(0.0, 0.0, 0.0);
    luv(10.0f, 20.0f);                    //radius n height
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 960, 96);
    glColor3f(1.0, 0.0, 0.0);
    luv(9.5f, 18.5f);                    //radius n height
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, 960, 98);
    glColor3f(0.0, 0.0, 0.0);
    luv(9.0f, 18.5f);                    //radius n height
    glPopMatrix();
    glEnd();

    //jendela putih
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, 0.1);
    glVertex3f(-180.0, 300.0, 100.0);
    glVertex3f(180.0, 300.0, 100.0);
    glVertex3f(180.0, 500.0, 100.0);
    glVertex3f(-180.0, 500.0, 100.0);
    glEnd();
    glDisable(GL_BLEND);

    kalimat(-50.0f, 960.0f, 115.0, GLUT_STROKE_ROMAN, "Kopi", 0.4f, 10.0f);             //translatef, glut, "kalimat", size, thickness
    kalimat(-110.0f, 910.0f, 115.0, GLUT_STROKE_ROMAN, "Kenangan", 0.4f, 10.0f);

}

void pintu_masuk(){
    //lantai masuk
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-200.0, -83.0, -50.0);
    glVertex3f(200.0, -83.0, -50.0);
    glVertex3f(200.0, -83.0, 82.0);
    glVertex3f(-200.0, -83.0, 82.0);
    glEnd();

    //kaca pintu
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, 0.3);
    glVertex3f(-200.0, -100.0, -50.0);
    glVertex3f(200.0, -100.0, -50.0);
    glVertex3f(200.0, 200.0, -50.0);
    glVertex3f(-200.0, 200.0, -50.0);
    glEnd();
    glDisable(GL_BLEND);

    //garis atas
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 199, -50);
    kotak(400, 2, 1);                   //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();

    //garis tengah
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 120, -50);
    kotak(400, 2, 1);
    glPopMatrix();
    glEnd();

    //garis bawah
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, -83, -50);
    kotak(400, 2, 1);
    glPopMatrix();
    glEnd();

    //garis for jendela1
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(100, 0, -50);
    kotak(2, 400, 1);
    glPopMatrix();
    glEnd();

    //garis for jendela2
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(200, 0, -50);
    kotak(2, 400, 1);
    glPopMatrix();
    glEnd();

    //garis for jendela3
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0, 0, -50);
    kotak(2, 400, 1);
    glPopMatrix();
    glEnd();

    //garis for jendela4
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-100, 0, -50);
    kotak(2, 400, 1);
    glPopMatrix();
    glEnd();

    //garis for jendela5
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-200, 0, -50);
    kotak(2, 400, 1);
    glPopMatrix();
    glEnd();

    //garis for pintu1
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-101, 0, -50);
    kotak(5, 240, 1);
    glPopMatrix();
    glEnd();

    //garis for pintu2
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-198, 0, -50);
    kotak(5, 240, 1);
    glPopMatrix();
    glEnd();

    //garis for pintu3
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-150, 119, -50);
    kotak(100, 5, 1);
    glPopMatrix();
    glEnd();

    //garis for pintu4
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-150, -80, -50);
    kotak(100, 5, 1);
    glPopMatrix();
    glEnd();

    //gagang pintu
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-110, 0, -50);
    kotak(2, 50, 1);                   //(lebar(kiri-kanan), tinggi, lebar(depan-belakang))
    glPopMatrix();
    glEnd();
}

void outdoor(){

    glPushMatrix();
    glTranslatef(0, -400, 550);
    tangga_masuk();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, -400, 550);
    pintu_masuk();
    glPopMatrix();
    glEnd();

    /*
    //bagian love dijalan
    glPushMatrix();
    glTranslatef(0, -300, 1100);
    glColor3f(0.0, 0.0, 0.0);               // Red color
    luv(5.0f, 10.0f);                    //radius n height
    glPopMatrix();
    glEnd();


    glPushMatrix();
    glTranslatef(0, -300, 1102);
    glColor3f(1.0, 0.0, 0.0);               // Red color
    luv(4.5f, 8.5f);                    //radius n height
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, -300, 1104);
    glColor3f(0.0, 0.0, 0.0);            // Red color
    luv(4.0f, 8.5f);                    //radius n height
    glPopMatrix();
    glEnd();
    */

}

void draw() {

    glPushMatrix();
    ground();
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glTranslatef(0, -400, 550);  //(kiri-kanan, atas-bawah, depan-belakang)       //ground ori -400
    bangunan_utama();
    glPopMatrix();
    glEnd();

    outdoor();


    cout << x_geser << ", " << y_geser << ", " << z_geser << endl;
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera();
    draw();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, 16.0 / 9.0, 2, 10000);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutWarpPointer(width / 2, height / 2);
    glutTimerFunc(1000 / FPS, timer, 0);
}

void passive_motion(int x, int y) {
    int dev_x, dev_y;
    dev_x = (width / 2) - x;
    dev_y = (height / 2) - y;
    yaw += (float)dev_x / 20.0;
    pitch += (float)dev_y / 20.0;
}

void camera() {
    if (motion.Forward) {
        camX += cos((yaw + 90) * TO_RADIANS) * 2;
        camZ -= sin((yaw + 90) * TO_RADIANS) * 2;
    }
    if (motion.Backward) {
        camX += cos((yaw + 90 + 180) * TO_RADIANS) * 2;
        camZ -= sin((yaw + 90 + 180) * TO_RADIANS) * 2;
    }
    if (motion.Left) {
        camX += cos((yaw + 90 + 90) * TO_RADIANS) * 2;
        camZ -= sin((yaw + 90 + 90) * TO_RADIANS) * 2;
    }
    if (motion.Right) {
        camX += cos((yaw + 90 - 90) * TO_RADIANS) * 2;
        camZ -= sin((yaw + 90 - 90) * TO_RADIANS) * 2;
    }
    if (motion.Naik) {
        terbang -= 2.0;
    }
    if (motion.Turun) {
        terbang += 2.0;
    }

    if (pitch >= 70)
        pitch = 70;
    if (pitch <= -90)
        pitch = -90;

    glRotatef(-pitch, 1.0, 0.0, 0.0);
    glRotatef(-yaw, 0.0, 1.0, 0.0);

    glTranslatef(-camX, -terbang, -350 - camZ);
    if (terbang < 25 - 500)
        terbang = 24 - 500;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'W':
    case 'w':
        motion.Forward = true;
        break;
    case 'A':
    case 'a':
        motion.Left = true;
        break;
    case 'S':
    case 's':
        motion.Backward = true;
        break;
    case 'D':
    case 'd':
        motion.Right = true;
        break;
    case 'E':
    case 'e':
        motion.Naik = true;
        break;
    case 'Q':
    case 'q':
        motion.Turun = true;
        break;
    case '6':
        x_geser += 1.5;
        break;
    case '4':
        x_geser -= 1.5;
        break;
    case '9':
        y_geser += 1.5;
        break;
    case '3':
        y_geser -= 1.5;
        break;
    case '8':
        z_geser -= 1.5;
        break;
    case '5':
        z_geser += 0.5;
        break;
    case '`': // KELUAR DARI PROGRAM
        exit(1);
    }
}

void keyboard_up(unsigned char key, int x, int y) {
    switch (key) {
    case 'W':
    case 'w':
        motion.Forward = false;
        break;
    case 'A':
    case 'a':
        motion.Left = false;
        break;
    case 'S':
    case 's':
        motion.Backward = false;
        break;
    case 'D':
    case 'd':
        motion.Right = false;
        break;
    case 'E':
    case 'e':
        motion.Naik = false;
        break;
    case 'Q':
    case 'q':
        motion.Turun = false;
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Kopi Kenangan-kaloka");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passive_motion);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);

    glutMainLoop();
    return 0;
}

