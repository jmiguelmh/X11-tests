#include <X11/Xlib.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

#define ESC_KEY 9
#define X_KEY 53
#define Y_KEY 29
#define Z_KEY 52
#define SPACE_KEY 65

int height, width;

Display *d;
Window w;
XEvent e;
GC gc;
int s;
bool exitProgram;

double rotationSpeed;

struct point {
    double x;
    double y;
    double z;

    point() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    point(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

vector<point> points;
vector<pair<point,point>> edges;
point camera;
double zProyectionPlane;

void setPoints() {
    points.push_back(point(1, -1, -1));
    points.push_back(point(1, 1, -1));
    points.push_back(point(-1, 1, -1));
    points.push_back(point(-1, -1, -1));
    points.push_back(point(1, -1, 1));
    points.push_back(point(1, 1, 1));
    points.push_back(point(-1, 1, 1));
    points.push_back(point(-1, -1, 1));
}

void setEdges() {
    edges.push_back(pair(points[0], points[4]));
    edges.push_back(pair(points[4], points[7]));
    edges.push_back(pair(points[7], points[3]));
    edges.push_back(pair(points[3], points[0]));
    edges.push_back(pair(points[0], points[1]));
    edges.push_back(pair(points[4], points[5]));
    edges.push_back(pair(points[7], points[6]));
    edges.push_back(pair(points[3], points[2]));
    edges.push_back(pair(points[1], points[5]));
    edges.push_back(pair(points[5], points[6]));
    edges.push_back(pair(points[6], points[2]));
    edges.push_back(pair(points[2], points[1]));
}

void printPoints() {
    for(int i = 0; i < points.size(); i++) {
        cout << "X: " << points[i].x << "\tY: " << points[i].y << "\tZ: " << points[i].z << endl;
    }
    cout << endl;
}

void init() {
    height = 640;
    width = 480;
    rotationSpeed = M_PI / 180;
    setPoints();
    setEdges();
    camera = point(0, 0, -10);
    zProyectionPlane = 1000;

    d = XOpenDisplay(NULL);
    exitProgram = false;

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 100, 100, height, width, 1, BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);
    XStoreName(d, w, "X11 tests");
    gc = XCreateGC(d, w, 0, 0);
}

void clear() {
    XClearWindow(d, w);
}

void draw() {
    for (int i = 0; i < edges.size(); i++) {
        XDrawLine(d, w, gc,
            (abs(camera.z - zProyectionPlane) * edges[i].first.x) / (camera.z + edges[i].first.z) + height / 2,
            (abs(camera.z - zProyectionPlane) * edges[i].first.y) / (camera.z + edges[i].first.z) + width / 2,
            (abs(camera.z - zProyectionPlane) * edges[i].second.x) / (camera.z + edges[i].second.z) + height / 2,
            (abs(camera.z - zProyectionPlane) * edges[i].second.y) / (camera.z + edges[i].second.z) + width / 2
        );
    }
}

void rotateX() {
    for (int i = 0; i < points.size(); i++) {
        points[i].y = (points[i].y * cos(rotationSpeed)) + (points[i].z * sin(rotationSpeed));
        points[i].z = (-points[i].y * sin(rotationSpeed) + (points[i].z * cos(rotationSpeed)));

        double length = sqrt(points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z);
        double scaleBy = sqrt(3) / length;

        points[i].y = points[i].y * scaleBy;
        points[i].z = points[i].z * scaleBy;
    }
    
    edges.clear();
    setEdges();
}

void rotateY() {
    for (int i = 0; i < points.size(); i++) {
        points[i].x = (points[i].x * cos(rotationSpeed)) + (points[i].z * sin(rotationSpeed));
        points[i].z = (-points[i].x * sin(rotationSpeed) + (points[i].z * cos(rotationSpeed)));

        double length = sqrt(points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z);
        double scaleBy = sqrt(3) / length;

        points[i].x = points[i].x * scaleBy;
        points[i].z = points[i].z * scaleBy;
    }
    
    edges.clear();
    setEdges();
}

void rotateZ() {
    for (int i = 0; i < points.size(); i++) {
        points[i].x = (points[i].x * cos(rotationSpeed)) + (-points[i].y * sin(rotationSpeed));
        points[i].y = (points[i].x * sin(rotationSpeed) + (points[i].y * cos(rotationSpeed)));

        double length = sqrt(points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z);
        double scaleBy = sqrt(3) / length;

        points[i].x = points[i].x * scaleBy;
        points[i].y = points[i].y * scaleBy;
    }
    
    edges.clear();
    setEdges();
}

void update() {
    rotateX();
    rotateY();
    rotateZ();
}

int main(void)
{   
    init();

    while (!exitProgram)
    {
        XNextEvent(d, &e);
        if (e.type == KeyPress)
        {
            switch (e.xkey.keycode)
            {
                case ESC_KEY:
                    exitProgram = true;
                    break;
                
                case X_KEY:
                    rotateX();
                    break;
                
                case Y_KEY:
                    rotateY();
                    break;
                
                case Z_KEY:
                    rotateZ();
                    break;
                
                case SPACE_KEY:
                    update();
            }

            printPoints();
        }
        
        clear();
        draw();
    }

    XCloseDisplay(d);
    return 0;
}