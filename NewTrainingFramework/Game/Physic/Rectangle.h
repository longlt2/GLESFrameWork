#pragma once

#include "Object.h"

class Rect : public Object
{
private:

protected:
    int m_width;
    int m_height;
public:

    Rect();
    Rect(int x, int y, int w, int h);
    Rect(int x, int y, int mass, glm::vec2 const & velocity, int w, int h);
    Rect(glm::vec2 const &p1, int w, int h);
    Rect(glm::vec2 const &p1, int mass, glm::vec2 const & velocity, int w, int h);
    Rect(Rect const &r);
    virtual ~Rect();

    virtual void Update(float frameTime);
    virtual void Render(GUtils::VideoDriver *video, float prediction = 0) const;
    virtual bool isInside(glm::vec2 const &p) const;

    friend class SceneManager;
    friend class CollisionManager;
};
