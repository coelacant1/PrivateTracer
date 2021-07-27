for (int l = 0; l < scene->numLights; l++) {
    Vector3D lVector = scene->lights[l].p;// - tempInt;

    float angle = triangles[triangle]->normal.DotProduct(lVector.UnitSphere());

    if (angle > 0) {
        float lDistance = scene->lights[l].p.CalculateEuclideanDistance(intersect) / scene->lights[l].falloff;
        float intensity = 1.0f / (1.0f + lDistance * scene->lights[l].a + powf(lDistance / scene->lights[l].falloff, 2.0f) * scene->lights[l].b);
        
        color = color + (scene->lights[l].intensity * angle * intensity);//add intensity drop with distance?
    }
}