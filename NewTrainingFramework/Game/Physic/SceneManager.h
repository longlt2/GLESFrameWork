#pragma once

#include <vector>

#include "Object.h"
#include "CollisionManager.h"

class SceneManager
{
private:
    std::vector<Object *> m_ObjVector;

    static SceneManager * sInstance;

protected:
    SceneManager();
    virtual ~SceneManager();

public:
    void Render(GUtils::VideoDriver *video) const;
    void Render(GUtils::VideoDriver *video, float prediction) const;
    void Update(float frameTime);

    void AddObject(Object *obj);
    int Init(char const* url);
    Object * const & getTouched(float x, float y) const;
    Object * const & getTouched(glm::vec2 const &p) const;

    void touchActionDown(float x, float y);
    void touchActionMove(float x, float y);
    void touchActionUp(float x, float y);

    static SceneManager * GetInstance(void);
};
