# VehicleCamera解读 - DesignYourDream - 博客园
# [VehicleCamera解读](https://www.cnblogs.com/designyourdream/p/5235326.html)
坐标系：
 z-axis                                  
   ^                               
   |                               
   |   y-axis                   
   |  /                         
   | /                           
   |/                             
   +---------------->   x-axis     
围绕Z轴旋转叫做偏航角，**Yaw**；围绕X轴旋转叫做 俯仰角，**Pitch**；围绕Y轴旋转，叫做滚转角，**Roll **
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
```
相机的标记：
```
//使用三个标记，记录相机行进方向；相机无非就是向前，向后，旋转，这三个状态
int m_flags;             
int m_forwardFlags;            
int m_backwardsFlags;
```
初始化时：
```
m_flags = eVCam_goingForwards | eVCam_canRotate; // 向前 + 可旋转
    m_forwardFlags = eVCam_rotationSpring | eVCam_rotationClamp; //带悬架的旋转 + 限制旋转角度
    m_backwardsFlags = m_forwardFlags;//和上面那个值一样，所以感觉没什么用处
```
```
接受用户输入的旋转：
```
void CVehicleViewSteer::OnAction(const TVehicleActionId actionId, int activationMode, float value)
{//看看上面的图就了然了
    if (m_flags & eVCam_canRotate)
    {
        if (actionId == eVAI_RotateYaw)
        {
            m_rotatingAction.z += value;
        }
        else if (actionId == eVAI_RotatePitch)
        {        
            m_rotatingAction.x += value;
        }
    }
}
```
//////////////////////////////////////////////////////////////////////////////////////////////////
```
IEntity * pEntity = m_pVehicle->GetEntity();
    assert(pEntity);
    CVehicleMovementBase * pVehicleMovement = static_cast<CVehicleMovementBase*>(m_pVehicle->GetMovement());
    const pe_status_dynamics& dynStatus = pVehicleMovement->GetPhysicsDyn(); //这个貌似是取出了物理系统
    SMovementState movementState;    
    pVehicleMovement->GetMovementState(movementState);
    const float pedal = pVehicleMovement->GetEnginePedal();//获取坦克引擎力
    const float maxSpeed = movementState.maxSpeed;//获取当前坦克的最大速度
    const Matrix34& pose = m_pAimPart ? m_pAimPart->GetWorldTM() : pEntity->GetWorldTM();//取出坦克的世界坐标到局部坐标的矩阵
    const Vec3 entityPos = pose.GetColumn3();
    const Vec3 xAxis = pose.GetColumn0();
    const Vec3 yAxis = pose.GetColumn1();
    const Vec3 zAxis = pose.GetColumn2();
    const float forwardSpeed = dynStatus.v.dot(yAxis);//当前坦克的速度，在Y轴上的投影，就是Y轴方向上的速度了
    const float speedNorm = clamp(forwardSpeed / maxSpeed, 0.0f, 1.0f);//计算出这个Y轴的速度与最大速度比例，其实就是个百分比
    const Vec3 maxRotation = m_maxRotation + speedNorm * (m_maxRotation2 - m_maxRotation);//这个不知道
    CalcLookAt(pose);//根据世界坐标矩阵，计算 m_lookAt，看起来是将look at转到坦克本地坐标系下 
    if (m_lookAt.IsValid())
    {
        if (!m_lastOffset.IsValid())
        {
　　　　　　　//如果之前没有设置过last offset
            m_position = pose * m_localSpaceCameraOffset; //这个应该是想把相机设置到坦克局部坐标系下，同时偏移了一个offset
            m_lastOffset = m_position - m_lookAt;// 基本上相当于相机和坦克之间的向量
            m_lastOffsetBeforeElev = m_lastOffset;
        }
        Vec3 offset = m_lastOffsetBeforeElev;
        if (pedal<0.1f && forwardSpeed<1.0f)
        {//Y轴速度小于 1，并且当前引擎马力小于 0.1，说明相机要向后移动
            // Going Backwards
            m_flags &= ~(eVCam_goingForwards | m_forwardFlags);
            m_flags |= m_backwardsFlags;
        }
        if (offset.dot(yAxis) < 0.8f && forwardSpeed>1.f)
        {//Y轴速度大于1，并且上次的偏移在Y轴上的投影大于等于0.8，说明相机要向前移动
            // Going Forwards
            m_flags &= ~m_backwardsFlags;
            m_flags |= eVCam_goingForwards | m_forwardFlags;
        }
         
　　　　　  
        float sensitivity = (1.f-speedNorm) * m_stickSensitivity.z + speedNorm * m_stickSensitivity2.z;//影响值。speedNorm是个比例，意义是，在stickSensitivity1,2之间做插值
        float rotate = -m_rotatingAction.z * sensitivity;//输入的Z要乘以个0.5，即sensitivity的值
        rotate = rotate * dt;//再乘以时间？
        
        if (zAxis.z > 0.1f)
        {//如果坦克的高度超过 0.1
            // Safe to update curYaw
            Vec3 projectedX = xAxis; projectedX.z = 0.f;
            Vec3 projectedY = yAxis; projectedY.z = 0.f;
            const float newYaw = atan2_tpl(offset.dot(projectedX), -(offset.dot(projectedY)));
            const float maxChange = DEG2RAD(270.f)*dt;
            const float delta = clamp(newYaw - m_curYaw, -maxChange, +maxChange);
            m_curYaw += delta;
        }
    
        // Rotation Action
        {
            if (m_flags & eVCam_rotationClamp)
            {
                float newYaw = clamp(m_curYaw + rotate, -maxRotation.z, +maxRotation.z);
                rotate = newYaw - m_curYaw;
                rotate = clamp(newYaw - m_curYaw, -fabsf(rotate), +fabsf(rotate));
                m_rotation.z += rotate;
            }
            else
            {
                m_rotation.z=0.f;
            }
            if (speedNorm > 0.1f)
            {
                float reduce = dt * 1.f;
                m_rotation.z = m_rotation.z - reduce*m_rotation.z/(fabsf(m_rotation.z) + reduce);
            }
        }
        // Ang Spring
        {
            float angSpeedCorrection = dt*dt*m_angSpeedCorrection/(dt*m_angSpeedCorrection+1.f)*dynStatus.w.z;
            if ((m_flags & eVCam_rotationSpring)==0)
            {
                m_angReturnSpeed = 0.f;
                angSpeedCorrection = 0.f;
            }
            float difference = m_rotation.z - m_curYaw;
            float relax = difference * (m_angReturnSpeed*dt) / ((m_angReturnSpeed*dt) + 1.f);
            const float delta = +relax + angSpeedCorrection + rotate;
            m_curYaw += delta;
            Matrix33 rot = Matrix33::CreateRotationZ(delta);
            offset = rot * offset;
            // Lerp the spring speed
            float angSpeedTarget = m_angReturnSpeed1 + speedNorm * (m_angReturnSpeed2 - m_angReturnSpeed1);
            m_angReturnSpeed += (angSpeedTarget - m_angReturnSpeed) * (dt/(dt+0.3f));
            m_angSpeedCorrection += (m_angSpeedCorrection0 - m_angSpeedCorrection) * (dt/(dt+0.3f));
        }
        if (!offset.IsValid()) offset = m_lastOffset;
        // Velocity influence
        Vec3 displacement = -((2.f-speedNorm) * dt) * dynStatus.v;//车辆产生位移，这里算的是位移和2倍位移之间的插值，例如，-5和-10之间的插值
        float dot = offset.dot(displacement); //Offset 是从相机的偏移位置 到 坦克位置的向量，这个向量和坦克的移动做点积，根据两个向量的方向 ，判断是向前还是向后，再做修正。
        if (dot < 0.f)
        {
            displacement = displacement + offset * -0.1f * (offset.dot(displacement) / offset.GetLengthSquared());
        }
        offset = offset + displacement;//这个部分计算的应该是跟随，车辆的移动，需要对偏移值做修正
        const float radius0 = fabsf(m_localSpaceCameraOffset.y);
        const float minRadius = radius0 * m_radiusMin;
        const float maxRadius = radius0 * m_radiusMax;
        float radiusXY = sqrtf(sqr(offset.x) + sqr(offset.y));
        Vec3 offsetXY = offset; offsetXY.z = 0.f;
        Vec3 accelerationV = (dynStatus.v - m_lastVehVel);//速度变化，加速度
        float acceleration = offsetXY.dot(accelerationV) / radiusXY; //  
        m_lastVehVel = dynStatus.v;
        m_radiusVel -= acceleration;
        m_radius += m_radiusVel * dt - dt*m_radiusVelInfluence * offsetXY.dot(dynStatus.v)/radiusXY;
        m_radiusVel *= expf(-dt*m_radiusDampRate);
        m_radius += (radius0-m_radius)*(dt*m_radiusRelaxRate)/(dt*m_radiusRelaxRate+1.f);
        m_radius = clamp(m_radius, minRadius, maxRadius);
        offset = offset * (m_radius/radiusXY);
    
        // Vertical motion
        float targetOffsetHeight = m_localSpaceCameraOffset.z * (m_radius/radius0);//
        float oldOffsetHeight = offset.z;
        offset.z += (targetOffsetHeight - offset.z)*(dt/(dt+0.3f));
        Limit(offset.z, targetOffsetHeight - 2.f, targetOffsetHeight + 2.f);
        float verticalChange = offset.z - oldOffsetHeight;
        m_lastOffsetBeforeElev = offset;
        
        // Add up and down camera tilt
        {
            offset.z -= verticalChange;
            m_rotation.x += dt * m_stickSensitivity.x * m_rotatingAction.x;
            m_rotation.x = clamp(m_rotation.x, -maxRotation.x, +maxRotation.x);
            float elevAngleVehicle = m_inheritedElev * yAxis.z;         // yAxis.z == approx elevation angle
            float elevationAngle = m_rotation.x - elevAngleVehicle;
            float sinElev, cosElev;
            cry_sincosf(elevationAngle, &sinElev, &cosElev);
            float horizLen = sqrtf(offset.GetLengthSquared2D());
            float horizLenNew = horizLen * cosElev - sinElev * offset.z;
            if (horizLen>1e-4f)
            {
                horizLenNew /= horizLen;
                offset.x *= horizLenNew;
                offset.y *= horizLenNew;
                offset.z = offset.z * cosElev + sinElev * horizLen;
            }
            offset.z += verticalChange;
        }
        if (!offset.IsValid()) offset = m_lastOffset;
        m_position = m_lookAt + offset;
        // intersection check...
        {
            IPhysicalEntity* pSkipEntities[10];
            int nSkip = 0;
            if(m_pVehicle)
            {
                nSkip = m_pVehicle->GetSkipEntities(pSkipEntities, 10);
            }
            primitives::sphere sphere;
            sphere.center = m_lookAt;
            sphere.r = g_CameraRadius;
            Vec3 dir = m_position-m_lookAt;
            geom_contact *pContact = 0;          
            float hitDist = gEnv->pPhysicalWorld->PrimitiveWorldIntersection(sphere.type, &sphere, dir, ent_static|ent_terrain|ent_rigid|ent_sleeping_rigid,
                    &pContact, 0, (geom_colltype_player<<rwi_colltype_bit) | rwi_stop_at_pierceable, 0, 0, 0, pSkipEntities, nSkip);
            if(hitDist > 0.0f)
            {
                // Interpolate the offset
                Vec3 newPos = m_lookAt + hitDist * dir.GetNormalizedSafe();
                offset = newPos - m_lookAt;
            }
        }
                
        Interpolate(m_lastOffset, offset, 10.f, dt);
        m_position = m_lookAt + m_lastOffset;
    }
    else
    {
        CRY_ASSERT_MESSAGE(0, "camera will fail because lookat position is invalid");
    }
    //float sensitivity = (m_pSensitivity) ? m_pSensitivity->GetFVal() : 1.0f;
    m_rotatingAction.zero();
```

