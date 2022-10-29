#include <X11/Xlib.h>
#include <iostream>
#include <vector>

using namespace std;

#define ESC_KEY 9
#define X_KEY 53
#define Y_KEY 29
#define Z_KEY 52

Display *d;
Window w;
XEvent e;
GC gc;
int s;
bool exitProgram;

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
}

void init() {
    setPoints();
    setEdges();
    camera = point(0, 0, -10);
    zProyectionPlane = 1000;

    d = XOpenDisplay(NULL);
    exitProgram = false;

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 100, 100, 640, 480, 1, BlackPixel(d, s), WhitePixel(d, s));
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
            (abs(camera.z - zProyectionPlane) * edges[i].first.x) / (camera.z + edges[i].first.z) + 320,
            (abs(camera.z - zProyectionPlane) * edges[i].first.y) / (camera.z + edges[i].first.z) + 240,
            (abs(camera.z - zProyectionPlane) * edges[i].second.x) / (camera.z + edges[i].second.z) + 320,
            (abs(camera.z - zProyectionPlane) * edges[i].second.y) / (camera.z + edges[i].second.z) + 240
        );
        /*
        cout << edges.size() << endl;
        cout << "X1: " << edges[i].first.x << " Y1: " << edges[i].first.y << " Z1: " << edges[i].first.z << " X2: " << edges[i].second.x << " Y2: " << edges[i].second.y << " Z2: " << edges[i].second.z << endl;
        */
    }
}

void rotateX() {
    
    edges.clear();
    setEdges();
}

void rotateY() {
    
    edges.clear();
    setEdges();
}

void rotateZ() {

    edges.clear();
    setEdges();
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
            }

            printPoints();
        }
        
        clear();
        draw();
    }

    XCloseDisplay(d);
    return 0;
}