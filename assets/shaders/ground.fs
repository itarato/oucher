#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

uniform int x_offset=0;
uniform float surface_ys[10000];

const float renderHeight=400.;

void main()
{
    int mx=int(gl_FragCoord.x+x_offset)/2;
    int my=int(gl_FragCoord.y)/2;
    
    vec4 color;
    
    int type=(mx*my)%(my%5+mx%5);
    
    if(type==0){
        color=vec4(.4549,.4235,.3725,1.);
    }else if(type==1){
        color=vec4(.4196,.3529,.2392,1.);
    }else if(type==2){
        color=vec4(.5333,.4863,.4039,1.);
    }else{
        color=vec4(.6314,.5804,.4902,1.);
    }
    
    float surface_y=renderHeight-surface_ys[int(gl_FragCoord.x)+x_offset];
    
    if(surface_y-8.<gl_FragCoord.y){
        float mul=2.-(surface_y-gl_FragCoord.y)/8.;
        vec4 green=vec4(.1333,.4588,.0353,1.)*mul;
        finalColor=green;
    }else{
        vec4 brown=vec4(.2275,.2,.1529,1.);
        finalColor=mix(brown,color,smoothstep(surface_y-150.,surface_y-0.,gl_FragCoord.y));
    }
}
