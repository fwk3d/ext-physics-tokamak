struct CRenderPrimitive {
    int type = 0;
    vec3 dims = {0,0,0};
    unsigned color = ~0u;
    void SetGraphicBox(float x, float y, float z) {
        type = 0;
        dims = vec3(x,y,z);
    }
    void SetGraphicCylinder(float r, float h) {
        type = 1;
        dims = vec3(r,h,0);
    }
    void SetGraphicSphere(float r) {
        type = 2;
        dims = vec3(r,0,0);
    }
    void SetDiffuseColor(vec4 c) {
        color = rgba(c.r * 255, c.g * 255, c.b * 255, c.a * 255);
    }
    void Render(void *device, neT3* v) { // neT3 is a mat33+pos3
        /*
            mat44
            [RGT.x] [RGT.y] [RGT.z] [  ]
            [UPV.x] [UPV.y] [UPV.z] [  ]
            [BCK.x] [BCK.y] [BCK.z] [  ]
            [POS.x] [POS.y] [POS.z] [US]
            RGT,UPV,BCK = right/up/back vectors, POS = position, US = uniform scale
        */

        vec3 pos = ptr3(&v->pos[0]); // M[12],M[13],M[14]

        ddraw_color(color);

        if(type == 0) {
            ddraw_cube33(pos, dims, &v->rot.M[0][0]); // M[0][0]..M[2][2]
        }

        if(type == 1) {
            vec3 upv = norm3(ptr3(&v->rot.M[1][0])); // M[4],M[5],M[6]
            ddraw_circle(pos, upv, dims.x);
        }

        if(type == 2) {
            float radius = dims.x;
#if 1
            ddraw_sphere(pos, radius);
#else
            ddraw_circle(pos, vec3(1,0,0), radius);
            ddraw_circle(pos, vec3(0,1,0), radius);
            ddraw_circle(pos, vec3(0,0,1), radius);

            vec3 facing = norm3(sub3(pos, camera_get_active()->position));
            ddraw_circle(pos, facing, radius);
#endif
        }
    }
    struct mesh_t {
        void Destroy() {}
    } mMesh;
};
camera_t cam;
void appInit();
void  appInput();
void  appTick();
void  appDraw();
void appQuit();
int main() {
    app_create(0.75, 0);

    appInit();

    cam = camera();

    while(app_swap()) {
        // fps camera
        camera_freefly(&cam, 0);

        // app
        appInput();
        appTick();
        appDraw();
    }

    appQuit();
}
