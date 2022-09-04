#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

uniform int x_offset=0;

const float renderHeight=400.;

void main()
{
    int mx=int(gl_FragCoord.x+x_offset)/8;
    int my=int(gl_FragCoord.y)/8;
    
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
    
    // finalColor=color;
    vec4 brown=vec4(.4196,.3529,.2392,1.);
    finalColor=mix(color,brown,(renderHeight-gl_FragCoord.y)/renderHeight);
}

