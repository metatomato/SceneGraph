//
//  RenderPass.cpp
//  helloGL
//
//  Created by tomato on 9/18/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "RenderPass.h"
#include "../Renderer/Entity.h"




/*
RenderPass RenderPass::RenderToDBG(const RenderPass& pass){
    vector<GusGLObject*> entities_DBG;
    for (auto it = pass.entities.begin(); it!= pass.entities.end(); ++it) {
        entities_DBG.push_back((GusGLObject*)(*it)->GetEntityDGB());
    }
    return RenderPass(entities_DBG,pass.lights.front(),pass.camera);
}
*/

template <class T>
RenderPass<T>::RenderPass(vector<T*> entity_vec, vector<T*> light_vec, T* camera_in, RENDER_PASS r_type):
    name(""),
    rType(r_type),
    sType(S_OBJ),
    entities(entity_vec),
    lights(light_vec),
    camera(camera_in),
    primitive(-1),
    material(-1){
}

template class RenderPass<GusGLObject>;
template class RenderPass<SceneObject>;
