
// Vector3f TracingRender::getRadiance(Ray &ray, int bounce) {
//     if (bounce > maxBounce) {
//         return Vector3f(0.0);
//     }

//     Interaction interaction = castRay(ray);
//     if (!interaction.flag) {
//         // background
//         return background;
//     }

//     // 1. mode witte style
//     // 1. phone style specular diffuse + fresnel

//     Vector3f ret(0.0,0,0);
//     Vector3f hitPoint = interaction.hitPoint;
//     Sphere* hitOBject = interaction.hitObject;
//     Vector3f N;
//     hitOBject->getSurfaceProperties(hitPoint, N);
//     if (hitOBject->reflectType == utils::REFLECTION_AND_REFRACTION) {
//         float fresnelCoeffient = fresnel(ray.dir, N, hitOBject->ior);
//         Vector3f reflectRayDir = normalize(reflect(ray.dir, N));
//         Vector3f reflectRayOri = Dot(reflectRayDir,N) > 0 ? hitPoint + N * EPS : hitPoint - N * EPS;

//         Vector3f refractRayDir = normalize(refract(ray.dir, N, hitOBject->ior));
//         Vector3f refractRayOri = Dot(refractRayDir,N) > 0? hitPoint + N * EPS : hitPoint - N * EPS;

//         Ray reflectRay(reflectRayOri, reflectRayDir);
//         Ray refractRay(refractRayOri, refractRayDir);
//         ret = getRadiance(reflectRay, bounce + 1) * fresnelCoeffient + getRadiance(refractRay, bounce + 1) * (1 - fresnelCoeffient);
//     } else if (hitOBject->reflectType == utils::REFLECTION) {
//         float fresnelCoeffient = fresnel(ray.dir, N, hitOBject->ior);
//         Vector3f reflectRayDir = normalize(reflect(ray.dir, N));
//         Vector3f reflectRayOri = Dot(reflectRayDir, N)> 0 ? hitPoint + N * EPS : hitPoint - N * EPS;

//         Ray reflectRay(reflectRayOri, reflectRayDir);
//         ret = getRadiance(reflectRay, bounce + 1) * fresnelCoeffient;
//     } else if (hitOBject->reflectType == utils::DIFFUSE) {
//         for (Light light : lights) {
//             Vector3f lightDir = ( light.pos - hitPoint);
//             float lightDistance2 = lightDir.lengthSquared();
//             lightDir = normalize(lightDir);
            
//             Vector3f outgoingDir = reflect(lightDir, N);
//             Vector3f halfVector = normalize(outgoingDir + lightDir);
//             // polygon offset 类似shadow map避免精度问题
//             Vector3f temporigin = Dot(ray.dir, N) < 0 ? hitPoint + N * EPS : hitPoint - N * EPS;
//             Ray shadowRay(temporigin, lightDir);
//             Interaction shadowRayInteraction = castRay(shadowRay);
//             float diffuseCoefficient = std::max(Dot(lightDir, N), 0.f) ;
//             if (shadowRayInteraction.flag) {
//                 Vector3f tempShadowRayDir = shadowRay.o - shadowRayInteraction.hitPoint;
//                 float tempShandowRayDistance2 = tempShadowRayDir.lengthSquared();
//                 if (tempShandowRayDistance2 < lightDistance2) {
//                     // shadow without diffuse
//                     diffuseCoefficient = 0;
//                 }
//             }

//             Vector3f specularColor = light.intensity * std::pow(Dot(halfVector,N),hitOBject->specularExponent); 

//             if (diffuseCoefficient == 0) {
//                 Vector3f foo;
//             }
//             Vector3f diffuseRadiance = hitOBject->diffuseColor * diffuseCoefficient * hitOBject->kd;
//             Vector3f specularRadiance =  specularColor * hitOBject->ks;
//             ret += diffuseRadiance + specularRadiance;
//         }
//     }

//     return ret;
//     // return Vector3f(1.0,0.0,0.0);

//     // return radiance
// }