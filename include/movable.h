#ifndef MOVABLE_H
#define MOVABLE_H

class Movable {
public:
    virtual void move() = 0;
    virtual ~Movable() = default;
};

#endif