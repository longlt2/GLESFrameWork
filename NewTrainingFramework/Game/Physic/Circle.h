#pragma once

#include "Object.h"
#include "Physic.h"

class Circle : public Object
{
private:

protected:
    int m_radius;

public:
    Circle();
    Circle(int radius);
    Circle(int x, int y, int radius);
    Circle(int x, int y, int mass, glm::vec2 const &v, int radius);
    Circle(glm::vec2 const &p, int radius);
    Circle(glm::vec2 const &p, int mass, glm::vec2 const &v, int radius);
    Circle(Circle const &cl);
    virtual ~Circle();

    virtual void Update(float frameTime);
	virtual void Render(GUtils::VideoDriver *video, float prediction = 0) const;
    virtual bool isInside(glm::vec2 const &p) const;

    friend class SceneManager;
    friend class CollisionManager;
};
