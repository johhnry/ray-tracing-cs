#version 460 core

// Uniforms
uniform float time;
uniform vec3 cameraPos;
uniform vec3 cameraLookAt;

// Inputs
layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) uniform image2D img_output;

// Structs
struct Sphere {
  vec3 center;
  float radius;
  vec3 color;
  bool metal;
};

struct HitRecord {
  float t;
  vec3 p;
  vec3 normal;
  vec3 color;
  bool reflection;
};

// Globals
const int samplesPerPixel = 1;
const float t_max = 1000000;
const int maxDepth = 10;

// Aspect ratio and FOV
const float aperture = 0.0;

const int nSpheres = 40;


/**
 * From : https://thebookofshaders.com/10/?lan=fr
 */
float random(vec2 st) {
  return fract(sin(dot(st.xy,
                       vec2(12.9898,78.233)))*
               43758.5453123);
}

vec3 randomVec(vec2 st) {
  return vec3(random(st), random(st.xy), random(st.yx));
}

vec3 random_in_unit_sphere(vec2 st) {
  while (true) {
    vec3 p = randomVec(st);
    if (dot(p, p) >= 1) continue;
    return p;
  }
}

vec3 random_in_unit_disk(vec2 st) {
	while (true) {
  	vec3 p = vec3(random(st), random(st), 0);
    if (dot(p, p) >= 1) continue;
    return p;
  }
}

float easeInOutQuint(float x) {
  return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}

vec3 rayColor(vec3 rayOrigin, vec3 rayDirection, vec2 st) {
  vec3 pixel = vec3(1.0);

  vec3 ray_o = rayOrigin;
  vec3 ray_d = rayDirection;
  int depth = maxDepth;

  while (depth >= 0) {
    if (depth == 0) {
      pixel *= 0.5 * vec3(0.0);
      break;
    }

    // Max hit record
    HitRecord max_hit = {t_max, vec3(0), vec3(0), vec3(0), false};

    // Object intersections
    for (int j = 0; j < nSpheres; j++) {
      Sphere sphere = Sphere(vec3(0.0, -100, 0.0), 100.0, vec3(0.8), false);

      if (j != 0) {
        const float n = (j / float(nSpheres));
        const float angle = n * 3.14 * 2.0;
        const float sphere_r = (cos(angle + time * 2.0) + 1.5) / 5.0;
        const float radius = sin(angle * 2.0 + time) * 5.0;
        const vec3 sphere_c = vec3(cos(angle) * radius, sphere_r + abs(sin(angle * 3 + time)), sin(angle) * radius);
        sphere = Sphere(sphere_c, sphere_r, vec3(n * 1.0, abs(cos(angle + time * 2.0)), abs(sin(cos(angle) * angle + time))), j % 3 != 0);
      }

      // Sphere intersection
      vec3 oc = ray_o - sphere.center;
      float a = dot(ray_d, ray_d);
      float half_b = dot(oc, ray_d);
      float c = dot(oc, oc) - sphere.radius * sphere.radius;

      float discriminant = half_b*half_b - a*c;

      // If there's solutions
      if (discriminant >= 0.0) {
        float t = (-half_b - sqrt(discriminant)) / a;

        // If hit is closer than previous one
        if (t < max_hit.t && t >= 0) {
          const vec3 pointOnSphere = ray_o + ray_d * t;
          const vec3 N = normalize(pointOnSphere - sphere.center);
          max_hit = HitRecord(t, pointOnSphere, N, sphere.color, sphere.metal);
        }
      }
    }

    // If didn't hit something
    if (max_hit.t == t_max) {
      // Display background
      float v = 0.5 * (ray_d.y + 1);
      pixel *= vec3((1.0 - v) * vec3(1.0, 1.0, 1.0) + v * vec3(0.5, 0.7, 1.0));
      break;
    }

    // If we hit something
    pixel *= max_hit.color;

    // Compute next ray
    ray_o = max_hit.p;
    if (max_hit.reflection) {
    	ray_d = reflect(normalize(ray_d), max_hit.normal);
    } else {
      vec3 target = max_hit.p + max_hit.normal + random_in_unit_sphere(st);
      ray_d = target - max_hit.p;
    }

    depth -= 1;
  }

  return pixel;
}

void main() {
  // Get image size and pixel location
  const ivec2 pixel_coords = ivec2(gl_GlobalInvocationID);
  const ivec2 image_size = imageSize(img_output);

  const float aspect_ratio = image_size.x / image_size.y;

  const float vfov = (easeInOutQuint((sin(time / 4.0) + 1.0) / 2.0)) * 50.0 + 20.0;
	const float theta = radians(vfov);
	const float h = tan(theta/2);
	const float viewport_height = 2.0 * h;
	const float viewport_width = aspect_ratio * viewport_height;
	const float focal_length = 1.0;

	// Defocus blur
	const float lens_radius = aperture / 2.0;
	const float focus_dist = length(cameraPos - cameraLookAt);

  // Camera lookat coordinate system
  const vec3 camW = normalize(cameraPos - cameraLookAt);
  const vec3 camU = normalize(cross(vec3(sin(time) / 2.0, 1.0, sin(time / 2.0)), camW));
  const vec3 camV = cross(camW, camU);

  //const vec3 horizontal = viewport_width * camU;
  //const vec3 vertical = viewport_height * camV;
  const vec3 horizontal = focus_dist * viewport_width * camU;
  const vec3 vertical = focus_dist * viewport_height * camV;

  // Initial pixel color
  vec3 pixel = vec3(0.0);

  // Do multi sampling
  for (int i = 0; i < samplesPerPixel; i++) {
    // UV coordinates in the window
    const float u = float(pixel_coords.x + random(pixel_coords.xy)) / (image_size.x - 1);
    const float v = float(pixel_coords.y + random(pixel_coords.yx)) / (image_size.y - 1);

    // Compute ray direction
    const vec3 lower_left_corner = cameraPos - horizontal / 2.0 - vertical / 2.0 - focus_dist * camW;
    const vec3 ray_d = lower_left_corner + u * horizontal + v * vertical - cameraPos;

    const vec3 rd = lens_radius * random_in_unit_disk(pixel_coords);
		const vec3 offset = camU * rd.x + camV * rd.y;

    pixel += rayColor(cameraPos + offset, ray_d, pixel_coords);
  }

	// Apply gamma correction
  pixel = sqrt(pixel / float(samplesPerPixel));

  imageStore(img_output, pixel_coords, vec4(pixel, 1.0));
}
