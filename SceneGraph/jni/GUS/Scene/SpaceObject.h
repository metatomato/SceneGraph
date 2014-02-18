//
//  SpaceObject.h
//  helloGL
//
//  Created by tomato on 9/19/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once
#include "../Common/GUS.h"


struct Box_t {
    GusVec3                             position;
    GusVec3                             halfVec;
    map<AXIS, GLfloat>                  boxExtrema;
    GLfloat                             box[24];
    const static GLuint                 boxIndex[24];
    
    Box_t():position(),halfVec(),boxExtrema(){
        for(int i = 0; i < 24; i++){box[i] = 0.0f;}
        for(auto it = boxExtrema.begin();it!=boxExtrema.end();++it){it->second = 0.0f;}
    }
    
    Box_t(const GusVec3& center,const GusVec3& half_vec):position(center),halfVec(half_vec),boxExtrema(){
        for(int i = 0; i < 24; i++){box[i] = 0.0f;}
        CenterExtrema();
        SetBox();
    }
    
    Box_t(const Box_t& copy){
        position = copy.position;
        halfVec  = copy.halfVec;
        CenterExtrema();
        for(int i = 0; i < 24; i++){box[i] = copy.box[i];}
    }
    
    ~Box_t(){

    }

    GusVec3 GetMaxima()const {return GusVec3(boxExtrema.at(AXIS::X_POS),boxExtrema.at(AXIS::Y_POS),boxExtrema.at(AXIS::Z_POS));}
    GusVec3 GetMinima()const {return GusVec3(boxExtrema.at(AXIS::X_NEG),boxExtrema.at(AXIS::Y_NEG),boxExtrema.at(AXIS::Z_NEG));}
    
    
    
    void InitExtrema(const GusVec3& vec){
        boxExtrema[AXIS::X_POS] = vec.x ; boxExtrema[AXIS::X_NEG] = vec.x ;
        boxExtrema[AXIS::Y_POS] = vec.y ; boxExtrema[AXIS::Y_NEG] = vec.y ;
        boxExtrema[AXIS::Z_POS] = vec.z ; boxExtrema[AXIS::Z_NEG] = vec.z ;
    }
    
    void CenterExtrema(){
        boxExtrema[AXIS::X_POS] = halfVec.x ; boxExtrema[AXIS::X_NEG] = -halfVec.x ;
        boxExtrema[AXIS::Y_POS] = halfVec.y ; boxExtrema[AXIS::Y_NEG] = -halfVec.y ;
        boxExtrema[AXIS::Z_POS] = halfVec.z ; boxExtrema[AXIS::Z_NEG] = -halfVec.z ;
    }
    
    void SetBoxExtrema(GusVec3 vec){
        if (vec.x > boxExtrema[AXIS::X_POS]) { boxExtrema[AXIS::X_POS] = vec.x ; }
        if (vec.x < boxExtrema[AXIS::X_NEG]) { boxExtrema[AXIS::X_NEG] = vec.x ; }
        if (vec.y > boxExtrema[AXIS::Y_POS]) { boxExtrema[AXIS::Y_POS] = vec.y ; }
        if (vec.y < boxExtrema[AXIS::Y_NEG]) { boxExtrema[AXIS::Y_NEG] = vec.y ; }
        if (vec.z > boxExtrema[AXIS::Z_POS]) { boxExtrema[AXIS::Z_POS] = vec.z ; }
        if (vec.z < boxExtrema[AXIS::Z_NEG]) { boxExtrema[AXIS::Z_NEG] = vec.z ; }
    }
    
    void SetHalfVecFromExtrema() {
        halfVec.x = (boxExtrema[AXIS::X_POS] - boxExtrema[AXIS::X_NEG])/2.f;
        halfVec.y = (boxExtrema[AXIS::Y_POS] - boxExtrema[AXIS::Y_NEG])/2.f;
        halfVec.z = (boxExtrema[AXIS::Z_POS] - boxExtrema[AXIS::Z_NEG])/2.f;
    }
    
    void SetPositionFromExtrema() {
        position.x = boxExtrema[AXIS::X_NEG] + halfVec.x;
        position.y = boxExtrema[AXIS::Y_NEG] + halfVec.y;
        position.z = boxExtrema[AXIS::Z_NEG] + halfVec.z;
    }
    
    void SetBox() {
        box[0]  =  halfVec.x ; box[1]  =  halfVec.y ; box[2]  =  halfVec.z ;
        box[3]  = -halfVec.x ; box[4]  =  halfVec.y ; box[5]  =  halfVec.z ;
        box[6]  = -halfVec.x ; box[7]  = -halfVec.y ; box[8]  =  halfVec.z ;
        box[9]  =  halfVec.x ; box[10] = -halfVec.y ; box[11] =  halfVec.z ;
        box[12] =  halfVec.x ; box[13] =  halfVec.y ; box[14] = -halfVec.z ;
        box[15] = -halfVec.x ; box[16] =  halfVec.y ; box[17] = -halfVec.z ;
        box[18] = -halfVec.x ; box[19] = -halfVec.y ; box[20] = -halfVec.z ;
        box[21] =  halfVec.x ; box[22] = -halfVec.y ; box[23] = -halfVec.z ;
    }
    
    void BuildBoxFromExtrema() {
        SetHalfVecFromExtrema();
        SetPositionFromExtrema();
        SetBox();
        CenterExtrema();
    }
    
    Box_t operator+=(const Box_t& in_box){
        //GusVec3 minima = GetMinima() + position;
        //GusVec3 maxima = GetMaxima() + position;

    	GusVec3 minima = position - halfVec;
		GusVec3 maxima = position + halfVec;

        InitExtrema(minima);
        
        SetBoxExtrema(maxima);
        
        //SetBoxExtrema(in_box.GetMaxima() + in_box.position);
        //SetBoxExtrema(in_box.GetMinima() + in_box.position);
        
        SetBoxExtrema(in_box.position + in_box.halfVec);
		SetBoxExtrema(in_box.position - in_box.halfVec);
        
        //position.x += in_box.position;
        //position /= 2.0f;
        SetHalfVecFromExtrema();
        SetPositionFromExtrema();
        SetBox();
        CenterExtrema();
        return *this;
    }
    
    void PrintBox(string name) const{
        cout << "[SPACEOBJECT][BOX_T] " << endl
        << "          "                   << fixed << setprecision(1) << boxExtrema.at(AXIS::Y_NEG) << fixed
        << " "                            <<setprecision(1) << boxExtrema.at(AXIS::Y_POS)<< endl
        << "            ^"                << endl
        << "            |"                << endl
        << "         ---|----"            << endl
        << "       /    |   /|"           << endl
        << "      /        / |"           << endl
        << "     /        /  |"           << endl
        << "     --------    |"           << endl
        << "    |        |  --------->  " << fixed << setprecision(1) << boxExtrema.at(AXIS::X_NEG) << " " << setprecision(1) << boxExtrema.at(AXIS::X_POS)
                                          << "             " << name << endl
        << "    |        |  /"            << "                                pos " << position.x << " " << position.y << " " << position.z << endl
        << "    |   /    | /"             << "                                 hv " << halfVec.x  << " " << halfVec.y  << " " << halfVec.z << endl
        << "    |  /     |/"              << endl
        << "     -/------"                << endl
        << "     /"                       <<endl
        << "    v"                        <<endl
        << "  " << setprecision(1) << boxExtrema.at(AXIS::Z_NEG) <<" "<< fixed << setprecision(1) << boxExtrema.at(AXIS::Z_POS) << endl;
    }
};
/*
          Y_POS

            ^
            |
         ---|----
       /    |   /|
      /        / |
     /        /  |
     --------
    |        | --------->  X_POS
    |        |  /
    |   /    | /
    |  /     |/
     -/------
     /
    v
Z_POS
*/
class SpaceObject {
        public :
    SpaceObject();
    SpaceObject(const GusVec3& pos, const GusVec3& halfvec);
    SpaceObject(const GusVec3& pos, const GusVec3& halfvec, const GusVec3& dir);
    SpaceObject(Box_t& in_box);
    ~SpaceObject(){}
    
    static SpaceObject			GetDummy();

    GusVec3                     GetPosition()          {return box.position;};
    GusVec3                     GetPosition()   const  {return box.position;};
    GusVec3&                    GetPosition_m()        {return box.position;};
    void                        SetPosition(const GusVec3& in_pos)  {box.position = in_pos;};
    
    GusVec3                     GetDirection()          {return direction;};
    GusVec3                     GetDirection()  const   {return direction;};
    GusVec3&                    GetDirection_m()        {return direction;};
    void                        SetDirection(const GusVec3& in_dir){direction = in_dir;};
    
    GusVec3                     GetUp()          {return up;};
    GusVec3                     GetUp()  const   {return up;};
    GusVec3&                    GetUp_m()        {return up;};
    void                        SetUp(const GusVec3& in_up){up = in_up;};
    
    GusVec3                     GetHalfVec()        {return box.halfVec;};
    GusVec3                     GetHalfVec() const  {return box.halfVec;};
    
    GusVec3                     GetMaxima()     {return box.GetMaxima();};
    GusVec3                     GetMinima()     {return box.GetMinima();};
    
    GLfloat*                    GetData()       {return box.box;};
    const Box_t&                GetBox() const  {return box;};
    Box_t&                      GetBox()        {return box;};
    void                        SetBox(const Box_t& in_box){box = in_box;};
    
    GLfloat *                   GetMODEL_v(){ return MODEL;};
    const GLfloat *   		    GetMODEL() const{ return MODEL;};
    
    void                        Print(string name)const{box.PrintBox(name);};
    
    //void                        Sync(SpaceObject& space_obj){CopyMatrix(MODEL,space_obj.MODEL);};
    
        protected :
    Box_t                       box;
    GusVec3                     direction;
    GusVec3                     up;
    GLfloat                     MODEL[16];

};






















