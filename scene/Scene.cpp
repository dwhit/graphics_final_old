#include "Scene.h"
#include "camera/Camera.h"
#include "CS123SceneData.h"
//#include "CS123ISceneParser.h"
#include <QtGlobal>


Scene::Scene() //: m_camera(NULL)
{

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
    for(int i = 0; i < m_primitives.length(); i++)
    {
        delete m_primitives[i].material.textureMap;
        //m_primitives[i].material.textureMap = NULL;
    }

}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

//    CS123SceneGlobalData global;
//    CS123SceneLightData light;

//    parser->getGlobalData(global);
//    sceneToFill->setGlobal(global);

//    for(int i = 0; i < parser->getNumLights(); ++i)
//    {
//        parser->getLightData(i, light);
//        sceneToFill->addLight(light);
//    }

////    for(int i = 0; i < sceneToFill->m_lights.length(); i++)
////    {
////        cout << sceneToFill->m_lights[i].id << endl;
////    }

//    glm::mat4x4 identity;
//    sceneToFill->traverse(parser->getRootNode(), identity);
}

void Scene::traverse(CS123SceneNode *root, glm::mat4x4 ctm)
{
//    //update the ctm from the parent node to this node
//    for(int i = 0; i < root->transformations.size(); i++)
//    {
//        switch (root->transformations[i]->type)
//        {
//        case TRANSFORMATION_TRANSLATE:
//            ctm = glm::translate(ctm, root->transformations[i]->translate);
//            break;
//        case TRANSFORMATION_ROTATE:
//            ctm = glm::rotate(ctm, root->transformations[i]->angle, root->transformations[i]->rotate);
//            break;
//        case TRANSFORMATION_SCALE:
//            ctm = glm::scale(ctm, root->transformations[i]->scale);
//            break;
//        case TRANSFORMATION_MATRIX:
//            ctm = ctm * root->transformations[i]->matrix;
//            break;
//        }
//    }
//    //add each primitive at this node
//    for(int i = 0; i < root->primitives.size(); i++)
//    {
//        this->addPrimitive(*(root->primitives[i]), ctm);
//    }
//    //traverse each child
//    for(int i = 0; i < root->children.size(); i++)
//    {
//        this->traverse(root->children[i], ctm);
//    }
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix)
{
    //add this primitive and its transformations to the lists. Cooresponding pairs
    //will be at the same index
    CS123ScenePrimitive prim;
    prim.material.blend = scenePrimitive.material.blend;
    prim.material.bumpMap = scenePrimitive.material.bumpMap;
    //prim.material.cAmbient.operator =(scenePrimitive.material.cAmbient);
    prim.material.cAmbient = scenePrimitive.material.cAmbient;
    prim.material.cDiffuse.operator =(scenePrimitive.material.cDiffuse);
    prim.material.cEmissive.operator =(scenePrimitive.material.cEmissive);
    prim.material.cReflective.operator =(scenePrimitive.material.cReflective);
    prim.material.cSpecular.operator =(scenePrimitive.material.cSpecular);
    prim.material.cTransparent = scenePrimitive.material.cTransparent;
    prim.material.ior = scenePrimitive.material.ior;
    prim.material.shininess = scenePrimitive.material.shininess;
    CS123SceneFileMap *tMap = new CS123SceneFileMap;
    //if(scenePrimitive.material.textureMap->isUsed == true)
    //{
        *tMap = *(scenePrimitive.material.textureMap);
        prim.material.textureMap = tMap;
    //}
    //else
    //{
    //    prim.material.textureMap = NULL;
    //}
    //prim.material.textureMap->operator =(*scenePrimitive.material.textureMap);
    //prim.material.textureMap->filename = scenePrimitive.material.textureMap->filename;
//    prim.material.textureMap->isUsed = scenePrimitive.material.textureMap->isUsed;
//    prim.material.textureMap->repeatU = scenePrimitive.material.textureMap->repeatU;
//    prim.material.textureMap->repeatV = scenePrimitive.material.textureMap->repeatV;
//    prim.material.textureMap->texid = scenePrimitive.material.textureMap->texid;
    prim.meshfile = scenePrimitive.meshfile;
    prim.type = scenePrimitive.type;

    m_primitives.append(prim);
    m_transformations.append(matrix);
}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
    m_lights.append(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
    m_global.ka = global.ka;
    m_global.kd = global.kd;
    m_global.ks = global.ks;
    m_global.kt = global.kt;
}

CS123SceneGlobalData Scene::getGlobal()
{
    return m_global;
}

QList<CS123SceneLightData> Scene::getLights()
{
    return m_lights;
}

QList<CS123ScenePrimitive> Scene::getPrimitives()
{
    return m_primitives;
}

QList<glm::mat4> Scene::getTransformations()
{
    return m_transformations;
}

