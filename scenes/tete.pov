// tete.pov: Exported from Wings 3D 1.4.1.git9a2473e 

#include "rad_def.inc"

global_settings{
	 assumed_gamma 2.200000
	 max_trace_level 5
	 ambient_light rgb <0.000000, 0.000000, 0.000000>
}
background { rgb <0.000000, 0.000000, 0.000000> }
#declare camera_location = <-5.126851, 3.380946, 5.126851>;
camera{
	 perspective
	 location camera_location
	 right (320 / 240) * x
	 up y
	 angle 57.822402
	 sky <0.298836, 0.906308, -0.298836>
	 look_at <0.000000, 0.000000, 0.000000>
}
#declare wl_1_Infinite = light_source {
	 <-5.236851, 3.380946, 6.120851>
	 color rgb <0.700000, 0.700000, 0.700000>
	 fade_distance 1.000000
	 fade_power 0.000000
	 media_interaction on
	 media_attenuation off
	 parallel
	 point_at <0.000000, 0.000000, 0.000000>
}
#declare wm_default = texture{
	 pigment{
		 color rgbf <0.789854, 0.813333, 0.694044, 0.000000>
	 }
	 finish {
		 ambient rgb <0.000000, 0.000000, 0.000000>
		 diffuse 0.700000
		 brilliance 1.000000
		 metallic 0.000000
		 specular 1.000000
		 roughness 0.010000
	 }
}
#declare wo_0_tetrahedron1 = mesh2{
	 vertex_vectors { 4, <0.000000, 1.088662, 0.000000>, <0.000000, -0.544331, 1.154701>, <1.000000, -0.544331, -0.577350>, <-1.000000, -0.544331, -0.577350>}
	 //#local average_center = <0.000000, -0.136083, -0.000000>;
	 normal_vectors { 4, <0.000000, -1.000000, 0.000000>, <0.000000, 0.333333, -0.942809>, <-0.816497, 0.333333, 0.471405>, <0.816497, 0.333333, 0.471405>}
	 uv_vectors { 0}
	 texture_list { 1, texture{wm_default}}
	 face_indices { 4, <0, 2, 1>, 0, <2, 3, 1>, 0, <1, 3, 0>, 0, <0, 3, 2>, 0}
	 normal_indices { 4, <0, 2, 1>, <2, 3, 1>, <1, 3, 0>, <0, 3, 2>}
	 uv_indices { 4, <0, 0, 0>, <0, 0, 0>, <0, 0, 0>, <0, 0, 0>}
}
object{ wo_0_tetrahedron1
	 interior {
		 ior 1.000000
	 }
}
object{ wl_0_Ambient
}
object{ wl_1_Infinite
}
