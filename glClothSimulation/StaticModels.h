#pragma once
#include <string>
#include <set>
#include "AABB.h"


class SceneManager;
class IMesh;
class MaterialManager;
class OctreeNode;

class StaticModels
{
public:
    StaticModels(SceneManager* pRenderingSytem);
    ~StaticModels();

    void loadModel(const std::string& path);
    AABB getBoundingBox();

    void buildOctree();

    void drawOctree();

    bool segment_intersection(const vect3& p1, const vect3& p2, vect3 hitPos);
    
private:
    OctreeNode*   m_octreeRoot;
    SceneManager* m_pSceneManager;
    std::set<IMesh*> m_staticMeshes;
    IMesh*           m_octreeMesh;
};

