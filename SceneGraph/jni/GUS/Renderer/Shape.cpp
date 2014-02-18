//
//  Shape.cpp//  helloGL
//
//  Created by tomato on 4/3/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Shape.h"
#include "../Scene/Scene.h"
#include "../DataCenter/DataCenter.h"
#include "Mesh.h"




Shape::Shape(SHAPE shape, string name):Entity(name) {

	Mesh* mesh;
	Material* material = new Material();
	if(CUBE == shape){
		mesh = DataCenter::GetBaseShape("Cube_Cube.001_None");
	}
	if(SPHERE == shape){
		mesh = DataCenter::GetBaseShape("Sphere_None");
	}
	if(CYLINDER == shape){
		mesh = DataCenter::GetBaseShape("Cylinder_None");
	}
	if(SUZANNE == shape){
		mesh = DataCenter::GetBaseShape("Suzanne_None");
	}
	attribs.mesh = mesh;
	attribs.material = material;
	attribs.name = mesh->_GetName();

	_SetPostAttrib();
}



void Shape::SetMaterial(Material* mat) {
	attribs.material = mat;
	_SetPostAttrib();
}












Shape* CreateShape(Shape::SHAPE type) {
	Shape* shape = nullptr;

		if(type == Shape::SHAPE::CUBE){
			shape = new Shape(Shape::CUBE, "cube");
		}
		if(type == Shape::SHAPE::SPHERE){
			shape = new Shape(Shape::SPHERE, "sphere");
		}
		if(type == Shape::SHAPE::CYLINDER){
			shape = new Shape(Shape::CYLINDER, "cylinder");
		}
		if(type == Shape::SHAPE::SUZANNE){
			shape = new Shape(Shape::SUZANNE, "suzanne");
		}
		return shape;
}



























