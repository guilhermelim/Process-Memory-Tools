#ifndef FLY_MODE_H
#define FLY_MODE_H
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Função para converter graus para radianos
float degreesToRadians(float degrees) {
    return degrees * (M_PI / 180.0f);
}

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

template<typename T>
struct Coordinates {
    T x;
    T y;
    T z;
};

template<typename T>
Coordinates<T> flyMode(T initialX, T initialY, T initialZ, T cameraAngleX, T cameraAngleY, T distance, Direction direction) {
    // Convert camera angles from degrees to radians
    T radCameraX = degreesToRadians(cameraAngleX);
    T radCameraY = degreesToRadians(cameraAngleY);

    // Calculate the displacements in each direction based on the movement direction    
    T dx = 0, dy = 0, dz = 0;
    switch (direction) {
    case FORWARD:
        dx = distance * cos(radCameraY) * sin(radCameraX);
        dy = distance * sin(radCameraY);
        dz = distance * cos(radCameraY) * cos(radCameraX);
        break;
    case BACKWARD:
       dx = distance * cos(radCameraY) * sin(radCameraX) * -1;
       dy = distance * sin(radCameraY) * -1;
       dz = distance * cos(radCameraY) * cos(radCameraX) * -1;
        break;
    case LEFT:
        dx = distance * sin(radCameraX + M_PI / 2) * -1;
        dz = distance * cos(radCameraX + M_PI / 2) * -1;
        break;
    case RIGHT:
        dx = -distance * sin(radCameraX + M_PI / 2) * -1;
        dz = -distance * cos(radCameraX + M_PI / 2) * -1;
        break;  
    }


    // Create the coordinates structure with the new values
    Coordinates<T> newCoords;
    newCoords.x = initialX + dx;
    newCoords.y = initialY + dy;
    newCoords.z = initialZ + dz;

    return newCoords;
}

#endif // FLY_MODE_H
