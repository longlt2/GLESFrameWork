#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Physic.h"

class Plane : public Object
{
private:

protected:

public:
    Plane();
    Plane(glm::vec2 const &, glm::vec2 const &, glm::vec2 const &);
    Plane(Plane const &);
    virtual ~Plane();

    virtual void Render(GUtils::VideoDriver *video, float prediction) const;
    virtual bool isInside(glm::vec2 const &p) const;
    virtual void Update(float frameTime);

    friend class SceneManager;
    friend class CollisionManager;
};

#endif

