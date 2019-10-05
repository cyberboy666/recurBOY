If you want to add your own shaders to recurBOY from shadertoy, you have to convert them manually to GLES.

# HelloShaderCircle

We took this simple example from Shadertoy
(A Simple Circle)[https://www.shadertoy.com/view/XsjGDt]

## ShaderToy Default Variables:

The ShaderToy variable names are slightly different from the ones in GLSL Sandbox. The default variables are:

```
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
```

## ShaderToy Original Example:

```
/**
 * @author jonobr1 / http://jonobr1.com/
 */

/**
 * Convert r, g, b to normalized vec3
 */
vec3 rgb(float r, float g, float b) {
	return vec3(r / 255.0, g / 255.0, b / 255.0);
}

/**
 * Draw a circle at vec2 `pos` with radius `rad` and
 * color `color`.
 */
vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) {
	float d = length(pos - uv) - rad;
	float t = clamp(d, 0.0, 1.0);
	return vec4(color, 1.0 - t);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {

	vec2 uv = fragCoord.xy;
	vec2 center = iResolution.xy * 0.5;
	float radius = 0.25 * iResolution.y;

    // Background layer
	vec4 layer1 = vec4(rgb(210.0, 222.0, 228.0), 1.0);
	
	// Circle
	vec3 red = rgb(225.0, 95.0, 60.0);
	vec4 layer2 = circle(uv, center, radius, red);
	
	// Blend the two
	fragColor = mix(layer1, layer2, layer2.a);

}
```

## GLSLSandbox (recurBOY) format:

you have to change the variable names from fragCoord to gl_fragCoord and other ones like time to itime. Also the parameters f0, f1 and f2
