##################################################
# Bullet Physics
##################################################
MY_LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE           := bullet
LOCAL_CFLAGS           := -isystem $(SYSROOT)/usr/include
LOCAL_PATH             := $(MY_LOCAL_PATH)
LOCAL_SRC_FILES        := LinearMath/btGeometryUtil.cpp \
                          LinearMath/btSerializer.cpp \
                          LinearMath/btAlignedAllocator.cpp \
                          LinearMath/btConvexHull.cpp \
                          LinearMath/btQuickprof.cpp \
                          LinearMath/btConvexHullComputer.cpp \
                          LinearMath/btPolarDecomposition.cpp \
                          LinearMath/btVector3.cpp \
                          BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
                          BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
                          BulletCollision/BroadphaseCollision/btDbvt.cpp \
                          BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
                          BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
                          BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
                          BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
                          BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
                          BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
                          BulletCollision/BroadphaseCollision/btDispatcher.cpp \
                          BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
                          BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
                          BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btCollisionObject.cpp \
                          BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
                          BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
                          BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
                          BulletCollision/CollisionDispatch/btGhostObject.cpp \
                          BulletCollision/CollisionDispatch/btUnionFind.cpp \
                          BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btManifoldResult.cpp \
                          BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
                          BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
                          BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
                          BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
                          BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
                          BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
                          BulletCollision/CollisionShapes/btBoxShape.cpp \
                          BulletCollision/CollisionShapes/btCollisionShape.cpp \
                          BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
                          BulletCollision/CollisionShapes/btShapeHull.cpp \
                          BulletCollision/CollisionShapes/btConcaveShape.cpp \
                          BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
                          BulletCollision/CollisionShapes/btConvex2dShape.cpp \
                          BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
                          BulletCollision/CollisionShapes/btCompoundShape.cpp \
                          BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
                          BulletCollision/CollisionShapes/btCapsuleShape.cpp \
                          BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
                          BulletCollision/CollisionShapes/btEmptyShape.cpp \
                          BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
                          BulletCollision/CollisionShapes/btBox2dShape.cpp \
                          BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
                          BulletCollision/CollisionShapes/btCylinderShape.cpp \
                          BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
                          BulletCollision/CollisionShapes/btTriangleMesh.cpp \
                          BulletCollision/CollisionShapes/btConvexHullShape.cpp \
                          BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
                          BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
                          BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
                          BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
                          BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
                          BulletCollision/CollisionShapes/btConvexShape.cpp \
                          BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
                          BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
                          BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
                          BulletCollision/CollisionShapes/btSphereShape.cpp \
                          BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
                          BulletCollision/CollisionShapes/btConeShape.cpp \
                          BulletCollision/CollisionShapes/btTriangleCallback.cpp \
                          BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
                          BulletCollision/Gimpact/gim_memory.cpp \
                          BulletCollision/Gimpact/btTriangleShapeEx.cpp \
                          BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
                          BulletCollision/Gimpact/gim_tri_collision.cpp \
                          BulletCollision/Gimpact/gim_contact.cpp \
                          BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
                          BulletCollision/Gimpact/btContactProcessing.cpp \
                          BulletCollision/Gimpact/btGImpactShape.cpp \
                          BulletCollision/Gimpact/gim_box_set.cpp \
                          BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
                          BulletCollision/Gimpact/btGImpactBvh.cpp \
                          BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
                          BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
                          BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
                          BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
                          BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
                          BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
                          BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
                          BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
                          BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
                          BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
                          BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
                          BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
                          BulletDynamics/Character/btKinematicCharacterController.cpp \
                          BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
                          BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
                          BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
                          BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
                          BulletDynamics/Vehicle/btRaycastVehicle.cpp \
                          BulletDynamics/Vehicle/btWheelInfo.cpp \
                          BulletDynamics/Dynamics/Bullet-C-API.cpp \
                          BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
                          BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
                          BulletDynamics/Dynamics/btRigidBody.cpp
                          #MiniCL/MiniCL.cpp \
                          MiniCL/MiniCLTask/MiniCLTask.cpp \
                          MiniCL/MiniCLTaskScheduler.cpp \
                          BulletMultiThreaded/SpuContactManifoldCollisionAlgorithm.cpp \
                          BulletMultiThreaded/SpuLibspe2Support.cpp \
                          BulletMultiThreaded/SpuNarrowPhaseCollisionTask/boxBoxDistance.cpp \
                          BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuCollisionShapes.cpp \
                          BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.cpp \
                          BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.cpp \
                          BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuContactResult.cpp \
                          BulletMultiThreaded/Win32ThreadSupport.cpp \
                          BulletMultiThreaded/SpuCollisionTaskProcess.cpp \
                          BulletMultiThreaded/GpuSoftBodySolvers/CPU/btSoftBodySolver_CPU.cpp \
                          BulletMultiThreaded/GpuSoftBodySolvers/DX11/btSoftBodySolver_DX11.cpp \
                          BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/btSoftBodySolver_OpenCL.cpp \
                          BulletMultiThreaded/PosixThreadSupport.cpp \
                          BulletMultiThreaded/SpuCollisionObjectWrapper.cpp \
                          BulletMultiThreaded/SpuGatheringCollisionDispatcher.cpp \
                          BulletMultiThreaded/SpuSampleTask/SpuSampleTask.cpp \
                          BulletMultiThreaded/SequentialThreadSupport.cpp \
                          BulletMultiThreaded/SpuFakeDma.cpp \
                          BulletMultiThreaded/btThreadSupportInterface.cpp \
                          BulletMultiThreaded/SpuSampleTaskProcess.cpp \
                          BulletMultiThreaded/btGpu3DGridBroadphase.cpp \
                          BulletMultiThreaded/btParallelConstraintSolver.cpp
                          
LOCAL_LDLIBS           := -lc -lm -llog -landroid -ldl -lGLESv2

include $(BUILD_SHARED_LIBRARY)