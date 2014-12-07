#ifndef SCENE_H
#define SCENE_H

#include <CS123Common.h>
#include "CS123SceneData.h"
#include <QList>

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene
{
public:
    Scene();
    virtual ~Scene();

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    CS123SceneGlobalData getGlobal();

    QList<CS123SceneLightData> getLights();

    QList<CS123ScenePrimitive> getPrimitives();

    QList<glm::mat4> getTransformations();

protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    virtual void traverse(CS123SceneNode *root, glm::mat4x4 ctm);

    CS123SceneGlobalData m_global;
    QList<CS123SceneLightData> m_lights;
    QList<CS123ScenePrimitive> m_primitives;
    QList<glm::mat4> m_transformations;

};

#endif // SCENE_H
