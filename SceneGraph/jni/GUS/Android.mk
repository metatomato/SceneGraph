#######################################
#	GUS 
#######################################

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../Bullet
LOCAL_MODULE    := gus
LOCAL_CFLAGS    := -fpermissive -frtti -std=c++11 -w
LOCAL_LDLIBS    := -lstdc++ -lm  -ldl -lc -llog -lGLESv2
LOCAL_SRC_FILES :=  Common/GUS.cpp\
					Common/GusObject.cpp\
					Common/Interpolator.cpp\
					Common/Matrix.cpp\
					Common/RenderPass.cpp\
					Common/Time.cpp\
					DataCenter/DataCenter.cpp\
					DataCenter/Import.cpp\
					DataCenter/Library.cpp\
					Logic/Action.cpp\
					Logic/Cortex.cpp\
					Logic/Logic.cpp\
					Logic/State.cpp\
					Renderer/Box.cpp\
					Renderer/Camera.cpp\
					Renderer/Engine.cpp\
					Renderer/Entity.cpp\
					Renderer/EntityAnimated.cpp\
					Renderer/EntityDBG.cpp\
					Renderer/GusGLObject.cpp\
					Renderer/GusGLObjectFactory.cpp\
					Renderer/Light.cpp\
					Renderer/Material.cpp\
					Renderer/md5.cpp\
					Renderer/md5animation.cpp\
					Renderer/Mesh.cpp\
					Renderer/RenderColor.cpp\
					Renderer/RenderColorDBG.cpp\
					Renderer/Renderer.cpp\
					Renderer/RenderShadow.cpp\
					Renderer/Shader.cpp\
					Renderer/ShadersRef.cpp\
					Renderer/Shape.cpp\
					Scene/CameraControl.cpp\
					Scene/CharacterControl.cpp\
					Scene/Control.cpp\
					Scene/GusPhysics.cpp\
					Scene/Scene.cpp\
					Scene/SceneObject.cpp\
					Scene/SpaceObject.cpp\
				    System/BroadCastingService.cpp\
				    System/ChannelOffice.cpp\
				    System/GusTV.cpp\
				    System/Program.cpp\
				    System/ProgramFactory.cpp\


LOCAL_SHARED_LIBRARIES := bullet

include $(BUILD_SHARED_LIBRARY)
