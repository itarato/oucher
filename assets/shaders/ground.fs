#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

uniform int x_offset=0;
uniform float surface_ys[10000];

const float renderHeight=400.;

const vec4 color_green=vec4(.1333,.4588,.0353,1.);
const vec4 color_brown1=vec4(.3686,.2706,.1412,1.);
const vec4 color_brown2=vec4(.4,.302,.1765,1.);

void main(){
    float surface_y=renderHeight-surface_ys[int(gl_FragCoord.x)+x_offset];
    
    if(surface_y-8.<gl_FragCoord.y){
        finalColor=color_green;
    }else{
        int mx=int(gl_FragCoord.x+x_offset)/16;
        int my=int(gl_FragCoord.y)/16;
        
        if((mx+my)%2==0){
            finalColor=color_brown1;
        }else{
            finalColor=color_brown2;
        }
    }
}
